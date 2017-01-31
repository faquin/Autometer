#include <RBD_Timer.h>
#include <RBD_Button.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>

#include <avr/sleep.h>

/*
  Version10 : Same as Version 6 for Arduino Micro.
  The user press the side button to increment a timer value. When the rotating handle is down, the counter starts to decrease.
  When it reaches 0, the buzzer activates until the roating handle is up again. This also shutdown the display.
*/

#define digitOFF 10

int pinA0 = A0;
int pinA1 = A1;
int pinA2 = A2;
int pinA3 = A4;
int pinSegmA = 7;
int pinSegmB = 8;
int pinSegmC = 9;
int pinSegmD = 10;
int pinSegmE = 11;
int pinSegmF = 12;
int pinSegmG = 1;
int pinEN = 0;
int pinButtonHandle = 4;
int pinButtonDP = 2;
int pinBuzzer = 3;

byte numbers[15][7] = {
  {0, 0, 0, 0, 0, 0, 1}, //zero
  {1, 0, 0, 1, 1, 1, 1}, //one
  {0, 0, 1, 0, 0, 1, 0}, //two
  {0, 0, 0, 0, 1, 1, 0}, //three
  {1, 0, 0, 1, 1, 0, 0}, //four
  {0, 1, 0, 0, 1, 0, 0}, //five
  {0, 1, 0, 0, 0, 0, 0}, //six
  {0, 0, 0, 1, 1, 1, 1}, //seven
  {0, 0, 0, 0, 0, 0, 0}, //eight
  {0, 0, 0, 0, 1, 0, 0}, //nine
  {1, 1, 1, 1, 1, 1, 1}, //off
  {0, 0, 0, 1, 0, 0, 0}, // A
  {0, 1, 1, 0, 0, 0, 1}, // C
  {0, 1, 1, 0, 0, 0, 0}, // E
  {0, 1, 1, 1, 0, 0, 0} // F
}; // manquent: H, L, n, o, P, U,

int DigitValue[13];
int isrActiveDigit = 0;
int count = 0;

int minutes = 0;
int secondes = 0;

//SoftwareSerial SerialParticle(A5, A4); // RX, TX

RBD::Button button_Handle(pinButtonHandle);
RBD::Button button_DP(pinButtonDP);

RBD::Timer timer_countdown(1000); // triggers every seconds // 250 for 4Mhz instead of 1000


void setDigitValue(int digit, int value) {
  // digit is from 0 to 12
  if (0 <= digit && digit < 13) {
    if (0 <= value && value < 15) {
      DigitValue[digit] = value;
    }
  }
}


void setDigitChar(int digit, char character) {
  // digit is from 0 to 12
  if (0 <= digit && digit < 13) {
    switch (character) {
      case 'A':
        DigitValue[digit] = 11;
        break;
      case 'C':
        DigitValue[digit] = 12;
        break;
      case 'E':
        DigitValue[digit] = 13;
        break;
      case 'F':
        DigitValue[digit] = 14;
        break;
    }
  }
}


void DigitOFF(int digit) {
  if (0 <= digit && digit < 13) {
    DigitValue[digit] = digitOFF;
  }
}

void setLineValue(int line, int digit1, int digit2, int digit3, int digit4, int digit5) {
  // line is 1 or 2 or 3
  if (line == 1) {
    setDigitValue(4, digit1);
    setDigitValue(3, digit2);
    setDigitValue(2, digit3);
    setDigitValue(1, digit4);
    setDigitValue(0, digit5);
  }
  else if (line == 2) {
    setDigitValue(8, digit1);
    setDigitValue(7, digit2);
    setDigitValue(6, digit3);
    setDigitValue(5, digit4);
  }
  else if (line == 3) {
    setDigitValue(12, digit1);
    setDigitValue(11, digit2);
    setDigitValue(10, digit3);
    setDigitValue(9, digit4);
  }
  
}

void setLineStr(int line, String strDigits) {
  // for line1, strDigit is 5 char (+1). For the other lines, StrDigit is 4 char (+1)
  int a, b, c, d, e;
  if (strDigits[0] == 32) // 32 is " " in ASCII
    a = digitOFF;
  else
    a = strDigits[0] - 48; //-48 because "0" is ASCII code 0x48

  if (strDigits[1] == 32)
    b = digitOFF;
  else
    b = strDigits[1] - 48;

  if (strDigits[2] == 32)
    c = digitOFF;
  else
    c = strDigits[2] - 48;

  if (strDigits[3] == 32)
    d = digitOFF;
  else
    d = strDigits[3] - 48;

  if (strDigits[4] == 32)
    e = digitOFF;
  else
    e = strDigits[4] - 48;

  setLineValue(line, a, b, c, d, e);
}

