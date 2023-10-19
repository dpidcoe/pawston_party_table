
#define NUM_LEDS 50

extern volatile bool g_element_latches[4];
extern volatile bool g_element_lightshow;

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

void clear_latches ()
{
  for(int i = 0; i < 4; i++)
  {
    g_element_latches[i] = false;
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

void lightShow ()
{
  static uint8_t hue = 0; // rotating "base color" used by many of the patterns
  Serial.println ("doing lightshow");
  for (int i = 0; i < 250; i++)
  {
    EVERY_N_MILLISECONDS ( 20 ) { hue++; } // slowly cycle the "base color" through the rainbow
    // confetti - random colored speckles that blink in and fade smoothly
    fadeToBlackBy ( leds1, NUM_LEDS, 10);
    fadeToBlackBy ( leds2, NUM_LEDS, 10);
    int pos = random16 (NUM_LEDS);
    leds1[pos] += CHSV ( hue + random8(64), 200, 255);
    leds2[pos] += CHSV ( hue + random8(64), 200, 255);
    FastLED.show ();
    delay (20);
  }
}

//note that this will take longer as more colors are unlocked
void update_leds ()
{
  if (g_element_lightshow)
  {
    g_element_lightshow = false;
    lightShow ();
    clear_latches ();
    return;
  }
  
  bool playDefaultWhite = true;
  //add more colors to the slide as they latch
  //note that each of these blocks until the light effect finishes playing
  if (g_element_latches [0])
  {
    slide(CRGB::Red);
    playDefaultWhite = false;
  }
  if (g_element_latches [1])
  {
    slide(CRGB::Green);
    playDefaultWhite = false;
  }
  if( g_element_latches [2])
  {
    slide(CRGB::Blue);
    playDefaultWhite = false;
  }
  if (g_element_latches [3])
  {
    slide(CRGB::Yellow);
    playDefaultWhite = false;
  }
  if (playDefaultWhite)
  {
    slide(CRGB (255,225,128));
  }
  
}
