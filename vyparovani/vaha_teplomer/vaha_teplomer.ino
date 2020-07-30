/*
 * Autor: Jaroslav Karlík
 * Pro napsání byly využity kódy Nathana Seidla(SparkFun Electronics, November 19th, 2014) a Arduino DS18B20 temp sensor tutorial
 * Licence: veřejná
 */
 


#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define calibration_factor -7050 /*-7050.0*/ //Do tohoto řádku zadejte kalibrační faktor, nejnižší je -7050

#define DOUT  3         //Výstup z převodníku s označením DOUT nebo DT připojte k pinu 3
#define CLK  2          //Výstup s označením CLK nebo SCK připojte k pinu 2
#define ONE_WIRE_BUS 4  //Výstup signálu z teploměru připojte k pinu 4

HX711 scale;

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);



void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");
  sensors.begin();

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();                         //Při spuštění nastaví váhu na 0. Při spuštění nesmí být váha zatížena.

}

void loop() {
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0)); //Vypíše do sériového monitoru teplotu ve °C
  Serial.print(scale.get_units(), 1);         //Vypíše hmotnost v g
  Serial.println();                           //Zalomí řádek
  delay(5000);                                //Počká 5 sekund před vypsáním další hodnoty
}
