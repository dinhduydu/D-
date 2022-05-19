#include<Arduino.h>
int aka = 90;
void setup()
{
    Serial.begin(115200);
    //Serial.print(char(169));
    Serial.println("Hello I'm from Arduino Uno");
    delay(2000);
}

void loop ()
{
    Serial.print("Hello I'm from Arduino");
    delay(2000);
    Serial.print("Alo ");
    delay(2000);
    aka ++;
    Serial.print(aka);
    delay(500);
}