/*
Diagnostic test for the water pump
*/

void setup() {
  // initialize digital pin motor dc as an output.
  pinMode(34, OUTPUT);

}

void loop() {
  digitalWrite(34, HIGH);   // turn the motor on (HIGH is the voltage level)
  delay(3000);                       // wait for 3 seconds
    digitalWrite(34, LOW);   // turn the motor off (LOW is the voltage level)
  delay(3000); 

}
