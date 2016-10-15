// Cabo 1 sem clip
// 1 5v     = WHITE   = GREEN
// 2 Clock  = BLUE    = WHITE  RED
// 3 Latch  = YELLOW  = YELLOW ORRANGE
// 4 Data   = RED     = RED    YELLOW
// 7 Ground = BROWN   = BLACK

// Cabo 2 com Clip
// 1 5v     = WHITE   = GREEN
// 2 Clock  = BLUE    = WHITE
// 3 Latch  = YELLOW  = YELLOW
// 4 Data   = RED     = RED
// 7 Ground = BROWN   = BLACK

// Button order
// B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R

#define SNES_CLOCK 2        // Red wire
#define SNES_LATCH 3        // Orange wire
#define SNES_P1_DATA  4     // Yellow wire
#define SNES_P2_DATA  6     // Yellow wire

#define LATCH_DELAY 1

#include "SPI.h"

volatile int p1_button;
volatile int p2_button;

volatile int bitCount;

volatile int p1_bit;
volatile int p2_bit;

volatile int latch_count;

#define ARRAYSIZE 12
String button[ARRAYSIZE] = { "B" ,"Y" ,"SELECT" ,"START" ,"UP" ,"DOWN" ,"LEFT" ,"RIGHT" ,"A" ,"X" ,"L" ,"R" };


String inputString = "";
unsigned char pressedButton;

void ClockNES() {
    digitalWrite(SNES_P1_DATA, bitCount == p1_bit);
    bitCount++;
}

void LatchNES() {
    if (p1_button != 0xFF) {
        p1_bit = p1_button;
        bitCount = 0;
        digitalWrite(SNES_P1_DATA, bitCount == p1_bit);
        bitCount++;
        latch_count++;
        if (latch_count > LATCH_DELAY) {
            p1_button = 0xFF;
            latch_count = 0;
        }
    }
}

void OnPress(String event, String data) {
    // B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R
    p1_button = 4;
    // for (int i =0; i< ARRAYSIZE; i++){
    //     if(data == button[i] ){
    //         p1_button = i;
    //         break;
    //     }
    // }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print(inChar);
    if (inChar == ';') {
        OnPress("any", inputString);
        inputString = "";
        Serial.println(p1_button);
        Serial.println(p1_bit);
        Serial.println("OK");
    } else {
        inputString += inChar;
    }
  }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Started SNESPad");

    inputString.reserve(200);

    pinMode(SNES_CLOCK, INPUT); // Set SNES controller clock as an input
    pinMode(SNES_LATCH, INPUT); // Set SNES controller latch as an input
    pinMode(SNES_P1_DATA, OUTPUT); // Set SNES controller data as an output

    attachInterrupt(SNES_CLOCK, ClockNES, FALLING);  // When NES clock ends, execute ClockNES
    attachInterrupt(SNES_LATCH, LatchNES, RISING);   // When NES latch fires, execure LatchNES
    latch_count = 0;
}

void loop() {
}
