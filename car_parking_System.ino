#include <LiquidCrystal.h>
#include <Servo.h>

// LCD Pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

Servo gate;

// Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 10;

// IR Sensor
const int irPin = 11;

// Servo
const int servoPin = 12;

// Parking Slots
int slots = 5;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irPin, INPUT);

  gate.attach(servoPin);
  gate.write(0); // Gate closed

  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("CAR PARKING");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM");
  delay(2000);

  lcd.clear();
}

void loop()
{
  long duration;
  int distance;

  // Measure Distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Display Slots
  lcd.setCursor(0, 0);
  lcd.print("Slots Left:");
  lcd.print(slots);
  lcd.print(" ");

  if (slots == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("Parking Full   ");
    gate.write(0);
    return;
  }

  lcd.setCursor(0, 1);
  lcd.print("Waiting Car    ");

  // Car detected within 15 cm
  if (distance > 0 && distance <= 15)
  {
    lcd.setCursor(0, 1);
    lcd.print("Gate Opening   ");

    gate.write(90); // Open gate
    delay(2000);

    // Wait until IR sensor detects the car
    unsigned long startTime = millis();

    while (millis() - startTime < 5000)
    {
      if (digitalRead(irPin) == LOW) // Object detected
      {
        slots--;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Car Entered");
        lcd.setCursor(0, 1);
        lcd.print("Slots:");
        lcd.print(slots);

        delay(2000);

        break;
      }
    }

    gate.write(0); // Close gate
    delay(1000);

    lcd.clear();
  }
}