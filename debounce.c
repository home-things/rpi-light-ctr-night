#include "isr.h"
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

void debounceImpulse(void (*onImpulse)(void), unsigned int pin, bool_t *state, bool_t *lastButtonState, unsigned long *lastDebounceTime)
{
  // read the state of the switch into a local variable:
  bool_t reading = digitalRead(pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != *lastButtonState)
  {
    // reset the debouncing timer
    *lastDebounceTime = millis();
  }

  if ((millis() - *lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != state)
    {
      state = reading;

      // only toggle the LED if the new button state is HIGH
      if (state == HIGH) (*onImpulse)();
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  *lastButtonState = reading;
}
