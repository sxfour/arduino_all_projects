#include <MFRC522.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define RST_PIN 5
#define SS_PIN 4

bool is_active_alarm = false;

const char* ssid = "SET";
const char* password = "SET";
byte tries = 10;

MFRC522 mfrc522(4, 5);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  SPI.begin();

  mfrc522.PCD_Init();

  Serial.println(F("Ready"));

  WiFi.begin(ssid, password);
  while (--tries && WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Non Connecting to WiFi.."); }
  else { Serial.println(""); Serial.println("WiFi connected"); Serial.println("IP address: "); Serial.println(WiFi.localIP()); }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  ValidateCard(CheckCard());

  bool door = ControlDoor();

  if (is_active_alarm && door) { Serial.print(F("OK\n")); } 
  else if ((is_active_alarm == false && door) || (is_active_alarm == false && door == false)) {  Serial.print(F("Sleep...\n")); } 
  else if (is_active_alarm && door == false) { Serial.print(F("ALARM\n")); SendRestAPI(); }
  else { Serial.print(F("Error\n")); SendRestAPI(); }

  delay(1000);
}

int CheckCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid_;

    uid_ += String(mfrc522.uid.uidByte[1]) + String(mfrc522.uid.uidByte[2]) + String(mfrc522.uid.uidByte[3]) + String(mfrc522.uid.uidByte[4]);
    
    digitalWrite(LED_BUILTIN, LOW);

    return uid_.toInt();
  }
   
  return 0;
}
bool ControlDoor() {
  int ir_door = digitalRead(0);

  if (ir_door == 0) { return true; } 
  else if (ir_door == 1) { return false; }
  else { return false; }
}
void SendRestAPI() {
  if (WiFi.status() == WL_CONNECTED) {
      WiFiClient wifiClient;
      HTTPClient http;

      http.begin(wifiClient, "SET");

      int httpCode = http.PUT("{\"user_id\":1, \"username\":\"admin\", \"status\":\"ALARM\"}");

      if (httpCode > 0) { String payload = http.getString(); Serial.println(payload); }
      http.end();

      delay(1000);
  }
}
void ValidateCard(int rfid_card) {
  if (rfid_card == # SET INT # && is_active_alarm == false) { is_active_alarm = true; }
  else if (rfid_card == # SET INT # && is_active_alarm) { is_active_alarm = false; }
}
