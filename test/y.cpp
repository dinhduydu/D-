#include<Arduino.h>
#include<SoftwareSerial.h>
SoftwareSerial ArduinoUno (5,6);

void setup() {
  //Serial Begin at 9600 Baud
  Serial.begin(9600);
  ArduinoUno.begin(9600);
}

void loop() {

  while(ArduinoUno.available()>0)
  {
    float val = ArduinoUno.parseFloat();
    if(ArduinoUno.read() =='\n')
    {
      Serial.println(val);
      Serial.println("Arduino hello");
    }
  
  delay(500);
  }
  
}