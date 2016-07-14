#include "pitches.h"

int ledPin = 5; // LED is connected to digital pin 13 
int handVibeBoard = 3; // this is the pin where the vibe board 
                       // potentially use the human to complete the circuit and always have the board turned to on
                       // still want to check that the board is completed to incorporate human touch into mood indicator
int tempSensorPin = A1; //sure whatever pin
int humanTouch = 2; //this is an open circuit that is completed by human contact
int switchValue; // a variable to keep track of when switch is pressed
int moodIndicator; //a variable for the robots mood setting
int ledMarqueePinA = A3; //half the outside marquee for the robot is on pin 10
int ledMarqueePinB = A2; //other half the outside marquee for the robot is on pin 11
int panicLevel; //a variable to see if something terrible has happened
int speakerPin = 7; // the speaker is connected to a pin
int playASong; //this variable is used to decide whether to play a song: negative if is the song is being played, 1 is start a new song
int human = 0;
int cycleTime = 1200;
int songProgress = 0;

int happySong[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int happySongDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int nyanMelody[] = {
  NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_C4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_C4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_G3,NOTE_E3,NOTE_F3,NOTE_G3,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_G3,NOTE_C4,NOTE_C4,NOTE_B4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_C4,NOTE_B4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_C4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_C4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_G3,NOTE_E3,NOTE_F3,NOTE_G3,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_G3,NOTE_C4,NOTE_C4,NOTE_B4,NOTE_C4,NOTE_G3,NOTE_A4,NOTE_C4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_C4,NOTE_D4
};


// note durations: 4 = quarter note, 8 = eighth note, etc.:
int nyanNoteDurations[] = {
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,16,16,8,8,16,16,16,16,16,16,16,16,16,16,16,16,8,16,16,8,16,16,16,16,16,16,16,16,16,16,8,16,16,16,16,16,16,16,16,16,16,8,8,8,16,16,8,16,16,16,16,16,16,16,16,16,16,16,16,8,8,16,16,16,16,16,16,16,16,16,16,16,16,8,16,16,8,16,16,16,16,16,16,16,16,16,16,8,16,16,16,16,16,16,16,16,16,16,8,8
};

void setup()   
{
  pinMode(ledPin, OUTPUT); // sets the ledPin to be an output
  pinMode(ledMarqueePinA, OUTPUT); // sets the ledPin to be an output
  pinMode(ledMarqueePinB, OUTPUT); // sets the ledPin to be an output
  pinMode(handVibeBoard, OUTPUT); //sets the handvibe to an output
  pinMode(tempSensorPin, INPUT); // sets the switchPin to be an input
  pinMode(humanTouch, INPUT); // sets the switchPin to be an input
  digitalWrite(humanTouch, HIGH);
//  digitalWrite(ledMarqueePinA, HIGH);
//  digitalWrite(ledMarqueePinB, HIGH);
//  pinMode(switchPin, INPUT); // sets the switchPin to be an input
  //digitalWrite(switchPin, HIGH); // sets the default (unpressed) state of switchPin to HIGH 
}
  
void loop() // run over and over again 
{
 // checkBeverage();
  checkHuman();
  calculateMood();
  runMarquee();
  playSong();
 // delay(500);
}

void calculateMood()
{
  //human = digitalRead(humanTouch);
  if (human == HIGH)
  {
    moodIndicator = 2;
    cycleTime = 1200;
  }
  else
  {
    moodIndicator = 1;
    cycleTime = 1200;
  }
}

void playSong()
{
  // play a note from the song in here song
  // to calculate the note duration, take one second
  // divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int musicDurationTimer = 0;
  while (musicDurationTimer <= cycleTime)
  {
    int noteDuration = cycleTime / nyanNoteDurations[songProgress];
    musicDurationTimer += noteDuration;
    tone(speakerPin, nyanMelody[songProgress], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
    if (songProgress>=116)
    {
      songProgress=0;
    }
    else
    {songProgress +=1;}
  }

}

void checkBeverage()
{
  int tempReading = analogRead(tempSensorPin);
  float voltage = tempReading*5.0;
  voltage /= 1024.0;
  float tempC = (voltage-0.5)*100;
  if (tempC>15)
  {
    panicLevel += 1;
  }
  else
  {
    panicLevel = 0;
  }
}

void checkHuman()
{
  human = digitalRead(humanTouch);
  if (human == HIGH)
  {
    digitalWrite(handVibeBoard, HIGH);
  }
  else
  {
    digitalWrite(handVibeBoard, LOW);
  }
}

void runMarquee()
{
  switch(moodIndicator){
    case 1:
      if (digitalRead(ledMarqueePinA) == LOW)
      {
        digitalWrite(ledMarqueePinA, HIGH);
        digitalWrite(ledMarqueePinB, LOW);
      }
      else
      {
        digitalWrite(ledMarqueePinA, LOW);
        digitalWrite(ledMarqueePinB, HIGH);
      }
      break;
    case 2:
      if (digitalRead(ledMarqueePinA) == LOW)
      {
        digitalWrite(ledMarqueePinB, HIGH);
        digitalWrite(ledMarqueePinA, HIGH);
      }
      else
      {
        digitalWrite(ledMarqueePinA, LOW);
        digitalWrite(ledMarqueePinB, LOW);
      }
      break;
  }
}

