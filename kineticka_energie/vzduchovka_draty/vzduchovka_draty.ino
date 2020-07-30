/*
 * Kód byl napsán za účelem měření rychlosti projektilu.
 * Měření probíhalo tak, že za sebe do známé vzdálenosti byly postaveny dva měděné drátky a poté byly přestřeleny.
 * Po přestřelení drátu, dojde k přerušení obvodu a k úbytku napětí, které je pro Arduino signálem k zaznamenání času.
 * Autor: Jaroslav Karlík
 * Datum: 30. 7. 2020
 * Licence: veřejná
 * Kód byl použit v bakalářské práci Vybranené školní experimenty z mechaniky s využitím Arduina
 * 
 */


#define startGate 2 //První brána je připojena na pin 2
#define endGate 3   //Druhá brána na pin 3
                    // Tyto piny podporují funkci "attachInterrupt"
 
//stanovení proměnných 
volatile long startTime = 0;
volatile long endTime = 0;
long bulletTime = 0;
float bulletSpeed = 0;
bool bang = false; 
 
void setup()
{
  Serial.begin(9600);
  pinMode(startGate, INPUT_PULLUP); //Funkce INPUT_PULLUP využívá vnitřní rezistor v Arduinu.
  pinMode(endGate, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startGate), UpdateTimeStart, RISING); //Při přerušení prvního drátku volá funkci UpdateTimeStart
  attachInterrupt(digitalPinToInterrupt(endGate), UpdateTimeEnd, RISING);     //Po přerušení druhého drátku volá fci UpdateTimeEnd

  //Po spuštění programu vypíše do sériového monitoru, zda drátky protéká proud a jsou-li tedy uzavřeny obvody.
  if (digitalRead(startGate) == LOW){
      Serial.println("startGate ready");}
      else{Serial.println("startGate problem");}
  if (digitalRead(endGate) == LOW){
      Serial.println("endGate ready");}
      else{Serial.println("endGate problem");}  
}
 
void loop() {
  if (startTime > 0 && endTime > 0 && bang == false){
    noInterrupts();
    bulletTime = endTime - startTime;                 //Vypočte čas letu projektilu mezi drátky.
    Serial.print("Start: ");
    Serial.print(startTime);
    Serial.print(" End: ");
    Serial.print(endTime);
    Serial.print(" Time: ");
    Serial.println(bulletTime);
    interrupts();
    bang = true;
  }
  if(millis() > 20000){              //Po uplynutí 20 sekund se do sériového monitoru vypíší všechny hodnoty, měření je potřeba stihnout před tím.
        Serial.print("Start: ");
        Serial.print(startTime);
        Serial.print(" End: ");
        Serial.print(endTime);
        Serial.print(" Time: ");
        Serial.println(bulletTime);}
}
 
 
void UpdateTimeStart() {
  startTime = micros(); //Funkce micros() vypíše uplynulý čas od spuštění programu v microsekundách. startTime je čas přestřelení prvního drátku
}
 
void UpdateTimeEnd() {
  endTime = micros();   //Čas přestřelení druhého drátku v microsekundách
}
