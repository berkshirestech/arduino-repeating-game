const int buttonPin = 2;     // the number of the pushbutton pin
const int outputLedPin = 13;      // the number of the LED pin
const int successLedPin = 12;
const int failureLedPin = 11;
const int ACTION_LENGTH = 17;

enum ActionType {
  FLASH, PAUSE
};

struct LedAction {
  long length;
  ActionType type;
};

LedAction ledActions[ACTION_LENGTH] = {};
int count = 0;
bool isRecording = true;

void playback()
{
  int numActions = sizeof(ledActions)/sizeof(ledActions[0]);

  for (int i = 0; i < numActions; i++)  {
    LedAction action = ledActions[i];
    if(action.type == PAUSE) {
      delay(action.length);
    } else {
      flash(action.length);
    }
  }
}

int lastTime;

void record()
{
  if(count == 0) {
    success();
    failure();
    success();
  }
  if(digitalRead(buttonPin) == LOW){
    if(count > 0) {
      int currTime = millis();
      LedAction pause = {currTime - lastTime, PAUSE};  
      ledActions[count] = pause;
      count++; 
    }

    long holdStart = millis();
    while(digitalRead(buttonPin) == LOW);
    long holdEnd = millis();
    lastTime = holdEnd;
    LedAction flash = {holdEnd - holdStart, FLASH};  
    ledActions[count] = flash; 
    count++;
    
    if(count == ACTION_LENGTH){
        count = 0;
        isRecording = false;
    }
  }
}

void flashLed(int pin, long length) {
 digitalWrite(pin, HIGH);
 delay(length);
 digitalWrite(pin, LOW);
}

void success() {
  flashLed(successLedPin, 1000);
}

void failure() {
  flashLed(failureLedPin, 1000);
}

void flash(long flashLength)
{
  flashLed(outputLedPin, flashLength);
}

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  // initialize the pins
  pinMode(outputLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if(isRecording){
    record();
  }else{
    failure();
    success();
    failure();
    playback();
    isRecording = true;
  }
}
