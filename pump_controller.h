/*
 * Pump Controller for Duck Pond Auto-Cleaning System
 * Handles the cleaning cycle: drain, settle, refill
 */

#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include "config.h"

// Cleaning phases
enum CleaningPhase {
  IDLE,
  DRAINING,
  SETTLING,
  REFILLING,
  COMPLETING
};

// Global variables
extern SystemStatus currentStatus;
extern CleaningPhase currentPhase;

void initializePumpController();
void updatePumpController();
void startCleaningCycle(String reason);
void stopCleaningCycle();
void emergencyStop();
bool canStartCleaning();
String getPhaseString(CleaningPhase phase);

#endif
