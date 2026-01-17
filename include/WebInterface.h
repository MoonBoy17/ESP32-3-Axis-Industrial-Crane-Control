#pragma once
#include <ESPAsyncWebServer.h>
#include "MotionController.h"

class WebInterface {
public:
    WebInterface(MotionController& controller);
    void begin();
    void loop(); // For cleanup if needed

private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    MotionController& motionCtrl;

    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
};
