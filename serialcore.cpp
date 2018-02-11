#include "serialcore.h"     

void serialcoreInit()
{
	Serial.begin(9600);
    Serial.print("V1.0.3");

}

void sensorLog()
{
  Serial.print("Soil0:");
  //get soil moist
  Serial.print(" Soil1:");
  //get soil moist
  Serial.print(" AirHu:");
  Serial.print(dht.readHumidity());
  Serial.print(" AirTemp:");
  Serial.print(dht.readTemperature());
}  

char* logStrings[]={"light:", "water:", "fan:",
"heater", "AUX:"};

void log(){
  Serial.print("time:");
  Serial.print(t.hour, DEC);
  Serial.print(":");
  Serial.print(t.min, DEC);
  Serial.print(":");
  Serial.print(t.sec, DEC);
  for(int i=0;i<5;i++)
  {
  	Serial.print(logStrings[i]);
    Serial.print(" status:");
  	Serial.print(out_state[i]);

  }
  Serial.println("");
}



char serialread;

void serialComm()
{
  /*
  while(Serial.available() > 0)
  {
      serialread[Se] = Serial.read();
      Se++;
  }
  */
  if(Serial.available())
  {
    serialread = serial.read();
  }

  if(serialread == '0');//reset serial commands
  if(serialread == 'B')
  {
     bruteFout(1,0,0,0,0);
  }
  if(serialread == 'T')
  {
     setTime(20,33,00);
  }
  if(serialread == 'L')
  {
    sensorLog();
    serialread = '0';
  }
}