#ifndef JURACONNECT_H
#define JURACONNECT_H
#include <Arduino.h>
#include <SoftwareSerial.h>

// Serial PINs
#define JURA_TX D2
#define JURA_RX D1

class JuraInterface
{
private:
    SoftwareSerial juraSerial;

public:
    JuraInterface(/* args */);
    ~JuraInterface();

    void init();
    void run();
    byte sendCommand();
    byte readCommand(bool enableSerial);

    bool juraRUN_Enable = false;
    String juraCMD;
};

#endif