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
  pinMode(startGate, INPUT_PULLUP);
  pinMode(endGate, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startGate), UpdateTimeStart, RISING);
  attachInterrupt(digitalPinToInterrupt(endGate), UpdateTimeEnd, RISING);
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
