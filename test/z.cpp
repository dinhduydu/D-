#include <SoftwareSerial.h>

#include<Arduino.h>
SoftwareSerial mySerial(5,6); // RX,TX
void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);
}

void loop ()
{
    int data = 50;
   
    if (mySerial.available()>0)
    {
        char c = mySerial.read();
        if (c=='s')
        {
            mySerial.write(data);
        }
    }
    digitalWrite(7,1);
    delay(500);
    digitalWrite(7,0);
    delay(500);
}