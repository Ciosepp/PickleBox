#include <DS3231.h>

//#include <liquidCrystal.h>
#include <Wire.h>
#include "DS3231.h"
#include <EEPROM.h>
#include <Timeout.h>

#include "Pins.h"
#include "conf.h"
#include "errorBlinker.h"
#include "AnalogRaw.h"
#include "serialcore.h"
//#define rtc_address 0x68
DS3231 rtc(A4, A5);
DHT dht(airSense_pin, DHTTYPE);

Time t;
Timeout t2;
Timeout water_Safety_timer;
Timeout dht_refresh;
int water_Safety_time = 20; //s

bool logFlag;

bool debug = false;

int error_flag = 0;


int ch_pins[5] = {13, //light
                  12, //water
                  11, //fan1
                  10, //Heater
                  9 //aux
                 }; //    !!!!set pins!!!!!
bool out_state[5];


bool flag1, flag_water;
///////////////////////////LIGHTS`//////////
int light_on_hour = 16;
int light_on_minute = 30;
int light_off_hour = 10;
int light_off_minute = 30;
bool lightEnable = true;
//////////////////////////WATER/////////////
int water_on_hour = 20;
int water_on_minute = 35;
int waterTime = 6; //seconds
bool waterFlag = true;
bool waterEnable = true;
bool timed_water = false;
float min_Soil_Moisture = 0.20;
float max_Soil_Moisture = 0.20;
//////////////////////////FAN1/////////////
int min_Air_Moist = 40;
int max_Air_Moist = 70;
//////////////////////////HEATER//////////////
float max_Temp = 25.00;
float min_Temp = 18.00;
float err_temp = 35.00;
/////////////////////////AUX////////////////
int aux_on_hour = 00;
int aux_on_minute = 00;
int aux_off_hour = 00;
int aux_off_minute = 01;
bool auxEnable = false;
////////////////////////ch is disabled
//int lightValue[]={light_on_hour,light_on_minute,light_off_hour,light_off_minute};
//////timer2//////
long unsigned timerT2 = 0;
bool outT2;

//char serialread;


void setup()
{
  serialcoreInit();
  //Serial.begin(9600);
  //Serial.print("V1.0.3");
  rtc.begin();
  dht.begin();
  statusBlinkInit(clock_time, Error_pin);
  //out_init();
  for (int i = 9; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 0; i < 5; i++) //5 ch
  {
    out_state[i] = EEPROM.read(i * 10);
  }
  dht_refresh.startTimer(dht_refresh_time);

  if (!timed_water && waterEnable)
  {

    while (!probeCheck())
    {
      statusBlink(3);
      Serial.println("soilProbe 0 and/or 1 unconnected!!");
    }
  }

}

void loop()
{
  if (logFlag) {
    log();
    logFlag = false;
  }
  serialComm();
  statusBlink(error_flag);

  t = rtc.getTime();

  logic();
  setoutput();

  if (dht_refresh.out)
  {
    dht_refresh.startTimer(dht_refresh_time);
    float air_moist_read = dht.readHumidity();
    float air_temp_read = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan( air_moist_read) || isnan(air_temp_read)) {
      Serial.println("Failed to read from DHT sensor!");
      error_flag = 3; // unconnected sensor
    }

  }
}

