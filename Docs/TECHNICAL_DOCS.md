# Duck Pond Auto-Cleaning System - Technical Documentation

## Overview
This ESP32-based system automatically monitors and cleans a duck pond using turbidity sensors, water level sensors, and controlled pumps/valves.

## Hardware Setup

### Required Components
- ESP32 Development Board
- Turbidity Sensor (Analog output)
- 2x Water Level Sensors (Float switches)
- 2x Relay Modules (for pump and valve control)
- Submersible Water Pump
- Solenoid Valve (for clean water inlet)
- Manual Push Button
- LED indicator
- Buzzer
- Power Supply (12V recommended)

### Pin Connections
```
ESP32 Pin    | Component
-------------|------------------
A0 (GPIO36)  | Turbidity Sensor (analog)
GPIO25       | Water Level High Sensor
GPIO26       | Water Level Low Sensor  
GPIO27       | Manual Button
GPIO16       | Pump Relay
GPIO17       | Valve Relay
GPIO18       | Status LED
GPIO19       | Buzzer
GND          | All ground connections
3.3V         | Sensor power (if 3.3V)
5V           | Relay module power
```

## Software Architecture

### Main Components

1. **Sensor Management** (`sensors.cpp/h`)
   - Turbidity monitoring with moving average
   - Water level detection
   - Sensor calibration functions

2. **Pump Controller** (`pump_controller.cpp/h`)
   - Cleaning cycle state machine
   - Safety mechanisms
   - Phase management (Drain → Settle → Refill → Complete)

3. **Web Interface** (`web_interface.cpp/h`)
   - REST API endpoints
   - Real-time dashboard
   - Remote control capabilities

4. **Configuration** (`config.h`)
   - Pin assignments
   - Thresholds and timing
   - System parameters

### Cleaning Cycle Phases

1. **DRAINING**: Pump removes dirty water until low level reached or timeout
2. **SETTLING**: 5-second pause to allow sediment to settle
3. **REFILLING**: Valve opens to fill with clean water until high level reached
4. **COMPLETING**: Final checks and cleanup

### Safety Features

- Maximum pump runtime protection (5 minutes)
- Water level monitoring during operation
- Emergency stop functionality
- Minimum interval between cleaning cycles (30 minutes)
- System enable/disable controls

## Web Interface

### Dashboard Features
- Real-time status monitoring
- Water quality display
- Manual cleaning controls
- System enable/disable
- Cleaning cycle progress
- System logs

### API Endpoints
- `GET /api/status` - System status
- `GET /api/settings` - Configuration
- `POST /api/clean` - Start manual cleaning
- `POST /api/stop` - Stop cleaning
- `POST /api/enable` - Enable system
- `POST /api/disable` - Disable system

## Installation

1. **Hardware Assembly**
   - Connect all components according to pin diagram
   - Ensure proper power supply ratings
   - Use waterproof enclosures for electronics
   - Position sensors at appropriate pond locations

2. **Software Upload**
   - Install Arduino IDE with ESP32 support
   - Install required libraries: WiFi, WebServer, ArduinoJson
   - Configure WiFi credentials in `config.h`
   - Upload code to ESP32

3. **Calibration**
   - Run turbidity sensor calibration
   - Test water level sensors
   - Verify pump and valve operation
   - Set appropriate thresholds

## Configuration

### WiFi Setup
Edit `config.h`:
```cpp
#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"
```

### Sensor Thresholds
Adjust based on your pond conditions:
```cpp
#define TURBIDITY_THRESHOLD 800    // Higher = dirtier water
#define DRAIN_TIME 60000          // Drain duration (ms)
#define REFILL_TIME 120000        // Refill duration (ms)
```

## Troubleshooting

### Common Issues

1. **WiFi Connection Failed**
   - Check SSID/password
   - Verify WiFi range
   - System works offline with manual controls

2. **Sensors Not Reading**
   - Check connections
   - Verify power supply
   - Use serial monitor for debugging

3. **Pump Not Starting**
   - Check relay connections
   - Verify power supply capacity
   - Check safety interlocks

4. **Web Interface Not Loading**
   - Check ESP32 IP address in serial monitor
   - Verify network connectivity
   - Try direct IP access

### Debug Information
Enable serial output (115200 baud) to see:
- Sensor readings
- System state changes
- Error messages
- WiFi connection status

## Maintenance

1. **Regular Checks**
   - Clean turbidity sensor monthly
   - Inspect water level sensors
   - Check pump operation
   - Verify relay function

2. **Seasonal Maintenance**
   - Recalibrate sensors
   - Update thresholds if needed
   - Check waterproof seals
   - Test emergency stop

## Future Enhancements

- Solar power integration
- Data logging to SD card
- Email/SMS alerts
- Temperature monitoring
- pH level sensing
- Mobile app development
- Cloud data storage

## Support

For technical support or improvements:
- Check serial output for error messages
- Verify all connections
- Test individual components
- Review configuration settings

---

**Made with ❤️ for healthy ducks!** 🦆
