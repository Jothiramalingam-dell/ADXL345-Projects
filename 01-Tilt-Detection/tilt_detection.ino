#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <LiquidCrystal_I2C.h>

// LCD address 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_ADXL345_Unified accel = 
    Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  if (!accel.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    Serial.println("ADXL345 not found!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("ADXL345 Ready!");
  Serial.println("ADXL345 Found!");
  delay(2000);
  lcd.clear();
}

String getTiltDirection(float x, 
                        float y, 
                        float z) {
  if (z > 8.0)  return "FLAT        ";
  if (z < -8.0) return "UPSIDE DOWN ";
  if (x > 6.0)  return "TILT RIGHT  ";
  if (x < -6.0) return "TILT LEFT   ";
  if (y > 6.0)  return "TILT FWD    ";
  if (y < -6.0) return "TILT BACK   ";
  return "TILTED      ";
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  String dir = getTiltDirection(x, y, z);

  // LCD Row 1 - Direction
  lcd.setCursor(0, 0);
  lcd.print(dir);

  // LCD Row 2 - X Y values
  lcd.setCursor(0, 1);
  lcd.print("X:");
  lcd.print(x, 1);
  lcd.print(" Y:");
  lcd.print(y, 1);
  lcd.print("    ");

  // Serial Monitor
  Serial.print("Dir: ");
  Serial.print(dir);
  Serial.print("| X:");
  Serial.print(x);
  Serial.print(" Y:");
  Serial.print(y);
  Serial.print(" Z:");
  Serial.println(z);

  delay(300);
}
