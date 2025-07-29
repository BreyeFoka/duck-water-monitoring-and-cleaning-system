# Arduino Libraries Setup

This document lists the required libraries for the Duck Pond Controller project.

## Required Libraries

Install these libraries through the Arduino IDE Library Manager:

### Core Libraries (Built-in with ESP32)
- **WiFi** - ESP32 WiFi connectivity
- **WebServer** - HTTP server functionality
- **EEPROM** - Non-volatile storage

### External Libraries (Install via Library Manager)

1. **ArduinoJson**
   - **Author**: Benoit Blanchon
   - **Version**: 6.x.x or later
   - **Purpose**: JSON parsing and generation for API responses
   - **Installation**: Search "ArduinoJson" in Library Manager

## Installation Steps

1. Open Arduino IDE
2. Go to **Tools > Manage Libraries...**
3. Search for each library by name
4. Click **Install** for each required library
5. Wait for installation to complete

## Library Usage in Project

### WiFi Library
```cpp
#include <WiFi.h>
// Used for ESP32 WiFi connection management
```

### WebServer Library
```cpp
#include <WebServer.h>
// Handles HTTP requests and responses
```

### ArduinoJson Library
```cpp
#include <ArduinoJson.h>
// Creates and parses JSON for API endpoints
```

### EEPROM Library
```cpp
#include <EEPROM.h>
// Stores configuration and cleaning cycle count
```

## Troubleshooting

### Library Installation Issues
- Ensure you have the latest Arduino IDE
- Check internet connection
- Try manual library installation if automatic fails
- Clear Arduino IDE cache if needed

### Compilation Errors
- Verify all libraries are properly installed
- Check library versions compatibility
- Ensure ESP32 board package is installed

### Missing ESP32 Support
If ESP32 is not available in boards:
1. Go to **File > Preferences**
2. Add this URL to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Go to **Tools > Board > Boards Manager**
4. Search "ESP32" and install the package

## Version Compatibility

This project is tested with:
- Arduino IDE 1.8.x or 2.x.x
- ESP32 Board Package 2.x.x
- ArduinoJson 6.x.x

For best results, use the latest stable versions of all libraries.
