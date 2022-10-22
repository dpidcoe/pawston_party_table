
#define NUM_LEDS 100

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

void fadeall() 
{
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds1[i].nscale8(250);
    leds2[i].nscale8(250);
  } 
}

void slide(CRGB input)
{
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) 
  {
    // Set the i'th led to red 
    leds1[i] = input;
    leds2[i] = input;
    // Show the leds
    FastLED.show();
    delay(10);
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) 
  {
    // Set the i'th led to red 
    leds1[i] = input;
    leds2[i] = input;
    // Show the leds
    FastLED.show();
    delay(20);
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(5);
  }
}
