#define BLYNK_PRINT Serial // Enables Serial Monitor

// Following includes are for Arduino Ethernet Shield (W5100)
// If you're using another shield, see Boards_* examples
#include "userinfo.h"
#include <BlynkSimpleEsp8266.h>
//#include <BlynkSimpleEthernet.h>
#include <TimeLib.h>

SimpleTimer timer;

extern const char auth[];
extern const char ssid[];
extern const char pass[];

int touch = 1;

int speakerPin = 14;

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

BLYNK_WRITE(V30) 
{
  touch = param.asInt();
  Serial.print("receive:");
  Serial.println(touch);  
}


void setup()
{
  // See the connection status in Serial Monitor
  Serial.begin(9600);

  pinMode(speakerPin, OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(200L, sendSensor);

  
}

void loop()
{
  // All the Blynk Magic happens here...
  Blynk.run();
  timer.run(); 
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

void sendSensor()
{
  Serial.println(touch);
  if(touch == 1)
  {
    // 연주
    for (int i = 0; i < length; i++) 
    {
      if (notes[i] == ' ') 
      {
        delay(beats[i] * tempo); // rest
      } else 
      {
        playNote(notes[i], beats[i] * tempo);
      }
  
      // pause between notes
      delay(tempo / 2);
    }

    
    touch = 0;    
    delay(2000);
  }
}


void playTone(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2) 
  {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}


void playNote(char note, int duration) 
{
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) 
  {
    if (names[i] == note) 
    {
      playTone(tones[i], duration);
    }
  }
}
