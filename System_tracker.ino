#include <Servo.h>

// ===== السيرفوهات =====
Servo servo1;
Servo servo2;
Servo servo3;

// ===== Ultrasonic =====
#define trig1 9
#define echo1 10

#define trig2 11
#define echo2 12

// ===== Laser =====
#define laserPin 7

// ===== متغيرات =====
int angle1 = 0;
int angle2 = 0;

bool dir1 = true;
bool dir2 = true;

// ===== توقيت =====
unsigned long previousMillis = 0;
const int interval = 10;

// ===== قراءة المسافة =====
long readDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  servo1.attach(6);
  servo2.attach(5);
  servo3.attach(4);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);

  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(laserPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  long d1 = readDistance(trig1, echo1);
  long d2 = readDistance(trig2, echo2);

  bool sensor1 = (d1 > 0 && d1 < 50);
  bool sensor2 = (d2 > 0 && d2 < 50);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // ===== الحساس التاني (Tracking) =====
    if (sensor2) {

      // وقف الاتنين
      servo1.write(angle1);
      servo2.write(angle2);

      // ⭐️ التوجيه المستمر للسيرفو التالت
      servo3.write(angle2);

      digitalWrite(laserPin, HIGH);
    }

    // ===== الحساس الأول =====
    else if (sensor1) {
      servo1.write(angle1);

      // السيرفو التاني يكمل
      servo2.write(angle2);

      if (dir2) {
        angle2 += 2;
        if (angle2 >= 180) dir2 = false;
      } else {
        angle2 -= 2;
        if (angle2 <= 0) dir2 = true;
      }

      digitalWrite(laserPin, LOW);
    }

    // ===== الوضع الطبيعي =====
    else {

      digitalWrite(laserPin, LOW);

      // سيرفو 1
      servo1.write(angle1);
      if (dir1) {
        angle1 += 2;
        if (angle1 >= 180) dir1 = false;
      } else {
        angle1 -= 2;
        if (angle1 <= 0) dir1 = true;
      }

      // سيرفو 2
      servo2.write(angle2);
      if (dir2) {
        angle2 += 2;
        if (angle2 >= 180) dir2 = false;
      } else {
        angle2 -= 2;
        if (angle2 <= 0) dir2 = true;
      }
    }
  }
}
