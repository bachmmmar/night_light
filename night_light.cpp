#include <Arduino.h>
#include "src/timer/Timer.h"
#include "src/NeoPixel/Adafruit_NeoPixel.h"


#define ONOFF_PIN   9
#define LED_PIN     12
#define POWER_PIN   4
#define NEOPIXEL_PIN 3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels
#define INITIAL_DELAY 10000

//Configuration config;
Timer timer;

bool is_on;
int current_pixel;


/* **** Logging ***** */
// uncomment define to enable logging to serial console
//#define LOG_ENABLED

void log(const char str[]) {
#ifdef LOG_ENABLED
    Serial.print(str);
    Serial.print("\n");
#endif
}


bool isButtonPressed() {
    return !static_cast<bool>(digitalRead(ONOFF_PIN));
}

void setLEDOn(void) {
    digitalWrite(LED_PIN, LOW);
}

void setLEDOff(void) {
    digitalWrite(LED_PIN, HIGH);
}

void setNeoPixelOn(void) {
    digitalWrite(POWER_PIN, HIGH);
    delay(100);
    pixels.begin();
}

void setNeoPixelOff(void) {
    digitalWrite(POWER_PIN, LOW);
    pinMode(NEOPIXEL_PIN, INPUT);
}


void turnOnAllPixels(){
    for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(50, 50, 30));
    }
    pixels.show();
}



/** Arduino setup function */
void setup() {
    #ifdef LOG_ENABLED
    Serial.begin(9600);
    #endif

    log("Start initializing inputs...");

    // Configure ON_OFF Pin as input
    pinMode(ONOFF_PIN, INPUT_PULLUP);

    // Configure
    pinMode(LED_PIN, OUTPUT);
    setLEDOn();

    // Configure Neopixel Power switch
    pinMode(POWER_PIN, OUTPUT);
    setNeoPixelOff();

    is_on = false;

}


/** Arduino loop function */
void loop() {
    timer.update();

    setLEDOn();

    if (isButtonPressed()) {
        setLEDOff();
        setNeoPixelOn();
        turnOnAllPixels();
        is_on = true;
        current_pixel = NUMPIXELS;

        delay(INITIAL_DELAY);
    }

    if (current_pixel==0) {
        is_on = false;
        setNeoPixelOff();
    }

    if (is_on) {
        delay(DELAYVAL); // Pause before next pass through loop
        current_pixel--;

        pixels.setPixelColor(current_pixel, pixels.Color(0, 0, 0));
        pixels.show();
    }
}
