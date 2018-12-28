#include <wiringPi.h>
#include <stdio.h>
#include "isr.h"

// the debounce time, seconds
unsigned long debounceDelay = 1000;

void debounceImpulse(void (*onImpulse)(void), unsigned int pin, bool_t *prevState, unsigned long *lastHighTime)
{
  // read the state of the switch into a local variable:
  bool_t state = digitalRead(pin);
  fprintf(stderr, "state: %d", state);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  bool_t isFirstImpulse = !(*lastHighTime);

  fprintf(stderr, "isFirstImpulse: %d", isFirstImpulse);

  if (state != *prevState || isFirstImpulse)
  {
    if (state == HIGH) {
      // reset the debouncing timer
      *lastHighTime = millis();
      fprintf(stderr, "state changed. time: %lu", *lastHighTime);
    }
  }

  if ((millis() - *lastHighTime) > debounceDelay)
  {
    // whatever the state is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    fprintf(stderr, "enough time: %lu", millis() - *lastHighTime);

    // only toggle the LED if the new button state is HIGH
    (*onImpulse)();
  }

  // save the state. Next time through the loop, it'll be the *prevState:
  *prevState = state;
}
