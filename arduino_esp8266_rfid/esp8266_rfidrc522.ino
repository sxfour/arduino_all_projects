#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 5
#define SS_PIN 4

int ir_door = 0;

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

  CheckCard();
  ControlDoor();
}

void CheckCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid_;

    uid_ += String(mfrc522.uid.uidByte[1]) + String(mfrc522.uid.uidByte[2]) + String(mfrc522.uid.uidByte[3]) + String(mfrc522.uid.uidByte[4]);
    
    Serial.print(F("[RFID] Card UID: "));
    Serial.println(uid_.toInt());
    
    digitalWrite(LED_BUILTIN, LOW);

    delay(1000);
  }
}

void ControlDoor() {
  ir_door = digitalRead(0);

  Serial.print(F("[IR] Door status: "));
  Serial.println(ir_door);

  delay(1000);
}
