- Power Input
  - 12Vdc +/-25%
  - Regulated with KA7805 on Power Supply Board 2
  - 5V reach Display Board on CON1
- Display Board
  - Front
    - INPUT: information from the wheel, used to measure the KM
    - CON1: 5V+GND, see 1)
    - Buzzer is soldered
    - DP: External Push Button
    - MICRO: Switch activated by rotating the “For Hire” handle
    - ATMEL ATMEGA48PA-PU with quartz 4Mhz
    - It seems there is one I2C connector not soldered (maks SDA/SCL)
  - Back
    - The numeric displayed are soldered directly on the back. They all have a comma, to see if it is connected
    - There are:
      - One line with 5 big digits (E10561-G)
      - One line with 4 big digits (E10561-G)
      - One line with 4 small digits (E10391-G)
    - The displays are controlled by a chip 74HCT154N
    - There are 13 transistors CTBC 557B. These are NPN between the 74HCT154N and each digit http://www.farnell.com/datasheets/296678.pdf through a resitor
- ATMEL ATMEGA48PA-PU chip
-Spec: http://www.atmel.com/images/doc8161.pdf
- Spec2: http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf
- Seems to be compatible with pin to pin with Arduino Atmega168 from Arduino duemilanove.

| PIN | ATMEGA48PA-PU PIN | ATMEGA 168 | Arduino micro |
| --- | ----------------- | ---------- | ------------- |
|  1  |  R18 - Vcc        | RESET      |               |
