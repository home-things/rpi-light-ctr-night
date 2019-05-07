// argv: d_hot, d_cold
// output format: yyyy-mm-ddThh:mm:ss d_hot d_cold

// wiringPi: digitalRead, wiringPiISR, pullUpDnControl, wiringPiSetup
#include <wiringPi.h>

// std
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// sleep
#include <unistd.h>

// dirname
#include <libgen.h>

#include "isr.h"
#include "debounce.h"
#include "rest.h"

#define DURATION 25 /* minutes, how long to be light since latest movement */

// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

//static volatile int globalCounter [8] ;

static unsigned int pirS = 21;     // wiringpi id; bcm: 5
static unsigned int nightLed = 23; // wiringpi id; bcm: 13

int lastMovingTime = 0; // sec
bool isLightOn = false;
bool prevMoving = false;
unsigned long startedAt = null; // sec, since 1970 aka epoch
const unsigned HOUR = 24 * 60;  // sec
const unsigned MIN = 60;        // sec

// // get time in seconds
// unsigned getSunset()
// {
//   fprintf(stderr, "json...\n");
//   const char *headers = "X-RapidAPI-Host: sun.p.rapidapi.com\nX-RapidAPI-Key: eb17b3b315msh1feb8f4a6f34475p117f34jsnf8487dd7ab50";
//   http_get("sun.p.rapidapi.com", "/api/sun/?latitude=55.797447&longitude=37.607969&date=2019-05-27", headers);

//   // printf("json: %s\n", buffer);
//   //cJSON *json = cJSON_Parse(buffer);
//   //printf("%s", json);
//   return 42;
// }

void int_str(int i, char *s)
{
  sprintf(s, "%d", i);
}

void date_time_str(char *result_str)
{
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);
  const unsigned int hour = lt->tm_hour + 3, min = lt->tm_min;
  char hour_s[10] = "", min_s[10] = "";
  int_str(hour, hour_s), int_str(min, min_s);
  strcat(result_str, hour_s), strcat(result_str, ":"), strcat(result_str, min_s);
}

void print_debug(const char *str)
{
  char buf[2048] = "";
  char date_time[40] = "";
  date_time_str(date_time);

  strcat(buf, date_time);
  strcat(buf, ": ");
  strcat(buf, str);
  fprintf(stderr, buf);
}

bool hasMoving(void)
{
  return digitalRead(pirS);
}

time_t seconds()
{
  return time(NULL);
}

bool toggleLight(bool isOn)
{
  if (isLightOn == isOn)
    return isOn;
  fprintf(stderr, "effective toggle light. current: %d / request: %d\n", isLightOn, isOn);
  system("mpg321 ./beep.mp3");
  digitalWrite(nightLed, isOn);
  isLightOn = isOn;
  return isLightOn;
}

void onMove(void)
{
  print_debug("> moving <\n");
  toggleLight(HIGH);
  lastMovingTime = seconds();
}
void checkDelay(void)
{
  bool shouldBeLight = seconds() - lastMovingTime <= DURATION * MIN;
  fprintf(stderr, "check: seconds: %ld / diff: %ld\n", seconds(), seconds() - lastMovingTime);
  if (!shouldBeLight)
    print_debug("moving timeout --> turn light off\n");
  toggleLight(shouldBeLight);
}

void setupPins()
{
  //pinMode(pirS, INPUT);
  //pinMode(nightLed, OUTPUT);
  //pullUpDnControl(pirS, PUD_DOWN); // out

  print_debug("wiringPiSetup\n");
  wiringPiSetup();

  print_debug("wiringPiISR...\n");
  wiringPiISR(pirS, INT_EDGE_RISING, &onMove); // in

  isLightOn = digitalRead(nightLed);
  print_debug(isLightOn ? "init: light is on\n" : "init: light is off\n");
}

// // @returns epoch secs
// unsigned long getTimestamp () {
//   return startedAt + seconds();
// }

/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL); // disable buffering. write logs immediately for best reliability
  setbuf(stderr, NULL); // disable buffering. write logs immediately for best reliability

  setupPins();

  //printf (" Int on pin %d: Counter: %5d\n", pin, globalCounter [pin]) ;
  print_debug("waiting...\n");

  // nope. keep working. look to wiringPiISR that doing actual irq listening work
  for (;;)
  {
    checkDelay();
    sleep(15); // seconds
  }

  return 0;
}
