/ Include the SevSeg library
#include "SevSeg.h"

// --- Create the SevSeg object ---
SevSeg sevseg;

// --- Define Your Pins ---

// 1. Input/Output Pins
const int sensorPin = 2;  // Connects to NE555 (Pin 3) output
const int resetButton = 3;  // Connects to reset button
const int alarmPin = 4;     // Connects to the buzzer's positive leg

// 2. Display Segment Pins (a-g)
const int segA = 5;
const int segB = 6;
const int segC = 7;
const int segD = 8;
const int segE = 9;
const int segF = 10;
const int segG = 11;

// 3. Display Digit Pins (Common Cathode control)
const int digit1 = A0; // Digit 1 (Thousands)
const int digit2 = A1; // Digit 2 (Hundreds)
const int digit3 = A3; // Digit 3 (Tens)
const int digit4 = A2; // Digit 4 (Ones)

// --- Global Variables ---
int parkingCount = 0;           // This holds the current count
const int parkingLimit =10;  // This is your "full" limit
int lastSensorState = LOW;      // Used to detect a new pulse

void setup() {
  // --- Setup the 7-Segment Display ---
  byte hardwareConfig = N_TRANSISTORS;
  byte numDigits = 4;
  byte digitPins[] = {digit1, digit2, digit3, digit4};
  byte segmentPins[] = {segA, segB, segC, segD, segE, segF, segG};
  
  // Initialize the SevSeg library
  // (We use 'true' for transistors on common pins)
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, true);
  
  sevseg.setBrightness(90); // Set brightness (0-100)

  // --- Setup Input/Output Pins ---
  pinMode(sensorPin, INPUT);
  pinMode(resetButton, INPUT); // Button has an external pull-down
  pinMode(alarmPin, OUTPUT);
  digitalWrite(alarmPin, LOW); // Make sure alarm is off
}


void loop() {

  // --- 1. Check for Sensor Pulse ---
  // We use edge detection to count only ONCE per pulse
  int sensorState = digitalRead(sensorPin);
  
  if (sensorState == HIGH && lastSensorState == LOW) {
    // A new pulse just arrived!
    if (parkingCount < parkingLimit) {
      parkingCount++; // Add 1 to the count
    }
  }
  lastSensorState = sensorState; // Remember the state for next loop


  // --- 2. Check for Reset Button ---
  if (digitalRead(resetButton) == HIGH) {
    parkingCount = 0; // Reset the count
  }


  // --- 3. Check the Limit ---
  if (parkingCount >= parkingLimit) {
    parkingCount = parkingLimit;  // Stop count from going over
    digitalWrite(alarmPin, HIGH); // Sound the alarm
  } else {
    digitalWrite(alarmPin, LOW);  // Keep the alarm off
  }


  // --- 4. Update the Display ---
  // This line is required. It runs the multiplexing.
  sevseg.refreshDisplay(); 

  // We set the number to be displayed.
  // The '0' means we want 0 decimal places.
  sevseg.setNumber(parkingCount, 0); 
}
