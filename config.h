/*
 * Configuration file for Duck Pond Controller
 * Contains pin assignments, thresholds, and WiFi settings
 */

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"

// Pin Assignments (ESP32)
// Avoid GPIO 6-11 (wired to the module's flash), GPIO 1/3 (USB serial)
// and the boot strapping pins (GPIO 0, 2, 5, 12, 15).
#define TURBIDITY_PIN 36           // ADC1 input-only pin (A0) for the turbidity sensor
#define WATER_LEVEL_HIGH_PIN 25    // High water level float switch (input, pull-up)
#define WATER_LEVEL_LOW_PIN 26     // Low water level float switch (input, pull-up)
#define MANUAL_BUTTON_PIN 27       // Manual cleaning button (input, pull-up)
#define PUMP_RELAY_PIN 16          // Relay to control water pump
#define VALVE_RELAY_PIN 17         // Relay to control inlet valve
#define STATUS_LED_PIN 18          // Status LED
#define BUZZER_PIN 19              // Alert buzzer

// Sensor Thresholds
#define TURBIDITY_THRESHOLD 800    // ADC value above which water is considered dirty
#define TURBIDITY_CLEAN_THRESHOLD 300  // ADC value below which water is considered clean

// Timing Configuration (in milliseconds)
#define DRAIN_TIME 60000           // 1 minute to drain pond
#define REFILL_TIME 120000         // 2 minutes to refill pond
#define SETTLE_TIME 5000           // 5 seconds settling time between drain/refill
#define MIN_CLEAN_INTERVAL 1800000 // 30 minutes minimum between cleaning cycles

// Safety Configuration
#define MAX_PUMP_RUN_TIME 300000   // 5 minutes maximum pump run time
#define WATER_LEVEL_CHECK_INTERVAL 2000  // Check water level every 2 seconds

// System Status Structure
struct SystemStatus {
  bool systemEnabled;
  bool cleaning;
  bool pumpRunning;
  bool valveOpen;
  int turbidity;
  bool waterLevelHigh;
  bool waterLevelLow;
  unsigned long lastCleanTime;
  unsigned long cleaningStartTime;
  String lastCleanReason;
  String currentPhase;
  int cleaningCycleCount;
};

// Settings Structure (stored in EEPROM)
struct Settings {
  int turbidityThreshold;
  int drainTime;
  int refillTime;
  int settleTime;
  int minCleanInterval;
  bool autoCleanEnabled;
  bool alertsEnabled;
};

// EEPROM Addresses
#define EEPROM_SETTINGS_ADDR 0
#define EEPROM_CLEAN_COUNT_ADDR 100

#endif
