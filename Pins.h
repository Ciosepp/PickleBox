/*
microcontroller: atmega328p-pu 16MHz

digital I/O  :20 [Dx]
ADV I/O		 :6  [Ax]

total GPIO   :20
D0  --  SerialRx
D1  --  SerialTx
D2  --  Air Sensor(DHT11)
D3  --  
D4  --  
D5	--  Water level min
D6	--  Error pin
D7  -- 	Soil_probe0 Enable
D8  --	Soil_probe1 Enable
D9  --  Aux channel
D10 --  Heater channel
D11 --  Fan1 channel
D12 --  Water feed
D13 --  Lights
A0  --  Soil Probe0
A1  --  Soil Probe1
A2  --  PowerMeter Vsense ?
A3  --  PowerMeter Isense ?
A4  --  I2C_SDA  
A5  --  I2C_SCL

*/
#define airSense_pin 2
#define Water_level_pin 5
#define Error_pin 6
#define Soil_Probe0_EN 7
#define Soil_Probe1_EN 8
#define ch_aux 9
#define ch_heater 10
#define ch_Fan1 11
#define ch_water 12
#define ch_light 13

#define SoilMoist0_pin   14
#define SoilMoist1_pin   15
//#define SoilTemp0_pin    16
#define PowerMeterVsense 16
#define PowerMeterIsense 17

////ADDRESSES/////////
#define ch_light 0
#define ch_water 1
#define ch_fan1 2
#define ch_Heater 3
#define ch_aux 4
#define adr_light 00
#define adr_water 10
#define adr_fan1 20
#define adr_Heater 30
#define adr_aux 40
