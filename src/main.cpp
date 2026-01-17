#include <Arduino.h>
#include <WiFi.h>
#include "CraneConfig.h"
#include "MotionController.h"
#include "WebInterface.h"

// Global Objects
MotionController motionCtrl;
WebInterface webInterface(motionCtrl); // Pass reference

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize Crane Systems
    motionCtrl.begin();
    webInterface.begin();
    
    Serial.println("Crane System Ready");
}

void loop() {
    motionCtrl.loop();
    webInterface.loop();
    
    // Optional: periodic status report via WebSocket
    static unsigned long lastStatus = 0;
    if (millis() - lastStatus > 200) {
        // Report positions or state
        lastStatus = millis();
    }
}