void logic(void)
{
  if (lightEnable & t.min == light_on_minute & t.hour == light_on_hour) {
    out_state[ch_light] = 1;
    EEPROM.write(adr_light, 1);
    logFlag = true;
  }

  if (t.min == light_off_minute & t.hour == light_off_hour) {
    out_state[ch_light] = 0;
    EEPROM.write(adr_light, 0);
    logFlag = true;
  }

  ////////////////////////// WATER //////////////////////////////////////
  if (timed_water)
  {

    if (waterEnable & t.min == water_on_minute & t.hour == water_on_hour & waterFlag)
    {
      t2.startTimer(waterTime * 1000);
      waterFlag = false;
    }
    if (!(waterFlag & t.min == water_on_minute & t.hour == water_on_hour))
    {
      waterFlag = true;
    }

    if (!t2.isFinish())
    {
      out_state[ch_water] = 1;
      EEPROM.write(adr_water, 1);
      logFlag = true;
    }
    else
    {
      out_state[ch_water] = 0;
      EEPROM.write(adr_water, 0);
      logFlag = true;
    }
  }
  else
  {
    if (waterEnable && ( (getSoilMoist0(soilSampleTime) < min_Soil_Moisture) || (getSoilMoist1(soilSampleTime) < min_Soil_Moisture) ))
    {
      out_state[ch_water] = 1;
      EEPROM.write(adr_water, 1);
      logFlag = true;
      bool waterInterlock = false;
      if (!waterInterlock)
      {
        water_Safety_timer.startTimer(water_Safety_time);
        waterInterlock = true;
      }
    }
    if ( ((getSoilMoist0(soilSampleTime) > max_Soil_Moisture) || (getSoilMoist1(soilSampleTime) > max_Soil_Moisture) || water_Safety_timer.out))

      out_state[ch_water] = 0;
    EEPROM.write(adr_water, 0);
    logFlag = true;
    water_Safety_timer.out = false;
  }
  if (water_Safety_timer.out) //            !!!!maybe timer will force it? !!!!!
  {
    Serial.println("water overflow allert!!");
    error_flag = 1; //water overflow
    out_state[ch_water] = 0;
    EEPROM.write(adr_water, 0);
    logFlag = true;
  }





//////////////////////////  timedFAN1 ////////////////////////////////////////
/*if(fan1Enable & t.min==fan1_on_minute & t.hour==fan1_on_hour){
   out_state[ch_fan1]=1;
   EEPROM.write(adr_fan1,1);
   logFlag=true;
  }

  if(t.min==fan1_off_minute & t.hour==fan1_off_hour){
   out_state[ch_fan1]=0;
   EEPROM.write(adr_fan1,0);
   logFlag=true;
  }
*/
////////////////////////// Sensor FAN1 ////////////////////////////////////////


if (air_moist_read > max_Air_Moist)
{
  out_state[ch_fan1] = 1;
  EEPROM.write(adr_fan1, 1);
  logFlag = true;
}

if (air_moist_read < min_Air_Moist)
{
  out_state[ch_fan1] = 0;
  EEPROM.write(adr_fan1, 0);
  logFlag = true;

}
//////////////////////////  HEATER ////////////////////////////////////////
if (air_temp_read < min_Temp)
{
  out_state[ch_Heater] = 1;
  EEPROM.write(adr_Heater, 1);
  logFlag = true;
}

if (air_temp_read > max_Temp)
{
  out_state[ch_Heater] = 0;
  EEPROM.write(adr_Heater, 0);
  logFlag = true;
}
}
void setoutput()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(ch_pins[i], out_state[i]);
  }
}


////////////////////////////////////////////////////////
void setTime(int hh, int mm, int ss)
{
  Serial.print("Time Setting:");
  rtc.setTime(hh, mm, ss);
  Serial.print(hh);
  Serial.print(mm);
  Serial.println(ss);

}


/*
  void out_init() 	//restores out states form eeprom(state setting in logic())
  {

  for(int i=9;i<14;i++)
    {
      pinMode(i,OUTPUT);
    }
  for(int i=0;i<5;i++)  //5 ch
    {
      out_state[i]=EEPROM.read(i*10);
    }

  }

*/
int a, b, c, d, e;
void bruteFout(int a, int b, int c, int d, int e)
{
  EEPROM.write(adr_light, a);
  EEPROM.write(adr_water, b);
  EEPROM.write(adr_fan1, c);
  EEPROM.write(adr_Heater, d);
  EEPROM.write(adr_aux, e);
}













