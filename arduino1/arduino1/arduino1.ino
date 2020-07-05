
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int ledPin = 13;    // chọn chân 13 báo hiệu LED
int switchPin = 12;  
int inputPin = 3;   // chọn ngõ tín hiệu vào cho PIR
int pirState = LOW; // Bắt đầu với không có báo động
int val = 0;
int pinSpeaker = 10; //chọn chân cho chuông khi có đột nhập
int switchValue = 0;

String data = ""; // data sent to nodemcu


int RXPin = 8;
int TXPin = 9;

int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial (RXPin,TXPin);

int count = 0;
int MAX_COUNT = 200;

SoftwareSerial s(6, 7);

void setup()
{
    Serial.begin(9600);
    delay(10);
    s.begin(9600);
    delay(10);
    pinMode(ledPin, OUTPUT);
    pinMode(inputPin, INPUT);
    pinMode(switchPin, INPUT);
    pinMode(pinSpeaker, OUTPUT);
    digitalWrite(pinSpeaker, HIGH);
    digitalWrite(ledPin, LOW);

      gpsSerial.begin(GPSBaud);
}

void loop()
{
    val = digitalRead(inputPin); // đọc giá trị đầu vào.
    switchValue = digitalRead(switchPin);
    
    // get GPS location
    while (gpsSerial.available() > 0)
    {
      if (gps.encode(gpsSerial.read()))
      {
        if (gps.location.isValid())
        {
          Serial.print("Latitude: ");
          Serial.println(gps.location.lat(), 6);
          Serial.print("Longitude: ");
          Serial.println(gps.location.lng(), 6);
          data = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
        }
      }  
    }

    Serial.println(String(switchValue));
     
    if (val == HIGH && switchValue == 1)             // nếu giá trị ở mức cao.(1)
    {
        digitalWrite(ledPin, HIGH); // LED On
        playTone(300, 160);         // thời gian chuông kêu
        delay(150);

        if (pirState == LOW)
        {
            Serial.println("\nMotion detected!");
            pirState = HIGH;           
            
            // Send to NodeMCU

            if (data.length() == 0) {
              // not initial GPS location
              Serial.println("Not initial GPS location!" + data);
            }else {
              String sentData = "1," + data;
              char* buf = (char*) malloc(sizeof(char)*sentData.length()+1);
              sentData.toCharArray(buf,sentData.length()+1);
              s.write(buf);
              
              Serial.println("Sent!" + String(buf));
              free(buf);  
            }
            
        }
    }
    else
    {
        digitalWrite(ledPin, LOW);
        playTone(0, 0);
        delay(300);
        if (pirState == HIGH)
        {
            Serial.println("Motion ended!");
            pirState = LOW;
        }
    }
}

void playTone(long duration, int freq)
{
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration)
    {
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, HIGH);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
