/****************************************************************/
/* Project Title: Text-Me-A-Song                                */
/* SYDE 2020                                                    */
/* Creators: Bianca Guderian                                    */
/*           Brian Chen                                         */
/*           Qile Wang                                          */
/* SYDE 192L Project                                            */
/* July 18, 2016                                                */
/****************************************************************/

#include "pitches.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>


#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* interrupt variables that store the state of each button */
volatile int button1State = LOW;  //Stores button 1 state
volatile int button2State = LOW;  //Stores button 2 state
volatile int button3State = LOW;  //Stores button 3 state

//volatile int interruptCounter = 0; //Keeps track or how many times interrupts runs --> ensures it doesn't run multiple times

/* button pins */
int buttonPinInterrupt = 2;  //pin for the button interrupts

int button1 = 4;  //pin input for button 1 (Pause Button)
int button2 = 5;  //pin input for button 2 (Mute Button)
int button3 = 6;  //pin input for button 3 (Unmute Button)

int ledOutputPin = 12; //pin to detect output
int musicInputPin = 13;

bool donePlaying = true;  //ensures that nothing is read when the program is not done playing
bool hasNotes = false;
bool pause = false; //stores the state of whether of not the music is paused or not
bool mute = false;  //mute or no

//Initializing reading from external
SoftwareSerial ser(10, 11);

/* Rewriting Millis() */
// software millis()
long CPU_FREQUENCY = 16000000L; //16MHz
long clockCyclesPerMicrosecond = CPU_FREQUENCY / 10000000L; //16

// Prescaler of 256 allows for 4 seconds of working room
int prescaler = 256; 
int rollover = 256;

// New Millis will overflow after 4 seconds
long MS_PER_TIMER2_OVERFLOW = ((prescaler * rollover) * 1000)/(CPU_FREQUENCY/1000); //4096 microseconds
long MILLIS_INC = 8; //MS_PER_TIMER2_OVERFLOW / 1000; // milliseconds

// Shift Fract_Inc to fit in a byte
int FRACT_INC = 6; //(MS_PER_TIMER2_OVERFLOW % 1000) >> 4; // 
int FRACT_MAX = 63; //1000 >> 4; // 250
volatile unsigned long new_timer2_millis = 0;
volatile unsigned char new_timer2_fract = 0;
volatile unsigned int new_timer2_overflow_count = 0;

/* New millis() */

unsigned long new_millis()
{
    unsigned long m;
    uint8_t oldSREG = SREG;
    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer2_millis)
    cli();
    m = new_timer2_millis;
    SREG = oldSREG;
    //sei(); -->SREG restores everything
 
    return m;
}
ISR(TIMER2_OVF_vect){
    unsigned long m = new_timer2_millis;
    unsigned char f = new_timer2_fract;
 
    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX){
        f -= FRACT_MAX;
        m += 1;
    }
 
    new_timer2_fract = f;
    new_timer2_millis = m;
    new_timer2_overflow_count++;
}

/* Program runs once */
void setup() {
  //setting the pins
  pinMode(ledOutputPin, OUTPUT);
  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);

  //TCCR0B = (_BV(CS02) | _BV(CS00)); //Set Timer0 prescaling factor
  //counter_freq = 16e6/64; //Calculate actual counter frequency 16Mhz/prescale factor
 // counter_period = ;
 
  //attaching interrupt to button
  attachInterrupt(digitalPinToInterrupt(buttonPinInterrupt), button_ISR, RISING);
  
  //Begin Serial Reader
  Serial.begin(9600);
  Serial.println("SERIAL: BEGIN");
  Serial.println("..");

  // Begin Software Serial
  ser.begin(9600);

  //Shift Timer
  TCCR2B = _BV(CS22);
  // enable timer 2 overflow interrupt
  TIMSK2 = _BV(TOIE2);
  TCNT2 = 0;
  
}

