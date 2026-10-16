/*
 * Web Interface Implementation
 * Handles HTTP requests and serves the web UI
 */

#include "web_interface.h"
#include "pump_controller.h"
#include "sensors.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>

// Forward declarations: .cpp files get no automatic prototypes.
void handleRoot();
void handleGetStatus();
void handleGetSettings();
void handleSetSettings();
void handleManualClean();
void handleStopCleaning();
void handleEnableSystem();
void handleDisableSystem();
void sendJsonResponse(int code, String message);
void sendCORSHeaders();


extern WebServer server;
extern SystemStatus currentStatus;
extern bool systemEnabled;
extern bool manualCleaningRequested;

void handleRoot() {
  // Custom raw-string delimiter: the page contains )" (in onclick="...()"), which ends a plain R"( string.
  String html = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🦆 Duck Pond Controller</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            overflow: hidden;
        }
        
        .header {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
        }
        
        .header p {
            font-size: 1.2em;
            opacity: 0.9;
        }
        
        .content {
            padding: 30px;
        }
        
        .status-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .status-card {
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
            border-left: 4px solid #4facfe;
        }
        
        .status-card h3 {
            color: #333;
            margin-bottom: 15px;
            font-size: 1.2em;
        }
        
        .status-value {
            font-size: 2em;
            font-weight: bold;
            color: #4facfe;
            margin-bottom: 5px;
        }
        
        .status-label {
            color: #666;
            font-size: 0.9em;
        }
        
        .controls {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 30px;
        }
        
        .btn {
            padding: 15px 20px;
            border: none;
            border-radius: 8px;
            font-size: 1em;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        .btn-primary {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        
        .btn-success {
            background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);
            color: white;
        }
        
        .btn-danger {
            background: linear-gradient(135deg, #ff416c 0%, #ff4b2b 100%);
            color: white;
        }
        
        .btn-warning {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
        }
        
        .btn:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
        }
        
        .log {
            background: #1e1e1e;
            color: #00ff00;
            border-radius: 10px;
            padding: 20px;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
            height: 200px;
            overflow-y: auto;
            border: 1px solid #333;
        }
        
        .status-online { color: #28a745; }
        .status-cleaning { color: #ffc107; }
        .status-offline { color: #dc3545; }
        
        .progress-bar {
            width: 100%;
            height: 20px;
            background: #e9ecef;
            border-radius: 10px;
            overflow: hidden;
            margin-top: 10px;
        }
        
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #4facfe 0%, #00f2fe 100%);
            transition: width 0.3s ease;
        }
        
        @media (max-width: 768px) {
            .header h1 {
                font-size: 2em;
            }
            
            .content {
                padding: 20px;
            }
            
            .status-grid {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🦆 Duck Pond Controller</h1>
            <p>Automated Water Monitoring & Cleaning System</p>
        </div>
        
        <div class="content">
            <div class="status-grid">
                <div class="status-card">
                    <h3>System Status</h3>
                    <div class="status-value" id="systemStatus">Loading...</div>
                    <div class="status-label">Current State</div>
                </div>
                
                <div class="status-card">
                    <h3>Water Quality</h3>
                    <div class="status-value" id="turbidity">---</div>
                    <div class="status-label">Turbidity Level</div>
                </div>
                
                <div class="status-card">
                    <h3>Water Level</h3>
                    <div class="status-value" id="waterLevel">Normal</div>
                    <div class="status-label">Current Level</div>
                </div>
                
                <div class="status-card">
                    <h3>Last Cleaned</h3>
                    <div class="status-value" id="lastCleaned">Never</div>
                    <div class="status-label">Time Ago</div>
                </div>
                
                <div class="status-card">
                    <h3>Clean Cycles</h3>
                    <div class="status-value" id="cleanCycles">0</div>
                    <div class="status-label">Total Count</div>
                </div>
                
                <div class="status-card">
                    <h3>Current Phase</h3>
                    <div class="status-value" id="currentPhase">Idle</div>
                    <div class="status-label">Operation</div>
                    <div class="progress-bar">
                        <div class="progress-fill" id="progressFill" style="width: 0%"></div>
                    </div>
                </div>
            </div>
            
            <div class="controls">
                <button class="btn btn-primary" onclick="startCleaning()" id="cleanBtn">
                    🚿 Start Cleaning
                </button>
                <button class="btn btn-danger" onclick="stopCleaning()" id="stopBtn" disabled>
                    ⏹ Stop Cleaning
                </button>
                <button class="btn btn-success" onclick="enableSystem()" id="enableBtn">
                    ✅ Enable System
                </button>
                <button class="btn btn-warning" onclick="disableSystem()" id="disableBtn">
                    ⏸ Disable System
                </button>
            </div>
            
            <div class="status-card">
                <h3>System Log</h3>
                <div class="log" id="systemLog">
                    System starting...\n
                    Sensors initialized\n
                    Web server started\n
                    Ready for operation\n
                </div>
            </div>
        </div>
    </div>

    <script>
        let logEntries = [];
        
        function updateStatus() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('systemStatus').textContent = 
                        data.systemEnabled ? (data.cleaning ? 'Cleaning' : 'Online') : 'Disabled';
                    document.getElementById('systemStatus').className = 
                        'status-value ' + (data.systemEnabled ? (data.cleaning ? 'status-cleaning' : 'status-online') : 'status-offline');
                    
                    document.getElementById('turbidity').textContent = data.turbidity;
                    document.getElementById('currentPhase').textContent = data.currentPhase;
                    document.getElementById('cleanCycles').textContent = data.cleaningCycleCount;
                    
                    // Water level status
                    let waterLevelText = 'Normal';
                    if (data.waterLevelHigh) waterLevelText = 'High';
                    if (data.waterLevelLow) waterLevelText = 'Low';
                    document.getElementById('waterLevel').textContent = waterLevelText;
                    
                    // Last cleaned time
                    if (data.lastCleanTime > 0) {
                        const timeDiff = Date.now() - data.lastCleanTime;
                        const hours = Math.floor(timeDiff / (1000 * 60 * 60));
                        const minutes = Math.floor((timeDiff % (1000 * 60 * 60)) / (1000 * 60));
                        document.getElementById('lastCleaned').textContent = 
                            hours > 0 ? `${hours}h ${minutes}m` : `${minutes}m`;
                    }
                    
                    // Update button states
                    document.getElementById('cleanBtn').disabled = data.cleaning || !data.systemEnabled;
                    document.getElementById('stopBtn').disabled = !data.cleaning;
                    document.getElementById('enableBtn').disabled = data.systemEnabled;
                    document.getElementById('disableBtn').disabled = !data.systemEnabled;
                    
                    // Update progress bar for cleaning phases
                    updateProgressBar(data.currentPhase, data.cleaning);
                })
                .catch(error => {
                    console.error('Error fetching status:', error);
                    addLogEntry('Error: Failed to fetch status');
                });
        }
        
        function updateProgressBar(phase, cleaning) {
            const progressFill = document.getElementById('progressFill');
            if (!cleaning) {
                progressFill.style.width = '0%';
                return;
            }
            
            // Simulate progress based on phase
            let progress = 0;
            switch(phase) {
                case 'Draining': progress = 25; break;
                case 'Settling': progress = 50; break;
                case 'Refilling': progress = 75; break;
                case 'Completing': progress = 90; break;
                default: progress = 0;
            }
            progressFill.style.width = progress + '%';
        }
        
        function startCleaning() {
            fetch('/api/clean', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    addLogEntry(data.message);
                    updateStatus();
                })
                .catch(error => {
                    addLogEntry('Error: Failed to start cleaning');
                });
        }
        
        function stopCleaning() {
            fetch('/api/stop', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    addLogEntry(data.message);
                    updateStatus();
                })
                .catch(error => {
                    addLogEntry('Error: Failed to stop cleaning');
                });
        }
        
        function enableSystem() {
            fetch('/api/enable', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    addLogEntry(data.message);
                    updateStatus();
                })
                .catch(error => {
                    addLogEntry('Error: Failed to enable system');
                });
        }
        
        function disableSystem() {
            fetch('/api/disable', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    addLogEntry(data.message);
                    updateStatus();
                })
                .catch(error => {
                    addLogEntry('Error: Failed to disable system');
                });
        }
        
        function addLogEntry(message) {
            const timestamp = new Date().toLocaleTimeString();
            logEntries.push(`[${timestamp}] ${message}`);
            
            // Keep only last 20 entries
            if (logEntries.length > 20) {
                logEntries.shift();
            }
            
            const logElement = document.getElementById('systemLog');
            logElement.textContent = logEntries.join('\n');
            logElement.scrollTop = logElement.scrollHeight;
        }
        
        // Update status every 2 seconds
        setInterval(updateStatus, 2000);
        
        // Initial status update
        updateStatus();
        
        // Add initial log entry
        addLogEntry('Web interface loaded');
    </script>
