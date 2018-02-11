#ifndef conf_h
#define conf_h


//////////////error reporting config //////
#define clock_time 250 //ms


////////////AIR MOISTURE SENSOR////

#include <DHT.h>
#define DHTPIN airSense_pin 
#define DHTTYPE DHT11     //////maybe dht11  !!!!!
#define dht_refresh_time 1000


#endif