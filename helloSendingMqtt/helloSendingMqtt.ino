void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

unsigned long Time = millis();
unsigned long timeDelay = 2000;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String data = Serial.readString();
    if(data.startsWith("#")){
      Serial.println(data.substring(1,data.length())); 
    }
  }
  if(millis()-Time>timeDelay){
    Serial.println("Hello");   
    Time = millis(); 
  }

}
