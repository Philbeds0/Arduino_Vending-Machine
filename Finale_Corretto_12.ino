#define trigPin 8 //trigger ultrasuoni
#define echoPin 9 //rilevatore ultrasuoni
#define ledgiallo 10 //indica la distanza > di tot
#define ledblu 11  //indica la distanza < di tot
int quantopost = 5000;
int quantopre = 5000;
int quantogusto = 1500;
int quantaacqua = 5500; //quanta acqua erogare 12000 = 500ml
long durata, distanza;  //Per il calcolo della distanza e durata dell'ultrasuono
byte bridgepompa = 7; //pin per il bridge del transistor che mi porterà i 12v
byte bridgemotoregusto = 17;
byte bridgemotorepre = 14;
byte bridgemotorepost = 2;
byte ledgenerali = 26;
char watersensor = A15;
unsigned long tempo_now;
byte distaborraccia = 6;
int realtimeacqua = 0;
int altezzaacqua = 250;

//includo libreria per la comunicazione con device SPI
#include <SPI.h>
//includo libreria per la comunicazione RFID
#include <RFID.h>


//Pin 53 per
#define SS_PIN 53
//Pin 5 al reset della scheda RFID
#define RST_PIN 5

// Codice della chiave per il post
#define masnum0 55
#define masnum1 241
#define masnum2 222
#define masnum3 121
#define masnum4 97

// Codice della chiave per il pre
#define prenum0 23
#define prenum1 139
#define prenum2 163
#define prenum3 64
#define prenum4 127

#define ledGreen 13  // Pin su cui colleghiamo il LED Verde
#define ledRed 12  // Pin su cui colleghiamo il LED Rosso

//Assegno schedarfid come RFID aventi pin 53 per ss e reset 7
RFID schedarfid(SS_PIN, RST_PIN);

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
  SPI.begin(); //Apro la comunicazione SPI
  schedarfid.init();//inizializzo scheda rfid

  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  digitalWrite(ledGreen, LOW);
  lcd.setCursor(2, 0);
  lcd.print("Ciao!");
  lcd.setCursor(2, 1);
  lcd.print("Avvicina il tuo");
  lcd.setCursor(2, 2);
  lcd.print("smartphone o la");
  lcd.setCursor(2, 3);
  lcd.print("tessera");
  Serial.print("Accensione led con RFID Pronto");


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledgiallo, OUTPUT);
  pinMode(ledblu, OUTPUT);
  pinMode(bridgepompa, OUTPUT);
  pinMode(bridgemotoregusto, OUTPUT);
  pinMode(bridgemotorepost, OUTPUT);
  pinMode(bridgemotorepre, OUTPUT);
  pinMode(ledgenerali, OUTPUT);
  pinMode(watersensor, INPUT);

}

