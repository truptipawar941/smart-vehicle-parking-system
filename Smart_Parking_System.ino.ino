#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

// Gate Sensor (Sensor 5)
#define gateTrig 13
#define gateEcho 14

// Slot 1
#define s1Trig 27
#define s1Echo 26

// Slot 2
#define s2Trig 25
#define s2Echo 33

// Slot 3
#define s3Trig 32
#define s3Echo 35

// Slot 4
#define s4Trig 18
#define s4Echo 19

// Servo
#define servoPin 23

bool vehicleDetected = false;

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  gateServo.attach(servoPin);
  gateServo.write(0);

  pinMode(gateTrig, OUTPUT);
  pinMode(gateEcho, INPUT);

  pinMode(s1Trig, OUTPUT);
  pinMode(s1Echo, INPUT);

  pinMode(s2Trig, OUTPUT);
  pinMode(s2Echo, INPUT);

  pinMode(s3Trig, OUTPUT);
  pinMode(s3Echo, INPUT);

  pinMode(s4Trig, OUTPUT);
  pinMode(s4Echo, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");

  lcd.setCursor(0, 1);
  lcd.print("SMART PARKING");

  delay(3000);
}

void loop() {

  long gateDistance = getDistance(gateTrig, gateEcho);

  // Vehicle Entry Detection
  if (gateDistance < 10 && !vehicleDetected) {

    vehicleDetected = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("VEHICLE");

    lcd.setCursor(0, 1);
    lcd.print("ENTERED");

    gateServo.write(90); // Open Gate
    delay(3000);

    gateServo.write(0); // Close Gate
    delay(1000);
  }

  if (gateDistance > 15) {
    vehicleDetected = false;
  }

  // Slot Detection
  bool slot1 = getDistance(s1Trig, s1Echo) < 10;
  bool slot2 = getDistance(s2Trig, s2Echo) < 10;
  bool slot3 = getDistance(s3Trig, s3Echo) < 10;
  bool slot4 = getDistance(s4Trig, s4Echo) < 10;

  // All Slots Full
  if (slot1 && slot2 && slot3 && slot4) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PARKING FULL");

    lcd.setCursor(0, 1);
    lcd.print("GO NEXT AREA");

    delay(2000);
  }
  else {

    // Show Slot 1 & Slot 2
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("S1:");
    lcd.print(slot1 ? "FULL " : "EMPTY");

    lcd.setCursor(0, 1);
    lcd.print("S2:");
    lcd.print(slot2 ? "FULL " : "EMPTY");

    delay(2000);

    // Show Slot 3 & Slot 4
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("S3:");
    lcd.print(slot3 ? "FULL " : "EMPTY");

    lcd.setCursor(0, 1);
    lcd.print("S4:");
    lcd.print(slot4 ? "FULL " : "EMPTY");

    delay(2000);
  }

  // Serial Monitor
  Serial.print("S1=");
  Serial.print(slot1 ? "FULL" : "EMPTY");

  Serial.print(" S2=");
  Serial.print(slot2 ? "FULL" : "EMPTY");

  Serial.print(" S3=");
  Serial.print(slot3 ? "FULL" : "EMPTY");

  Serial.print(" S4=");
  Serial.println(slot4 ? "FULL" : "EMPTY");
}