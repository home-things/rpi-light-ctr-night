// digitalRead, wiringPiISR, pullUpDnControl, wiringPiSetup
#include <wiringPi.h>

// std
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

// ? ? ?
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

double hotUsage, coldUsage; // m3

/*
 * myInterrupt:
 *********************************************************************************
 */

void printTime(void)
{
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  strftime(s, sizeof(s), "%c", tm);
  printf("%s", s);
}

void onHotImpulse(void)
{
  hotUsage += 0.01;
  printf("onHotImpulse: %f\n", hotUsage);
}
void onHotIrq(void)
{
  static bool_t state;                       // the current reading from the input pin
  static bool_t lastButtonState = LOW;       // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

  //++globalCounter [0] ;
  //digitalWrite(lightPin, digitalRead(pirSensPin));
  printTime();
  printf(" onHotIrq: %d\n", digitalRead(hotWPin));
  debounceImpulse(onHotImpulse, hotWPin, &state, &lastButtonState, &lastDebounceTime);
}

void onColdImpulse(void)
{
  coldUsage += 0.01;
  printf("onColdImpulse: %f\n", coldUsage);
}

void onColdIrq(void)
{
  static bool_t state;                       // the current reading from the input pin
  static bool_t lastButtonState = LOW;       // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

  //++globalCounter [0] ;
  //digitalWrite(lightPin, digitalRead(pirSensPin));
  printTime();
  printf(" onColdIrq: %d\n", digitalRead(coldWPin));
  debounceImpulse(onColdImpulse, coldWPin, &state, &lastButtonState, &lastDebounceTime);
}

void loadUsage(char *__dirname, double *hotUsage, double *coldUsage)
{
  char usagePath[300];
  strcpy(usagePath, __dirname);
  strcat(usagePath, "/usage.txt");
  FILE *fp = fopen(usagePath, "r");
  if (!fp)
  {
    perror("File opening failed: usage.txt");
    exit(EXIT_FAILURE);
  }

  // int c;
  // while ((c = fgetc(fp)) != EOF) {
  //   putchar(c);
  // }
  fscanf(fp, "%lf %lf", hotUsage, coldUsage);

  if (ferror(fp))
  {
    perror("I/O error when reading");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
}

/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main(int argc, char *argv[])
{
  printf("%s\n", dirname(argv[0]));

  char __path[300];
  strcpy(__path, argv[0]);
  char *__dirname = dirname(__path);
  loadUsage(__dirname, &hotUsage, &coldUsage);

  printTime();
  printf("wiringPiSetup\n");
  wiringPiSetup();

  pullUpDnControl(coldWPin, PUD_DOWN);
  pullUpDnControl(hotWPin, PUD_DOWN);

  printf("wiringPiISR...\n");
  wiringPiISR(hotWPin, INT_EDGE_FALLING, &onHotIrq);
  wiringPiISR(coldWPin, INT_EDGE_FALLING, &onColdIrq);

  //printf (" Int on pin %d: Counter: %5d\n", pin, globalCounter [pin]) ;
  printf("waiting...\n");

  for (;;)
  {
    sleep(100000);
  }

  return 0;
}