void loop() {

  //###Iizio Firmware RFID###
  realtimeacqua = analogRead(watersensor);

  if (realtimeacqua >= altezzaacqua) {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Vaschetta piena!");

    while (realtimeacqua >= altezzaacqua) {
      realtimeacqua = analogRead(watersensor);
      Serial.println("Vaschetta piena");
      Serial.println(realtimeacqua);

    }
    lcd.clear();
    standby();
  }

  // Rileva un tag...
  if (schedarfid.isCard()) {
    // Legge il seriale...
    if (schedarfid.readCardSerial()) {
      sernum0 = schedarfid.serNum[0];
      sernum1 = schedarfid.serNum[1];
      sernum2 = schedarfid.serNum[2];
      sernum3 = schedarfid.serNum[3];
      sernum4 = schedarfid.serNum[4];

      // Se il seriale letto è uguale al seriale per il post
      if (sernum0 == masnum0 && sernum1 == masnum1 && sernum2 == masnum2 && sernum3 == masnum3 && sernum4 == masnum4)
      {
        //controlla se il led è spento e poi stampa il messaggio e accendi la luce verde, aspetta 3000 ms e poi spegnila
        digitalWrite(ledgenerali, HIGH);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Ricaricati!");
        lcd.setCursor(1, 2);
        lcd.print("Ecco la tua bevanda ");
        lcd.setCursor(1, 3);
        lcd.print("post allenamento");
        Serial.println("CHIAVE VALIDA LED ACCESO");
        digitalWrite(ledGreen, HIGH);
        delay(3000);
        digitalWrite(ledGreen, LOW);

        //###Inizio Firmware degli ultrasuoni###
        if (loopon != 1) {
          lcd.clear();
          lcd.setCursor(3, 1);
          lcd.print("Posiziona la");
          lcd.setCursor(3, 2);
          lcd.print("tua borraccia");
          while (loopon != 1) {


            //Controllo la distanza
            ultrasuoni();


            //se la distanza è minore di 4
            if (distanza < distaborraccia) {
              Serial.println("Rilevo qualcosa, aspetto 4 secondi e poi ricontrollo");
              digitalWrite(ledblu, HIGH);
              digitalWrite(ledgiallo, LOW);
              delay(4000);

              //Ricontrollo se la borraccia è ancora li
              //Controllo la distanza
              ultrasuoni();
              //se la borraccia è ancora li
              if (distanza < distaborraccia) {
                digitalWrite(ledblu, LOW);
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Inizio a erogare");
                lcd.setCursor(0, 2);
                lcd.print("il post allenamento");
                Serial.println("OK, la borraccia è in posizione, inizio a erogare fra:");
                Serial.print("3 ");
                delay(500);
                Serial.print("2 ");
                delay(500);
                Serial.print("1 ");
                delay(500);
                Serial.println("GO");

                //###Fine Firmware degli ultrasuoni###


                //###Inizio Firmware motore proteine###

                //Controllo la distanza
                ultrasuoni();


                //controllo a quanti millis sono
                tempo_now = millis();




                //finché i millis attuali sono minori dei millis attuali+ il periodo che deve aspettare
                while (distanza <= distaborraccia && millis() < tempo_now + quantogusto)
                {
                  //Controllo la distanza
                  ultrasuoni();
                  //apri transistor pompa
                  digitalWrite(bridgepompa, HIGH);
                  Serial.println("Erogo acqua");
                  digitalWrite(bridgemotoregusto, HIGH);
                  Serial.println("Erogo gusto");
                }
                digitalWrite(bridgepompa, LOW);
                digitalWrite(bridgemotoregusto, LOW);

                long unsigned tempo_now2 = millis();

                while (distanza <= distaborraccia && millis() < tempo_now2 + quantopost )
                {
                  //Controllo la distanza
                  ultrasuoni();
                  //apri transistor pompa
                  Serial.println("Erogo post");
                  digitalWrite(bridgemotorepost, HIGH);
                  Serial.println("Erogo acqua");
                  digitalWrite(bridgepompa, HIGH);
                }
                digitalWrite(bridgepompa, LOW);
                digitalWrite(bridgemotorepost, LOW);

                long unsigned tempo_now3 = millis();

                while (distanza <= distaborraccia && millis() < tempo_now3 + quantaacqua ) {
                  //Controllo la distanza
                  ultrasuoni();
                  digitalWrite(bridgepompa, HIGH);
                  Serial.println("Erogo acqua rimanente");
                }



                digitalWrite(bridgepompa, LOW);

                Serial.println("Finito di erogare acqua");
                //chiudi transistor

                loopon = 1; //Dico che ha erogato acqua così esce dal loop degli ultrasuoni
                delay(2000);
                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Ritira la bevanda");
                ultrasuoni();
                while (distanza <= distaborraccia) {
                  ultrasuoni();
                }
              }

            }

            //###Fine Firmware della pompa acqua###

            //### Firmware di controllo degli ultrasuoni###


            //se la borraccia non è in posizione accendo led giallo
            else {
              digitalWrite(ledblu, LOW);
              digitalWrite(ledgiallo, HIGH);
            }

            // se la distanza è fuori dal range dell'ultrasuono
            if (distanza >= 200 || distanza <= 0) {
              Serial.print("Troppo lontano:");
              Serial.print(distanza);
              Serial.println(" cm");
            }
            //se e dentro al range
            else {
              Serial.print(distanza);
              Serial.println(" cm");
            }

            delay(500);

            //### Fine Firmware di controllo degli ultrasuoni###



          }
        }
        loopon = 0;
        delay(3000);
        lcd.clear();
        Serial.println("Simulazione del post finita");
        lcd.setCursor(6, 1);
        lcd.print("A presto");
        delay(2000);
        digitalWrite(ledgenerali, LOW);
        standby();

      }

      //###//Se invece il seriale letto è quello del pre###
      else if (sernum0 == prenum0 && sernum1 == prenum1 && sernum2 == prenum2 && sernum3 == prenum3 && sernum4 == prenum4)
      {

        digitalWrite(ledgenerali, HIGH);

        //controlla se il led è spento e poi stampa il messaggio e accendi la luce verde, aspetta 3000 ms e poi spegnila
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("Ciao Elisabetta!");
        lcd.setCursor(2, 2);
        lcd.print("Buon allenamento");
        Serial.println("CHIAVE VALIDA LED ACCESO");
        digitalWrite(ledGreen, HIGH);
        delay(3000);
        digitalWrite(ledGreen, LOW);

        //###Inizio Firmware degli ultrasuoni###
        //###Inizio Firmware degli ultrasuoni###
        if (loopon != 1) {
          lcd.clear();
          lcd.setCursor(3, 1);
          lcd.print("Posiziona la");
          lcd.setCursor(3, 2);
          lcd.print("tua borraccia");
          while (loopon != 1) {

            //Controllo la distanza
            ultrasuoni();

            //se la distanza è minore di 4
            if (distanza < distaborraccia) {
              Serial.println("Rilevo qualcosa, aspetto 4 secondi e poi ricontrollo");
              digitalWrite(ledblu, HIGH);
              digitalWrite(ledgiallo, LOW);
              delay(4000);

              //Ricontrollo se la borraccia è ancora li
              ultrasuoni();

              //se la borraccia è ancora li
              if (distanza < distaborraccia) {
                digitalWrite(ledblu, LOW);
                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Inizio a erogare");
                lcd.setCursor(1, 2);
                lcd.print("il pre allenamento");
                Serial.println("OK, la borraccia è in posizione, inizio a erogare fra:");
                Serial.print("3 ");
                delay(500);
                Serial.print("2 ");
                delay(500);
                Serial.print("1 ");
                delay(500);
                Serial.println("GO");

                //###Fine Firmware degli ultrasuoni###


                //###Inizio Firmware motore proteine###
                ultrasuoni();


                //controllo a quanti millis sono
                tempo_now = millis();




                //finché i millis attuali sono minori dei millis attuali+ il periodo che deve aspettare
                while (distanza <= distaborraccia && millis() < tempo_now + quantogusto)
                {
                  ultrasuoni();
                  Serial.println(distanza);
                  //apri transistor pompa
                  digitalWrite(bridgepompa, HIGH);
                  Serial.println("Erogo acqua");
                  digitalWrite(bridgemotoregusto, HIGH);
                  Serial.println("Erogo gusto");
                }
                digitalWrite(bridgepompa, LOW);
                digitalWrite(bridgemotoregusto, LOW);

                long unsigned tempo_now2 = millis();

                while (distanza <= distaborraccia && millis() < tempo_now2 + quantopost )
                {
                  ultrasuoni();
                  Serial.println(distanza);
                  //apri transistor pompa
                  Serial.println("Erogo post");
                  digitalWrite(bridgemotorepre, HIGH);
                  Serial.println("Erogo acqua");
                  digitalWrite(bridgepompa, HIGH);
                }
                digitalWrite(bridgepompa, LOW);
                digitalWrite(bridgemotorepre, LOW);

                long unsigned tempo_now3 = millis();

                while (distanza <= distaborraccia && millis() < tempo_now3 + quantaacqua ) {
                  ultrasuoni();
                  Serial.println(distanza);
                  digitalWrite(bridgepompa, HIGH);
                  Serial.println("Erogo acqua rimanente");
                }



                digitalWrite(bridgepompa, LOW);
                Serial.println("Finito di erogare acqua");
                //chiudi transistor

                loopon = 1; //Dico che ha erogato acqua così esce dal loop degli ultrasuoni
                delay(2000);

                lcd.clear();
                lcd.setCursor(1, 1);
                lcd.print("Ritira la bevanda");
                ultrasuoni();
                while (distanza <= distaborraccia) {
                  ultrasuoni();
                }
              }

            }

            //###Fine Firmware della pompa acqua###

            //### Firmware di controllo degli ultrasuoni###


            //se la borraccia non è in posizione accendo led giallo
            else {
              digitalWrite(ledblu, LOW);
              digitalWrite(ledgiallo, HIGH);
            }

            // se la distanza è fuori dal range dell'ultrasuono
            if (distanza >= 200 || distanza <= 0) {
              Serial.print("Troppo lontano:");
              Serial.print(distanza);
              Serial.println(" cm");
            }
            //se e dentro al range
            else {
              Serial.print(distanza);
              Serial.println(" cm");
            }

            delay(500);

            //### Fine Firmware di controllo degli ultrasuoni###



          }
        }
        loopon = 0;
        delay(3000);
        lcd.clear();
        Serial.println("Simulazione del pre finita");
        lcd.setCursor(6, 1);
        lcd.print("A presto");
        delay(2000);
        digitalWrite(ledgenerali, LOW);
        standby();



      }
      else {
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("Abbonamento ");
        lcd.setCursor(2, 2);
        lcd.print("non riconosciuto");
        Serial.println("Chiave non valida, accendo led rosso");
        digitalWrite(ledRed, HIGH);
        delay(3000);
        digitalWrite(ledRed, LOW);
        digitalWrite(ledgenerali, LOW);
        standby();

      }
    }
  }

}


void standby() {
  lcd.setCursor(2, 0);
  lcd.print("Ciao!");
  lcd.setCursor(2, 1);
  lcd.print("Avvicina il tuo");
  lcd.setCursor(2, 2);
  lcd.print("smartphone o la");
  lcd.setCursor(2, 3);
  lcd.print("tessera");
  Serial.print("Accensione led con RFID Pronto");
  //###Fine Firmware RFID###
}

void ultrasuoni() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durata = pulseIn(echoPin, HIGH);
  distanza = (durata / 2) / 29.1;


}
