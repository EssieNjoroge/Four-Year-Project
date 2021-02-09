
//Temperature libraries
#include <OneWire.h>
#include <DallasTemperature.h>
//WiFi library
#include <ESP8266WiFi.h>

//DS18B20 data pin
#define ONE_WIRE_BUS 2 // D4 

//Temperature library objects' creation
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Heart rate sensor realted declarations
int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;


//Credentials
const char *ssid =  "JTL Faiba";
const char *pass =  "Ndetto@321";
const char* server = "https://fourth-year-eb7e4.firebaseio.com/";


//ESP8266 WiFi library object
WiFiClient client;
 
void setup() 
{
  //Setup baud rate for communication
  Serial.begin(115200);

}

void loop() 
{
  //Request temperature sensor's value
  Serial.println("\nRequesting parameters from DS18B20...");
  sensors.requestTemperatures();
//  Serial.println("DONE");
  int temp = sensors.getTempCByIndex(0);
    Serial.print(temp);

  //Serial.print((char)176);//shows degrees character

  //Computing BPM
  Serial.println("\nComputing BPM...");
  reading = analogRead(0);
  if(reading > UpperThreshold && IgnoreReading == false)
  {
    if(FirstPulseDetected == false)
    {
      FirstPulseTime = millis();
      FirstPulseDetected = true;
    }
    else
    {
      SecondPulseTime = millis();
      PulseInterval = SecondPulseTime - FirstPulseTime;
      FirstPulseTime = SecondPulseTime;
    }
    IgnoreReading = true;
  }
  if(reading < LowerThreshold)
  {
    IgnoreReading = false;
  }  
  BPM = (1.0/PulseInterval) * 60.0 * 10000;
  /*Serial.print(reading);
    Serial.print("\t");
    Serial.print(PulseInterval);
    Serial.print("\t");*/
  Serial.print(BPM);
  Serial.println(" BPM");

  
  
  Serial.println("Waiting for 15 second to send next pair of values...");
  delay(1000);
}
