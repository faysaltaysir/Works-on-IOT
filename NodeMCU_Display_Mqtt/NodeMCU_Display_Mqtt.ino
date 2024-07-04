#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define mqtt_broker "broker.hivemq.com"

WiFiClient esp;
PubSubClient client(esp);

const char* ssid="DREM";
const char* password="44571948";
const char* dataReceiveTopic= "Taysir/Receive";
const char* dataSendTopic= "Taysir/Send";
String message="";
String data="";
int red=D5;
int green=D7;
int DHTPin = D6;                
String Temperature;
String Humidity;
unsigned long Time=millis();
unsigned long timeDelay=2000;
int pos=1;
//display
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
//#include "images.h"

SSD1306Wire display(0x3c, 14, 12);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h

#define DEMO_DURATION 3000
typedef void (*Demo)(void);
void drawFontFaceDemo();
int demoMode = 0;
int counter = 1;
Demo demos[] = {drawFontFaceDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;



void setup() {
  // put your setup code here, to run once:
  display.init();
 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  Serial.begin(115200);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(DHTPin, INPUT);
//  dht.begin();

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting");
    display.drawString(0, 10, "Connecting");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.println("Ip Address: ");
  Serial.println(WiFi.localIP());
  display.drawString(0, 10, "Connected");

  client.setServer(mqtt_broker,1883);
  while(!client.connected()){
    if(client.connect("broker.hivemq.com")){
      Serial.print("Broker Connected");
    }else{
      Serial.println("Broker not connected");
      Serial.println("Retring in 2s");
      delay(2000);
    }
  }

  client.subscribe(dataReceiveTopic);
  client.setCallback(callback);
}

void loop() {

  display.clear();
  // draw the current demo method
  demos[demoMode]();

  //message = "sdfds";
  // put your main code here, to run repeatedly:
  if(data.startsWith("ledgreen")){
//    message = data.substring(8, data.length()); 
//message = "abr"; 
     //digitalWrite(message);
     Serial.println(message);
   }
  if(data.startsWith("ledred")){
     analogWrite(red,data.substring(6, data.length()).toInt());
     //Serial.println("got2");
  }
//  Temperature = (String)dht.readTemperature();
//  Humidity = (String)dht.readHumidity();
//  
  Temperature = "34.35 deg";
  Humidity = "30%";
  
  if(millis()-Time>timeDelay && pos==1){
    String str="temperature"+Temperature;
    //Serial.println(str);
    client.publish(dataSendTopic, str.c_str());
    Time=millis();
    pos=2;
  }

  if(millis()-Time>timeDelay && pos==2){
    String str="humidity"+Humidity;
    //Serial.println(str);
    client.publish(dataSendTopic, str.c_str());
    Time=millis();
    pos=1;
  }
  display.display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
//  delay(10);
  client.loop();
  
}

void callback(char* topic, byte* payload, unsigned int length){
  String dt="";
  for(int i=0; i< length; i++){
    dt+= (char) payload[i];
  }

  data=dt;
  message = data;
  //Serial.println(data);
}



void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.setFont(ArialMT_Plain_16);
  display.drawString(25, 0, "Welcome");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, message);
//  display.setFont(ArialMT_Plain_24);
//  display.drawString(0, 26,message);
//  Serial.println(message);
}
