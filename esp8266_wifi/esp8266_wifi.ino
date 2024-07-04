#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "SSD1306Wire.h" // Include the OLED library

const char* ssid = "DREM";
const char* password = "44571948";

ESP8266WebServer server(80);

// Initialize OLED display
SSD1306Wire display(0x3c, 14, 12); // SDA pin 14, SCL pin 12

const char* htmlContent = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Send Message</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f4;
      margin: 0;
      padding: 0;
    }
    .container {
      max-width: 400px;
      margin: 50px auto;
      padding: 20px;
      background-color: #fff;
      border-radius: 5px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    label {
      display: block;
      font-weight: bold;
      margin-bottom: 5px;
    }
    input[type="text"] {
      width: 100%;
      padding: 8px;
      margin-bottom: 15px;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
    }
    button[type="submit"] {
      background-color: #4CAF50;
      color: white;
      padding: 10px 20px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    button[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <form id="messageForm">
      <label for="message">Message:</label>
      <input type="text" id="message" name="message">
      <button type="submit">Send</button>
    </form>
  </div>

  <script>
    document.getElementById('messageForm').addEventListener('submit', function(event) {
      event.preventDefault();
      var message = document.getElementById('message').value;
      sendMessage(message);
    });

    function sendMessage(message) {
      var xhr = new XMLHttpRequest();
      xhr.open('POST', '/message');
      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
      xhr.onload = function() {
        if (xhr.status === 200) {
          console.log('Message sent successfully');
        } else {
          console.error('Failed to send message');
        }
      };
      xhr.send('message=' + encodeURIComponent(message));
    }
  </script>
</body>
</html>

)";

void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

void handleMessage() {
  if (server.hasArg("message")) {
    String message = server.arg("message");
    Serial.println("Received message: " + message);
    
    // Display received message on OLED
    display.clear();
    display.drawString(0, 0, "Received message:");
    display.drawString(0, 16, message);
    display.display();
  }
  server.send(200, "text/plain", "Message received");
}

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.clear();
  display.display();

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  // Start the server
  server.on("/", handleRoot);
  server.on("/message", HTTP_POST, handleMessage);
  server.begin();
  Serial.println("Server started");

  // Print the IP address of the ESP8266
  Serial.print("Please open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser");
}

void loop() {
  server.handleClient();
}
