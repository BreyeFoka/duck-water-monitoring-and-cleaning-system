/*
 * Web Interface for Duck Pond Controller
 * Provides REST API and web UI for remote monitoring and control
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>
#include "config.h"

// Web server handlers
void handleRoot();
void handleGetStatus();
void handleGetSettings();
void handleSetSettings();
void handleManualClean();
void handleStopCleaning();
void handleEnableSystem();
void handleDisableSystem();

// Utility functions
void sendJsonResponse(int code, String message);
void sendCORSHeaders();

#endif
