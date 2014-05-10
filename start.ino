const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

long flashLength = 100; // milliseconds
long pauses[] = { 1000, 250, 250, 500, 250};
int count = 0;
bool isRecording = true;

void playback()
{
  int numPauses = sizeof(pauses)/sizeof(pauses[0]);

  for (int i = 0; i < numPauses; i++)
  {
    flash();
    delay(pauses[i]);
  }
  flash();
}

int lastTime;

void record()
{
  if(digitalRead(buttonPin) == LOW){
    int currTime = millis();
    if(count > 0) {
      pauses[count] = currTime - lastTime;
    }

    lastTime = currTime;
    while(digitalRead(buttonPin) == LOW);
    count++;
    if(count == 6){
        count = 0;
        isRecording = false;
    }
  }
}

void flash()
{
 digitalWrite(ledPin, HIGH);
 delay(flashLength);
 digitalWrite(ledPin, LOW);
}

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  // initialize the pins
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if(isRecording){
    record();
  }else{
    playback();
    isRecording = true;
  }
}
