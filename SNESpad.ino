// Cabo 1 sem clip
// 1 5v     = WHITE   = GREEN
// 2 Clock  = BLUE    = WHITE
// 3 Latch  = YELLOW  = YELLOW
// 4 Data   = RED     = RED
// 7 Ground = BROWN   = BLACK

// Cabo 2 com Clip
// 1 5v     = WHITE   = GREEN
// 2 Clock  = BLUE    = WHITE
// 3 Latch  = YELLOW  = YELLOW
// 4 Data   = RED     = RED
// 7 Ground = BROWN   = BLACK

// Button order
// B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R

#define SNES_CLOCK 12                                   // Red wire

#define SNES_P1_LATCH 10                                // Orange wire
#define SNES_P1_DATA  8                                 // Yellow wire

#define SNES_P2_LATCH 10                                // Orange wire
#define SNES_P2_DATA  8                                 // Yellow wire


#include "SPI.h"

volatile unsigned char latchedByte;              // Controller press byte value = one letter in tweet
volatile unsigned char bitCount;                    // A single LDA $4017 (get one bit from "controller press")
volatile unsigned char byteCount;                   // How many bytes have already been printed
volatile unsigned char bytesToTransfer;             // How many bytes are left to print
String inputString = "";
unsigned char pressedButton;

void ClockNES() {
    digitalWrite(SNES_P1_DATA, latchedByte & 0x01);
    latchedByte >>= 1;
    // bitCount++;
}

void LatchNES() {
    if (pressedButton != 0xFFFF) {
        latchedByte = pressedButton;
        digitalWrite(SNES_P1_DATA, latchedByte & 0x01);
        latchedByte >>=1;
        // bitCount = 0;
        pressedButton = 0xFFFF;
    }
}

void OnPress(String event, String data) {
    // B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R
    if (data == "B") {
        pressedButton = B10000000; // pow(2, 0)
    } else if (data == "Y") {
        pressedButton = pow(2, 1);
    } else if ( data == "SELECT" ) {
        pressedButton = pow(2, 2);
    } else if ( data == "START" ) {
        pressedButton = pow(2, 3);
    } else if ( data == "UP" ) {
        pressedButton = pow(2, 4);
    } else if ( data == "DOWN" ) {
        pressedButton = pow(2, 5);
    } else if ( data == "LEFT" ) {
        pressedButton = pow(2, 6);
    } else if ( data == "RIGHT" ) {
        pressedButton = pow(2, 7);
    }
    //  else if ( data == "A" ) {
    //     pressedButton = pow(2, 8);
    // }
    // else if ( data == "X" ) {
    //     pressedButton = pow(2, 9);
    // } else if ( data == "L" ) {
    //     pressedButton = pow(2, 10);
    // } else if ( data == "R" ) {
    //     pressedButton = pow(2, 11);
    // }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '8') {
       OnPress("any", inputString);
       inputString = "";
       Serial.println("OK");
    }
  }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Started SNESPad");

    inputString.reserve(200);

    pinMode(SNES_CLOCK, INPUT); // Set SNES controller clock as an input
    pinMode(SNES_P1_LATCH, INPUT); // Set SNES controller latch as an input
    pinMode(SNES_P1_DATA, OUTPUT); // Set SNES controller data as an output

    attachInterrupt(SNES_CLOCK, ClockNES, FALLING);  // When NES clock ends, execute ClockNES
    attachInterrupt(SNES_P1_LATCH, LatchNES, RISING);   // When NES latch fires, execure LatchNES

    byteCount = 0;                                  // Initialize byteCount at zero, no letters printed to screen
    bytesToTransfer = 0;                            // Initialize bytesToTransfer at zero, no letters waiting to print to screen

    pressedButton = 0xFFFF;

    OnPress("any", "B");
}

void loop() {
}
