#define trigPin 8 //trigger ultrasounds
#define echoPin 9 //receive ultrasounds
#define yellowled 10 //led to tell that object distance is higher than tot
#define blueled 11  //led to tell that object distance is lower than tot
int howmuchpost = 5000;
int howmuchpre = 5000;
int howmuchflavor = 1500;
int howmuchwater = 5500; //how much water 12000 = 500ml
long howlongultrasound, ultrasound_distance;  //To calculate bottle distance from the tap
byte pump_bridge = 7; //pump transistor's bridge pin (12v)
byte bridge_flavormotor = 17;
byte bridge_premotor = 14;
byte bridge_postmotor = 2;
byte general_led = 26;
char watersensor = A15;
unsigned long time_now;
byte bottle_distance = 6;
int leaked_water = 0;
int water_lvldanger = 250;

//SPI communication device library
#include <SPI.h>
//RFID Library
#include <RFID.h>


//Pin 53 
#define SS_PIN 53
//Pin 5 reset RFID
#define RST_PIN 5

// Keycode to activate post workout beverage
#define masnum0 55
#define masnum1 241
#define masnum2 222
#define masnum3 121
#define masnum4 97

// Keycode to activate pre workout beverage
#define prenum0 23
#define prenum1 139
#define prenum2 163
#define prenum3 64
#define prenum4 127

#define ledGreen 13  
#define ledRed 12  

//rfid_card pin 53 for  ss and reset 7
RFID rfid_card(SS_PIN, RST_PIN);

int sernum0;
int sernum1;
int sernum2;
int sernum3;
int sernum4;

