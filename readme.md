# 💡 ESP32 MQTT LED Control

Remote LED control over the internet using an **ESP32** microcontroller and the **MQTT** protocol via the free [HiveMQ](https://www.hivemq.com/) public broker.

Publish `"on"` or `"off"` to the **order** topic from any MQTT client (phone app, Node-RED, Python script, another ESP32, etc.) and the LED responds instantly. The board also publishes its current state back on a **status** topic so dashboards can stay in sync.

---

## 📋 Features

| Feature | Details |
|---|---|
| **WiFi connectivity** | Connects to a 2.4 GHz / 5 GHz WiFi network |
| **MQTT messaging** | Uses the free HiveMQ public broker (`broker.hivemq.com:1883`) |
| **LED control** | Accepts `"on"` / `"off"` commands on `esp32/led/order` |
| **Status feedback** | Publishes LED state to `esp32/led/status` after every change |
| **Auto-reconnect** | Automatically reconnects to WiFi and MQTT if the connection drops |
| **Unique client ID** | Appends the chip's MAC address to avoid broker collisions |

---

## 🔌 Hardware Requirements

- **ESP32** development board (any variant — DevKit V1, NodeMCU-32S, etc.)
- **LED** + **220 Ω resistor** connected to **GPIO 15** (or use the onboard LED if available on GPIO 15)
- USB cable for programming and serial monitoring

### Wiring Diagram

```
ESP32 GPIO 15 ──── 220Ω ──── LED (Anode +) ──── LED (Cathode -) ──── GND
```

---

## 🛠️ Software Requirements

| Tool | Version |
|---|---|
| [Arduino IDE](https://www.arduino.cc/en/software) | 2.x or later |
| ESP32 Board Package | Install via **Board Manager** → search `esp32` |
| [PubSubClient](https://github.com/knolleary/pubsubclient) | Install via **Library Manager** → search `PubSubClient` by Nick O'Leary |

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/esp32_mqtt.git
cd esp32_mqtt
```

### 2. Configure WiFi & Broker

Open `esp32_mqtt.ino` and update the WiFi credentials:

```cpp
const char *WIFI_SSID     = "Your_WiFi_SSID";
const char *WIFI_PASSWORD = "Your_WiFi_Password";
```

> **Note:** The sketch uses the **HiveMQ public broker** by default — no account or setup required. You can replace `MQTT_BROKER` with your own broker address if needed.

### 3. Upload to ESP32

1. Open `esp32_mqtt.ino` in the **Arduino IDE**.
2. Select your board: **Tools → Board → ESP32 Dev Module** (or your specific variant).
3. Select the correct **COM port**.
4. Click **Upload** (→).

### 4. Monitor Serial Output

Open the **Serial Monitor** at **115200 baud**. You should see:

```
=== ESP32 MQTT + LED Control ===

Connecting to WiFi: Your_WiFi_SSID
.....
WiFi connected!
IP address: 192.168.1.xxx
Connecting to HiveMQ broker... connected!
Subscribed to: esp32/led/order
```

---

## 📡 MQTT Topics

| Topic | Direction | Payload | Description |
|---|---|---|---|
| `esp32/led/order` | **Subscribe** (incoming) | `"on"` / `"off"` | Send commands to turn the LED on or off |
| `esp32/led/status` | **Publish** (outgoing) | `"on"` / `"off"` | Board reports the current LED state |

---

## 🧪 Testing

You can test the project using any MQTT client. Here are a few options:

### Using MQTTX (Desktop App)

1. Download [MQTTX](https://mqttx.app/).
2. Create a new connection to `broker.hivemq.com` on port `1883`.
3. Subscribe to `esp32/led/status` to see feedback.
4. Publish `on` or `off` to `esp32/led/order`.

### Using Mosquitto CLI

```bash
# Turn LED ON
mosquitto_pub -h broker.hivemq.com -t "esp32/led/order" -m "on"

# Turn LED OFF
mosquitto_pub -h broker.hivemq.com -t "esp32/led/order" -m "off"

# Watch status updates
mosquitto_sub -h broker.hivemq.com -t "esp32/led/status"
```

### Using HiveMQ Web Client

1. Go to [HiveMQ Web Client](http://www.hivemq.com/demos/websocket-client/).
2. Click **Connect**.
3. Subscribe to `esp32/led/status`.
4. Publish `on` or `off` to `esp32/led/order`.

---

## 📁 Project Structure

```
esp32_mqtt/
├── esp32_mqtt.ino   # Main firmware sketch
└── README.md        # This file
```

---

## ⚙️ Configuration Reference

| Constant | Default | Description |
|---|---|---|
| `WIFI_SSID` | `"Ibrahim_5G"` | Your WiFi network name |
| `WIFI_PASSWORD` | `"..."` | Your WiFi password |
| `MQTT_BROKER` | `"broker.hivemq.com"` | MQTT broker hostname |
| `MQTT_PORT` | `1883` | MQTT broker port (non-TLS) |
| `TOPIC_LED_ORDER` | `"esp32/led/order"` | Topic to receive LED commands |
| `TOPIC_LED_STATUS` | `"esp32/led/status"` | Topic to publish LED state |
| `LED_PIN` | `15` | GPIO pin connected to the LED |

---

## 🔒 Security Notice

> ⚠️ **This project uses a public MQTT broker with no authentication or encryption.** Anyone can publish to the same topics. For production use, consider:
> - Using a **private MQTT broker** with TLS and authentication.
> - Adding **unique topic prefixes** to avoid collisions.
> - Switching to MQTT over **TLS (port 8883)**.

---

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to open an issue or submit a pull request.

