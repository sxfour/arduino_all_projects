#include <MFRC522.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define RST_PIN 5
#define SS_PIN 4

const char* ssid = "RT-GPON-C4F8";
const char* password = "24e8qd\"\"\"B";

byte tries = 10;

MFRC522 mfrc522(4, 5);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  SPI.begin();

  mfrc522.PCD_Init();

  // Serial.println(F("Ready"));

  WiFi.begin(ssid, password);
  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    // Serial.println("Non Connecting to WiFi..");
  }
  else {
    // Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  int rfid_uid = CheckCard();
  bool door = ControlDoor();


  if (rfid_uid == 2010130 && door) { 
    // Serial.print(F("[STATUS] Alarm off, please close a door at 120 seconds.\n")); 
    delay(120000);
  } 
  else if (door) { 
    // Serial.print(F("[STATUS] Alarm off, door is closed.\n"));
  }
  else if (door == false) { 
    // Serial.print(F("[ATTENTION] Alarm on, door is open.\n"));

    SendRestAPI();
  } 
  else { 
    // Serial.print(F("[ERROR] Error func\n"));
  }

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

      http.begin(wifiClient, "http://92.53.105.30:8000/users");

      int httpCode = http.PUT("{\"user_id\":1, \"username\":\"admin\", \"status\":\"ALARM\"}");

      if (httpCode > 0) {
        String payload = http.getString();
        
        // Serial.println(payload);

      }
      http.end();

      delay(60000);
  }
}