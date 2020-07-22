#define startGate 2
#define endGate 3
 
 
volatile long startTime = 0;
volatile long endTime = 0;
long bulletTime = 0;
float bulletSpeed = 0;
bool bang = false; 
 
void setup()
{
  Serial.begin(9600);
  pinMode(startGate, INPUT);
  pinMode(endGate, INPUT);
  attachInterrupt(digitalPinToInterrupt(startGate), UpdateTimeStart, FALLING);
  attachInterrupt(digitalPinToInterrupt(endGate), UpdateTimeEnd, FALLING);
  if (digitalRead(startGate) == HIGH){
      Serial.println("startGate ready");}
      else{Serial.println("startGate problem");}
  if (digitalRead(endGate) == HIGH){
      Serial.println("endGate ready");}
      else{Serial.println("endGate problem");}  
}
 
void loop() {
  if (startTime > 0 && endTime > 0 && bang == false){
    noInterrupts();
    bulletTime = endTime - startTime;
    //bulletSpeed = ((float)6,36)/(float)bulletTime;
    Serial.print("Start: ");
    Serial.print(startTime);
    Serial.print(" End: ");
    Serial.print(endTime);
    Serial.print(" Time: ");
    Serial.println(bulletTime);
    //Serial.print(" Speed: ");
    //Serial.print(bulletSpeed);
    //Serial.println(" mm/ms");
    interrupts();
    bang = true;
  }
  if(millis() > 20000){
        Serial.print("Start: ");
        Serial.print(startTime);
        Serial.print(" End: ");
        Serial.print(endTime);
        Serial.print(" Time: ");
        Serial.println(bulletTime);}
}
 
 
void UpdateTimeStart() {
  startTime = micros();
}
 
void UpdateTimeEnd() {
  endTime = micros();
}
