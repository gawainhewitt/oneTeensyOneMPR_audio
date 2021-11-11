/*
Simple arduino sketch to show one MPR121 board working triggering wav files using the
Teensy Audio shield on a Teensy 4

Four note polyphonic with note stealing

Use standard address is tied to Ground which gives the default address of 0x5A

gawainhewitt.co.uk
https://github.com/gawainhewitt
*/

#include "constants.h"
#include "mpr121.h"
#include "wavFilePlayer.h"
#include "reboot.h"

void setup() {
Serial.begin(9600);
init_mpr121();
init_player();
pinMode(rebootButton, INPUT_PULLUP);
delay(500);
}


void loop() {
currtouched1 = mprBoard_A.touched();

if(digitalRead(rebootButton) == LOW){
    doReboot();
}

//For A----------------------------------------------------------
for (uint8_t i=0; i<12; i++) {
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i)) ) {
    Serial.print(i); Serial.println(" touched of A");
    playSound(i);
    }

    if (!(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {
    Serial.print(i); Serial.println(" released of A");
    stopSound(i);
    }
}

lasttouched1 = currtouched1;
// kill_dead_notes();
return;
}
