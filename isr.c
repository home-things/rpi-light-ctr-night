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

// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

//static volatile int globalCounter [8] ;

static unsigned int hotWPin = 0;  // wiringpi id
static unsigned int coldWPin = 2; // wiringpi id

double hot_usage, cold_usage; // m3
char __dirname[300];

/*
 * myInterrupt:
 *********************************************************************************
 */

void getTime(char *buf)
{
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  // char s[64];
  // strftime(s, sizeof(s), "%c", tm);
  // fprintf(stderr, "%s", s);
  sprintf(buf, "%d-%d-%dT%d:%d:%d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void writeUsage(double hot_usage, double cold_usage)
{
  char time_s[50];
  getTime(time_s);
  printf("%f %f %s\n", hot_usage, cold_usage, time_s);
}

void onHotImpulse(void)
{
  hot_usage += 0.01;
  fprintf(stderr, "onHotImpulse: %f\n", hot_usage);
  writeUsage(hot_usage, cold_usage);
}
void onHotIrq(void)
{
  static bool_t prevState = LOW;       // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

  char time_s[50];
  getTime(time_s);
  fprintf(stderr, " onHotIrq: %s %d\n", time_s, digitalRead(hotWPin));
  debounceImpulse(onHotImpulse, hotWPin, &prevState, &lastDebounceTime);
}

void onColdImpulse(void)
{
  cold_usage += 0.01;
  fprintf(stderr, "onColdImpulse: %f\n", cold_usage);
  writeUsage(hot_usage, cold_usage);
}

void onColdIrq(void)
{
  static bool_t prevState = LOW;       // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

  char time_s[50];
  getTime(time_s);
  fprintf(stderr, "onColdIrq: %s %d\n", time_s, digitalRead(coldWPin));
  debounceImpulse(onColdImpulse, coldWPin, &prevState, &lastDebounceTime);
}

void loadUsage(char *argv[], double *hot_usage, double *cold_usage)
{
  *hot_usage = atof(argv[1]);
  *cold_usage = atof(argv[2]);
}

/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main(int argc, char *argv[])
{
  char __path[300];
  strcpy(__path, argv[0]);
  dirname(__path);
  strcpy(__dirname, __path);
  loadUsage(argv, &hot_usage, &cold_usage);
  fprintf(stderr, "hot: %f; cold: %f\n", hot_usage, cold_usage);

  fprintf(stderr, "wiringPiSetup\n");
  wiringPiSetup();

  pullUpDnControl(coldWPin, PUD_DOWN);
  pullUpDnControl(hotWPin, PUD_DOWN);

  fprintf(stderr, "wiringPiISR...\n");
  wiringPiISR(hotWPin, INT_EDGE_FALLING, &onHotIrq);
  wiringPiISR(coldWPin, INT_EDGE_FALLING, &onColdIrq);

  //printf (" Int on pin %d: Counter: %5d\n", pin, globalCounter [pin]) ;
  fprintf(stderr, "waiting...\n");

  // nope. keep working. look to wiringPiISR that doing actual irq listening work
  for (;;) sleep(60); // seconds

  return 0;
}
