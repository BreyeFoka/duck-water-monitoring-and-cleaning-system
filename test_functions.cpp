/*
 * Test and Calibration Functions for Duck Pond Controller
 * Use these functions to test individual components and calibrate sensors
 */

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "sensors.h"
#include "pump_controller.h"

// Unlike .ino files, .cpp files get no automatic prototypes, so declare these first.
void setupTestMode();
void handleTestCommands();
void testAllSensors();
void testPump();
void testValve();
void testIndicators();
void monitorSensors();
void testFullCycle();
void printSystemStatus();
void printNetworkInfo();
bool verifyHardwareConnections();

// Test mode flags
bool testMode = false;
bool serialCommands = true;

void setupTestMode() {
  Serial.begin(115200);
  Serial.println("\n=== Duck Pond Controller Test Mode ===");
  Serial.println("Available commands:");
  Serial.println("  test_sensors    - Test all sensors");
  Serial.println("  test_pump       - Test pump operation");
  Serial.println("  test_valve      - Test valve operation");
  Serial.println("  test_leds       - Test LED and buzzer");
  Serial.println("  calibrate       - Calibrate turbidity sensor");
  Serial.println("  monitor         - Monitor sensor readings");
  Serial.println("  full_cycle      - Test complete cleaning cycle");
  Serial.println("  help            - Show this help");
  Serial.println("=====================================\n");
}

void handleTestCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    
    if (command == "test_sensors") {
      testAllSensors();
    }
    else if (command == "test_pump") {
      testPump();
    }
    else if (command == "test_valve") {
      testValve();
    }
    else if (command == "test_leds") {
      testIndicators();
    }
    else if (command == "calibrate") {
      calibrateTurbidySensor();
    }
    else if (command == "monitor") {
      monitorSensors();
    }
    else if (command == "full_cycle") {
      testFullCycle();
    }
    else if (command == "help") {
      setupTestMode();
    }
    else {
      Serial.println("Unknown command. Type 'help' for available commands.");
    }
  }
}

void testAllSensors() {
  Serial.println("\n--- Testing All Sensors ---");
  
  // Test turbidity sensor
  Serial.print("Turbidity Sensor (Pin A0): ");
  int turbidity = analogRead(TURBIDITY_PIN);
  Serial.print(turbidity);
  Serial.print(" (");
  if (turbidity < 300) Serial.print("CLEAN");
  else if (turbidity < 800) Serial.print("MODERATE");
  else Serial.print("DIRTY");
  Serial.println(")");
  
  // Test water level sensors
  Serial.print("Water Level High (Pin 2): ");
  bool levelHigh = digitalRead(WATER_LEVEL_HIGH_PIN) == LOW;
  Serial.println(levelHigh ? "TRIGGERED" : "NORMAL");
  
  Serial.print("Water Level Low (Pin 3): ");
  bool levelLow = digitalRead(WATER_LEVEL_LOW_PIN) == LOW;
  Serial.println(levelLow ? "TRIGGERED" : "NORMAL");
  
  // Test manual button
  Serial.print("Manual Button (Pin 4): ");
  bool buttonPressed = digitalRead(MANUAL_BUTTON_PIN) == LOW;
  Serial.println(buttonPressed ? "PRESSED" : "RELEASED");
  
  Serial.println("Sensor test complete.\n");
}

void testPump() {
  Serial.println("\n--- Testing Pump ---");
  Serial.println("Starting pump for 5 seconds...");
  
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  Serial.println("Pump ON");
  
  delay(5000);
  
  digitalWrite(PUMP_RELAY_PIN, LOW);
  Serial.println("Pump OFF");
  Serial.println("Pump test complete.\n");
}

void testValve() {
  Serial.println("\n--- Testing Valve ---");
  Serial.println("Opening valve for 5 seconds...");
  
  digitalWrite(VALVE_RELAY_PIN, HIGH);
  Serial.println("Valve OPEN");
  
  delay(5000);
  
  digitalWrite(VALVE_RELAY_PIN, LOW);
  Serial.println("Valve CLOSED");
  Serial.println("Valve test complete.\n");
}

