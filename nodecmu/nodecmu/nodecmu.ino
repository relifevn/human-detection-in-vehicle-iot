// Robo India Tutorial
// Simple code upload the tempeature and humidity data using thingspeak.com
// Hardware: NodeMCU,DHT11

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6, D5);
String data = ""; // data received from Arduino

String apiKey = "F72FD054C190F505B93F09690BA99C5B"; //  Enter your Write API key from ThingSpeak

const char *ssid = "realme C3"; // "Nhiem vi rut"; // replace with your wifi ssid and wpa2 key
const char *pass = "tomorrow"; // "272380297";

const char *server = "nhom-khkt-hiep-phuoc.herokuapp.com";
const char *vehicleId = "v1";
const char *schoolId = "school_1";

void setup()
{
  Serial.begin(115200);
  delay(10);
  s.begin(9600);
  delay(10);

  Serial.println("Connecting to " + String(ssid));
  //Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

void loop()
{
//  if (s.available() > 0)
//  {
    Serial.println("Available... Listening to Arduino");
    data = ""; // failed
    data = s.readString();
    Serial.println(data);
    if (data == "")
      return;

    Serial.println("connecting to " + String(server));

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(server, httpPort))
    {
      Serial.println("connection failed");
      return;
    }

//    String isHasPerson;
//    if (data > 0)
//    {
//      isHasPerson = "true";
//    }
//    else
//    {
//      isHasPerson = "false";
//    }

    // We now create a URI for the request
    String url = "/upload?apiKey=" + String(apiKey) + "&vehicleId=" + String(vehicleId) + "&schoolId=" + String(schoolId) + "&isHasPerson=" + String(true) + "&location=" + data;
    Serial.println("Requesting URL: " + url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + String(server) + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(500);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

    Serial.println(" Plz waiting...");
    // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
    delay(10000);
    Serial.println(" OK ! READY FOR NEXT ");
//  }
//  else
//  {
//    delay(100);
//  }
}
