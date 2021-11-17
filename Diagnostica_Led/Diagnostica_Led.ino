/*
 Diagnostic test fro LEDs
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin red as an output.
  pinMode(26, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(26, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for 2 seconds
  digitalWrite(26, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for 2 seconds
}
