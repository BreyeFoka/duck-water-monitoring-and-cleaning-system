/*
 * Pump Controller Implementation
 * Controls the water pump and valve for cleaning cycles
 */

#include "pump_controller.h"
#include "sensors.h"
#include <Arduino.h>
#include <EEPROM.h>

// Forward declarations: .cpp files get no automatic prototypes.
void initializePumpController();
void updatePumpController();
void handleDrainingPhase(unsigned long elapsed);
void handleSettlingPhase(unsigned long elapsed);
void handleRefillingPhase(unsigned long elapsed);
void handleCompletingPhase(unsigned long elapsed);
void startCleaningCycle(String reason);
void stopCleaningCycle();
void emergencyStop();
bool canStartCleaning();
void startPump();
void stopPump();
void startRefill();
void stopRefill();
void setPhase(CleaningPhase newPhase);
String getPhaseString(CleaningPhase phase);
void soundAlert(int beeps);


// Cleaning state variables
CleaningPhase currentPhase = IDLE;
unsigned long phaseStartTime = 0;
unsigned long pumpStartTime = 0;
bool safetyShutdown = false;

void initializePumpController() {
  currentPhase = IDLE;
  currentStatus.cleaning = false;
  currentStatus.pumpRunning = false;
  currentStatus.valveOpen = false;
  currentStatus.currentPhase = "Idle";
  
  // Load cleaning count from EEPROM
  EEPROM.get(EEPROM_CLEAN_COUNT_ADDR, currentStatus.cleaningCycleCount);
  if (currentStatus.cleaningCycleCount < 0 || currentStatus.cleaningCycleCount > 10000) {
    currentStatus.cleaningCycleCount = 0;
  }
  
  Serial.println("Pump controller initialized");
}

void updatePumpController() {
  if (currentPhase == IDLE) {
    return;
  }
  
  // Safety check - emergency stop if pump runs too long
  if (currentStatus.pumpRunning && (millis() - pumpStartTime > MAX_PUMP_RUN_TIME)) {
    Serial.println("EMERGENCY: Pump running too long - shutting down");
    emergencyStop();
    return;
  }
  
  // Safety check - water level monitoring during operation
  if (currentStatus.pumpRunning && !isWaterLevelSafe() && currentPhase != DRAINING) {
    Serial.println("SAFETY: Unsafe water level detected - stopping pump");
    emergencyStop();
    return;
  }
  
  unsigned long phaseElapsed = millis() - phaseStartTime;
  
  switch (currentPhase) {
    case DRAINING:
      handleDrainingPhase(phaseElapsed);
      break;
      
    case SETTLING:
      handleSettlingPhase(phaseElapsed);
      break;
      
    case REFILLING:
      handleRefillingPhase(phaseElapsed);
      break;
      
    case COMPLETING:
      handleCompletingPhase(phaseElapsed);
      break;
      
    default:
      break;
  }
}

void handleDrainingPhase(unsigned long elapsed) {
  if (elapsed >= DRAIN_TIME || currentStatus.waterLevelLow) {
    // Draining complete
    Serial.println("Draining phase complete");
    stopPump();
    setPhase(SETTLING);
  }
}

void handleSettlingPhase(unsigned long elapsed) {
  if (elapsed >= SETTLE_TIME) {
    // Settling complete, start refilling
    Serial.println("Settling phase complete, starting refill");
    setPhase(REFILLING);
    startRefill();
  }
}

void handleRefillingPhase(unsigned long elapsed) {
  if (elapsed >= REFILL_TIME || currentStatus.waterLevelHigh) {
    // Refilling complete
    Serial.println("Refilling phase complete");
    stopRefill();
    setPhase(COMPLETING);
  }
}

void handleCompletingPhase(unsigned long elapsed) {
  if (elapsed >= 2000) { // 2 second completion delay
    // Cleaning cycle complete
    Serial.println("Cleaning cycle completed successfully");
    stopCleaningCycle();
  }
}

void startCleaningCycle(String reason) {
  if (!canStartCleaning()) {
    Serial.println("Cannot start cleaning - conditions not met");
    return;
  }
  
  Serial.println("Starting cleaning cycle: " + reason);
  
  currentStatus.cleaning = true;
  currentStatus.cleaningStartTime = millis();
  currentStatus.lastCleanReason = reason;
  safetyShutdown = false;
  
  // Start draining
  setPhase(DRAINING);
  startPump();
  
  // Alert
  soundAlert(3); // 3 beeps to indicate start
}

void stopCleaningCycle() {
  Serial.println("Stopping cleaning cycle");
  
  // Stop all equipment
  stopPump();
  stopRefill();
  
  // Update status
  currentStatus.cleaning = false;
  currentStatus.lastCleanTime = millis();
  currentStatus.cleaningCycleCount++;
  currentPhase = IDLE;
  currentStatus.currentPhase = "Idle";
  
  // Save cleaning count to EEPROM
  EEPROM.put(EEPROM_CLEAN_COUNT_ADDR, currentStatus.cleaningCycleCount);
  EEPROM.commit();
  
  // Alert
  soundAlert(2); // 2 beeps to indicate completion
}

void emergencyStop() {
  Serial.println("EMERGENCY STOP ACTIVATED");
  
  // Stop all equipment immediately
  stopPump();
  stopRefill();
  
  // Set safety shutdown flag
  safetyShutdown = true;
  
  // Update status
  currentStatus.cleaning = false;
  currentPhase = IDLE;
  currentStatus.currentPhase = "Emergency Stop";
  
  // Sound continuous alert
  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}

bool canStartCleaning() {
  // Check if system is enabled
  if (!currentStatus.systemEnabled) {
    return false;
  }
  
  // Check if already cleaning
  if (currentStatus.cleaning) {
    return false;
  }
  
  // Check minimum interval since last cleaning
  if (millis() - currentStatus.lastCleanTime < MIN_CLEAN_INTERVAL) {
    return false;
  }
  
  // Check if in safety shutdown
  if (safetyShutdown) {
    return false;
  }
  
  return true;
}

void startPump() {
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  currentStatus.pumpRunning = true;
  pumpStartTime = millis();
  Serial.println("Pump started");
}

void stopPump() {
  digitalWrite(PUMP_RELAY_PIN, LOW);
  currentStatus.pumpRunning = false;
  Serial.println("Pump stopped");
}

void startRefill() {
  digitalWrite(VALVE_RELAY_PIN, HIGH);
  currentStatus.valveOpen = true;
  Serial.println("Refill valve opened");
}

void stopRefill() {
  digitalWrite(VALVE_RELAY_PIN, LOW);
  currentStatus.valveOpen = false;
  Serial.println("Refill valve closed");
}

void setPhase(CleaningPhase newPhase) {
  currentPhase = newPhase;
  phaseStartTime = millis();
  currentStatus.currentPhase = getPhaseString(newPhase);
  Serial.println("Phase changed to: " + currentStatus.currentPhase);
}

String getPhaseString(CleaningPhase phase) {
  switch (phase) {
    case IDLE: return "Idle";
    case DRAINING: return "Draining";
    case SETTLING: return "Settling";
    case REFILLING: return "Refilling";
    case COMPLETING: return "Completing";
    default: return "Unknown";
  }
}

void soundAlert(int beeps) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    if (i < beeps - 1) {
      delay(200);
    }
  }
}
