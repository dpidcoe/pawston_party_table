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
//button counters for buttons 0-4
uint8_t button_debounce_counter[4];
//how many timer cycles before we consider a button pressed
const uint8_t minimum_debounce = 3;
//global state for setting valid element colors
volatile bool g_element_latches[4];
//global for when there's a lightshow
volatile bool g_element_lightshow = false;

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
    
    //button has been released after b eing held down for at least the minimum time
    if (!buttonState && button_debounce_counter[i] > minimum_debounce)
    {
      g_element_latches[i] = true;
    }
  }
  
  update_relays ();
  
  
}

void update_relays ()
{
  int count = 0;
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(RELAY_PINS[i], g_element_latches[i]);
    count += g_element_latches[i];
  }
  
  //
  if(count >= 4)
  {
    clear_latches ();
    g_element_lightshow = true;
  }
}

void clear_latches ()
{
  for(int i = 0; i < 4; i++)
  {
    g_element_latches[i] = 0;
  }
}
