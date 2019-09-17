#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//WorkShop
const char* ssid = "TECNO WX3LTE";
const char* password = "12Eric201";
//m,./@1234
WebServer server(80);

String centistring;
 String meterstring;
 String feetstring;
 float distancefeet;
 float distanceinmeters;
 String Data;
 
#define trigger 12
#define echo 13

float timeone=0,distance=0;
float first_distance=0;
// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,16,2 );

// Your final calculated height
//int height;

// Convert your height to string this way

  // Create a string Object
 //String str;

// str = String(height);

// Sending your height



String convertFloatToString(float value){ 
  // begin function

  char val[10];
  String valueAsString;
    
    // perform conversion
    dtostrf(value,2,2,val);
    
    // create string object
  valueAsString = String(val);
  
  return valueAsString;
  
} // end function


void sendHeightInCm() {
  // Sending height as a response
  server.send(200, "text/plain", Data );
 Serial.println("Sent height in centimeters, meters and feet are : ");
 Serial.println(Data);

}

void sendHeightInMeter() {
  // Sending height as a response
  server.send(200, "text/plain", meterstring);
 Serial.println("Sent height in meters is: ");
 Serial.println(meterstring);
}
void sendHeightInFeet() {
  // Sending height as a response
  server.send(200, "text/plain", feetstring);
 Serial.println("Sent height in feet is: ");
 Serial.println(feetstring);
}


// Send request fails
void requestFailed(){
    // Sending failed response
    server.send(403,"text/plain", "No height recorded");
    Serial.println("Request failed");
  }



void setup(void) {


 // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
 lcd.setCursor(0,0);
 lcd.print("Distance Meter");
 delay(2000);
 lcd.clear();
 lcd.print(" Circuit Digest");
 delay(2000);


  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/send", sendHeightInCm);
  //server.on("/send", sendHeightInMeter);
  //server.on("/send", sendHeightInFeet);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {

 lcd.clear();
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 digitalWrite(trigger,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 timeone=pulseIn(echo,HIGH);
 first_distance=timeone*340/20000;
distance =220-first_distance;
 Serial.println("Sensor Values");
 lcd.clear();
 lcd.print("Dis:");
 lcd.print(distance);
 Serial.println(distance);
 lcd.print("cm");
 lcd.setCursor(0,1);
 lcd.print("Dis:");
 
 distanceinmeters = distance/100;
 lcd.print(distanceinmeters);
 Serial.println(distanceinmeters);
 lcd.print("m");
 
 distancefeet= distanceinmeters*3.281;
 lcd.print("ft");
 //lcd.print(distancefeet);
 Serial.println(distancefeet);
// convertion from float to string
//float f = 1.23456678;

//s=convertFloatToString(f);

 centistring =convertFloatToString(distance);
 meterstring=convertFloatToString(distanceinmeters);
 feetstring=convertFloatToString(distancefeet);
 Data = centistring+","+meterstring+","+feetstring;
 
 delay(20000);

  
  server.handleClient();
}
