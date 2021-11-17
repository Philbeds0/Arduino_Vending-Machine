/*
Diagnostic test for the pre, post workout motors and flavor motor
*/


void setup() {
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(46, OUTPUT);
  Serial.begin(9600);
}

void loop() {


  digitalWrite(2, HIGH);   // turn the post motor on (HIGH is the voltage level)
  Serial.println("open");
  delay(2000);                       // wait for 2 seconds
  digitalWrite(2, LOW);    // turn the post motor off by making the voltage LOW
  Serial.println("close");
  delay(2000); 
  
  digitalWrite(14, HIGH);   // turn the pre motor on (HIGH is the voltage level)
  Serial.println("open");
  delay(2000);                       // wait for 2 seconds
  digitalWrite(14, LOW);    // turn the pre motor off by making the voltage LOW
  Serial.println("close");
  delay(2000);  
  
  digitalWrite(17, HIGH);   // turn the flavor motor on (HIGH is the voltage level)
  Serial.println("open");
  delay(2000);                       // wait for 2 seconds
  digitalWrite(17, LOW);    // turn the flavor motor off by making the voltage LOW
  Serial.println("close");
  delay(2000); 

  // wait for 2 seconds
}
