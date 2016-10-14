
const int stopButtonPin = 4;     
const int goButtonPin = 2;
const int stopLedPin =  8;
const int goLedPin =  12;
const int buttonThreshold = 1000; //how manyframes we need to see the button pressed to trigger


int stopButtonCounter = 0;
int goButtonCounter = 0;
int stopButtonState = 0;
int goButtonState = 0;


void setup() {
  // initialize the LEDs pin as an output:
  pinMode(stopLedPin, OUTPUT);
  pinMode(goLedPin, OUTPUT);

  // initialize the pushbuttons pin as an input:
  pinMode(stopButtonPin, INPUT);
  pinMode(goButtonPin, INPUT);
}

void handleInputs()
  {
    //track state for the 'stop' button
    if (digitalRead(stopButtonPin) == 1){
      stopButtonCounter++;
      if (stopButtonCounter > buttonThreshold)
        {
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
      if (goButtonCounter > buttonThreshold && stopButtonState == 0)
        {
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
}
