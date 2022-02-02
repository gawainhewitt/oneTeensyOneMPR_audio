#include <Audio.h>
// #include <Wire.h> // already in mpr121.h
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const char *mySounds[] = {"CELLO0.WAV", "CELLO1.WAV", "CELLO2.WAV",
                     "CELLO3.WAV", "CELLO4.WAV", "CELLO5.WAV", "CELLO6.WAV",
                     "CELLO7.WAV", "CELLO8.WAV", "CELLO9.WAV", "CELLO10.WAV",
                     "CELLO11.WAV"}; // filenames are always uppercase 8.3 format

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav4;     //xy=246,418
AudioPlaySdWav           playSdWav3;     //xy=248,366
AudioPlaySdWav           playSdWav2;     //xy=250,322
AudioPlaySdWav           playSdWav1;     //xy=251,275
AudioMixer4              mixer1;         //xy=687,408
AudioMixer4              mixer2;         //xy=698,536
AudioAmplifier           amp1;           //xy=967,495
AudioAmplifier           amp2;           //xy=974,589
AudioOutputI2S           i2s1;           //xy=1095,733
AudioConnection          patchCord1(playSdWav4, 0, mixer1, 3);
AudioConnection          patchCord2(playSdWav4, 1, mixer2, 3);
AudioConnection          patchCord3(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav3, 1, mixer2, 2);
AudioConnection          patchCord5(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord6(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord7(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord8(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord9(mixer1, amp1);
AudioConnection          patchCord10(mixer2, amp2);
AudioConnection          patchCord11(amp1, 0, i2s1, 0);
AudioConnection          patchCord12(amp2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=842,836
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
//#define SDCARD_CS_PIN    BUILTIN_SDCARD
//#define SDCARD_MOSI_PIN  11  // not actually used
//#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

void init_player() {

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

// Map of which voice has which key playing
int active[MAX_POLY];
// Map of when each voice started playing, for note stealing
long when[MAX_POLY];
// Map of when we started the fade out so we know when to kill the note after the fade
long faded_ms[MAX_POLY];

byte get_free_voice() {
  // Look for a free voice and either return it or steal one & return it
  for(byte voice=0;voice<MAX_POLY;voice++) {
    if(active[voice] == -1) return voice;
  }
  // All are active, steal the oldest one

   long oldest_time = millis();
  byte oldest = 0;
  for(byte voice=0;voice<MAX_POLY;voice++) {
    if(when[voice] < oldest_time) {
      oldest = voice;
      oldest_time = when[voice];
    }
  }
  active[oldest] = -1;
  return oldest;
}

void playSound(int track) {
    int player = get_free_voice();
    Serial.print("notezzzz");
    Serial.println(player);

        switch(player)
    {
        case 0:
            playSdWav1.play(mySounds[track]);
            active[0] = track;
            when[0] = millis();
            break;
        case 1:
            playSdWav2.play(mySounds[track]);
            active[1] = track;
            when[1] = millis();
            break;
        case 2:
            playSdWav3.play(mySounds[track]);
            active[2] = track;
            when[2] = millis();
            break;
        case 3:
            playSdWav4.play(mySounds[track]);
            active[3] = track;
            when[3] = millis();
            break;
    }
    Serial.print("active ");
    Serial.println(active[0]);
}

void stopSound(int track) {
    for(int i = 0; i < MAX_POLY; i++){
        if(active[i] == track){
            switch(i)
    {
        case 0:
            playSdWav1.stop();
            active[0] = -1;
            break;
        case 1:
            playSdWav2.stop();
            active[1] = -1;
            break;
        case 2:
            playSdWav3.stop();
            active[2] = -1;
            break;
        case 3:
            playSdWav4.stop();
            active[3] = -1;
            break;
    }
        }
    }
}
