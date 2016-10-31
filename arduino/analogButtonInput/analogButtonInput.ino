 
 void setup() {
   pinMode(A0, INPUT);
   pinMode(13, OUTPUT);
   Serial.begin(9600);
 }

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(A0);
  Serial.println(reading);

}