boolean loopon;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup() {
  Serial.begin (9600);
  lcd.begin();
  lcd.backlight();
  SPI.begin(); //Open SPI communication
  rfid_card.init();//Initialize RFID sensor

  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  digitalWrite(ledGreen, LOW);
  lcd.setCursor(2, 0);
  lcd.print("Hi!");
  lcd.setCursor(2, 1);
  lcd.print("Place your smartphone");
  lcd.setCursor(2, 2);
  lcd.print("or card near");
  lcd.setCursor(2, 3);
  lcd.print("the logo");
  Serial.print("switch on led and rfid ready");


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(yellowled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(pump_bridge, OUTPUT);
  pinMode(bridge_flavormotor, OUTPUT);
  pinMode(bridge_postmotor, OUTPUT);
  pinMode(bridge_premotor, OUTPUT);
  pinMode(general_led, OUTPUT);
  pinMode(watersensor, INPUT);

}

void loop() {

  //###Start RFID Firmware ###
  leaked_water = analogRead(watersensor);

  if (leaked_water >= water_lvldanger) {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Water level too high!");

    while (leaked_water >= water_lvldanger) {
      leaked_water = analogRead(watersensor);
      Serial.println("Water level too high!");
      Serial.println(leaked_water);

    }
    lcd.clear();
    standby();
  }

  // if a rfid tag is detected...
  if (rfid_card.isCard()) {
    // Read serial...
    if (rfid_card.readCardSerial()) {
      sernum0 = rfid_card.serNum[0];
      sernum1 = rfid_card.serNum[1];
      sernum2 = rfid_card.serNum[2];
      sernum3 = rfid_card.serNum[3];
      sernum4 = rfid_card.serNum[4];

      // If the read serial is equal to the post workout beverage keycard
      if (sernum0 == masnum0 && sernum1 == masnum1 && sernum2 == masnum2 && sernum3 == masnum3 && sernum4 == masnum4)
      {
        //check if led is switched off, then print a message and switch on green led  wait for 3000 ms and then switch it off
        digitalWrite(general_led, HIGH);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Recharge yourself!");
        lcd.setCursor(1, 2);
        lcd.print("Here's your post");
        lcd.setCursor(1, 3);
        lcd.print("workout beverage");
        Serial.println("Valid key, led switched on");
        digitalWrite(ledGreen, HIGH);
        delay(3000);
        digitalWrite(ledGreen, LOW);

        //###Ultrasounds firmware on###
        if (loopon != 1) {
          lcd.clear();
          lcd.setCursor(3, 1);
          lcd.print("Move your ");
          lcd.setCursor(3, 2);
          lcd.print("bottle closer");
          while (loopon != 1) {


            //Check ultrasound_distance
            ultrasound();


            //if ultrasound_distance is less than 4
            if (ultrasound_distance < bottle_distance) {
              Serial.println("Something has been detected, I'll wait for 4 seconds and I'll check again");
              digitalWrite(blueled, HIGH);
              digitalWrite(yellowled, LOW);
              delay(4000);

              //Recheck if the bottle is still in position
              // ultrasound_distance check
              ultrasound();
              //If the water bottle is still there
              if (ultrasound_distance < bottle_distance) {
                digitalWrite(blueled, LOW);
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Starting to dispense");
                lcd.setCursor(0, 2);
                lcd.print("your post workout beverage");
                Serial.println("OK, water bottle is in position, starting in:");
                Serial.print("3 ");
                delay(500);
                Serial.print("2 ");
                delay(500);
                Serial.print("1 ");
                delay(500);
                Serial.println("GO");

                //###End ultrasounds firmware###


                //###Start protein motor firmware###

                //check ultrasound_distance
                ultrasound();


                //check millis 
                time_now = millis();




                //while actual millis are less than actual millis + timeframe i have to wait
                while (ultrasound_distance <= bottle_distance && millis() < time_now + howmuchflavor)
                {
                  //Check ultrasound_distance
                  ultrasound();
                  //Open water pump transistor
                  digitalWrite(pump_bridge, HIGH);
                  Serial.println("Dispensing water");
                  digitalWrite(bridge_flavormotor, HIGH);
                  Serial.println("Dispensing flavor");
                }
                digitalWrite(pump_bridge, LOW);
                digitalWrite(bridge_flavormotor, LOW);

                long unsigned time_now2 = millis();

                while (ultrasound_distance <= bottle_distance && millis() < time_now2 + howmuchpost )
                {
                  //Check ultrasound_distance
                  ultrasound();
                  //Open water punmp transistor
                  Serial.println("Dispensing post");
                  digitalWrite(bridge_postmotor, HIGH);
                  Serial.println("Dispensing water");
                  digitalWrite(pump_bridge, HIGH);
                }
                digitalWrite(pump_bridge, LOW);
                digitalWrite(bridge_postmotor, LOW);

                long unsigned time_now3 = millis();

                while (ultrasound_distance <= bottle_distance && millis() < time_now3 + howmuchwater ) {
                  //Check ultrasound_distance
                  ultrasound();
                  digitalWrite(pump_bridge, HIGH);
                  Serial.println("Dispensing water rimanente");
                }



                digitalWrite(pump_bridge, LOW);

                Serial.println("Finito di erogare water");
                //close transistor

                loopon = 1; //Water has been dispensed, by assigning 1 the firmware will exit from the ultrasounds loop
                delay(2000);
                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Your beverage is ready");
                ultrasound();
                while (ultrasound_distance <= bottle_distance) {
                  ultrasound();
                }
              }

            }

            //###End water pump firmware###

            //### Ultrasounds control firmware###


            //If the water bottle is not in position anymore switch on yellow led
            else {
              digitalWrite(blueled, LOW);
              digitalWrite(yellowled, HIGH);
            }

            // If ultrasound_distance is outside from the ultrasound range
            if (ultrasound_distance >= 200 || ultrasound_distance <= 0) {
              Serial.print("Troppo lontano:");
              Serial.print(ultrasound_distance);
              Serial.println(" cm");
            }
            //if it is inside the determined range
            else {
              Serial.print(ultrasound_distance);
              Serial.println(" cm");
            }

            delay(500);

            //### End Ultrasounds control firmware###



          }
        }
        loopon = 0;
        delay(3000);
        lcd.clear();
        Serial.println("Ended simulation");
        lcd.setCursor(6, 1);
        lcd.print("bye");
        delay(2000);
        digitalWrite(general_led, LOW);
        standby();

      }

      //If the read serial is the pre workout one
      else if (sernum0 == prenum0 && sernum1 == prenum1 && sernum2 == prenum2 && sernum3 == prenum3 && sernum4 == prenum4)
      {

        digitalWrite(general_led, HIGH);

        //check if led is switched off, then print a message and switch on green led  wait for 3000 ms and then switch it off
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("Hi Elizabeth!");
        lcd.setCursor(2, 2);
        lcd.print("Happy workout");
        Serial.println("Valid key, led switched on");
        digitalWrite(ledGreen, HIGH);
        delay(3000);
        digitalWrite(ledGreen, LOW);

        //###Starting ultrasounds firmware###
        if (loopon != 1) {
          lcd.clear();
          lcd.setCursor(3, 1);
          lcd.print("Posiziona la");
          lcd.setCursor(3, 2);
          lcd.print("tua borraccia");
          while (loopon != 1) {

            //Check ultrasound_distance
            ultrasound();

            //If ultrasound_distance is less than 4
            if (ultrasound_distance < bottle_distance) {
              Serial.println("I detected something, I'll wait and then I'll check again");
              digitalWrite(blueled, HIGH);
              digitalWrite(yellowled, LOW);
              delay(4000);

              //Check if the water bottle is still there
              ultrasound();

              //If the water bottle is still there
              if (ultrasound_distance < bottle_distance) {
                digitalWrite(blueled, LOW);
                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Starting to dispense");
                lcd.setCursor(1, 2);
                lcd.print("your pre workout beverage");
                Serial.println("OK, water bottle in position, starting in:");
                Serial.print("3 ");
                delay(500);
                Serial.print("2 ");
                delay(500);
                Serial.print("1 ");
                delay(500);
                Serial.println("GO");

                //###End ultrasounds firmware###


                //###Starting protein motor firmware###
                ultrasound();


                //check millis
                time_now = millis();




                //while actual millis are less than actual millis + timeframe i have to wait
                while (ultrasound_distance <= bottle_distance && millis() < time_now + howmuchflavor)
                {
                  ultrasound();
                  Serial.println(ultrasound_distance);
                  //Open water punmp transistor
                  digitalWrite(pump_bridge, HIGH);
                  Serial.println("Dispensing water");
                  digitalWrite(bridge_flavormotor, HIGH);
                  Serial.println("Dispensing flavor");
                }
                digitalWrite(pump_bridge, LOW);
                digitalWrite(bridge_flavormotor, LOW);

                long unsigned time_now2 = millis();

                while (ultrasound_distance <= bottle_distance && millis() < time_now2 + howmuchpost )
                {
                  ultrasound();
                  Serial.println(ultrasound_distance);
                  //Open water punmp transistor
                  Serial.println("Dispensing post");
                  digitalWrite(bridge_premotor, HIGH);
                  Serial.println("Dispensing water");
                  digitalWrite(pump_bridge, HIGH);
                }
                digitalWrite(pump_bridge, LOW);
                digitalWrite(bridge_premotor, LOW);

                long unsigned time_now3 = millis();

                while (ultrasound_distance <= bottle_distance && millis() < time_now3 + howmuchwater ) {
                  ultrasound();
                  Serial.println(ultrasound_distance);
                  digitalWrite(pump_bridge, HIGH);
                  Serial.println("Dispensing remaining water");
                }



                digitalWrite(pump_bridge, LOW);
                Serial.println("Finished dispensing water");
                //close transistor

                loopon = 1; //water has been dispensed, exit from the loop
                delay(2000);

                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Your beverage is ready");
                ultrasound();
                while (ultrasound_distance <= bottle_distance) {
                  ultrasound();
                }
              }

            }

            //###End water pump firmware###

            //### Ultrasounds control firmware###


            //If the water bottle is not in position anymore switch on yellow led
            else {
              digitalWrite(blueled, LOW);
              digitalWrite(yellowled, HIGH);
            }

            // If ultrasound_distance is outside range
            if (ultrasound_distance >= 200 || ultrasound_distance <= 0) {
              Serial.print("Too far:");
              Serial.print(ultrasound_distance);
              Serial.println(" cm");
            }
            //if it is inside range
            else {
              Serial.print(ultrasound_distance);
              Serial.println(" cm");
            }

            delay(500);

            //### end Ultrasounds control firmware###



          }
        }
        loopon = 0;
        delay(3000);
        lcd.clear();
        Serial.println("preworkout simualtion ended");
        lcd.setCursor(6, 1);
        lcd.print("Bye");
        delay(2000);
        digitalWrite(general_led, LOW);
        standby();



      }
      else {
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("Subscription ");
        lcd.setCursor(2, 2);
        lcd.print("not recognized");
        Serial.println("Invalid key, red led switched on");
        digitalWrite(ledRed, HIGH);
        delay(3000);
        digitalWrite(ledRed, LOW);
        digitalWrite(general_led, LOW);
        standby();

      }
    }
  }

}


void standby() {
  lcd.setCursor(2, 0);
  lcd.print("Hi!");
  lcd.setCursor(2, 1);
  lcd.print("Move your smartphone");
  lcd.setCursor(2, 2);
  lcd.print("near the ");
  lcd.setCursor(2, 3);
  lcd.print("logo");
  Serial.print("Switched on led, rfid ready");
  //###Fine Firmware RFID###
}

void ultrasound() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  howlongltrasound = pulseIn(echoPin, HIGH);
  ultrasound_distance = (howlongltrasound / 2) / 29.1;


}
