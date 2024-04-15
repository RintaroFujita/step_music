#include <SPI.h>

#define L6470_SS_PIN 10

int rotationCount = 0;
int speeds[] = {40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180};

void setup() {
  SPI.begin();
  pinMode(L6470_SS_PIN, OUTPUT);
  digitalWrite(L6470_SS_PIN, HIGH);

  L6470_setup();

  Serial.begin(9600);
}

void loop() {
  // Perform 10 rotations forward at varying speeds
  if (rotationCount < 10) {
    rotateAndDelay(true, getNextSpeed(), 128, 1000); // Forward rotation, full step
  } else {
    rotationCount = 0;
  }
}

void rotateAndDelay(bool direction, int speed, int microsteps, int delayTime) {
  // Only handle forward direction (direction argument ignored)
  rotateMotorFullStep(speed);

  delay(delayTime);

  stopMotor();
  delay(500);

  rotationCount++;
}

void L6470_send(unsigned char value) {
  digitalWrite(L6470_SS_PIN, LOW);
  SPI.transfer(value);
  digitalWrite(L6470_SS_PIN, HIGH);
}

void L6470_setup() {
  L6470_send(0x0B);  
  L6470_send(0x0D);  
}

void rotateMotorFullStep(int speed) {
  // Enforce forward direction (positive speed)
  speed = abs(speed);

  speed = constrain(speed, 0, 1023);
  byte speedHigh = speed / 256;
  byte speedLow = speed % 256;
  L6470_send(0x50); // Forward direction
  L6470_send(speedHigh);
  L6470_send(speedLow);

  // For full step, send 200 steps
  for (int i = 0; i < 128; i++) {
    L6470_send(0x55); // Send step command
  }
}

void stopMotor() {
  L6470_send(0xB8); // Stop motor
}

int getNextSpeed() {
  int index = rotationCount % 20; // Ensure index stays within array bounds
  return speeds[index]; // Return speed from the array based on rotation count
}
