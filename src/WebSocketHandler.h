#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <WebSocketsClient.h>
#include <FastLED.h>

// Declare the function
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

#endif // WEBSOCKETHANDLER_H