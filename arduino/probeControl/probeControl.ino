#include <SoftwareSerial.h>

const int stopButtonPin = 4;     
const int goButtonPin = 2;
const int stopLedPin =  8;
const int goLedPin =  12;
const int backwardLedPin =  13;
const int forwardLedPin =  7;
const int buttonThreshold = 1000; //how many frames we need to see the button pressed to trigger
const int stepInterval = 449;
const int blinkInterval = 47;
const int displayInterval = 103;


SoftwareSerial mySerial(3,6); // pin 3 = RX (unused), pin 2 = TX, 

int movesArray[] = {10, -5, -5}; //the in, out, out steps
const int movesCount = 3;
int moving = 0; //are we in the act of completing a move
int moveIndex = -1; //the index in the array we should be on
int depth = 0;
char depthString[10];

int stopButtonCounter = 0;
int goButtonCounter = 0;
int stopButtonState = 0;
int goButtonState = 0;
unsigned long stepPreviousMillis = 0;
unsigned long blueLedPreviousMillis = 0;
unsigned long displayPreviousMillis = 0;


void clearDisplay()
  {
    resetCursor();
    mySerial.write("                "); // clear display
    mySerial.write("                ");
    resetCursor();
    mySerial.write("Depth: ");
  }

void resetCursor()
  {
    mySerial.write(254); // move cursor to beginning of first line
    mySerial.write(128);
  }

void updateDisplay()
  {
    mySerial.write(254);
    mySerial.write(136);
    sprintf(depthString,"%4d",depth);
    mySerial.write(depthString);
    mySerial.write(254);
    mySerial.write(141);
    if (moveIndex == -1 || movesArray[moveIndex] > 0){
        mySerial.write('v');
      } else {
        mySerial.write('^');
      }    
  }


void move() 
  {
    if (movesArray[moveIndex] > 0 ){
      movesArray[moveIndex]--;
      depth--;
    } else if (movesArray[moveIndex] < 0 ){
      movesArray[moveIndex]++;
      depth++;
    } else if (movesArray[moveIndex] == 0 ){
      stopMove();
    }
  }
  
void stopMove()
  {
    digitalWrite(forwardLedPin, 0);
    moving = 0;
  }

void startMove()
  {
    if (moveIndex == -1 || movesArray[moveIndex] == 0){
      moveIndex++;
    }
    moving = 1;
  }

void setup() {
  // initialize the LEDs pin as an output:
  pinMode(stopLedPin, OUTPUT);
  pinMode(goLedPin, OUTPUT);
  pinMode(forwardLedPin, OUTPUT);
  // initialize the pushbuttons pin as an input:
  pinMode(stopButtonPin, INPUT);
  pinMode(goButtonPin, INPUT);
  mySerial.begin(9600); // set up serial port for 9600 baud
  delay(500); // wait for display to boot up
  clearDisplay();
  updateDisplay();
}

void handleInputs()
  {
    //track state for the 'stop' button
    if (digitalRead(stopButtonPin) == 1){
      stopButtonCounter++;
      if (stopButtonCounter > buttonThreshold){
          stopButtonState = 1;
        }
      } 
      else 
      {
        stopButtonCounter = 0; 
        stopButtonState = 0;
      }
    //track state for the 'stop' button
    
    if (digitalRead(goButtonPin) == 1){
      goButtonCounter++;
      if (goButtonCounter > buttonThreshold && stopButtonState == 0){
          goButtonState = 1;
        }
      } 
      else 
      {
        goButtonCounter = 0; 
        goButtonState = 0;
      }
  }


void loop() {
  handleInputs();
  digitalWrite(stopLedPin, stopButtonState);
  digitalWrite(goLedPin, goButtonState);
    if (goButtonState == 1 && moving == 0)
    {
      startMove();
    } 
    else if (stopButtonState == 1)
    {
      stopMove();
    }

  
  unsigned long currentMillis = millis();
       
   // How much time has passed, accounting for rollover with subtraction!
   if ((unsigned long)(currentMillis - blueLedPreviousMillis) >= blinkInterval && moving) {
      digitalWrite(forwardLedPin, !digitalRead(forwardLedPin)); // Toggle the LED
       blueLedPreviousMillis = currentMillis;
   }
   if ((unsigned long)(currentMillis - displayPreviousMillis) >= displayInterval) {
    updateDisplay();
    displayPreviousMillis = currentMillis;
   }
   if ((unsigned long)(currentMillis - stepPreviousMillis) >= stepInterval && moving && moveIndex < movesCount ) {
    move();
    stepPreviousMillis = currentMillis;
   }
   


}
