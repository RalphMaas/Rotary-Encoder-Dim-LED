/*
 Rotary Encoder on Arduino UNO
 CLK - Pin A (GPIO 3)
 DT   -  Pin B (GPIO 4)
 SW  -  Push Button Switch (GPIO 8)
 +      -  5 volts (From the Arduino)
 GND -  Ground (From the Arduino)
 
 LED on 
 Anode - GRN
 Cathode - Pin 11
*/

// Used for generating interrupts using CLK signal
const int PinA = 2;

// Used for reading DT signal
const int PinB = 3;

// Used for the push button switch
const int PinSW = 8;

//simple PWN LED pin
const int PinLED = 11;

// Keep track of last rotary value
int lastCount = 20;

// Updated by the ISR (Interrupt Service Routine)
volatile int virtualPosition = 20;

// ------------------------------------------------------------------
// INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT
// ------------------------------------------------------------------
void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(PinB) == LOW)
    {
      virtualPosition-- ; // Could be -5 or -10
    }
    else {
      virtualPosition++ ; // Could be +5 or +10
    }

    // Restrict value from 0 to +100
    virtualPosition = min(20, max(0, virtualPosition));


  }
  // Keep track of when we were here last (no more than every 5ms)
  lastInterruptTime = interruptTime;
}

// ------------------------------------------------------------------
// SETUP    SETUP    SETUP    SETUP    SETUP    SETUP    SETUP
// ------------------------------------------------------------------
void setup() {
  // Just whilst we debug, view output on serial monitor
  Serial.begin(115200);

  // Rotary pulses are INPUTs
  pinMode(PinA, INPUT);
  pinMode(PinB, INPUT);

  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(PinSW, INPUT_PULLUP);

  // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(PinA), isr, LOW);

  // Ready to go!
  Serial.println("Start");
}

// ------------------------------------------------------------------
// MAIN LOOP     MAIN LOOP     MAIN LOOP     MAIN LOOP     MAIN LOOP
// ------------------------------------------------------------------
void loop() {

  // Is someone pressing the rotary switch?
  if ((!digitalRead(PinSW))) {
    virtualPosition = 20;
    while (!digitalRead(PinSW))
      delay(10);
    Serial.println("Reset");
  }

  // If the current rotary switch position has changed then update everything
  if (virtualPosition != lastCount) {

    // Write out to serial monitor the value and direction
    Serial.print(virtualPosition > lastCount ? "Up  :" : "Down:");
    Serial.println(virtualPosition);

    // Keep track of this new value
    lastCount = virtualPosition ;
  }
  analogWrite(PinLED, virtualPosition);
  //delay(100);
}
