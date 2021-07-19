#include "JuraInterface.h"

JuraInterface::JuraInterface(/* args */)
{
}

JuraInterface::~JuraInterface()
{
}

void JuraInterface::init()
{
    juraSerial.begin(9600, SWSERIAL_8N1, JURA_RX, JURA_TX);
}

void JuraInterface::run()
{
    if (juraRUN_Enable)
    {
        sendCommand();
        juraRUN_Enable = false;
    }
    readCommand(true);
}

byte JuraInterface::sendCommand()
{
    Serial.print("outbytes: ");
    Serial.println(juraCMD);

    if (juraCMD.compareTo("AN:0A") != 0 && juraCMD.compareTo("AN:0A\r\n") != 0)
    {

        for (int i = 0; i < juraCMD.length(); i++)
        {
            for (int s = 0; s < 8; s += 2)
            {
                char rawbyte = 255;
                bitWrite(rawbyte, 2, bitRead(juraCMD.charAt(i), s + 0));
                bitWrite(rawbyte, 5, bitRead(juraCMD.charAt(i), s + 1));

                Serial.print(rawbyte, HEX);
                Serial.print(" ");
                Serial.print(rawbyte, BIN);
                Serial.print(" | ");

                juraSerial.write(rawbyte);

                delay(1);
            }
            Serial.print(juraCMD.charAt(i));
            Serial.println();
            delay(7);
        }
    }
    else
    {
        Serial.println("!!! I will not commit death !!!!");
        return 1;
    }
    return 0;
}

byte JuraInterface::readCommand(bool enableSerial)
{
    byte d0;
    byte d1;
    byte d2;
    byte d3;
    byte x0;
    byte x1;
    byte x2;
    byte x3;
    byte x4;
    while (juraSerial.available())
    {
        delay(1);
        byte d0 = juraSerial.read();
        delay(1);
        byte d1 = juraSerial.read();
        delay(1);
        byte d2 = juraSerial.read();
        delay(1);
        byte d3 = juraSerial.read();
        delay(7);

        // Print hex and bin values of received UART bytes
        if (enableSerial)
        {
            Serial.print("From Coffemaker: ");
            Serial.print(d0, HEX);
            Serial.print(" ");
            Serial.print(d1, HEX);
            Serial.print(" ");
            Serial.print(d2, HEX);
            Serial.print(" ");
            Serial.print(d3, HEX);
            Serial.print("\t");

            Serial.print(d0, BIN);
            Serial.print(" ");
            Serial.print(d1, BIN);
            Serial.print(" ");
            Serial.print(d2, BIN);
            Serial.print(" ");
            Serial.print(d3, BIN);
            Serial.print("\t");
        }

        bitWrite(x4, 0, bitRead(d0, 2));
        bitWrite(x4, 1, bitRead(d0, 5));
        bitWrite(x4, 2, bitRead(d1, 2));
        bitWrite(x4, 3, bitRead(d1, 5));
        bitWrite(x4, 4, bitRead(d2, 2));
        bitWrite(x4, 5, bitRead(d2, 5));
        bitWrite(x4, 6, bitRead(d3, 2));
        bitWrite(x4, 7, bitRead(d3, 5));
        return Serial.println(char(x4));
        //return inbytes.substring(0, inbytes.length() - 2);
    }
    return 0;
}