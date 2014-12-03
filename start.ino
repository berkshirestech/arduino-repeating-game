const int buttonPin = 2;     // the number of the pushbutton pin
const int outputLedPin = 13;      // the number of the LED pin
const int recordingLedPin = 11;
const int playbackLedPin = 12;
const int ACTION_LENGTH = 5;
const int TONE_PIN = 5;

enum ActionType {
  FLASH, PAUSE
};

struct LedAction {
  long length;
  ActionType type;
};

LedAction recordedActions[ACTION_LENGTH] = {
};
LedAction generatedActions[ACTION_LENGTH] = {
};
int count = 0;
bool isRecording = true;

void playback(struct LedAction ledActions[])
{
  int numActions = ACTION_LENGTH;

  for (int i = 0; i < numActions; i++)  {
    LedAction action = ledActions[i];
    if(action.type == PAUSE) {
      delay(action.length);
    } 
    else {
      flash(action.length);
    }
  }
}

int lastTime;

void record()
{
  if(digitalRead(buttonPin) == LOW){
    if(count > 0) {
      int currTime = millis();
      LedAction pause = {
        currTime - lastTime, PAUSE      };  
      recordedActions[count] = pause;
      count++; 
    }

    long holdStart = millis();
    while(digitalRead(buttonPin) == LOW);
    long holdEnd = millis();
    lastTime = holdEnd;
    LedAction flash = {
      holdEnd - holdStart, FLASH    };  
    recordedActions[count] = flash; 
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

void turnOnLed(int pin) {
  digitalWrite(pin, HIGH);
}

void turnOffLed(int pin) {
  digitalWrite(pin, LOW);
}

void flash(long flashLength)
{
  tone(TONE_PIN, 800);
  flashLed(outputLedPin, flashLength);
  noTone(TONE_PIN);
}

boolean compareArrays(){
  for(int i=0; i<ACTION_LENGTH; i++){
    LedAction genAction = generatedActions[i];
    LedAction recAction = recordedActions[i];
    if(genAction.type == PAUSE) continue;//don't compare pause durations
    long diff = genAction.length - recAction.length;
    if(diff < -400 || diff > 400){
      return false;
    }
  }
  return true;
}

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  // initialize the pins
  pinMode(outputLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(recordingLedPin, OUTPUT);
  pinMode(playbackLedPin, OUTPUT);
  pinMode(TONE_PIN, OUTPUT);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  generatePattern();
  playback(generatedActions);
}

void generatePattern(){
  ActionType currType = FLASH;
  for(int i=0;i<ACTION_LENGTH;i++){
    long duration = currType == PAUSE ? 500 : random(1, 3) == 1 ? 300 : 1000;
    LedAction action = {
      duration, currType    };
    generatedActions[i] = action;
    currType = currType==FLASH?PAUSE:FLASH;
  }
}

void loop() {
  boolean success;
  if(isRecording){
    turnOffLed(playbackLedPin);
    turnOnLed(recordingLedPin);
    record();
  }
  else{
    turnOffLed(recordingLedPin);
    turnOnLed(playbackLedPin);
    if(success = compareArrays()){
      tone(TONE_PIN,1200,1000);
    }
    else{
      tone(TONE_PIN,80,1000);
    }
    delay(2000);

    if(success) {
      generatePattern();
      playback(generatedActions);
    } else
    {
      playback(recordedActions);
      delay(1000);
      playback(generatedActions);
    }
    
    isRecording = true;
  }
}

