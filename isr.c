/*
 * isr.c:
 *	Wait for Interrupt test program - ISR method
 *
 *	How to test:
 *	  Use the SoC's pull-up and pull down resistors that are avalable
 *	on input pins. So compile & run this program (via sudo), then
 *	in another terminal:
 *		gpio mode 0 up
 *		gpio mode 0 down
 *	at which point it should trigger an interrupt. Toggle the pin
 *	up/down to generate more interrupts to test.
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <time.h>


// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

//static volatile int globalCounter [8] ;

static int hotWPin = 0; // wiringpi id
static int coldWPin = 2; // wiringpi id

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

/*
 * myInterrupt:
 *********************************************************************************
 */

void onHotIrq (void) {
  //++globalCounter [0] ;
  //digitalWrite(lightPin, digitalRead(pirSensPin));
  //printf("%d-%d-%d %d:%d:%d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  printf("onHotIrq: %d\n", digitalRead(hotWPin));

}

void onColdIrq (void) {
  //++globalCounter [0] ;
  //digitalWrite(lightPin, digitalRead(pirSensPin));
  //printf("%d-%d-%d %d:%d:%d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  printf("onColdIrq: %d\n", digitalRead(coldWPin));
}

/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  strftime(s, sizeof(s), "%c", tm);
  printf("%s\n", s);



  printf("wiringPiSetup\n");
  wiringPiSetup ();

  printf("wiringPiISR...\n");
  wiringPiISR (hotWPin, INT_EDGE_FALLING, &onHotIrq) ;
  wiringPiISR (coldWPin, INT_EDGE_FALLING, &onColdIrq) ;

  //printf (" Int on pin %d: Counter: %5d\n", pin, globalCounter [pin]) ;
	printf("waiting...\n");

	for(;;) {
		sleep(100000);
	}

  return 0;
}
