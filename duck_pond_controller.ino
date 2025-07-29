/*
 * Duck Pond Auto-Cleaning System with ESP32
 * Main controller file with web interface and automatic cleaning
 * 
 * Features:
 * - Turbidity monitoring
 * - Water level monitoring
 * - Automatic and manual cleaning
 * - Web interface for remote control
 * - Safety mechanisms
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "config.h"
#include "sensors.h"
#include "pump_controller.h"
#include "web_interface.h"

// Global variables
WebServer server(80);
bool systemEnabled = true;
bool manualCleaningRequested = false;
unsigned long lastSensorCheck = 0;
unsigned long lastWebUpdate = 0;
SystemStatus currentStatus;

void setup() {
  Serial.begin(115200);
  Serial.println("Duck Pond Controller Starting...");
  
  // Initialize EEPROM for storing settings
  EEPROM.begin(512);
  
  // Initialize hardware
  initializePins();
  initializeSensors();
  initializePumpController();
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup web server routes
  setupWebServer();
  
  // Initialize system status
  currentStatus.systemEnabled = true;
  currentStatus.cleaning = false;
  currentStatus.lastCleanTime = 0;
  
  Serial.println("System initialized successfully!");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Check sensors every 5 seconds
  if (millis() - lastSensorCheck >= 5000) {
    updateSensorReadings();
    checkCleaningConditions();
    lastSensorCheck = millis();
  }
  
  // Update pump controller
  updatePumpController();
  
  // Handle manual button press
  handleManualButton();
  
  // Update web interface data every 2 seconds
  if (millis() - lastWebUpdate >= 2000) {
    updateWebData();
    lastWebUpdate = millis();
  }
  
  delay(100); // Small delay to prevent watchdog issues
}

void initializePins() {
  // Sensor pins
  pinMode(TURBIDITY_PIN, INPUT);
  pinMode(WATER_LEVEL_HIGH_PIN, INPUT_PULLUP);
  pinMode(WATER_LEVEL_LOW_PIN, INPUT_PULLUP);
  pinMode(MANUAL_BUTTON_PIN, INPUT_PULLUP);
  
  // Control pins
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(VALVE_RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initial states
  digitalWrite(PUMP_RELAY_PIN, LOW);
  digitalWrite(VALVE_RELAY_PIN, LOW);
  digitalWrite(STATUS_LED_PIN, HIGH); // System ready
  digitalWrite(BUZZER_PIN, LOW);
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Failed to connect to WiFi. Running in offline mode.");
  }
}

void setupWebServer() {
  // Serve main web page
  server.on("/", handleRoot);
  
  // API endpoints
  server.on("/api/status", HTTP_GET, handleGetStatus);
  server.on("/api/settings", HTTP_GET, handleGetSettings);
  server.on("/api/settings", HTTP_POST, handleSetSettings);
  server.on("/api/clean", HTTP_POST, handleManualClean);
  server.on("/api/stop", HTTP_POST, handleStopCleaning);
  server.on("/api/enable", HTTP_POST, handleEnableSystem);
  server.on("/api/disable", HTTP_POST, handleDisableSystem);
  
  // Start server
  server.begin();
  Serial.println("Web server started");
}

void checkCleaningConditions() {
  if (!systemEnabled || currentStatus.cleaning) {
    return;
  }
  
  // Check turbidity threshold
  if (currentStatus.turbidity > TURBIDITY_THRESHOLD) {
    Serial.println("High turbidity detected - starting cleaning cycle");
    startCleaningCycle("Auto - High turbidity");
    return;
  }
  
  // Check manual cleaning request
  if (manualCleaningRequested) {
    Serial.println("Manual cleaning requested");
    startCleaningCycle("Manual request");
    manualCleaningRequested = false;
    return;
  }
}

void handleManualButton() {
  static bool lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  
  bool currentButtonState = digitalRead(MANUAL_BUTTON_PIN);
  
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > 50) { // 50ms debounce
    if (currentButtonState == LOW && lastButtonState == HIGH) {
      // Button pressed
      manualCleaningRequested = true;
      Serial.println("Manual button pressed");
    }
  }
  
  lastButtonState = currentButtonState;
}

void updateWebData() {
  // This function can be used to prepare data for web interface
  // or send real-time updates via WebSocket if implemented
}
