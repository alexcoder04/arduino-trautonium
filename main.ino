#include "pitches.h"

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------
#define CONFIG_PIN_TUNE_DISABLE 2
#define INPUT_PIN A0
#define OUTPUT_PIN 9
#define TONE_LENGTH 100

const int tones[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
int TONE_NUMBER = 6;

// ----------------------------------------------------------------------------
// CONFIGUREABLE CONSTS (these can be changed during setup)
// ----------------------------------------------------------------------------
int R_LOW = 575;
int R_HIGH = 645;
int R_SPAN = R_HIGH - R_LOW;
int TONE_WIDHT = R_SPAN / TONE_NUMBER;
int TOLERANCE = 5;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES
// ----------------------------------------------------------------------------
int resistance;
int frequency;
int tone_index;

// ----------------------------------------------------------------------------
// HELPER FUNCTIONS
// ----------------------------------------------------------------------------
// average int from list of ints
int average(int values[], int len){
    long int sum = 0;
    for (int i = 0 ; i < len ; i++){ sum += values[i]; }
    return  ((int) sum) / len;
}

// read the resistance 30 times and return the average
int readMeanResist() { // measure resistance
    int resistances[30];
    for (int i = 0; i < 30; i++){
        resistances[i] = analogRead(INPUT_PIN); delay(100);
    }
    tone(OUTPUT_PIN, NOTE_C3, 1000); delay(1000);
    return average(resistances, 30);
}

// set low and high resistance; you need to press high and low positions if it beeps three times
void configure(){
    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);
    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);
    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);

    R_LOW = readMeanResist();

    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);
    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);
    tone(OUTPUT_PIN, NOTE_C5, 500); delay(1000);

    R_HIGH = readMeanResist();

    R_SPAN = R_HIGH - R_LOW;
    TONE_WIDHT = R_SPAN / TONE_NUMBER;

    tone(OUTPUT_PIN, NOTE_C5, 500); delay(500);
}

// ----------------------------------------------------------------------------
// SETUP
// ----------------------------------------------------------------------------
void setup() {
    pinMode(INPUT_PIN, INPUT);
    pinMode(OUTPUT_PIN, OUTPUT);
    Serial.begin(9600);
    if (digitalRead(CONFIG_PIN_TUNE_DISABLE) == 0){
        configure();
    }
}

// ----------------------------------------------------------------------------
// LOOP
// ----------------------------------------------------------------------------
void loop() {
    resistance = analogRead(INPUT_PIN); // read resistance
    // if in range of trautonium, so not 0 and not too high
    if ((resistance > R_LOW ) && (resistance < R_HIGH)) {
        // use same tone if difference too low
        if ( abs((tone_index * TONE_WIDHT) - (resistance - R_LOW)) > TOLERANCE ) {
            tone_index = (resistance-R_LOW)/TONE_WIDHT;
        }
        // play
        tone(OUTPUT_PIN, tones[tone_index], TONE_LENGTH); 
        Serial.println(tone_index); // print what we are playing
    }
    delay(TONE_LENGTH / 2);
}

