#include<Arduino.h>


void setup ()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Hello I'm from Arduino");
    delay(500);
    String msg = Serial.readStringUntil('\r');
    Serial.println(msg);
 
}