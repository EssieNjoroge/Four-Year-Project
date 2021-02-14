//#include <Firebase.h>
#include <FirebaseArduino.h>
//#include <FirebaseCloudMessaging.h>
//#include <FirebaseError.h>
//#include <FirebaseHttpClient.h>
//#include <FirebaseObject.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <TinyGPS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

HTTPClient http;

#define WIFI_SSID "strathmore" //change
#define WIFI_PASSWORD "5trathm0re" //change
#define FIREBASE_HOST "fourth-year-eb7e4.firebaseio.com"
#define FIREBASE_AUTH "WJPA1uBKgwbizPgcIIBe3Qj6GdXFFDNkPttHdaxq"

//Define variables
TinyGPS gps;  //Creates a new instance of the TinyGPS object
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


int latitude, longitude;

void setup()
{

  Serial.begin(9600);
  Serial.println("1");
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  SendSensorData();
  delay(1000);

}

//function to send sensor data
void SendSensorData() {

  // String gpsDataUrl = String("https://www.google.com/maps/@") + latitude + "," + longitude;

  String temperature_data, BPM_data;

  //get the reading from the function below and print it
  String gpsDataUrl = getGPS();
  int temp_value = getTemp();
  float BPM_value = getBPM();

  temperature_data = temp_value;
  BPM_data  = BPM_value;

  Serial.println("gps url");
  Serial.println(gpsDataUrl);


  String postData;
  //Post Data the moisture name
  postData = "GPS=" +  gpsDataUrl + "&temp=" + temperature_data + "&BPM=" + BPM_data;
  //ipaddress and the path to the php fil you created
  http.begin("http://10.51.35.224/bulk_sms/index.php");              //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpStatusCode = http.POST(postData);   //Send the request
  String responseBody = http.getString();    //Get the response payload
  Serial.print("HTTP CODE: ");
  Serial.println(httpStatusCode);//Print HTTP return code

  Serial.print("PAYLOAD : ");
  Serial.println(responseBody);    //Print request response payload

  http.end();  //Close connection

  delay(1000);  //Post Data at every 1 seconds

 /* Firebase.pushString("pulserate", BPM_data);
  if (Firebase.failed()) {
    Serial.print("sending pulserate failed:");
    Serial.println(Firebase.error());
    return;
  }*/
  Firebase.setInt("Temperature", temp_value);
  if (Firebase.failed()) {
    //Serial.print("sending Temperature failed:");
    Serial.println(Firebase.error());
    Serial.print("sending Temperature to firebase failed: 12345");
    return;
  }



  if (Firebase.failed()) {
    Serial.print("pushing /logs failed:");
    Serial.println(Firebase.error());
    return;
  }
//  Serial.print("pushed: /logs/");
//  Serial.println(name);
//  delay(1000);
}

String getGPS() {
  //Gsm.begin(9600);
  Serial.println("GPS is trying to reach");
  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);

  flat = TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
  //Gsm.print(" Longitude = ");
  Serial.print(",");
  flon = TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
  delay(1000);
  flat = -1.3067631;
  flon = 36.8168564;


  String mapUrl = String("http://maps.google.com/maps?q=loc:") + flat + "," + flon;

  return mapUrl;
}

int getTemp() {

  //Request temperature sensor's value
  Serial.println("\nRequesting parameters from DS18B20...");
  sensors.requestTemperatures();
  //  Serial.println("DONE");
  int temp = sensors.getTempCByIndex(0);
  Serial.print(temp);

  return temp;

}

float getBPM() {

  //Computing BPM
  Serial.println("\nComputing BPM...");
  reading = analogRead(0);
  if (reading > UpperThreshold && IgnoreReading == false)
  {
    if (FirstPulseDetected == false)
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
  if (reading < LowerThreshold)
  {
    IgnoreReading = false;
  }
  BPM = (1.0 / PulseInterval) * 60.0 * 10000;
  /*Serial.print(reading);
    Serial.print("\t");
    Serial.print(PulseInterval);
    Serial.print("\t");*/
  Serial.print(BPM);
  Serial.println(" BPM");

  return BPM;


}
