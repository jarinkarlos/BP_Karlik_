#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define calibration_factor -1000 /*-7050.0*/ //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT  3
#define CLK  2

HX711 scale;

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);



void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  Serial.println("Arduino Digital Temperature // Serial Monitor Version"); //Print a message
  sensors.begin();

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

 // Serial.println("Readings:");
}

void loop() {
  sensors.requestTemperatures();
  //Serial.println(sensors.getTempCByIndex(0));
  //Serial.print(",");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.println();
  delay(5000);
}
