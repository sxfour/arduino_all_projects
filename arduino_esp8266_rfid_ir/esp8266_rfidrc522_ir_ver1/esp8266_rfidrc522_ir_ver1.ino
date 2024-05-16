#include <MFRC522.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RST_PIN 5
#define SS_PIN 4

const char* ssid = "SET";
const char* password = "SET";

MFRC522 mfrc522(4, 5);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  SPI.begin();

  mfrc522.PCD_Init();

  Serial.println(F("Ready"));
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  int rfid_uid = CheckCard();
  bool door = ControlDoor();


  if (rfid_uid == # SET INT # && door) { 
    Serial.print(F("[STATUS] Alarm off, please close a door at 120 seconds.\n")); 
    delay(120000);
  } 
  else if (door) { 
    Serial.print(F("[STATUS] Alarm off, door is closed.\n"));
  }
  else if (door == false) { 
    Serial.print(F("[ATTENTION] Alarm on, door is open.\n"));
  } 
  else { 
    Serial.print(F("[ERROR] Error func\n"));
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