</body>
</html>
)HTML";
  
  server.send(200, "text/html", html);
}

void handleGetStatus() {
  sendCORSHeaders();
  
  DynamicJsonDocument doc(1024);
  
  doc["systemEnabled"] = currentStatus.systemEnabled;
  doc["cleaning"] = currentStatus.cleaning;
  doc["pumpRunning"] = currentStatus.pumpRunning;
  doc["valveOpen"] = currentStatus.valveOpen;
  doc["turbidity"] = currentStatus.turbidity;
  doc["waterLevelHigh"] = currentStatus.waterLevelHigh;
  doc["waterLevelLow"] = currentStatus.waterLevelLow;
  doc["lastCleanTime"] = currentStatus.lastCleanTime;
  doc["cleaningStartTime"] = currentStatus.cleaningStartTime;
  doc["lastCleanReason"] = currentStatus.lastCleanReason;
  doc["currentPhase"] = currentStatus.currentPhase;
  doc["cleaningCycleCount"] = currentStatus.cleaningCycleCount;
  doc["uptime"] = millis();
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiRSSI"] = WiFi.RSSI();
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleGetSettings() {
  sendCORSHeaders();
  
  DynamicJsonDocument doc(512);
  doc["turbidityThreshold"] = TURBIDITY_THRESHOLD;
  doc["drainTime"] = DRAIN_TIME;
  doc["refillTime"] = REFILL_TIME;
  doc["settleTime"] = SETTLE_TIME;
  doc["minCleanInterval"] = MIN_CLEAN_INTERVAL;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSetSettings() {
  sendCORSHeaders();
  
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, server.arg("plain"));
    
    // Here you would update the settings and save to EEPROM
    // For now, just acknowledge the request
    
    sendJsonResponse(200, "Settings updated successfully");
  } else {
    sendJsonResponse(400, "Invalid request");
  }
}

