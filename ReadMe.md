# 🦆 Duck Pond Auto-Cleaning System with ESP32

An automated system to monitor and clean a duck pond using sensors and an ESP32. Designed for small-scale duck farms, especially in regions where African duck species require regular access to clean water.

---

## 📌 Project Goals
- Automatically detect dirty pond water using a turbidity sensor.
- Trigger cleaning (drain and refill) automatically or manually.
- Prevent over-draining or overflowing with water level sensors.
- Optionally control and monitor the system remotely via Wi-Fi.

---

## 🧩 Components Used

| Component              | Purpose                                    |
|------------------------|--------------------------------------------|
| ESP32                  | Central controller with Wi-Fi              |
| Turbidity Sensor       | Detect dirty water                         |
| Water Level Sensor     | Prevent overflow or overdrain              |
| Relay Module           | Control high power to pump                 |
| Submersible Water Pump | Drain and refill pond                      |
| Manual Button          | Manually trigger cleaning                  |
| Power Supply           | Power for ESP32 and pump                   |
| (Optional) Solenoid Valve | Control clean water input              |
| (Optional) LED/Buzzer  | Indicate system status                     |
| (Optional) Smartphone App | Remote monitoring and control         |

---

## 🖼️ System Diagram

  [ Smartphone / Web UI ]
                      |
                    Wi-Fi
                      |
                  [ ESP32 ]
                     /|\
     ----------------|-------------------
    |               |                 |
[Turbidity Sensor] [Water Level] [Manual Button] | | | Dirty? Level OK? Manual Start

                      |
               [Relay Module]
                      |
               [Water Pump Unit]
                 /         \
    [Drain Pipe]           [Inlet Pipe]
          |                    |
    To Wastewater        From Clean Tank

          [ Artificial Duck Pond ]
                  (with Ducks)

## 🧠 Software Logic

### Conditions:
- If **turbidity > threshold**, clean pond.
- If **manual button** is pressed, clean pond.
- If **water level** is too low/high, stop pump.

### Cleaning Steps:
1. Drain dirty water.
2. Wait for a few seconds.
3. Refill pond with clean water.
4. Check water level, then stop.

---

## 📱 Remote Control (Optional)
You can control the system remotely using:
- **A Web Interface** hosted on the ESP32.
- **Blynk App** or **Telegram Bot** for mobile access.

Functions available remotely:
- Trigger cleaning.
- Monitor turbidity and water levels.
- Get alerts or status updates.

---

## 🗂️ Code and Docs

| File | What it does |
| --- | --- |
| `duck_pond_controller.ino` | Main sketch: Wi-Fi, web server, cleaning decisions |
| `sensors.cpp/.h` | Turbidity and water level readings, calibration |
| `pump_controller.cpp/.h` | Drain, settle and refill sequence with safety limits |
| `web_interface.cpp/.h` | Web dashboard and JSON API served by the ESP32 |
| `config.h` | Pins, thresholds, timings and Wi-Fi settings |
| `test_functions.cpp` | Hardware checks for each sensor and relay |

Wiring, pin assignments and power: [`Docs/WIRING_GUIDE.md`](Docs/WIRING_GUIDE.md). Libraries: [`Docs/LIBRARIES.md`](Docs/LIBRARIES.md). Design and API details: [`Docs/TECHNICAL_DOCS.md`](Docs/TECHNICAL_DOCS.md).

**Build:** install the ESP32 board package and ArduinoJson, set your Wi-Fi in `config.h`, then upload `duck_pond_controller.ino`.

**Pins:** turbidity on GPIO36, float switches on GPIO25 and GPIO26, button on GPIO27, relays on GPIO16 and GPIO17, LED on GPIO18, buzzer on GPIO19. These avoid the ESP32's flash, serial and boot pins.

---

## 🧪 Calibration and Testing
- Test turbidity readings with clean vs dirty water.
- Set a threshold value (e.g., 800 ADC).
- Check refill time based on pump flow rate.
- Calibrate water level sensors (high/low threshold).

---

## 🛠️ Enclosure & Deployment
- Use a waterproof box for electronics.
- Protect sensors and cables from ducks and weather.
- Place pump intake in pond at desired depth.

---

## ✅ Final Checklist
- [ ] ESP32 is powered and responsive
- [ ] Sensors calibrated and stable
- [ ] Relay and pump working properly
- [ ] Manual and remote control functioning
- [ ] Ducks are happy and healthy 🦆

---

## 📸 Future Improvements
- Add solar power with battery for off-grid use
- Add camera for live monitoring
- Log turbidity data over time to SD card or cloud
- Improve UI for mobile remote access

---

## 🧑‍🔧 Made with ❤️ by Breye Foka & Family

If you like this project or have improvements to suggest, feel free to fork, improve, and share it. Let’s keep our ducks clean and healthy!