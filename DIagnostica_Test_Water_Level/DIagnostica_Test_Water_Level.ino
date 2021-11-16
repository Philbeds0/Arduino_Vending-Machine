/* Arduino Tutorial - Watel Level Sensor 40mm  
 *  Per quello del kit Elegoo MAX 367 di output
 *  Quindi: 80 = 1 cm e 320 = 4cm
   More info: */

const int read = A15; //Sensor AO pin to Arduino pin A0
int value=0;          //Variable to store the incomming data

void setup()
{
  //Begin serial communication
  Serial.begin(9600);
  
}

void loop()
{
  value = analogRead(read); //Read data from analog pin and store it to value variable
  Serial.println(value);
  if (value<=10){ 
    Serial.println("Water level: 0mm - Empty!"); 
  }
  else if (value>10 && value<=40){ 
    Serial.println("Water level: 0mm to 5mm"); 
  }
  else if (value>40 && value<=80){ 
    Serial.println("Water level: 5mm to 10mm"); 
  }
  else if (value>81 && value<=120){ 
    Serial.println("Water level: 10mm to 15mm"); 
  } 
  else if (value>121 && value<=160){ 
    Serial.println("Water level: 15mm to 20mm"); 
  }
  else if (value>161 && value<=180){ 
    Serial.println("Water level: 20mm to 25mm"); 
  }
  else if (value>181 && value<=240){ 
    Serial.println("Water level: 25mm to 30mm"); 
  }
  else if (value>241 && value<=280){ 
    Serial.println("Water level: 30mm to 35mm"); 
  }
  else if (value>282){ 
    Serial.println("Water level: 35mm to 40mm"); 
  }
  else if (value>310){ 
    Serial.println("Water level: MAX LEVEL, THE TANK IS FULL"); 
  }
  
  delay(5000); // Check for new value every 5 sec
}
