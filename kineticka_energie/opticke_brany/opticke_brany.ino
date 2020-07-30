/*
 * Kód byl napsán za účelem měření rychlosti projektilu. K tomu byli využity dvě optické brány FC-33.
 * Autor: Jaroslav Karlík
 * Datum: 30. 7. 2020
 * Licence: veřejná
 * Kód byl použit v bakalářské práci Vybranené školní experimenty z mechaniky s využitím Arduina
 * 
 */



#define startGate 2 //První brána je připojena na pin 2
#define endGate 3  //Druhá brána na pin 3
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
  pinMode(startGate, INPUT);  //startGate i endGate jsou vstupy
  pinMode(endGate, INPUT);
  attachInterrupt(digitalPinToInterrupt(startGate), UpdateTimeStart, FALLING); //Při přerušení paprsku v první bráně volá funkci UpdateTimeStart
  attachInterrupt(digitalPinToInterrupt(endGate), UpdateTimeEnd, FALLING);     //Po přerušení druhého paprsku volá fci UpdateTimeEnd
}
 
void loop() {
  if (startTime > 0 && endTime > 0 && bang == false){
    noInterrupts();
    bulletTime = endTime - startTime;   //Vypočte čas letu projektilu mezi branami.
    Serial.print("Start: ");           //Všechny hodnoty se vypíší na sériový monitor
    Serial.print(startTime);
    Serial.print(" End: ");
    Serial.print(endTime);
    Serial.print(" Time: ");
    Serial.println(bulletTime);
    interrupts();
    bang = true;
  }
  if(millis() > 20000){               //Po uplynutí 20 sekund se do sériového monitoru vypíší všechny hodnoty, měření je potřeba stihnout před tím.
        Serial.print("Start: ");
        Serial.print(startTime);
        Serial.print(" End: ");
        Serial.print(endTime);
        Serial.print(" Time: ");
        Serial.println(bulletTime);}
}
 
 
void UpdateTimeStart() {
  startTime = micros();   //Funkce micros() vypíše uplynulý čas od spuštění programu v microsekundách. startTime je čas průchodu první branou
} 
 
void UpdateTimeEnd() {
  endTime = micros();    //Průchod druhou branou v microsekundách
}
