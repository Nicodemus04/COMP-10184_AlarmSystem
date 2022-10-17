// COMP-10184 – Mohawk College 
// Alarm System
//  
// This program is used emulate an alarm system which is trigered by a motion sensor and can be deavtivated with a button
// 
// @author  Nicodemus Allan
// @id   000838957
//
//I, Nicodemus Allan, student number 000838957, certify that this material is my original work. No other person's work has been used without due acknowledgment and I have not made my work available to anyone else.
 
#include <Arduino.h> 
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6 

//possible allarm states
#define ALARM_DISABLED  0 
#define ALARM_ENABLE    1 
#define ALARM_COUNTDOWN 2 
#define ALARM_ACTIVE    3 

//time in mills till alarm goes off
#define ALARM_COUNTDOWN_DURATION 10000
//interval for LED flashing
#define LED_COUNTDOWN_DURATION 125
//time in mills since the program started
long alarmTimerStart;
//current mills counter for alarm activation
long alarmCurrentTime;

//time in mills since the program started
long timerStartLED;
//current mills counter for LED interval
long currentTimeLED;


//current allarm state
int  iAlarmState; 

//state of motion detector
bool bPIR;

//current state of button
int currentButton;
//previous button state
int previousButton = HIGH;

bool ledOn = false;

//***********************************************************************
//
//checks the button state to avoid it from retrigering code
//
//@return boolean if its the first loop the button was pressed on it returns true if any other returnes false
bool checkButtonState()
{
  if (currentButton == LOW && previousButton == HIGH)
  {
    previousButton = currentButton;
    return true;
  }
  else
  {
    previousButton = currentButton;
    return false;
  }
}
//
//************************************************************************

//************************************************************************
//
//checks which current state the alarm is in
void checkAlarmState()
{
  switch (iAlarmState)
  {
    case ALARM_DISABLED:
      //if the button was pressed re-enable the alarm
      if (checkButtonState())
      {
        iAlarmState = ALARM_ENABLE;
      }
      break;

    case ALARM_ENABLE:
      if (bPIR)
      {
        timerStartLED = millis();
        alarmTimerStart = millis();
        iAlarmState = ALARM_COUNTDOWN;
      }
      //if the button was pressed disables the alarm
      if (checkButtonState())
      {
        iAlarmState = ALARM_DISABLED;
      }
      break;

    case ALARM_COUNTDOWN:
      //get the amount of millis seince the program started
      alarmCurrentTime = millis();
      //gets the amount of millis sence program started
      
      //if current time - timerStart is greater than COUNTDOWN alarmstae is active
      if((alarmCurrentTime - alarmTimerStart) >= ALARM_COUNTDOWN_DURATION)
      {
        iAlarmState = ALARM_ACTIVE;
      }

      currentTimeLED = millis();
      //if current time - timerStart is greater than COUNTDOWN alarmstae is active
      if((currentTimeLED - timerStartLED) >= LED_COUNTDOWN_DURATION)
      {
        
        if (ledOn)
        {
          digitalWrite(LED_BUILTIN, HIGH); 
          ledOn = false;
        }
        else if (!ledOn)
        {
          digitalWrite(LED_BUILTIN, LOW);
          ledOn = true;
        }
        timerStartLED = currentTimeLED;
      }

      //if the button was pressed gets out of countdown state and sets timerStart to currentTime
      if (checkButtonState())
      {
        alarmTimerStart = alarmCurrentTime;
        iAlarmState = ALARM_DISABLED;
        digitalWrite(LED_BUILTIN, HIGH);
      }

      break;

    case ALARM_ACTIVE:

      //turns LED on
      digitalWrite(LED_BUILTIN, LOW); 
      break;
    
    default:
      break;
  }
}
//
//************************************************************************

//************************************************************************
//
//gets digital inputes
void getInputs()
{

  // read PIR sensor (true = Motion detected!).  As long as there 
  // is motion, this signal will be true.  About 2.5 seconds after  
  // motion stops, the PIR signal will become false. 
  bPIR = digitalRead(PIN_PIR); 

  currentButton = digitalRead(PIN_BUTTON); 
}
//
//************************************************************************



//************************************************************************
void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  //sets the alarm to be active on start up
  iAlarmState = ALARM_ENABLE;

  //sets LED to off
  digitalWrite(LED_BUILTIN, HIGH);
} 
//************************************************************************

//************************************************************************
void loop() { 
  getInputs();
  checkAlarmState();
} 
//************************************************************************
