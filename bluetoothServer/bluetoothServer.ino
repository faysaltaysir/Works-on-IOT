#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
String message = "";
int led = 23;
void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32_Server"); // Set Bluetooth device name
  Serial.println("Bluetooth device started");
}

void loop() {
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if(incomingChar != '\n'){
      message += String(incomingChar);
    }else{
      message = "";
    }
    Serial.write(incomingChar);
    if(message == "led_on"){
      digitalWrite(led,HIGH);
      Serial.println(message);
    }
    else if(message == "led_off"){
      digitalWrite(led,LOW);
      //Serial.println(message);
    }
    Serial.println();
  }
  message = "";
  if (Serial.available()) {
    char send = Serial.read();
    SerialBT.write(send);
  }
  
  delay(20);
}
