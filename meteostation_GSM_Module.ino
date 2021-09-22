//#include "Init.h"
#include "ModuleRAK811.h"


#define TIMEOUT_LORA_RECEIVE        (2000) // ms
#define SIZE_IN_BUFF                (512)
#define PIN_PWR_GSM					(7)

#define VBAT_PIN                    (A0)
#define NUM_SAMPLES 10
#define WAIT_GSM_POWER_ON           (50000)

typedef enum
{
    OFF = 0,
    ON = 1,
}GSM_MODULE_state;


char inBuff[SIZE_IN_BUFF];
char tempBuff[SIZE_IN_BUFF];

float r1 = 11;
float r2 = 30;
float vbat = 0.0;            // calculated voltage

float ReadVbat(void);
void GSM_MODULE_switchPwr(GSM_MODULE_state state);

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
	
    // Power on GSM
    GSM_MODULE_switchPwr(OFF);


    /*
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
    // Power on GSM
    GSM_MODULE_switchPwr(ON);
    */
   
}

void loop()
{
    unsigned int cntSym=0;

    // if have message from meteostation
    if (RAK811_receiveData(inBuff, SIZE_IN_BUFF) > 0)
    {
        // Power on GSM
        GSM_MODULE_switchPwr(ON);
        
        // Search symbol ':'
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

        //clear buff
        for (int i=0;i<SIZE_IN_BUFF;i++)
        {
            tempBuff[i] = 0;
        }
        
        // Convert hex to Ascii
        RAK811_hexToAscii(&inBuff[cntSym+1],tempBuff);
        
		vbat = ReadVbat();
		
        // Insert converted data into buffer
        for (int i=0;i<SIZE_IN_BUFF;i++)
        {
            inBuff[cntSym+1+i]=tempBuff[i];
            if(0 == tempBuff[i])
            {
				char str_temp[10];
                dtostrf(vbat, 4, 2, str_temp);
				snprintf(&inBuff[cntSym+1+i],15,"|VbatGSM=%s",str_temp);
				break;
            }
        }
        
        
        
        Serial.println(inBuff);
        delay(WAIT_GSM_POWER_ON);
        Serial3.println("AT");
        delay(500);
        Serial3.println("AT+CMGF=1");                        // Выбирает формат SMS
        delay(500);
        Serial3.println("AT+CMGS=\"+79296232270\"");         // Отправка СМС на указанный номер
        delay(500);
        Serial3.write(inBuff); // Тест сообщения
        delay(500);
        Serial3.write(26);//???
        delay(5000);
        
        // Power off GSM
        GSM_MODULE_switchPwr(OFF);
    }
	

	
}




void GSM_MODULE_switchPwr(GSM_MODULE_state state)
{
    if (ON == state)
    {
        digitalWrite(PIN_PWR_GSM, HIGH);
    }
    else if (OFF == state)
    {
        digitalWrite(PIN_PWR_GSM, LOW);
    }
}



float ReadVbat(void)
{
  float voltage;
  float sum = 0;
  unsigned char sample_count = 0; // current sample number

      while (sample_count < NUM_SAMPLES) {
          sum += analogRead(VBAT_PIN);
          sample_count++;
      }
      // use 3.3 for a 3.3V ADC reference voltage
      voltage = 1.02857*(sum/NUM_SAMPLES)* (3.3/1024.0)/(r1/(r1+r2));
      // send voltage for display o n Serial Monitor
      // divides by 6.24/6 is the calibrated voltage divide

      return voltage;

}//end of ReadVbat
