#include "Init.h"
#include "ModuleRAK811.h"


#define TIMEOUT_LORA_RECEIVE        (2000) // ms
#defien SIZE_IN_BUFF                (512)

uint8_t inBuff[SIZE_IN_BUFF];
uint16_t cntByte=0;

void setup()
{
    Serial.begin(9600);
    Serial3.begin(9600); // UART for GSM module 
    
}

void loop()
{
    if (Serial3.available() > 0)
    {
        char ch = Serial3.read();
        Serial.ptint(ch);
        if ('\r' == ch)
        {
            cntByte = 0;
        }
        else
        {
            if (SIZE_IN_BUFF > cntByte)
            {
                inBuff[cntByte++] = ch;
            }
            else
            {
                Serial.println("\r\nError - Array index out of bounds");
                cntByte = 0;
            }
        }
    }
    
}

