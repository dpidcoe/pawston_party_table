#include <FastLED.h>
#include <Bounce2.h>
#include "led_effects.hpp"

#define LED_PIN 13

#define LED_ARRAY1_PIN 20
#define LED_ARRAY2_PIN 21

IntervalTimer myTimer;

//which pins go to what buttons
const int BUTTON_PINS[4] = {0, 1, 2, 3};
//which pins go to which relays
const int RELAY_PINS[4] = {5, 6, 7, 8};
//button counters for buttons 0-3
uint8_t button_debounce_counter[4];
//how many timer cycles before we consider a button pressed
const uint8_t minimum_debounce = 2;
//global state for setting valid element colors
volatile bool g_element_latches[4];
//global state for setting button edge detect
volatile int g_element_edging;
//global for when there's a lightshow
volatile bool g_element_lightshow = false;
// How long the other 3 dispensers should be off for when a color is pressed. Pouring takes about 3 seconds.
const int DISPENSER_OFF_TIME_SECS = 5;

bool LED_STATE = LOW;

String str = ""; //syntactic sugar to make printlns work

void setup ()
{
  myTimer.begin (update_buttons, 10000); //10,000 microseconds = 10 milliseconds
  
  FastLED.addLeds<WS2811, LED_ARRAY1_PIN>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2811, LED_ARRAY2_PIN>(leds2, NUM_LEDS);
  
  //button pin setup
  for(int i = 0; i < 4; i++)
  {
    //button pressed makes it go low
    pinMode (BUTTON_PINS[i], INPUT_PULLUP);
  }
  
  //relay pin setup
  for(int i = 0; i < 4; i++)
  {
    pinMode (RELAY_PINS[i], OUTPUT);
    digitalWrite (RELAY_PINS[i], LOW);
  }
  
  // LED SETUP
  pinMode (LED_PIN,OUTPUT);
  digitalWrite (LED_PIN,LED_STATE);
  
  //serial setup
  Serial.begin (115200);
  Serial.println ("Done with setup");
  
}

void loop()
{
  //do the shitty neopixel stuff here
  update_leds();
}

//callback for timer, should be on the order of ~10ms
void update_buttons ()
{
  //scan through buttons and tally count
  for (int i = 0; i < 4; i++)
  {
    int buttonState = digitalRead (BUTTON_PINS[i]);
    button_debounce_counter[i] += buttonState;
    
    //button has been released after being held down for at least the minimum time
    if (!buttonState && button_debounce_counter[i] > minimum_debounce)
    {
      g_element_edging = DISPENSER_OFF_TIME_SECS * 1000 / 10;
      g_element_latches[i] = true;
      single_color(i);
    }
  }
  
  update_relays();
}

// Turns off all dispensers except the selected one.
void single_color(int index)
{
  digitalWrite(RELAY_PINS[0], index == 0);
  digitalWrite(RELAY_PINS[1], index == 1);
  digitalWrite(RELAY_PINS[2], index == 2);
  digitalWrite(RELAY_PINS[3], index == 3);
}

void all_dispensers(void)
{
  digitalWrite(RELAY_PINS[0], HIGH);
  digitalWrite(RELAY_PINS[1], HIGH);
  digitalWrite(RELAY_PINS[2], HIGH);
  digitalWrite(RELAY_PINS[3], HIGH);
}

void update_relays ()
{
  int count = 0;
  if(g_element_edging == 0)
  {
    all_dispensers();
  }
  else
  {
    g_element_edging--;
  }
  
  if(count == 4)
  {
    g_element_lightshow = true;
  }
}
