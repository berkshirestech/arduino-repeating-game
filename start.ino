const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int ACTION_LENGTH = 17;
struct LedAction {
  long length;
  int type; // 0 = pause 1 = flash
};

LedAction ledActions[ACTION_LENGTH] = {};
int count = 0;
bool isRecording = true;

void playback()
{
  int numActions = sizeof(ledActions)/sizeof(ledActions[0]);

  for (int i = 0; i < numActions; i++)  {
    LedAction action = ledActions[i];
    if(action.type == 1) { // pause
      delay(action.length);
    } else {
      flash(action.length);
    }
  }
}

int lastTime;

void record()
{
  if(digitalRead(buttonPin) == LOW){
    int currTime = millis();
    if(count > 0) {
      LedAction pause = {currTime - lastTime, 0};  
      ledActions[count] = pause;
      count++; 
    }

    lastTime = currTime;
    long holdStart = millis();
    while(digitalRead(buttonPin) == LOW);
    
    LedAction flash = {millis() - holdStart, 1};  
    ledActions[count] = flash; 
    count++;
    
    if(count == ACTION_LENGTH){
        count = 0;
        isRecording = false;
    }
  }
}

void flash(long flashLength)
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
