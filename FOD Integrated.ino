#include <SoftwareSerial.h>

// Define the pins for the SoftwareSerial communication with the GSM module
#define RX 3
#define TX 2

SoftwareSerial mySerial(RX, TX);

// Define the pins for the FSR, LEDs, and button
#define fsrAnalogPin 0
#define redLedPin 11
#define greenLedPin 10
#define BUTTON_PIN 7

int fsrReading;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configure TEXT mode
  updateSerial();
}

void loop() {
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);

  if (fsrReading > 800) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    sendSMS("+918756332205", "WARNING: Bridge 007 needs attention");
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  if (digitalRead(BUTTON_PIN) == HIGH) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  }

  checkIncomingSMS();
}

void sendSMS(String phoneNumber, String message) {
  mySerial.println("AT+CMGS=\"+918756332205");
  updateSerial();
  mySerial.print(message);
  updateSerial();
  mySerial.write(26); // Ctrl+Z character to end SMS
}

void checkIncomingSMS() {
  if (mySerial.available()) {
    String receivedMsg = mySerial.readString();
    if (receivedMsg.indexOf("1") != -1) {
      sendSMS("+918756332205", "Landslide detected");
    }
  }
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
