#include "WebInterface.h"
#include <SPIFFS.h>

WebInterface::WebInterface(MotionController& controller) : server(API_PORT), ws("/ws"), motionCtrl(controller) {
}

void WebInterface::begin() {
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
        this->onWsEvent(server, client, type, arg, data, len);
    });
    
    server.addHandler(&ws);
    server.begin();
    Serial.println("HTTP server started");
}

void WebInterface::loop() {
    ws.cleanupClients();
}

void WebInterface::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if(type == WS_EVT_DATA){
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if(info->final && info->index == 0 && info->len == len){
            // Whole message in one packet
            if(info->opcode == WS_TEXT){
                data[len] = 0; // Null terminate
                handleWebSocketMessage(arg, data, len);
            }
        }
    }
}

void WebInterface::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    String msg = (char*)data;
    // Protocol: "CMD:VALUE"
    // e.g., "X:1", "X:-1", "X:0", "STOP:1", "HOME:1"
    
    if (msg.startsWith("STOP")) {
        motionCtrl.stopAll();
    } else if (msg.startsWith("HOME")) {
        motionCtrl.homeAll();
    } else if (msg.startsWith("RESET")) {
        motionCtrl.resetState();
    } else if (msg.startsWith("X:")) {
        int dir = msg.substring(2).toInt();
        motionCtrl.jogX(dir);
    } else if (msg.startsWith("Y:")) {
        int dir = msg.substring(2).toInt();
        motionCtrl.jogY(dir);
    } else if (msg.startsWith("Z:")) {
        int dir = msg.substring(2).toInt();
        motionCtrl.jogZ(dir);
    }
}
