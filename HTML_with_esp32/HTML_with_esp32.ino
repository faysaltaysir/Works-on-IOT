#include <WiFi.h>

const char* ssid = "DREM";     // Enter your WiFi SSID
const char* password = "44571948"; // Enter your WiFi password
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Your code here
  WiFiClient client = server.available();
  if(client){
    String request = client.readStringUntil('\r');
    int a = request.indexOf("ON");
    int b = request.indexOf("OFF");
    if(a!=-1){
      Serial.println("the light is On");
    }
    if(b!=-1){
      Serial.println("the light is OFF");
    }
    
//    Serial.println(request);
    String html = R"HTML(
                  <!DOCTYPE html>
                  <head> </head>
                  <body>
                  Hi From <b>Esp32 wroom module</b>
                  <a href="/ON"><button class='btn1'>ON</button></a>
                  <a href="/OFF"><button class='btn2'>OFF</button></a>
                   <style>
                      .btn1 {
                        border: 0px solid red;
                        background-color: red;
                        color:white;
                        width: 4rem;
                        height: 2rem;
                      }
                      .btn2 {
                        border: 0px solid red;
                        background-color: blue;
                        color:white;
                        width: 4rem;
                        height: 2rem;
                      }
                      .btn1:hover{
                        background-color:black;
                      }
                  </style>
                  </body>
                  </html>
                  )HTML";
    client.println(html);
//    Serial.println("<h1>Hi From Esp32 wroom module</h1>");
  }
}
