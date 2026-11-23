
#include <Wire.h>

#define SDA_PIN 5
#define SCL_PIN 6

#define MPU_ADDR 0x68

void writeMPU(byte reg, byte value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

int16_t read16() {
  int16_t value = Wire.read() << 8;
  value |= Wire.read();
  return value;
}

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  // Wake up MPU
  writeMPU(0x6B, 0x00);

  // Accelerometer: +/- 2g
  writeMPU(0x1C, 0x00);

  // Gyroscope: +/- 250 degrees/sec
  writeMPU(0x1B, 0x00);

  Serial.println("MPU Initialized");
}

void loop() {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 14);

  if (Wire.available() == 14) {

    int16_t ax = read16();
    int16_t ay = read16();
    int16_t az = read16();

    int16_t temp = read16();

    int16_t gx = read16();
    int16_t gy = read16();
    int16_t gz = read16();

    Serial.print("ACC: ");
    Serial.print(ax / 16384.0);
    Serial.print(" , ");
    Serial.print(ay / 16384.0);
    Serial.print(" , ");
    Serial.print(az / 16384.0);

    Serial.print("  |  GYRO: ");

    Serial.print(gx / 131.0);
    Serial.print(" , ");
    Serial.print(gy / 131.0);
    Serial.print(" , ");
    Serial.println(gz / 131.0);
  }

  delay(100);
}
