#include <Arduino.h>
#include <Bounce2.h>

// ========================
// Pins
// ========================

const uint8_t BUTTON_1_PIN = 3;
const uint8_t BUTTON_2_PIN = 0;

const uint8_t OUTPUT_1_PIN = 1;
const uint8_t OUTPUT_2_PIN = 2;


// ========================
// Bounce Objects
// ========================

Bounce button1 = Bounce();
Bounce button2 = Bounce();


// ========================
// Output States
// ========================

bool output1State = false;
bool output2State = false;


// ========================
// Setup
// ========================

void setup()
{

    // Outputs
    pinMode(OUTPUT_1_PIN, OUTPUT);
    pinMode(OUTPUT_2_PIN, OUTPUT);

    // Initial state
    digitalWrite(OUTPUT_1_PIN, LOW);
    digitalWrite(OUTPUT_2_PIN, LOW);


    // Attach buttons
    button1.attach(BUTTON_1_PIN, INPUT_PULLUP);
    button2.attach(BUTTON_2_PIN, INPUT_PULLUP);

    // Debounce interval
    button1.interval(25);
    button2.interval(25);
}


// ========================
// Loop
// ========================

void loop()
{
    // Update buttons
    button1.update();
    button2.update();


    // Button 1 pressed
    if (button1.fell())
    {
        output1State = !output1State;

        digitalWrite(
            OUTPUT_1_PIN,
            output1State ? HIGH : LOW
        );

       
    }


    // Button 2 pressed
    if (button2.fell())
    {
        output2State = !output2State;

        digitalWrite(
            OUTPUT_2_PIN,
            output2State ? HIGH : LOW
        );

       
    }
}