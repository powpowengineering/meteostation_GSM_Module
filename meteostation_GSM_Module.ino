//#include "Init.h"
#include "ModuleRAK811.h"


#define TIMEOUT_LORA_RECEIVE        (2000) // ms
#define SIZE_IN_BUFF                (512)
#define PIN_PWR_GSM					(7)


char inBuff[SIZE_IN_BUFF];
unsigned int cntByte=0;

void setup()
{
    RAK811_init();// RAK811 uses Serial2
    RAK811_confMode(RAK811_MODE_LORA_P2P);
    delay(1000);
    RAK811_confP2Pprm("869525000",12,0,1,8,20);
    delay(1000);
    RAK811_confTransferMode(RAK811_RECEIVER_MODE);
    delay(1000);
	
    Serial.begin(115200); 
	Serial3.begin(19200); // UART for GSM module 
	
	pinMode(PIN_PWR_GSM, OUTPUT);
	
	//Switch on power for GSM module
    digitalWrite(PIN_PWR_GSM, LOW);
	
	Serial3.println("AT");
  delay(1000);
	Serial3.println("AT+CMGF=1");                        // Выбирает формат SMS
 delay(1000);
	Serial3.println("AT+CMGS=\"+79296232270\"");         // Отправка СМС на указанный номер
 delay(1000);
	Serial3.print("I am future GSM module for yours meteo station :)"); // Тест сообщения
 delay(1000);
	Serial3.write(26);//???
 delay(1000);
}

void loop()
{
  if (RAK811_receiveData(inBuff, SIZE_IN_BUFF) > 0)
	{
		//Serial.write(Serial2.read());
		//Serial.println(">0");
		Serial.write(inBuff);
		Serial3.println("AT+CMGS=\"+79296232270\"");         // Отправка СМС на указанный номер
		delay(1000);
		Serial3.write(inBuff); // Тест сообщения
		delay(1000);
	  Serial3.write(26);//???
	}
	
    
}
