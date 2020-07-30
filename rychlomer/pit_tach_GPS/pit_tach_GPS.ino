/*Program pro měření rychlosti Pitotovou trubicí,Hallovým senzorem a GPS.
 *Součástí je i záznam na SD kartu a zobrazení na display.
 *Kód byl použit v bakalářské práci Vybrané školní experimenty z mechaniky s využitím Arduina. 
 *
 *Vypracoval: Jaroslav Karlík
 *Licence: veřejná
 *
 */

//použité knihovny
#include <SPI.h>
#include <SD.h>
#include <NMEAGPS.h>
#include <GPSport.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C
 
//display
SSD1306AsciiAvrI2c oled;

//SD karta
const int chipSelect = 4; //připojení modulu SD karty: CS-4, SCK-13, MOSI-11, MISO-12

//pitotova trubice:
float V_0 = 5.0;
float rho = 1.204; //hustora vzduchu
int offset = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;

//otáčkoměr:
const int interruptPin = 3; //Digitální výstup Hallova senzoru připojit na pin 3
const int debounce = 50; //Minimalni cas, ktery musí mít pulz, aby nebyl označen za šum
volatile long lastMillis = 0; //Všechny proměnné, které se mají měnit ve funkci volané přerušením, musí mít před sebou 'volatile'
volatile long newMillis = 0;
volatile long pulseTime = 0; //Uloží rozdíl času mezi pulzy
volatile long millisDiff = 0; //Uloží rozdíl času mezi pulzy do rozhodnutí, jestli jde o šum nebo ne
float TachoSpeed = 0;

//GPS
//připojení GPS k pinům:
//rxd na gps - 9
//txd na gps - 8 (piny jsou definovány v knihovně)

NMEAGPS  gps; // This parses the GPS characters
gps_fix  fix; // This holds on to the latest values
float GPSpeed = 0;

void setup() {
 Serial.begin(9600);
 
//SDkarta                                             //Program funguje pouze pokud je vložena SD karta
  while (!Serial) {
  }

  Serial.print("Načítání SD karty...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Karta selhala, nebo není přítomna");
    while (1);
  }
  Serial.println("Karta načtena.");


//pitotova trubice
  for (int ii=0;ii<offset_size;ii++){
    offset += analogRead(A0)-(1023/2);
  }
  offset /= offset_size;
  Serial.print("AhojPit");
  
//tachometr:
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), UpdatePulseTime, FALLING); //Nastavené přerušení na pin 3, bude reagovat na klesající napětí a volat funkci UpdatePulseTime
  Serial.print("AhojTacho");

//GPS
  gpsPort.begin(9600);
  Serial.print("AhojGPS");
  
//display
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(TimesNewRoman16);
  Serial.print("AhojDis");
}

void loop() {

//GPS
  while (gps.available(gpsPort)) {
    fix = gps.read();
    GPSpeed = fix.speed_kph()/3,6;
    Serial.print("GPSeed: ");
    if (fix.valid.speed);
    //Serial.println(fix.speed_kph());
    Serial.print(GPSpeed);
    Serial.print(", ");
}
  
// pitotova trubice:
  float adc_avg = 0; float veloc = 0.0;
  
// average a few ADC readings for stability
  for (int ii=0;ii<veloc_mean_size;ii++){
    adc_avg+= analogRead(A0)-offset;
  }
  adc_avg/=veloc_mean_size;
  
  // make sure if the ADC reads below 512, then we equate it to a negative velocity
  if (adc_avg>512-zero_span and adc_avg<512+zero_span){
  } else{
    if (adc_avg<512){
      veloc = -sqrt((-10000.0*((adc_avg/1023.0)-0.5))/rho);
    } else{
      veloc = sqrt((10000.0*((adc_avg/1023.0)-0.5))/rho);
    }
  }
  Serial.print("PitSpeed: ");
  Serial.print(veloc); // print velocity
  Serial.print(", ");

//tachometr:
  //Serial.print("PULSE TIME: "); //vypis info bez noveho radku
  //Serial.print(pulseTime);  // vypis delku pulsu s novym radkem
  Serial.print("TachoSpeed:"); //vypis info bez noveho radku
  if (pulseTime == 0 || millis() - lastMillis > 3000) TachoSpeed = 0;
  else TachoSpeed = 3000/(float)pulseTime;
  Serial.println(TachoSpeed);

//SD karta
  File dataFile = SD.open("kal.csv", FILE_WRITE);

  if (dataFile) {                                         //zapíše všechna data na SD kartu
    dataFile.print(fix.dateTime.hours);
    dataFile.print(":");
    dataFile.print(fix.dateTime.minutes);
    dataFile.print(":");
    dataFile.print(fix.dateTime.seconds);
    dataFile.print(", ");
    dataFile.print(fix.latitudeL());
    dataFile.print(", ");
    dataFile.print(fix.longitudeL());
    dataFile.print(", ");
    dataFile.print(GPSpeed);
    dataFile.print(", ");
    dataFile.print(veloc);
    dataFile.print(", ");
    dataFile.println(TachoSpeed);
    dataFile.close();
    Serial.println("úspěšně zapsáno");
  }

//display print           //vypíše hodnoty na display
  oled.clear();
  oled.print("P: ");
  oled.println(veloc);
  //oled.println(" mps");
  oled.print("T: ");
  oled.println(TachoSpeed);
  //oled.println(" mps");
  oled.print("G: ");
  oled.println(GPSpeed);
  //oled.println(" mps");

  delay(100);           //počká desetinu sekundy
}

void UpdatePulseTime() {
  newMillis = millis(); //Aktualizuje čas
  millisDiff = newMillis - lastMillis; //Spočítá rozdíl od posledního uloženého času
  if (millisDiff > debounce){ //Pokud je rozdíl větší než 50ms, provedou se následující dva řádky, pokud je menší je to šum.
    pulseTime = millisDiff; //Aktualizuje délku pulzu
    lastMillis = newMillis; // a aktualizuje poslední uložený čas
    }
}
