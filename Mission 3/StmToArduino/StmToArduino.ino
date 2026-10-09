#include <SoftwareSerial.h>

SoftwareSerial stmSerial(10,11); // rx, tx

void setup() {
  // Serial to PC (Serial Monitor)
  Serial.begin(9600);
  // SoftwareSerial to STM32 USART1 (PA9 TX -> Pin 10 RX)
  stmSerial.begin(9600);

  Serial.println("--- Arduino Ready: Listening to STM32 MPU-6500 at 9600 Baud ---");
}

void loop() {
  // Forward incoming data from STM32 immediately to Serial Monitor
  while (stmSerial.available()) {
    Serial.write(stmSerial.read());
  }
}