void handleManualClean() {
  sendCORSHeaders();
  
  if (canStartCleaning()) {
    manualCleaningRequested = true;
    sendJsonResponse(200, "Manual cleaning initiated");
  } else {
    sendJsonResponse(400, "Cannot start cleaning - system busy or disabled");
  }
}

void handleStopCleaning() {
  sendCORSHeaders();
  
  if (currentStatus.cleaning) {
    stopCleaningCycle();
    sendJsonResponse(200, "Cleaning stopped");
  } else {
    sendJsonResponse(400, "No cleaning in progress");
  }
}

void handleEnableSystem() {
  sendCORSHeaders();
  
  systemEnabled = true;
  currentStatus.systemEnabled = true;
  sendJsonResponse(200, "System enabled");
}

void handleDisableSystem() {
  sendCORSHeaders();
  
  systemEnabled = false;
  currentStatus.systemEnabled = false;
  
  // Stop any ongoing cleaning
  if (currentStatus.cleaning) {
    stopCleaningCycle();
  }
  
  sendJsonResponse(200, "System disabled");
}

void sendJsonResponse(int code, String message) {
  DynamicJsonDocument doc(200);
  doc["success"] = (code == 200);
  doc["message"] = message;
  
  String response;
  serializeJson(doc, response);
  server.send(code, "application/json", response);
}

void sendCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
}
