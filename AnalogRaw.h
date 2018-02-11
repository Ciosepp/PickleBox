#ifndef AnalogRaw1_h
#define AnalogRaw1_h
#include "Arduino.h"

#define soilSampleTime 1  //ms
float getSoilMoist0(int tMoistTime);
float getSoilMoist1(int tMoistTime);
bool probeCheck();
long soilSensor0Calib(int timeout);
bool SoilMoistOUTPUT(int lowTRG, int highTRG);

#endif
