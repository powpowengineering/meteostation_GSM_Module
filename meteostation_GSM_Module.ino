//#include "Init.h"
#include "ModuleRAK811.h"


#define TIMEOUT_LORA_RECEIVE        (2000) // ms
#define SIZE_IN_BUFF                (512)
#define PIN_PWR_GSM					(7)

typedef enum
{
    OFF = 0,
    ON = 1,
}GSM_MODULE_state;


char inBuff[SIZE_IN_BUFF];
char tempBuff[SIZE_IN_BUFF];

/*char dataHexTest[] = {"at+recv=-20,10,71:31382E20392E32317C31373A34313A20307C74313D20204E414E7C48756D3D20204E414E7C503D313030372E327C566261743D362E31357C436E745F313D327C436E745F323D30"};
char dataAsciiTest[256];
unsigned int cntByte=0;
*/
void setup()
{
    RAK811_init();// RAK811 uses Serial2
    RAK811_confMode(RAK811_MODE_LORA_P2P);
    delay(500);
    RAK811_confP2Pprm("869525000",12,0,1,8,20);
    delay(500);
    RAK811_confTransferMode(RAK811_RECEIVER_MODE);
    delay(500);
	
    Serial.begin(115200); 
	Serial3.begin(19200); // UART for GSM module 
	
	pinMode(PIN_PWR_GSM, OUTPUT);
	
	//Switch on power for GSM module
    digitalWrite(PIN_PWR_GSM, LOW);
/*
unsigned int cntSym=0;
    for(int i=0;i<32;i++)
    {
      if (':' == dataHexTest[i])
        {
            break;  
        }
        else
        {
            cntSym++;
        }  
    }
Serial.println(cntSym);

RAK811_hexToAscii(&dataHexTest[cntSym+1],dataAsciiTest);
Serial.print(dataAsciiTest);
while(1);
 */   
	
	Serial3.println("AT");
  delay(500);
	Serial3.println("AT+CMGF=1");                        // Выбирает формат SMS
 delay(500);
	Serial3.println("AT+CMGS=\"+79296232270\"");         // Отправка СМС на указанный номер
 delay(500);
	Serial3.print("I am future GSM module for yours meteo station :)"); // Тест сообщения
 delay(500);
	Serial3.write(26);//???
 delay(500);
}

void loop()
{
  unsigned int cntSym=0;
  
  
  // if have message from meteostation
  if (RAK811_receiveData(inBuff, SIZE_IN_BUFF) > 0)
	{
		//Serial.write(Serial2.read());
		//Serial.println(">0");
  
    for(int i=0;i<32;i++)
    {
      if (':' == inBuff[i])
        {
            break;  
        }
        else
        {
            cntSym++;
        }  
    }

    //clear buf
    for (int i=0;i<SIZE_IN_BUFF;i++)
    {
        tempBuff[i] = 0;
    }
    
    RAK811_hexToAscii(&inBuff[cntSym+1],tempBuff);
    for (int i=0;i<SIZE_IN_BUFF;i++)
    {
        inBuff[cntSym+1+i]=tempBuff[i];
        if(0 == tempBuff[i])
        {
            break;
        }
    }
    
		Serial.println(inBuff);
		Serial3.println("AT+CMGS=\"+79296232270\"");         // Отправка СМС на указанный номер
		delay(500);
		Serial3.write(inBuff); // Тест сообщения
		delay(500);
	  Serial3.write(26);//???
	}
	
    
}




void GSM_MODULE_switchPwr(GSM_MODULE_state state)
{
    if (ON == state)
    {
        digitalWrite(PIN_PWR_GSM, HIGH);
    }
    else (OFF == state)
    {
        digitalWrite(PIN_PWR_GSM, LOW);
    }
}


