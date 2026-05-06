# PES-project
Repository voor PES project jaar 2

Regels:
- Gebruik DoxeGen
- Codeer een beetje begrijpelijk en overzichtelijk
- Werk AUB vanuit de Development branch

Zie de volgende link om GitHub te gebruiken in de STM32IDE:
https://community.st.com/t5/stm32cubeide-mcus/how-to-use-git-branching-while-using-stm32cubeide/td-p/730876

````
**CANBUS REGELS EN AFSPRAKEN 2.0**

CAN-frame:
CAN_ID  (11-bit)	-> Type bericht
DATA    (0–8 bytes)	-> Inhoud van het bericht

0x00-0xff -> Hoogste prioriteit voor noodgevallen
0x1xx -> Sensoren die hun waarde sturen
0x2xx -> Het aansturen van actuatoren
0x3xx -> Het opvragen van specifieke waarden
0x4xx -> Voor errorberichten
0x7F0 – 0x7FE -> test/debug range

Indeling van de CAN-ID's:

**0x000 – 0x0FF -> CRITICAL / EMERGENCY**
Heeft de hoogste prioriteit op de bus
Alleen gebruiken voor:
  - Alarm triggers
  - Emergency stop
  - Kritieke fouten

**0x1xx -> SENSOREN**
- Wanneer een waarde significant verandert
- Wanneer een toestand verandert
- Periodiek als dat nodig is
*Een knop is in dit geval een sensor

0x10x - ENCODER PCB
0x100 -> Encoder position
DATA[0] = value high
DATA[1] = value low 

0x101 -> Encoderknop 
DATA[0] = state
0x00 = released
0x01 = pressed

0x102 -> Distance sensor 
DATA[0] = value high
DATA[1] = value low

0x11x — Matrix PCB
0x110 — Matrix knop status
DATA[0] = state
0x00 = released
0x01 = pressed

0x111 — Timer waarde
DATA[0] = value (hour)
DATA[1] = value (minute)

0x112 — Access status
DATA[0] = state
0x00 = closed
0x01 = open

0x113 — Access log event
DATA[0] = gebruiker ID (correct or wrong ID/pass)

0x12x — Gimbal boven PCB
0x120 — LED buttons
DATA[0] = LED button number (1-5)
DATA[1] = state
0x00 = released
0x01 = pressed

0x13x — Gimbal onder PCB
0x130 — Voltage meting
DATA[0] = voltage high
DATA[1] = voltage low

**0x2xx — ACTUATOREN** 
Actuatoren luisteren naar commands op deze IDs.
*Een LED is ook een actuator

0x20x — Encoder PCB actuatoren
0x200 — Encoder RGB LED
DATA[0] = state
0x00 = off
0x01 = red
0x02 = blue
0x03 = green

0x201 - LED button
DATA[0] = state
0x00 = off
0x01 = on

0x21x — Matrix PCB actuatoren
0x210 — Alarm
DATA[0] = 0x00 off
DATA[0] = 0x01 on

0x211 — Matrix LED
DATA[0] = 0x00 off
DATA[0] = 0x01 on

0x22x — Gimbal boven PCB actuatoren
0x220 -> LED buttons
DATA[0] = led number (1–5)
DATA[1] = state
0x00 = off
0x01 = on

0x221 — LED bar control
DATA[0] = matrix number (1–8)
DATA[1] = state
0x00 = off
0x01 = on

0x23x — Gimbal onder PCB actuatoren
0x230 — Servo onder
DATA[0] = position Gimbal 

0x231 — Servo boven
DATA[0] = position Gimbal

0x240 - Node Wemos Lamp
DATA[0] = red
DATA[1] = green
DATA[2] = blue
DATA[3] = brightness

**0x3xx — COMMANDO'S / REQUESTS**
0x300 — Sensor request
DATA[0] = high byte of CAN_ID
DATA[1] = low byte of CAN_ID

**0x4xx — ERRORS / SYSTEM**
0x400 -> system error
0x410 -> Encoder PCB error
0x420 -> Matrix PCB error
0x430 -> Gimbal PCB boven error
0x440 -> Gimbal PCB onder error
0x441-0x499 -> Other errors

**0x7F0 – 0x7FE = test/debug range**
To be decided
````



