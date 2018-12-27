#include "isr.h"
void debounceImpulse(void (*onImpulse)(void), unsigned int pin, bool_t *state, bool_t *lastButtonState, long *lastDebounceTime);
