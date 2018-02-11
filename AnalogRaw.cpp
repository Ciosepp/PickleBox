/*
----------Stand-alone unit----
(I/O 14)A0--Soil moist.0 
		D?--EN_Moist0
(I/O 15)A1--Soil moist.1
		D?--EN_Moist1
(I/O 16)A2--Soil temp.0
(I/O 17)A3--NC

--further analog values will be recorded by
ext_Adc(I2C ADC) so it will be:
0Xaa --Soil moist.0
0Xbb --Soil moist.1
0Xcc --Soil temp.0
...
^
state
|
on	    |---<--------<--|
|		| 				^
|       |				|
off.....>-------->------|
|                        
--0--------10--------20--------30
        ^				^
       on              off
function process:

Set on ENABLE_pin;
Start enable pin timer;
n sampling in t time;
Set low ENAVLE_PIN;
Acquired raw_value math
    return value
 -----end of process-----

*/ 
#include "Pins.h"
#include "TSD.h"
#include "AnalogRaw.h"
#define slopeCalibVal 4 // 4div/20ms ~ 1mv/ms
#define slopeCalibDt 20 //ms
int nSample;


bool read_Busy_flag0=0;
bool read_Busy_flag1=0;

Timeout tMoist0;
Timeout tMoist1;

int tMoistTime;//ms sampling time

long tMValBuffer0,tMValBuffer1;
float SoilMoist0Val;


float getSoilMoist0(int tMoistTime)
{
	if(!read_Busy_flag0)
	{
		digitalWrite(Soil_Probe0_EN,1);
		tMoist0.startTimer(tMoistTime);
		read_Busy_flag0 =true;
	}

	if(tMoist0.out)
	{
		digitalWrite(Soil_Probe0_EN,0);
		read_Busy_flag0 =false;
		tMoist0.out = false;
		nSample=0;
    }

	
	while(read_Busy_flag0)
	{
		tMValBuffer0 += analogRead(SoilMoist0_pin);
		nSample++;
	}
	tMValBuffer0 = tMValBuffer0/nSample;
	SoilMoist0Val = map(tMValBuffer0, 0.00 ,1023.00,0.00, 1.00);
	return SoilMoist0Val;

}
float SoilMoist1Val;
float getSoilMoist1(int tMoistTime)
{
	if(!read_Busy_flag1)
	{
		digitalWrite(Soil_Probe1_EN,1);
		tMoist0.startTimer(tMoistTime);
		read_Busy_flag1 =true;
	}

	if(tMoist0.out)
	{
		digitalWrite(Soil_Probe1_EN,0);
		read_Busy_flag1 =false;
		tMoist1.out = false;
		nSample=0;
    }

	
	while(read_Busy_flag1)
	{
		tMValBuffer0 += analogRead(SoilMoist1_pin);
		nSample++;
	}
	tMValBuffer1 = tMValBuffer1/nSample;
	SoilMoist1Val = map(tMValBuffer1, 0.00 ,1023.00,0.00,1.00);
	return SoilMoist1Val;
} 

float lowTRGcent, highTRGcent;
bool soilMoistBoolOut;

bool SoilMoistOUTPUT(int lowTRG, int highTRG)
{
	lowTRGcent = lowTRG/100;
	highTRGcent = highTRG/100;
	if((getSoilMoist0(soilSampleTime) < lowTRG) | (getSoilMoist1(soilSampleTime) < lowTRG))
	{
		soilMoistBoolOut = true;
	}
	if((getSoilMoist0(soilSampleTime) > highTRG) | (getSoilMoist1(soilSampleTime) > highTRG))
	{
		soilMoistBoolOut = false;
	}

	return soilMoistBoolOut;
}

bool probeCheck() //just init function
{
	digitalWrite(SoilMoist0_pin,1);
	digitalWrite(SoilMoist1_pin,1);
	delay(100);
	if(analogRead(SoilMoist0_pin)>1010 | analogRead(SoilMoist1_pin)>1010)
			return false;// one or both probe unconnected
	
	else
		return true; // check ok
   

}

Timeout soilCalib;
long nowTime;
long samplingCalibResult;
long soilSensor0Calib(int timeout)  // void setup function
{
	digitalWrite(SoilMoist0_pin,1);
	digitalWrite(SoilMoist1_pin,1);
	soilCalib.startTimer(timeout);

    nowTime = micros();

    while(slopeAnalyser(SoilMoist0_pin, slopeCalibDt) > slopeCalibVal){}
    return samplingCalibResult = micros() - nowTime;
    
}