void wakeUpNow (void) {
  // do nothing: we are awake that's all!
}

void goToSleep() {
  setLineStr(1, "     ");
  setLineStr(2, "    ");
  setLineStr(3, "    ");
  delay(770);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(pinButtonDP), wakeUpNow, LOW);
  sleep_mode();
  sleep_disable();         // first thing after waking from sleep:
  detachInterrupt(digitalPinToInterrupt(pinButtonDP));

}


void setup() {
  Serial.begin(9600);
  //SerialParticle.begin(9600);

  pinMode(pinA0, OUTPUT);  digitalWrite(pinA0, 0);
  pinMode(pinA1, OUTPUT);  digitalWrite(pinA1, 0);
  pinMode(pinA2, OUTPUT);  digitalWrite(pinA2, 0);
  pinMode(pinA3, OUTPUT);  digitalWrite(pinA3, 0);

  pinMode(pinSegmA, OUTPUT);
  pinMode(pinSegmB, OUTPUT);
  pinMode(pinSegmC, OUTPUT);
  pinMode(pinSegmD, OUTPUT);
  pinMode(pinSegmE, OUTPUT);
  pinMode(pinSegmF, OUTPUT);
  pinMode(pinSegmG, OUTPUT);

  pinMode(pinEN, OUTPUT);

  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, HIGH); // switch off buzzer

  Timer1.initialize(770); // set a timer of length 770 microseconds (=100Hz/13 displays) // for 4Mhz quartz value is 192
  Timer1.attachInterrupt( isrUpdate7Seg ); // attach the service routine here

  setLineStr(1, "     ");
  setLineStr(2, "    ");
  setLineStr(3, "    ");

  //Serial.println("application starting v10.0");
  //delay(1000);


   goToSleep();
  
}

void loop() {


  if (button_Handle.onReleased()) {
    timer_countdown.stop();
    // setLineStr(1, " 0000"); useless since we are going to sleep just after
    minutes = 0;
    secondes = 0;
    goToSleep();

  }

  if (button_Handle.isReleased()) {
    if (button_DP.onPressed()) { // increment the counter at beginning
      if (minutes < 60) {
        minutes++;
        //secondes+=10; //debug
        setDigitValue(3, minutes / 10);
        setDigitValue(2, minutes % 10);
        setDigitValue(1, secondes / 10);
        setDigitValue(0, secondes % 10);
      }
    }
  }


  if (button_Handle.onPressed()) {
    timer_countdown.restart(); // launch the timer
  }

  if (button_Handle.isPressed()) {
    if (timer_countdown.onRestart()) {
      if (secondes > 0) {
        secondes--;
      }
      else {
        if (minutes > 0) {
          minutes--;
          secondes = 59;
        }
        else {
          timer_countdown.stop(); // countdown is expired
        }
      }


      setDigitValue(3, minutes / 10);
      setDigitValue(2, minutes % 10);
      setDigitValue(1, secondes / 10);
      setDigitValue(0, secondes % 10);

    }

    if (timer_countdown.isStopped()) { // as long as the countdown is stopped whilehandle is activated, buzzzz
      digitalWrite(pinBuzzer, LOW);
      delay(100);
      digitalWrite(pinBuzzer, HIGH);
      delay(3000); // let the buzzer stop
    }


  }


  /*Serial.print("min=");
  Serial.print(minutes);
  Serial.print(" sec=");
  Serial.println(secondes);
  */

}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void isrUpdate7Seg()
{
  digitalWrite(pinEN, HIGH); // disable display during bits modifications

  isrActiveDigit = (isrActiveDigit + 1) % 13;
  digitalWrite(pinA0, bitRead(isrActiveDigit, 0));
  digitalWrite(pinA1, bitRead(isrActiveDigit, 1));
  digitalWrite(pinA2, bitRead(isrActiveDigit, 2));
  digitalWrite(pinA3, bitRead(isrActiveDigit, 3));

  digitalWrite(pinSegmA, numbers[DigitValue[isrActiveDigit]][0]);
  digitalWrite(pinSegmB, numbers[DigitValue[isrActiveDigit]][1]);
  digitalWrite(pinSegmC, numbers[DigitValue[isrActiveDigit]][2]);
  digitalWrite(pinSegmD, numbers[DigitValue[isrActiveDigit]][3]);
  digitalWrite(pinSegmE, numbers[DigitValue[isrActiveDigit]][4]);
  digitalWrite(pinSegmF, numbers[DigitValue[isrActiveDigit]][5]);
  digitalWrite(pinSegmG, numbers[DigitValue[isrActiveDigit]][6]);

  digitalWrite(pinEN, LOW); // enable display
}
