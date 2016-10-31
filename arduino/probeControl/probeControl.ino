#include <SoftwareSerial.h>

const int stopButtonPin = 4;     
const int goButtonPin = 2;
const int stopLedPin =  8;
const int goLedPin =  12;
const int backwardLedPin =  13;
const int forwardLedPin =  7;
const int buttonThreshold = 10000; //how many frames we need to see the button pressed to trigger
const int stopButtonThreshold = 1000; //how many frames we need to see the stop button pressed (much more sensitive)
const int blinkInterval = 47;
const int displayInterval = 103;
long defaultExecutionTime = 10000; //time, in milliseconds, for the total execution


SoftwareSerial mySerial(3,6); // pin 3 = RX (unused), pin 2 = TX, 

int movesArray[] = {10, -5, -5}; //the in, out, out steps
const int movesCount = 3;
int moving = 0; //are we in the act of completing a move
int moveIndex = -1; //the index in the array we should be on
int depth = 0;
char depthString[10];
char speedString[10];

int executionTime = defaultExecutionTime;
int stepInterval;
int speedUpButtonCounter = 0;
int speedDownButtonCounter = 0;
int stopButtonCounter = 0;
int goButtonCounter = 0;
int stopButtonState = 0;
int goButtonState = 0;
unsigned long stepPreviousMillis = 0;
unsigned long blueLedPreviousMillis = 0;
unsigned long displayPreviousMillis = 0;


void clearDisplay()
  {
    moveCursor(128);
    mySerial.write("                "); // clear display
    mySerial.write("                ");
    moveCursor(128);
    mySerial.write("Depth: ");
    moveCursor(192);
    mySerial.write("Speed: ");
    moveCursor(205);
    mySerial.write("sec");
  }

void moveCursor(int location)
  {
    mySerial.write(254);
    mySerial.write(location);
  }

void updateDisplay()
  {
    moveCursor(136);
    sprintf(depthString,"%4d",depth);
    mySerial.write(depthString);
    moveCursor(141);
    if (moveIndex == -1 || movesArray[moveIndex] > 0){
        mySerial.write('v');
      } else {
        mySerial.write('^');
      }    
    moveCursor(200);
    sprintf(speedString,"%4d",(executionTime/1000));
    mySerial.write(speedString);
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


void handleInputs()
  {
    //track state for the 'stop' button
    if (digitalRead(stopButtonPin) == 1){
      stopButtonCounter++;
      if (stopButtonCounter > stopButtonThreshold){
          stopButtonState = 1;
          return;
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
    int reading = analogRead(A0);
    if (reading > 1005){//white button, 100ohm
      speedUpButtonCounter++;
      if (speedUpButtonCounter > buttonThreshold && stopButtonState == 0){
          executionTime -= 2000;
          speedUpButtonCounter = 0;
        }
    } else if (reading > 900 && reading < 1000){
      speedDownButtonCounter++;
      if (speedDownButtonCounter > buttonThreshold && stopButtonState == 0){
          executionTime += 2000;
          speedDownButtonCounter = 0;
        }
    }

    
   
  }

void setup() {
  // initialize the LEDs pin as an output:
  pinMode(stopLedPin, OUTPUT);
  pinMode(goLedPin, OUTPUT);
  pinMode(forwardLedPin, OUTPUT);
  pinMode(A0, INPUT);
  // initialize the pushbuttons pin as an input:
  pinMode(stopButtonPin, INPUT);
  pinMode(goButtonPin, INPUT);
  mySerial.begin(9600); // set up serial port for 9600 baud
  delay(500); // wait for display to boot up
  clearDisplay();
  updateDisplay();
}

void loop() {
  stepInterval = executionTime/movesArray[0];
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
