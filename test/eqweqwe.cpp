#include <Arduino.h>

int data;

void setup()
{
    Serial.begin(9600);
    
}

void loop()
{
    data = Serial.read();
    delay(1000);
}