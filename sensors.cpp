/*
 * Sensor implementation for Duck Pond Controller
 */

#include "sensors.h"
#include <Arduino.h>

// Forward declarations: .cpp files get no automatic prototypes.
void initializeSensors();
void updateSensorReadings();
int readTurbidity();
bool isWaterLevelHigh();
bool isWaterLevelLow();
bool isWaterLevelSafe();
void calibrateTurbidySensor();


// Turbidity sensor calibration values
int turbidityMin = 0;
int turbidityMax = 1023;
int turbidityReadings[10];
int turbidityIndex = 0;

void initializeSensors() {
  // Initialize turbidity readings array
  for (int i = 0; i < 10; i++) {
    turbidityReadings[i] = 0;
  }
  
  Serial.println("Sensors initialized");
}

void updateSensorReadings() {
  // Update turbidity with moving average
  currentStatus.turbidity = readTurbidity();
  
  // Update water level sensors
  currentStatus.waterLevelHigh = isWaterLevelHigh();
  currentStatus.waterLevelLow = isWaterLevelLow();
  
  // Debug output
  Serial.print("Turbidity: ");
  Serial.print(currentStatus.turbidity);
  Serial.print(", Water Level High: ");
  Serial.print(currentStatus.waterLevelHigh);
  Serial.print(", Water Level Low: ");
  Serial.println(currentStatus.waterLevelLow);
}

int readTurbidity() {
  // Read raw analog value
  int rawReading = analogRead(TURBIDITY_PIN);
  
  // Add to moving average buffer
  turbidityReadings[turbidityIndex] = rawReading;
  turbidityIndex = (turbidityIndex + 1) % 10;
  
  // Calculate moving average
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += turbidityReadings[i];
  }
  
  return sum / 10;
}

bool isWaterLevelHigh() {
  // High level sensor is normally open, closes when triggered
  return digitalRead(WATER_LEVEL_HIGH_PIN) == LOW;
}

bool isWaterLevelLow() {
  // Low level sensor is normally open, closes when triggered
  return digitalRead(WATER_LEVEL_LOW_PIN) == LOW;
}

bool isWaterLevelSafe() {
  // Water level is safe if it's not too high and not too low
  return !isWaterLevelHigh() && !isWaterLevelLow();
}

void calibrateTurbidySensor() {
  Serial.println("Starting turbidity sensor calibration...");
  Serial.println("Place sensor in clean water and wait 10 seconds");
  
  delay(10000);
  
  // Read clean water values
  int cleanSum = 0;
  for (int i = 0; i < 50; i++) {
    cleanSum += analogRead(TURBIDITY_PIN);
    delay(100);
  }
  turbidityMin = cleanSum / 50;
  
  Serial.print("Clean water reading: ");
  Serial.println(turbidityMin);
  Serial.println("Now place sensor in dirty water and wait 10 seconds");
  
  delay(10000);
  
  // Read dirty water values
  int dirtySum = 0;
  for (int i = 0; i < 50; i++) {
    dirtySum += analogRead(TURBIDITY_PIN);
    delay(100);
  }
  turbidityMax = dirtySum / 50;
  
  Serial.print("Dirty water reading: ");
  Serial.println(turbidityMax);
  Serial.println("Calibration complete!");
}
