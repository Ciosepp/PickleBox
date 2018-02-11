/*
blink status-reference
0-1 quick strobe every 4s -- Status OK
1-1quick continuos blink(20ms) -- waterOverflow
2-2 blinks -- temp error
3-3 blinks -- unconnected soil_moisture probe
4-4 blinks -- low_Water

Here some graphs:

--- = clock_time(dc=50%)

           0     1		2     3     4     5     0  
           ---   ---   ---   ---   ---   ---   ---
	      	
clock   ---   ---   ---   ---   ---   ---   ---


           ---------------					   ---
                          						
EN(int n)--               ---------------------
[ex. n=3]

           ...   ...   ...					   ...

GPIO-Q  ...   ...   ...   .............   .....

*/


#include "errorBlinker.h"

Timeout Blink;

int c;
long status_clock_time;
int intLedPin;
bool statusFlag;



void statusBlinkInit(long inTime,int ledPin)
{
    status_clock_time = inTime*1000;
    intLedPin = ledPin;
	  pinMode(intLedPin, OUTPUT);
    Blink.clockInit(status_clock_time);
   
}

void statusBlink(int n)
{  

  if(Blink.clock_out & !statusFlag)
  { 
    if(c<6) c++;

    else c=0;
        statusFlag = true;
  }
    
    if(!Blink.clock()) statusFlag = false;
  
  digitalWrite(intLedPin , (Blink.clock() && (c+1) <= n));
    
  
}


