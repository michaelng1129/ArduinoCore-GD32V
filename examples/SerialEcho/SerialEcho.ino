// Serial echo test for the GD32VW553K-START.
// Serial is UART2 (PA6 TX / PA7 RX) to the on-board GD-Link USB serial.
// Open the serial monitor at 115200 baud, type characters, and they are
// echoed back. This exercises the UART2 RX interrupt + ring buffer.

void setup() {
  Serial.begin(115200);
  // Wait for the USB serial to come up (optional).
  delay(1000);
  Serial.println("GD32VW553-START Serial echo ready");
}

void loop() {
  while (Serial.available() > 0) {
    int c = Serial.read();
    Serial.write((uint8_t)c);
  }
}
