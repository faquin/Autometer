#Autometer kitchen timer
## Presentation
This project started in India where I bought a rickshaw autometer. I got the idea of opening it and using its display somehow.
After some investigation of the current circuit, I replaced the orignial Atmega by an arduino micro.

I created a kitchen timer with it. It is working great:

1. At first, the Arduino is sleeping
2. When the back button is pressed, the Arduino wakes up and increment the number of minutes to count. The button can be pressed several times
3. When the handle is triggered, the countdown starts
4. When the countdown is 0, the buzzer activates until the handle is reset
5. Finally the Arduino goes back to sleep

## Files
### Autometer.ino
The Arduino code. It uses libraries
- RBD_Timer from http://robotsbigdata.com/docs-arduino-timer.html
- RBD_Button from http://robotsbigdata.com/docs-arduino-button.html
- SoftwareSerial from https://www.arduino.cc/en/Reference/softwareSerial
- TimerOne from http://playground.arduino.cc/Code/Timer1

### Autometer.fzz
The adaptater I had to make between the Atmega connector and the Arduino pins.

## Existing circuit
See the [dedicated page](existing-circuit.md)