void testIndicators() {
  Serial.println("\n--- Testing Indicators ---");
  
  // Test LED
  Serial.println("Testing LED...");
  for (int i = 0; i < 5; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(200);
  }
  
  // Test buzzer
  Serial.println("Testing buzzer...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  
  Serial.println("Indicator test complete.\n");
}

void monitorSensors() {
  Serial.println("\n--- Sensor Monitor Mode ---");
  Serial.println("Press any key to exit monitor mode");
  
  while (!Serial.available()) {
    updateSensorReadings();
    
    Serial.print("Turbidity: ");
    Serial.print(currentStatus.turbidity);
    Serial.print(" | Water High: ");
    Serial.print(currentStatus.waterLevelHigh ? "YES" : "NO");
    Serial.print(" | Water Low: ");
    Serial.print(currentStatus.waterLevelLow ? "YES" : "NO");
    Serial.print(" | Button: ");
    Serial.println(digitalRead(MANUAL_BUTTON_PIN) == LOW ? "PRESSED" : "NORMAL");
    
    delay(1000);
  }
  
  // Clear serial buffer
  while (Serial.available()) {
    Serial.read();
  }
  
  Serial.println("Monitor mode exited.\n");
}

void testFullCycle() {
  Serial.println("\n--- Full Cleaning Cycle Test ---");
  Serial.println("This will run a complete cleaning cycle.");
  Serial.println("Make sure pond is ready and pumps are properly connected.");
  Serial.print("Continue? (y/n): ");
  
  while (!Serial.available()) {
    delay(100);
  }
  
  String response = Serial.readStringUntil('\n');
  response.trim();
  response.toLowerCase();
  
  if (response != "y" && response != "yes") {
    Serial.println("Test cancelled.\n");
    return;
  }
  
  Serial.println("Starting full cleaning cycle test...");
  
  // Force start cleaning cycle
  currentStatus.systemEnabled = true;
  startCleaningCycle("Full Cycle Test");
  
  // Monitor the cycle
  while (currentStatus.cleaning) {
    updatePumpController();
    
    Serial.print("Phase: ");
    Serial.print(currentStatus.currentPhase);
    Serial.print(" | Pump: ");
    Serial.print(currentStatus.pumpRunning ? "ON" : "OFF");
    Serial.print(" | Valve: ");
    Serial.println(currentStatus.valveOpen ? "OPEN" : "CLOSED");
    
    delay(2000);
  }
  
  Serial.println("Full cycle test complete.\n");
}

// Simple diagnostic functions
void printSystemStatus() {
  Serial.println("\n=== System Status ===");
  Serial.print("System Enabled: "); Serial.println(currentStatus.systemEnabled ? "YES" : "NO");
  Serial.print("Currently Cleaning: "); Serial.println(currentStatus.cleaning ? "YES" : "NO");
  Serial.print("Current Phase: "); Serial.println(currentStatus.currentPhase);
  Serial.print("Pump Running: "); Serial.println(currentStatus.pumpRunning ? "YES" : "NO");
  Serial.print("Valve Open: "); Serial.println(currentStatus.valveOpen ? "YES" : "NO");
  Serial.print("Turbidity: "); Serial.println(currentStatus.turbidity);
  Serial.print("Water Level High: "); Serial.println(currentStatus.waterLevelHigh ? "YES" : "NO");
  Serial.print("Water Level Low: "); Serial.println(currentStatus.waterLevelLow ? "YES" : "NO");
  Serial.print("Cleaning Cycles: "); Serial.println(currentStatus.cleaningCycleCount);
  Serial.print("Free Heap: "); Serial.println(ESP.getFreeHeap());
  Serial.print("Uptime: "); Serial.println(millis() / 1000);
  Serial.println("====================\n");
}

void printNetworkInfo() {
  Serial.println("\n=== Network Information ===");
  Serial.print("WiFi Status: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
    Serial.print("Web Interface: http://"); Serial.println(WiFi.localIP());
  } else {
    Serial.println("Disconnected");
  }
  Serial.println("===========================\n");
}

// Hardware verification
bool verifyHardwareConnections() {
  Serial.println("\n--- Hardware Verification ---");
  bool allGood = true;
  
  // Check if sensors respond
  int turbidityCheck = analogRead(TURBIDITY_PIN);
  if (turbidityCheck == 0 || turbidityCheck == 4095) {
    Serial.println("WARNING: Turbidity sensor may not be connected properly");
    allGood = false;
  }
  
  // Check relay connections by briefly toggling
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  delay(10);
  digitalWrite(PUMP_RELAY_PIN, LOW);
  
  digitalWrite(VALVE_RELAY_PIN, HIGH);
  delay(10);
  digitalWrite(VALVE_RELAY_PIN, LOW);
  
  Serial.println(allGood ? "Hardware verification passed" : "Hardware issues detected");
  Serial.println("-----------------------------\n");
  
  return allGood;
}
