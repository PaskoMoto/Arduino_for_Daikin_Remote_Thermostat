// PaskoMoto 2019

#include "DHT.h"
#include <DYIRDaikin.h>

#define DHTPIN 10     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

DYIRDaikin irdaikin;
int isOn;
int swing;
int mode;
int fan;
int temp;
char entrada[13];
byte leido;
DHT dht(DHTPIN, DHTTYPE);
int counter;

//-- Modo de uso: power,swing,mode,fan,temp
//-- power: 0 --> OFF 1 --> ON
//-- swing: 0 --> OFF, 1 --> ON
//-- mode:  0 FAN, 1 COOL, 2 DRY, 3 HEAT,4 AUTO
//-- fan: 0~4 SPEED,5 AUTO,6 MOON
//-- temp: 18 ~ 33

void setup()
{
  counter = 12; // Tramas a filtrar: counter * 5 segs = 60 segundos. Modificar tambien en el bucle loop
	Serial.begin(115200);
	irdaikin.begin(11);
  dht.begin();
	irdaikin.off();
	irdaikin.setSwing_off();// Turn Off
	irdaikin.setMode(0);// MODE to FAN
	irdaikin.setFan(6);//FAN speed to MOON
	irdaikin.setTemp(20);//Temp to 20
	//----everything is ok and to execute send command-----
	irdaikin.sendCommand();
}

void loop() {  
  
	if (Serial.available() > 0) {

    leido = Serial.readBytesUntil('\n', entrada, 11); 
    // read string of parameters from serial stream
    if (leido >= 10) {
    isOn = entrada[0] - 48;
    swing = entrada[2] - 48;
    mode = entrada[4] - 48;
    fan = entrada[6] - 48;
    temp = ( entrada[8] - 48 ) * 10 + ( entrada[9] - 48 );

		if (isOn == 0){
		irdaikin.off();
		}else{
		irdaikin.on();
		}
    if (swing == 0){
    irdaikin.setSwing_off();
    }else{
    irdaikin.setSwing_on();
    }
    irdaikin.setMode(mode);
    irdaikin.setFan(fan);
    irdaikin.setTemp(temp);
  	irdaikin.sendCommand();
    }
   }else{
      delay(5000);
      if (counter == 0){
      counter = 12; // Tramas a filtrar: counter * 5 segs = 60 segundos.
      char h[6],t[2];
      float hum = dht.readHumidity();
      dtostrf(hum,2,0,h);
      float temp = dht.readTemperature();
      dtostrf(temp,2,0,t);
      float hic = dht.computeHeatIndex(temp, hum, false);
      Serial.print("Humedad:");
      Serial.print(h);
      Serial.print(",");
      Serial.print("Temperatura:");
      Serial.print(t);
      Serial.print(",");
      Serial.print("Indice:");
      Serial.print(hic);
      Serial.println("");
      } else {
      counter = counter - 1;
      }
   }
}
