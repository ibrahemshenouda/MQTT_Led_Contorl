/*
 * ESP32 MQTT LED Control
 * ------------------------------------
 * - Connects to WiFi & HiveMQ public broker
 * - Subscribes to "esp32/led/order" (accepts "on" / "off")
 * - Controls LED on GPIO 15 accordingly (starts OFF)
 * - Publishes LED status ("on" / "off") to "esp32/led/status" after every change
 *
 * Required library: PubSubClient by Nick O'Leary
 */

#include <WiFi.h>
#include <PubSubClient.h>

// ============== CONFIGURATION ==============
const char* WIFI_SSID     = "Your_SSID";
const char* WIFI_PASSWORD = "Your_Pass";

// HiveMQ public broker
const char* MQTT_BROKER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;

// Topics
const char* TOPIC_LED_ORDER  = "esp32/led/order";
const char* TOPIC_LED_STATUS = "esp32/led/status";

// LED pin
const int LED_PIN = 15;

// Unique client ID (chip ID appended at runtime)
String MQTT_CLIENT_ID = "esp32_client_";
// ===========================================

WiFiClient   espClient;
PubSubClient mqttClient(espClient);

bool ledState = false;  // LED starts OFF

// ───────────── MQTT Callback ─────────────
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Build a string from the incoming payload
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.toLowerCase();  // normalize to lowercase

  Serial.print("Received on [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  // Only act on the LED order topic
  if (String(topic) == TOPIC_LED_ORDER) {
    if (message == "on") {
      digitalWrite(LED_PIN, HIGH);
      ledState = true;
      Serial.println("LED turned ON");
      mqttClient.publish(TOPIC_LED_STATUS, "on");
      Serial.println("Published status: on");
    }
    else if (message == "off") {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
      Serial.println("LED turned OFF");
      mqttClient.publish(TOPIC_LED_STATUS, "off");
      Serial.println("Published status: off");
    }
    else {
      Serial.println("Unknown command — use \"on\" or \"off\"");
    }
  }
}

// ───────────── WiFi ─────────────
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ───────────── MQTT ─────────────
void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to HiveMQ broker...");

    if (mqttClient.connect(MQTT_CLIENT_ID.c_str())) {
      Serial.println(" connected!");

      // Subscribe to LED order topic
      mqttClient.subscribe(TOPIC_LED_ORDER);
      Serial.print("Subscribed to: ");
      Serial.println(TOPIC_LED_ORDER);

      // Publish current LED status on (re)connect
      mqttClient.publish(TOPIC_LED_STATUS, ledState ? "on" : "off");
    } else {
      Serial.print(" failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" — retrying in 3 seconds");
      delay(3000);
    }
  }
}

// ───────────── Setup ─────────────
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== ESP32 MQTT + LED Control ===\n");

  // Configure LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Build unique client ID
  MQTT_CLIENT_ID += String((uint32_t)ESP.getEfuseMac(), HEX);

  // Connect to WiFi
  connectToWiFi();

  // Configure MQTT broker & callback
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  // Connect to MQTT
  connectToMQTT();
}

// ───────────── Loop ─────────────
void loop() {
  // Reconnect if needed (also re-subscribes)
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();
}
