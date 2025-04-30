#include "WebSocketHandler.h"
#include <Arduino.h>
#include <string>

extern String receivedScore;
extern int previousScore;
extern void updateDisplay(int score_int);

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("[INFO] Connected to WebSocket server");
      break;

    case WStype_TEXT: {
      Serial.println("[DEBUG] Received raw WebSocket payload:");
      for (size_t i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
      }
      Serial.println();

      receivedScore = String((char*)payload);
      Serial.print("[INFO] Received Score: ");
      Serial.println(receivedScore);

      int score = receivedScore.toInt();
      if (score >= 0 && score <= 9999 && score != previousScore) {  
        updateDisplay(score);  
        previousScore = score;
        Serial.print("[INFO] Updated display to score: ");
        Serial.println(score);
      } else {
        Serial.println("[WARNING] Invalid score received, ignoring...");
      }
      break;
    }
    
    case WStype_PONG:
    case WStype_PING:
      break;

    case WStype_DISCONNECTED:
      Serial.println("[ERROR] Disconnected from WebSocket server!");
      break;

    default:
      Serial.print("[DEBUG] Unhandled WebSocket event type: ");
      Serial.println(type);
      break;
  }
}