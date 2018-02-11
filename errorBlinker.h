
#ifndef errorBlink_h
#define errorBlink_h
#include "Arduino.h"
//#include <Pins.h>
#include <Timeout.h>

#define max_n_errors 6

void statusBlinkInit(long inTime,int ledPin);
void statusBlink(int n);

#endif
