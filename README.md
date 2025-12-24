# ESP8266 WiFi OLED Player (UDP Based)

A smart wireless audio player using ESP8266 to receive messages via UDP protocol, display content on an OLED screen, and play audio files using the DFPlayer Mini module. This project is a core component of the **PBL4-WEBCAM** ecosystem.

## 🚀 Features

* **WiFi Connectivity**: Automatically connects to a pre-configured hotspot.
* **UDP Communication**: Receives data packets from a PC/Server in a specific format (`English|Vietnamese|ID`).
* **Dynamic Display**: Shows English and Vietnamese translations on a 128x64 SSD1306 OLED screen.
* **Audio Sync**: Plays the corresponding track from the SD card based on the ID received.
* **Robust Error Handling**: Visual feedback on OLED for WiFi connection status and DFPlayer initialization.

## 🛠 Hardware Components

* **Microcontroller**: ESP8266 (NodeMCU/Wemos D1 Mini)
* **Display**: OLED SSD1306 (128x64) I2C
* **Audio Module**: DFPlayer Mini + SD Card
* **Speaker**: 4-8 Ohm, 3W
* **Power**: 5V DC

## 🔌 Pin Mapping (Schematics)

| Component | ESP8266 Pin | Note |
| --- | --- | --- |
| **OLED SDA** | **D2** | I2C Data |
| **OLED SCL** | **D1** | I2C Clock |
| **DFPlayer TX** | **D6** | Software Serial RX |
| **DFPlayer RX** | **D7** | Software Serial TX (Use a 1k resistor) |

## 📥 Installation

1. **Libraries**: Ensure you have the following libraries installed in your Arduino IDE:
* `ESP8266WiFi`
* `Adafruit_GFX` & `Adafruit_SSD1306`
* `DFRobotDFPlayerMini`
* `SoftwareSerial`


2. **WiFi Configuration**: Open `loaman.ino` and update your credentials:
```cpp
const char* ssid = "Your_WiFi_Name"; // Avoid special characters
const char* password = "Your_Password";

```


3. **Upload**: Select your board (e.g., NodeMCU 1.0) and hit **Upload**.

## 📡 Data Format

The device listens for UDP packets on port `4210`. The message should follow this structure:
`[English Text]|[Vietnamese Text]|[Audio_File_ID]`

*Example:* `Hello|Xin Chao|1` -> Displays "Hello" (large font), "Xin Chao" (small font), and plays the 2nd file on the SD card (index + 1).
