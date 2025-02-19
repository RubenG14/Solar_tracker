
// J. Ruben Garcia Villalobos
// Solar Tracker Project

// Libraries required
#include <Servo.h>
#include <math.h>

// Define variables and objects
// Servo related variables
Servo ServoH;         // Define Horizontal axis servo (180°)
Servo ServoV;         // Define Vertical axis servo (180°)
int servoAngleH = 0;  // Angle of Horizontal axis Servo
int servoAngleV = 0;  // Angle of Vertical axis Servo

// Declaring LDR related variables
int LDR1 = 0;
int LDR1pin = 1;  // Light Dependent Resistor 1 and Pin
int LDR2 = 0;
int LDR2pin = 2;  // Light Dependent Resistor 2 and Pin
int LDR3 = 0;
int LDR3pin = 3;  // Light Dependent Resistor 3 and Pin
int LDR4 = 0;
int LDR4pin = 4;          // Light Dependent Resistor 4 and Pin
int LDRH = 0;             // LDR 2 & 4 comparative
int LDRV = 0;             // LDR 1 & 3 comparative
int LDR_threshold = 100;  // Value difference when start to move
int Mov_Speed = 1;        // Speed at which the servos move (recommended 1)

// LED related variables
static const int LED_pin = 13;  // Board Integrated LED
int LED_H = 2;
int LED_V = 6;  // LED pins activated when servos are active

void setup() {
  // Default values and settings
  // Set serial port for communication
  Serial.begin(9600);  // Check information we want to display on digital monitor

  //Define Servo pins
  ServoH.attach(3);  // Digital pin 3 on board
  ServoV.attach(5);  // Digital pin 5 on board

  // Define if pins are Inputs or Outputs
  pinMode(LDR1pin, INPUT);
  pinMode(LDR2pin, INPUT);
  pinMode(LDR3pin, INPUT);
  pinMode(LDR4pin, INPUT);

  pinMode(LED_pin, OUTPUT);
  pinMode(LED_H, OUTPUT);
  pinMode(LED_V, OUTPUT);

  // Define default Servo's Angles
  servoAngleH = 60;
  servoAngleV = 90;
  ServoH.write(servoAngleH);
  ServoV.write(servoAngleV);

  digitalWrite(LED_pin, HIGH);
  delay(3000);
}

void loop() {

  // Read values from LDRs  0-4095
  LDR1 = analogRead(LDR1pin);
  LDR2 = analogRead(LDR2pin);
  LDR3 = analogRead(LDR3pin);
  LDR4 = analogRead(LDR4pin);
  // Print values on monitor
  Serial.print("LDR1: ");
  Serial.println(LDR1);
  Serial.print("LDR2: ");
  Serial.println(LDR2);
  Serial.print("LDR3: ");
  Serial.println(LDR3);
  Serial.print("LDR4: ");
  Serial.println(LDR4);

  // LDR array comparative to define movement in H horizontal and V vertical axis
  LDRH = abs(LDR1 - LDR3);
  LDRV = abs(LDR2 - LDR4);

  // Builtin LED will be off when working / on when idle
  if (LDRV > LDR_threshold || LDRH > LDR_threshold) {
    digitalWrite(LED_pin, LOW);

    // When one or both of the array comparatives is bigger than the threshold, start the servo's movement
    // The LDR with the biggest difference starts to move first
    if (LDRV > LDRH) {
      digitalWrite(LED_V, HIGH);
      digitalWrite(LED_H, LOW);
      if (LDR2 > LDR4 & servoAngleH < 90 & servoAngleV < 180) {
        servoAngleV = servoAngleV + Mov_Speed;
        ServoV.write(servoAngleV);
      }
      if (LDR2 < LDR4 & servoAngleH< 90 & servoAngleV > 0) {
        servoAngleV = servoAngleV - Mov_Speed;
        ServoV.write(servoAngleV);
      }
      // When Horizontal axis angle is more than 90, Vertical axis rotation is inverted
      if (LDR2 > LDR4 & servoAngleH >= 90 & servoAngleV > 0) {
        servoAngleV = servoAngleV - Mov_Speed;
        ServoV.write(servoAngleV);
      }
      if (LDR2 < LDR4 & servoAngleH >= 90 & servoAngleV < 180) {
        servoAngleV = servoAngleV + Mov_Speed;
        ServoV.write(servoAngleV);
      }
    }
    if (LDRV < LDRH) {
      digitalWrite(LED_H, HIGH);
      digitalWrite(LED_V, LOW);
      if (LDR1 > LDR3 & servoAngleH < 180) {
        servoAngleH = servoAngleH + Mov_Speed;
        ServoH.write(servoAngleH);
      }
      if (LDR1< LDR3 & servoAngleH > 0) {
        servoAngleH = servoAngleH - Mov_Speed;
        ServoH.write(servoAngleH);
      }
    }
  }
  // Builtin LED turns off when not working
  if (LDRV < LDR_threshold & LDRH < LDR_threshold) {
    digitalWrite(LED_pin, HIGH);
    digitalWrite(LED_H, LOW);
    digitalWrite(LED_V, LOW);
  }

  delay(30);
}