/* Program Loop */
void loop() 
{
  /* Reading Note s*/
  int numberOfNotes = 0; //keeps track of how many notes are there
  int inputLength = 0;

  //Serial.println("In First If");
  donePlaying = false;
  //check if input is available
  if (ser.available())
  {
    //Serial.println(ser.readString());
    String inputNotes = ser.readString();
    inputLength = inputNotes.length();  //stores the lenght of input
    hasNotes = true;
    numberOfNotes = inputLength/2;  //stores # of actual notes

    Serial.println(inputNotes);
    
    //Storing the input into notes and duration
    int melody[numberOfNotes];
    int melodyLength = 0;
    int melodyDuration[numberOfNotes];
    int durationLength = 0;

    //Preprogram array
    for(int i = 0; i < numberOfNotes; i++)
    {
      melody[i] = 0;
    }
    
    for(int i = 0; i < numberOfNotes; i++)
    {
      melodyDuration[i] = 0;
    }

    /* stores the notes for playing */
    for(int i = 0; i < inputLength; i++)
    {
      //stores notes if it's alphabet
      if(i%2 == 0)
      {
      //switch stuff
      switch(inputNotes[i])
      {
        case 'A':
        melody[melodyLength] = NOTE_A;
        break;
        case 'B':
        melody[melodyLength] = NOTE_B;
        break;
        case 'C':
        melody[melodyLength] = NOTE_C;
        break;
        case 'D':
        melody[melodyLength] = NOTE_D;
        break;
        case 'E':
        melody[melodyLength] = NOTE_E;
        break;
        case 'F':
        melody[melodyLength] = NOTE_F;
        break;
        case 'G':
        melody[melodyLength] = NOTE_G;
        break;
      }

         Serial.print("Note Read: ");
         Serial.println(melody[melodyLength]);
         melodyLength++;
      }
      //stores the duration of the notes
      else
      {
        melodyDuration[durationLength] = (int)(inputNotes[i] - '0');

        Serial.print("Duration: ");
        Serial.println(melodyDuration[durationLength]);
        
        durationLength++;
      }
    } //end of for loop

    unsigned long songStartTime = new_millis();
    /* Play the music */
    for(int currentNote = 0; currentNote < numberOfNotes; currentNote++)
    {
      int noteDuration = 2000/melodyDuration[currentNote];  //keep the duration of the notes

        //Pause the music
        while(pause)
        {
          Serial.print("Pausing Music");
          digitalWrite(ledOutputPin, HIGH);
          //Pause music here
        }
        digitalWrite(ledOutputPin, LOW);

      if(mute == false)
      {
       char temp = sendNote(melody[currentNote]);
       ser.write(temp);
       unsigned long startNote = new_millis();
       tone(musicInputPin, melody[currentNote], noteDuration);
       
       // to distinguish the notes, set a minimum time between them.
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);
       
       // stop the tone playing:
       noTone(musicInputPin);
       
       unsigned long endNote = new_millis();
       Serial.print("Note duration: ");
       unsigned long asd = endNote - startNote;
       Serial.println(asd);
      }
      else
      {
       char temp = sendNote(melody[currentNote]);
       ser.write(temp);
       unsigned long startNote = new_millis();
       tone(9, melody[currentNote], noteDuration);
       
       // to distinguish the notes, set a minimum time between them.
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);
       
       // stop the tone playing:
       noTone(9);
       
       unsigned long endNote = new_millis();
       Serial.print("Note duration: ");
       unsigned long asd = endNote - startNote;
       Serial.println(asd);
      }
    }
    ser.write('Z');
    unsigned long songEndTime = new_millis();
    unsigned totalTime = (songEndTime - songStartTime);

    Serial.print("Song Duration: ");
    Serial.println(totalTime);

  }//end of if(ser.available)
}

/* Interrupt Handler */
void button_ISR()
{
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    button3State = digitalRead(button3);

    //If input is detected on button 1
    if(button1State == HIGH)
    {
      if (pause == true)
      {
        pause = false;
      }
      else
      {
        pause = true; 
      }
    }
    //if input is detected on button 2
    if(button2State == HIGH)
    {
      mute = true;
    }

    if(button3State == HIGH)
    {
      mute = false;
    }
}

