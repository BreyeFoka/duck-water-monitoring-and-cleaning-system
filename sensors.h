/*
 * Sensor management for Duck Pond Controller
 * Handles turbidity and water level sensor readings
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"

// Global sensor variables
extern SystemStatus currentStatus;

void initializeSensors();
void updateSensorReadings();
int readTurbidity();
bool isWaterLevelHigh();
bool isWaterLevelLow();
bool isWaterLevelSafe();
void calibrateTurbidySensor();

#endif
