#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define PIN_BUZZER 13
#define PIN_BUTTON 5

const char* ssid = "----";
const char* password = "----";

const char* ntpServer = "pool.ntp.org";
const long gmtoffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;

int segarray[4] = { 0, 0, 0, 0 };
bool buzzer = false;
int settime[4] = { 1, 5, 4, 0 }; /*settime[0]hour_10,settime[1]hour_1,settime[2]min_10,settime[3]min_1*/

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtoffset_sec, daylightOffset_sec);

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  Serial.begin(115200);
  setupwifi();
  while (!Serial) {
    ;  // シリアルポートが接続されるまで待機
  }
  Serial.println("ESP32 Ready to receive data.");
}

void loop() {
  timeClient.update();
  int hour = timeClient.getHours();
  int min = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

  segarray[0] = hour / 10;
  segarray[1] = hour % 10;
  segarray[2] = min / 10;
  segarray[3] = min % 10;

  int cou = 0;
  for (int i = 0; i <= 3; i++) {
    if (settime[i] == segarray[i]) {
      cou++;
    }
  }

  if (digitalRead(PIN_BUTTON) == LOW) {
    buzzer = true;
  }

  if (cou == 4 && buzzer == false) {
    digitalWrite(PIN_BUZZER, HIGH);
  } else if (cou == 4 && buzzer == true) {
    digitalWrite(PIN_BUZZER, LOW);
  } else {
    buzzer = false;
    digitalWrite(PIN_BUZZER, LOW);
  }
  cou = 0;

  delay(2);

  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n');  // 改行まで読み取る
    if (receivedString.length() != 4) {  // 受信した文字列が4桁でない場合
        Serial.println("Error: Input must be exactly 4 digits.");
    } else {
        bool isValid = true;
        for (int i = 0; i < 4; i++) {
            if (!isdigit(receivedString.charAt(i))) {  // 数字でない文字が含まれているかチェック
                isValid = false;
                break;
            }
        }
        if (!isValid) {
            Serial.println("Error: Input must contain only digits.");
        } else {
            for (int i = 0; i < 4; i++) {
                settime[i] = receivedString.charAt(i) - '0';
                Serial.println(settime[i]);
            }
            Serial.print("Received: ");
            Serial.println(receivedString);  // 受信した文字列を出力
        }
    }
}

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