#include "isr.h"

void debounceImpulse(
  void (*onImpulse)(void),
  unsigned int pin,
  bool_t *prevState,
  unsigned long *lastHighTime
);
