#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char* ssid = "Anakin,where are the younglings?";
const char* password = "formynewempire";

WiFiServer server(80);

String openWeatherMapApiKey = "57dd333c2d82cdda58e5cbae88bd2dfa";
String city = "Cluj-Napoca";
String countryCode = "RO";

int button1 = 4;
int button2 = 5;
int button3 = 15;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
JSONVar myObject;

String jsonBuffer;

void setup() {
  Serial.begin(115200);

  while(!Serial) {
    delay(100);
  }
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (!WiFi.softAP("Proiect_2024", "password")) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  if (digitalRead(button1) == LOW) {
    city = "Cluj-Napoca";
  }

  if (digitalRead(button2) == LOW) {
    city = "Bucharest";
  }

  if (digitalRead(button3) == LOW) {
    city = "Iasi";
  }

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      // the temperature from openWeatherAPI is in Kelvin
      Serial.println(double(myObject["main"]["temp"]) - 273.15);

      if (city == "Cluj-Napoca") {
        Serial.println("1");
      }
      else if (city == "Bucharest") {
        Serial.println("2");
      }
      else if (city == "Iasi") {
        Serial.println("3");
      }

      Serial.println(myObject["weather"][0]["main"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  WiFiClient client = server.accept(); 
  if (client) {                            
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();            
        if (c == '\n') {                  
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

              client.println("<!DOCTYPE html>");
              client.println("<html lang=\"en\">");
              client.println("<head>");
              client.println("    <meta charset=\"UTF-8\">");
              client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
              client.println("    <title>Weather Information</title>");
              client.println("    <style>");
              client.println("        body {");
              client.println("            font-family: Arial, sans-serif;");
              client.println("            background-color: #f4f4f4;");
              client.println("            color: #333;");
              client.println("            margin: 20px;");
              client.println("        }");
              client.println("");
              client.println("        h1 {");
              client.println("            text-align: center;");
              client.println("            color: #0066cc;");
              client.println("        }");
              client.println("");
              client.println("        .weather-info {");
              client.println("            max-width: 400px;");
              client.println("            margin: 0 auto;");
              client.println("            background-color: #fff;");
              client.println("            padding: 20px;");
              client.println("            border-radius: 8px;");
              client.println("            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);");
              client.println("        }");
              client.println("");
              client.println("        .property {");
              client.println("            display: flex;");
              client.println("            justify-content: space-between;");
              client.println("            margin-bottom: 10px;");
              client.println("        }");
              client.println("    </style>");
              client.println("</head>");
              client.println("");
              client.println("<body>");
              client.println("    <h1>Weather Information for " + city + "</h1>");
              client.println("");
              client.println("    <div class=\"weather-info\">");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Coordinates:</strong>");
              client.print("            <span>Latitude: ");
              client.print(myObject["coord"]["lat"]);
              client.print(" Longitude: ");
              client.print(myObject["coord"]["long"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Weather:</strong>");
              client.print("            <span>");
              client.print(myObject["weather"][0]["main"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Temperature:</strong>");
              client.print("            <span>");
              client.print(double(myObject["main"]["temp"]) - 273.15);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Feels Like:</strong>");
              client.print("            <span>");
              client.print(double(myObject["main"]["feels_like"]) - 273.15);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Minimum temperature:</strong>");
              client.print("            <span>");
              client.print(double(myObject["main"]["temp_min"]) - 273.15);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Maximum temperature:</strong>");
              client.print("            <span>");
              client.print(double(myObject["main"]["temp_max"]) - 273.15);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Humidity:</strong>");
              client.print("            <span>");
              client.print(myObject["main"]["humidity"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Wind speed:</strong>");
              client.print("            <span>");
              client.print(myObject["wind"]["speed"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Sunrise:</strong>");
              client.print("            <span>");
              client.print(myObject["sys"]["sunrise"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("        <div class=\"property\">");
              client.println("            <strong>Sunset:</strong>");
              client.print("            <span>");
              client.print(myObject["sys"]["sunset"]);
              client.println("</span>");
              client.println("        </div>");
              client.println("    </div>");
              client.println("</body>");
              client.println("</html>");
            client.println();
            break;
         }
      }
    }
    // close the connection:
    client.stop();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}