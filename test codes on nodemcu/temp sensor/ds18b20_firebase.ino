#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

/* Sending Sensor Data to Firebase Database by CircuitDigest(www.circuitdigest.com) */

#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <OneWire.h>
#include <DallasTemperature.h>                               




#define FIREBASE_HOST "https://fourth-year-eb7e4.firebaseio.com/"                          // the project name address from firebase id
#define FIREBASE_AUTH "WJPA1uBKgwbizPgcIIBe3Qj6GdXFFDNkPttHdaxq"            // the secret key generated from firebase
#define WIFI_SSID "JTL Faiba"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "Ndetto@321"                                    //password of wifi ssid


// Data wire is plugged into digital pin 2 on the Nodemcu
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);


void setup(void) {
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  
  //delay(1000);  
                
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
  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
       Serial.println("Firebase testing");

}

void loop() { 
 Serial.println("Main code running");
sensors.requestTemperatures(); 
 Serial.println("Main code running 1");

//  //print the temperature in Celsius
 Serial.print("Temperature: ");
 Serial.print(sensors.getTempCByIndex(0));
 String fireTemp = String(sensors.getTempCByIndex(0)) + String("%");
Serial.print((char)176);//shows degrees character
 Serial.print("C  |  ");
 //Serial.println("Main code running 2");

 //print the temperature in Fahrenheit
//Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
// String fireTempFah = String((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0) + String("F");
 //Serial.print((char)176);//shows degrees character
//Serial.println("F");
 Serial.println("Main code running 3");

Firebase.pushString("/DS18B20/TemperatureC", fireTemp);                                  //setup path and send readings
 Serial.println("Main code running 4");

// Firebase.pushString("/DS18B20/TemperatureF", fireTempFah); 
      if (Firebase.failed()) 
    {
 
      Serial.print("pushing logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
   
}
