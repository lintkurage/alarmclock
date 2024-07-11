#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time.h"


const char* ssid = "----";
const char* password = "----";

const char* ntpServer = "pool.ntp.org";
const long gmtoffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;

int dataPin = 15;
int latchPin = 2;
int clockPin = 4;
int digitalPins[] = { 12, 14, 26, 27 };
int segarray[4] = { 0, 0, 0, 0 };


byte digitCodes[] = {
  0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
  0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtoffset_sec, daylightOffset_sec);

void setup() {
  Serial.begin(115200);
  setupwifi();
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i <= 3; i++) {
    pinMode(digitalPins[i], OUTPUT);
    digitalWrite(digitalPins[i], LOW);
  }
}

void loop() {
 
  realtime();
}

void realtime() {
  timeClient.update();
  int hour = timeClient.getHours();
  int min = timeClient.getMinutes();
  int counter = 0;

  Serial.println("現在時刻");
  Serial.println(timeClient.getFormattedTime());

  segarray[0] = hour / 10;
  segarray[1] = hour % 10;
  segarray[2] = min / 10;
  segarray[3] = min % 10;

  Serial.println("配列に入れて時間表示");
  Serial.print(segarray[0]);
  Serial.print(segarray[1]);
  Serial.print(":");
  Serial.print(segarray[2]);
  Serial.print(segarray[3]);
  Serial.println();

  for (int count = 0; count <= 1500; count++) {
    for (int i = 0; i <= 3; i++) {
      for (int j = 0; j <= 3; j++) {
        if (i == j) {
          digitalWrite(digitalPins[j], HIGH);
        } else {
          digitalWrite(digitalPins[j], LOW);
        }
      }
      writeData(segarray[i]);
      delay(1);
    }
  }
  delay(1);
}

void writeData(int value) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digitCodes[value]);
  digitalWrite(latchPin, HIGH);
}

void setupwifi() {
  Serial.println();
  Serial.print("Connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
}
