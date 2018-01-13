#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define SSID "IOTMASTER"
#define PASS "PASSWORD"
#define IP "TCP_SERVER_IP"
#define PORT "80";
#define ID "1";
#define ESP8266 ESP8266

const int gasPin = A2;
int sensorValue;
int i;
int fan_pin = 7 ;
int light;

float sensor_volt;
float RS_gas;             // Get value of RS in a GAS

const byte rxPin = A0;    // Wire this to Tx Pin of ESP8266
const byte txPin = A1;    // Wire this to Rx Pin of ESP8266

SoftwareSerial ESP8266 (rxPin, txPin);    // We'll use a software serial interface to connect to ESP8266
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  // rs,en,d4,d5,d6,d7


void setup() {
  
      lcd.begin(16, 2);
      Serial.begin(115200);
      ESP8266.begin(115200);
      
      delay(100);                       // Let the module self-initialize
      pinMode (fan_pin,OUTPUT);
      ESP8266.write("AT+RST\n");        //resret wifi
      ESP8266.write("AT=CWMODE=1");     //set in client mode  
      
      String cmd = "AT+CWJAP=\"";
      cmd += SSID;
      cmd += "\",\"";
      cmd += PASS;
      cmd += "\"";
      ESP8266.println(cmd);
      
      delay(100);
      String cmd1 = "AT+CIPSTART=1,\"TCP\",\"";
      cmd1 += IP;
      cmd1 += "\",80";
            
          //ESP8266.write("AT+CIPMUX=1\r\n");   //run in muplitplex connection
      delay(100);
  
}

void loop() {
    
      String tempC;
      sensorValue  = (analogRead(gasPin)) ;
      sensor_volt = (float)sensorValue / 1024 * 5.0;  /// 1024 was wrong as i found! ! when voltage was 5  !
      RS_gas = (5.0 - sensor_volt) / sensor_volt;     // omit *RL
      update_(tempC);
      recv();

}


void update_(String input) {
  
      String cmd = "AT+CIPSTART=\"TCP\",\"";
      cmd += IP;
      cmd += "\",80";
      ESP8266.println(cmd);
      delay(100);
      cmd = ID ;
      cmd += " .mq9   : ";
      cmd += sensorValue;
    
      ESP8266.print("AT+CIPSEND=");
      ESP8266.println(cmd.length());
      delay(100);
      ESP8266.print(cmd);

}

void recv() {

  ESP8266.setTimeout(1000);
  lcd.setCursor(0,0);
  lcd.print(sensorValue);

  if (ESP8266.find("1,OFF"))
    {

          digitalWrite(fan_pin,LOW);
          lcd.setCursor(0, 1);
          lcd.print("FAN OFF");
          lcd.print("-");
          lcd.print(sensorValue);

  }
  else
  {

    if (ESP8266.find("1,ON"))
    {
       
          digitalWrite(fan_pin,HIGH);
          lcd.setCursor(0, 1);
          lcd.print("FAN ON ");
          lcd.print(sensorValue);
          lcd.print("-");
          lcd.print(light);
      
    }
      
  }
  
    return;
}
