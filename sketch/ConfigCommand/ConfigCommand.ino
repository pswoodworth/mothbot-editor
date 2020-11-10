

int delayCounter = 0;
int blinkValue = HIGH;
int blinkInterval = 1000;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(5, OUTPUT);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

// the loop function runs over and over again forever
void loop() {
  while(delayCounter < blinkInterval){
    checkSerial();
    digitalWrite(5, blinkValue);
    delay(1); 
    delayCounter++;
  }
  delayCounter = 0;
  blinkValue = blinkValue == HIGH ? LOW : HIGH;
}

void checkSerial(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    blinkInterval = Serial.parseInt(SKIP_ALL);

    Serial.print("Setting blink interval: ");
    Serial.println(blinkInterval);
  }
}
