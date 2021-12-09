
//www.diyusthad.com
//www.youtube.com/c/diyusthad
//www.facebook.com/diyusthad
//program by diyusthad
//code for programming multiple functions to a single push button
#define sw 17 //pin at push sw is connected
#define red 21
#define green 19
#define blue 18
#define MULTI_FX 32
#define fx2 33

int freq = 2000;
int channel = 0;
int resolution = 8;
  
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
bool buttonState = LOW; //saving state of the switch
byte tapCounter; //for saving no. of times the switch is pressed
int timediff; //for saving the time in between each press and release of the switch
bool flag1, flag2; //just two variables
long double presstime, releasetime; //for saving millis at press and millis at release
void setup() {
  pinMode(MULTI_FX, OUTPUT); digitalWrite (MULTI_FX, HIGH);
  delay(1000);
  pinMode(fx2, INPUT);
  Serial.begin(115200); //for serial monitor
  pinMode(sw, INPUT_PULLUP); //setting pin 5 as input with internal pull up resistor, avoid floating
  pinMode(red, OUTPUT); digitalWrite(red, HIGH);
  pinMode(green, OUTPUT); digitalWrite(green, HIGH);
  pinMode(blue, OUTPUT); digitalWrite(blue, HIGH);

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(2, channel);
}

void loop() {

  //float i=100.0f*digitalRead(27)/1024.0f;
  //Serial.println(i);
  //delay(25);

  int reading = digitalRead(sw);
  int reading_2 = digitalRead(fx2);

  if (reading_2 != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading_2 != buttonState) {
      buttonState = reading_2;
    }
  }
  //Serial.println(buttonState);

  //when switch is pressed
  if (buttonState == 1 && flag2 == 0)
  {
    presstime = millis(); //time from millis fn will save to presstime variable
    flag1 = 0;
    flag2 = 1;
    tapCounter++; //tap counter will increase by 1
    delay(10); //for avoiding debouncing of the switch
  }
  //when sw is released
  if (buttonState == 0 && flag1 == 0)
  {
    releasetime = millis(); //time from millis fn will be saved to releasetime var
    flag1 = 1;
    flag2 = 0;

    timediff = releasetime - presstime; //here we find the time gap between press and release and stored to timediff var
    //Serial.println(timediff);
    delay(10);
  }

  if ((millis() - presstime) > 400 && buttonState == 0) //wait for some time and if sw is in release position
  {
    if (tapCounter == 1) //if tap counter is 1
    {
      if (timediff >= 400) //if time diff is larger than 400 then its a hold
      {
        Serial.println("Hold");
        hold(); //fn to call when the button is hold
      }
      else //if timediff is less than 400 then its a single tap
      {
        Serial.println("single tap");
        singleTap(); //fn to call when the button is single taped
      }
    }
    else if (tapCounter == 2 ) //if tapcounter is 2
    {
      if (timediff >= 400) // if timediff is greater than  400 then its single tap and hold
      {
        Serial.println("single tap and hold");
        tapAndHold(); //fn to call when the button is single tap and hold
      }
      else // if timediff is less than 400 then its just double tap
      {
        Serial.println("double tap");
        doubleTap(); //fn to call when doubletap
      }
    }
    else if (tapCounter == 3) //if tapcounter is 3 //then its triple tap
    {
      Serial.println("triple tap");
      tripleTap(); //fn to call when triple tap
    }
    else if (tapCounter == 4) //if tapcounter is 4 then its 4 tap
    {
      Serial.println("four tap");
      fourTap();//fn to call when four tap
    }
    tapCounter = 0;
  }
  lastButtonState = reading_2;
}
void nolight()
{
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  int freq=0;
  ledcWriteTone(channel, freq);
}
void singleTap()
{
  nolight();
  digitalWrite(MULTI_FX, LOW);
}
void doubleTap()
{
  nolight();
  digitalWrite(green, HIGH);
  
  ledcWrite(channel, 125);
    for (int freq = 1000; freq < 3000; freq = freq + 250){
  
     Serial.println(freq);
  
     ledcWriteTone(channel, freq);
     delay(1000);
    }
}

void tripleTap()
{
  nolight();
  digitalWrite(blue, HIGH);
}
void fourTap()
{
  nolight();
  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
}
void hold()
{
  nolight();
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
}
void tapAndHold()
{
  nolight();
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}
