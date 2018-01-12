#include <stdlib.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#include <avr/dtostrf.h>
#include <LiquidCrystal.h>
//#include <eeprom.h>
//#include <WiFi.h>


#define ONE_WIRE_BUS 8
#define SSID "IOTMASTER"
#define PASS "PASSWORD"
#define IP "TCP_SERVER_IP"
#define ID "0";
OneWire  ds(8);  // data bus on pin A0
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Temperature
// I need to diffrent vars to fill with data -temperature- 
//and compare theme with dynamic temp vars, so because of it i need to have two min and two max,....
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float celsius, min, max, tmp_max = 0.0, tmp_min = 30.00;
unsigned int raw_temp;
char msg[16];
boolean conversioninprogress = false;
boolean flag = false;
byte data[12];
byte addr[8];
unsigned long tempconvmillis;
unsigned long waitforesultmillis;


int analogInput = 1; ///voltage
int value = 0;
String GET = "temp is :";


int fan_pin = 10 ; 

const int gasPin = A2;    ///Gas sensor MQ9
int sensorValue;
float sensor_volt;
float RS_gas;             // Get value of RS in a GAS
float ratio;              // Get ratio RS_GAS/RS_air



void setup() {

      lcd.begin(16, 2);
      Serial.begin(115200);
      Serial3.begin(115200);        ///wifi
      pinMode(analogInput, INPUT); ///Voltmeter
      ////////////////////////
      ////  AT COMMAND
      ////////////////////////
    
      Serial3.write("AT+RST\n");
      Serial.write("At+cwmode=1");
      String cmd = "AT+CWJAP=\"";
      cmd += SSID;
      cmd += "\",\"";
      cmd += PASS;
      cmd += "\"";
      Serial3.println(cmd);
      delay (2);
      String cmd1 = "AT+CIPSTART=1,\"TCP\",\"";
      cmd1 += IP;
      cmd1 += "\",80";
      Serial3.println(cmd1);
      Serial3.println("AT+CIPMUX=1\r\n");
      pinMode(fan_pin, OUTPUT);

}

void loop() {

        ///TEMP
        if (!conversioninprogress)
        {
              ds.reset();
              delay(1);
              ds.skip();
              delay(1);
              ds.write(0x44, 0);                    // start conversion, with parasite power off at the end
              conversioninprogress = true;
              waitforesultmillis = millis();
         }
      
        if (((millis() - waitforesultmillis) > 1000) && conversioninprogress)     // maybe 750ms is enough, maybe not we might do a ds.depower() here, but the reset will take care of it.
          
          
        {
              byte i;
              byte present = 0;
              present = ds.reset();
              delay(1);
              ds.skip();
              delay(1);
              ds.write(0xBE, 0);                    // Read Scratchpad
              delay(1);
      
            for ( i = 0; i < 9; i++) 
            {              // we need 9 bytes
                data[i] = ds.read();
            }
        
      
            raw_temp = (data[1] << 8) | data[0];      // convert the data to actual temperature
        
            unsigned char t_mask[4] = {0x7, 0x3, 0x1, 0x0};
            byte cfg = (data[4] & 0x60) >> 5;
            raw_temp &= ~t_mask[cfg];
            conversioninprogress = false;
            celsius = (float)raw_temp / 16.0;
          
        }
        tempconvmillis = millis();
  ////PMET

  ////GAS


      sensorValue  = (analogRead(gasPin)) ;
      sensor_volt = (float)sensorValue / 1024 * 5.0; /// 1024 was wrong as i found!! and was 5 !
      RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL

  ///SAG

  ///////////////////////////////////////////////////////
  
      if (Serial3.available())                       
      {
          byte b = Serial3.read();                  /// reading from wifi writing to serial  
          Serial.write(b);
  
      }
        
      if (Serial.available()) 
      {
          byte c = Serial.read();                       //// reading fron serial writing to wifi ! 
          Serial3.write(c);
      }
    
  
      if ( min < celsius  ) 
      {
          max = celsius ;
          min = max ;
      }
      else if  ( min > celsius  ) 
      {
          min = celsius ;
      }
      if ( min < max ) 
      {
          min = celsius ;
          if ( min < tmp_min ) 
          {
              tmp_min = min ;
          }
      }
      if ( max > tmp_max ) 
      {
          tmp_max = max ;
      }

    ///lcd show


  ////////////////////////////////////////////////////////////

  ////call transmitter
      char buffer[10];
      String tempC = dtostrf(celsius, 4, 4, buffer);
      updateTemp(tempC);   //Sending value to func
      rcv();
}




///receiving data and message to separating volt and temp
void updateTemp(String input) {
      String cmd = "AT+CIPSTART=\"TCP\",\"";
      cmd += IP;
      cmd += "\",80";
      Serial3.println(cmd);
      delay(10);
    
      cmd = ID;
      cmd += " .tmp   :";
      cmd += input;
      cmd += "\n";
      cmd += ID;
      cmd += " .mq9   :";
      cmd += sensorValue ;
      cmd += "\n";
      
      
      Serial3.print("AT+CIPSEND=");
      Serial3.println(cmd.length());
      delay (100);
      Serial3.print(cmd);
      
      return ;
}



void rcv(){
      
      Serial3.setTimeout(1000);
      if (Serial3.find("0,OFF"))
      {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(".:MQ sens/act:.");
            lcd.setCursor(0, 1);
            lcd.print("... FAN OFF");
            lcd.print(sensorValue);
            lcd.print(".");
            digitalWrite(fan_pin, HIGH);
            delay(500);
      }
      else
      {
    
            if (Serial3.find("0,ON"))
            {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print(".:MQ sens/act:.");
                  lcd.setCursor(0, 1);
                  lcd.print("... FAN ON ");
                  delay(500);
                  
            }
      }
    
      sprintf(msg, "MQ,mn,mx,cu %i ", sensorValue);
      lcd.setCursor(0, 0);
      lcd.print(msg);
      sprintf(msg, "%.1f  %.1f %.2f" , tmp_min, tmp_max ,celsius );
      lcd.setCursor(0, 1);
      lcd.print(msg);
    
      return ; 
}
