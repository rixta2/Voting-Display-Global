#include <WiFi.h>
#include <FastLED.h>
#include <string>
#include <iostream>

#ifndef WIFI_SSID
  #define WIFI_SSID "Unknown_SSID"
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "Unknown_Password"
#endif

#ifndef FACTION_NAME
  #define FACTION_NAME "Unknown"
#endif

#ifndef SERVER
  #define SERVER "Unknown"
#endif

#define BRIGHTNESS  100

// Panel-specific configurations
#if defined(PANEL_TYPE_LARGE)
  #define NUM_LEDS    97
  const int segmentLengths[7] = {14, 13, 14, 14, 14, 15, 13};
  // 7-segment LED mapping for large panel
  const int segmentMap[7][15] = {
    { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 },   // Top left 1
    { 36, 37, 38, 39, 40, 55, 56, 57, 58, 59, 60, 61, 62 },       // Top 2
    { 30, 31, 32, 33, 34, 35, 63, 64, 65, 66, 67, 68, 69, 70 },   // Top Right 3
    { 24, 25, 26, 27, 28, 29, 71, 72, 73, 74, 75, 76, 77, 78 },   // Bottom Right 4
    { 19, 20, 21, 22, 23, 79, 80, 81, 82, 83, 84, 85, 86, 87 },   // Bottom 5
    { 13, 14, 15, 16, 17, 18, 88, 89, 90, 91, 92, 93, 94, 95, 96 }, // Bottom Left 6 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }                  // Middle 7
  };
#else
  // Default to small panel
  #define NUM_LEDS    30
  const int segmentLengths[7] = {4, 4, 4, 4, 4, 4, 4};  // All segments have 4 LEDs
  const int segmentMap[7][4] = {
    { 1, 2, 3, 4 },         // Top left
    { 5, 6, 7, 8 },         // Top
    { 9, 10, 11, 12 },      // Top Right
    { 14, 15, 16, 17 },     // Bottom Right
    { 18, 19, 20, 21 },     // Bottom
    { 22, 23, 24, 25 },     // Bottom Left
    { 26, 27, 28, 29 }      // Middle
  };
#endif

#define PANEL1_PIN 33  // Thousands
#define PANEL2_PIN 32  // Hundreds
#define PANEL3_PIN 21  // Tens
#define PANEL4_PIN 22  // Units

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];

// Digit-to-segment mapping (1 = ON, 0 = OFF)
const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 0, 1, 1, 0, 0, 0}, // 1
  {0, 1, 1, 0, 1, 1, 1}, // 2
  {0, 1, 1, 1, 1, 0, 1}, // 3
  {1, 0, 1, 1, 0, 0, 1}, // 4
  {1, 1, 0, 1, 1, 0, 1}, // 5
  {1, 1, 0, 1, 1, 1, 1}, // 6
  {0, 1, 1, 1, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 1, 0, 1}  // 9
};

void displayDigit(int digit, CRGB leds[]) {
  if (digit >= 0 && digit <= 9) {
    for (int segment = 0; segment < 7; segment++) {
      for (int i = 0; i < segmentLengths[segment]; i++) {
        if (i < segmentLengths[segment]) {  // Safety check for array bounds
          leds[segmentMap[segment][i]] = numbers[digit][segment] ? CRGB::Red : CRGB::Black;
        }
      }
    }
  } else {
    // Clear all LEDs if digit is not valid
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize the panels
  FastLED.addLeds<WS2812B, PANEL1_PIN, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, PANEL2_PIN, GRB>(leds2, NUM_LEDS);
  FastLED.addLeds<WS2812B, PANEL3_PIN, GRB>(leds3, NUM_LEDS);
  FastLED.addLeds<WS2812B, PANEL4_PIN, GRB>(leds4, NUM_LEDS);
  
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // Display panel type on serial
  #if defined(PANEL_TYPE_LARGE)
  Serial.println("[INFO] Panel test initialized with LARGE panel configuration");
  #else
  Serial.println("[INFO] Panel test initialized with SMALL panel configuration");
  #endif
}

void loop() {
  for (int digit = 0; digit <= 9; digit++) {
    Serial.print("[INFO] Displaying digit: ");
    Serial.println(digit);

    // Display the digit on PANEL1
    Serial.print("[INFO] PANEL1 (Pin ");
    Serial.print(PANEL1_PIN);
    Serial.println("):");
    displayDigit(digit, leds1);
    FastLED.show();
    delay(200); // Slight delay for PANEL1

    // Display the digit on PANEL2
    Serial.print("[INFO] PANEL2 (Pin ");
    Serial.print(PANEL2_PIN);
    Serial.println("):");
    displayDigit(digit, leds2);
    FastLED.show();
    delay(200); // Slight delay for PANEL2

    // Display the digit on PANEL3
    Serial.print("[INFO] PANEL3 (Pin ");
    Serial.print(PANEL3_PIN);
    Serial.println("):");
    displayDigit(digit, leds3);
    FastLED.show();
    delay(200); // Slight delay for PANEL3

    // Display the digit on PANEL4
    Serial.print("[INFO] PANEL4 (Pin ");
    Serial.print(PANEL4_PIN);
    Serial.println("):");
    displayDigit(digit, leds4);
    FastLED.show();
    delay(5000); // Wait 5 seconds before moving to the next digit
  }
}