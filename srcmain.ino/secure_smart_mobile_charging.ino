// === Libraries ===
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

// === Pin Definitions ===
#define COIN_PIN        2
#define SERVO_PIN       13
#define RELAY_COIN      26
#define RELAY_CHARGER   27
#define BUZZER_PIN      25

// === Globals ===
LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);
Servo locker;

uint8_t enrolledID = 1;
unsigned long chargeStartTime = 0;
bool charging = false;
bool deviceCollected = false;

// === Setup ===
void setup() {
  Serial.begin(115200);
  pinMode(COIN_PIN, INPUT);
  pinMode(RELAY_COIN, OUTPUT);
  pinMode(RELAY_CHARGER, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_COIN, HIGH);      // Enable coin acceptor
  digitalWrite(RELAY_CHARGER, LOW);    // Charging off
  digitalWrite(BUZZER_PIN, LOW);       // Buzzer off

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Insert Coin");

  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);
  if (!finger.verifyPassword()) {
    lcd.setCursor(0, 1);
    lcd.print("FP Error!");
    while (1);
  }

  locker.attach(SERVO_PIN);
  locker.write(0); // Lock
  delay(1000);
}

// === Loop ===
void loop() {
  // Wait for coin
  if (digitalRead(COIN_PIN) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("\xE2\x82\xB95 Detected");
    Serial.println("Coin Inserted");

    while (digitalRead(COIN_PIN) == HIGH); // wait till pulse ends

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enrolling...");

    while (!enrollFingerprint(enrolledID)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enroll Failed");
      lcd.setCursor(0, 1);
      lcd.print("Retrying...");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enrolling...");
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enroll Success");
    delay(2000);
    lcd.clear();

    locker.write(90);                      // Unlock to place device
    digitalWrite(RELAY_COIN, LOW);        // Disable coin acceptor
    digitalWrite(RELAY_CHARGER, HIGH);    // Start charging
    charging = true;
    chargeStartTime = millis();

    lcd.setCursor(0, 0);
    lcd.print("Charging...");

    while (charging) {
      if (checkFingerprint(enrolledID)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Verified");
        delay(1000);
        stopCharging();
        break;
      }

      if (millis() - chargeStartTime >= 60000 && !deviceCollected) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Time's Up!");
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(RELAY_CHARGER, LOW);
        charging = false;

        // Wait indefinitely until the enrolled fingerprint is verified
        while (!checkFingerprint(enrolledID)) {
          lcd.setCursor(0, 1);
          lcd.print("Verify Finger...");
          delay(1000);
        }

        stopCharging();
        break;
      }
      delay(100);
    }
    // Prevent re-triggering enrollment immediately
    while (digitalRead(COIN_PIN) == HIGH); // Wait until coin pin is low
    resetSystem();
  }
}

// === Helper: Enrollment ===
bool enrollFingerprint(uint8_t id) {
  int p = -1;
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  while ((p = finger.getImage()) != FINGERPRINT_OK);

  if (finger.image2Tz(1) != FINGERPRINT_OK) return false;

  lcd.setCursor(0, 1);
  lcd.print("Remove Finger");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER);

  lcd.setCursor(0, 1);
  lcd.print("Place Again...");
  while ((p = finger.getImage()) != FINGERPRINT_OK);

  if (finger.image2Tz(2) != FINGERPRINT_OK) return false;
  if (finger.createModel() != FINGERPRINT_OK) return false;
  return finger.storeModel(id) == FINGERPRINT_OK;
}

// === Helper: Verification ===
bool checkFingerprint(uint8_t id) {
  if (finger.getImage() != FINGERPRINT_OK) return false;
  if (finger.image2Tz(1) != FINGERPRINT_OK) return false;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return false;
  return finger.fingerID == id;
}

// === Helper: Stop charging ===
void stopCharging() {
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_CHARGER, LOW);
  locker.write(90); // Open for retrieval
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Collect Device");
  deviceCollected = true;
  delay(5000);
  resetSystem();
}

// === Helper: Reset ===
void resetSystem() {
  locker.write(0); // Lock
  digitalWrite(RELAY_COIN, HIGH);  // Enable coin acceptor
  charging = false;
  deviceCollected = false;
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Insert Coin");
}