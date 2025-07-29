/*
 * Configuration file for Duck Pond Controller
 * Contains pin assignments, thresholds, and WiFi settings
 */

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"

// Pin Assignments
#define TURBIDITY_PIN A0           // Analog pin for turbidity sensor
#define WATER_LEVEL_HIGH_PIN 2     // Digital pin for high water level sensor
#define WATER_LEVEL_LOW_PIN 3      // Digital pin for low water level sensor
#define MANUAL_BUTTON_PIN 4        // Manual cleaning button
#define PUMP_RELAY_PIN 5           // Relay to control water pump
#define VALVE_RELAY_PIN 6          // Relay to control inlet valve
#define STATUS_LED_PIN 7           // Status LED
#define BUZZER_PIN 8               // Alert buzzer

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
