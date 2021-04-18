///// MIDI Controller Code 
///// M.A.N.G.O. BOX
///// steht für Midi (to) All Noise Generating Operators
///// 
///// by Ismael Rittmann für das FAB LAB der ABK Stuttgart


///// MIDI
#include <MIDI.h>                 // Midi Library
MIDI_CREATE_DEFAULT_INSTANCE();   // Startet die Library

///// LED
#include "Ticker.h"               // Ticker Library

int LEDspeed = 1;
int LEDcounter = 0;

void LEDanzeiger();
Ticker LED(LEDanzeiger, 1);

///// Switch
int Switch = 0;
int Switch2 = 0;
int State = 0;

void Schalter();

///// Potis
//int PotiW01 = 0;   alte Variable, die nicht mehr benötigt wird
int SmoothW01 = 0;

//int PotiW02 = 0;
int SmoothW02 = 0;

//int PotiW03 = 0;
int SmoothW03 = 0;

//int PotiW04 = 0;
int SmoothW04 = 0;

int W01 = 0;
int W02 = 0;
int W03 = 0;
int W04 = 0;
int W05 = 0;
int W06 = 0;
int W07 = 0;
int W08 = 0;
int W09 = 0;
int W10 = 0;
int W11 = 0;
int W12 = 0;

int W01Alt = 0;
int W02Alt = 0;
int W03Alt = 0;
int W04Alt = 0;
int W05Alt = 0;
int W06Alt = 0;
int W07Alt = 0;
int W08Alt = 0;
int W09Alt = 0;
int W10Alt = 0;
int W11Alt = 0;
int W12Alt = 0;             // ausführlche schreibweise für alle Variablen

void writePotis ();

///////////////////////////////////////
// MIDI
byte midiCh = 1; // MIDI channel to be used
byte cc = 1; // Lowest MIDI CC to be used


  //Debug
  void debugWrite ();
  Ticker Debug(debugWrite,100);


///////////////////////////////////////
void setup() {
  
  pinMode(9, INPUT);
  pinMode(7, INPUT);
  pinMode(11, OUTPUT);

  LED.start();
  Debug.start();
  
  Serial.begin(115200);
  //Serial.begin(9600);   der Kanal fürs Debuggen wird nicht im finalen Programm verwendet
}

///////////////////////////////////////
void loop() {
  
  Schalter();             //liest in welcher Position sich der Schalter befindet
  LED.update();           //stellt die Helligkeit der LED ein (je nach Schaltestellung)
  writePotis();           //Liest alle Potentiometer aus, ordnet die Werte zu und sendet das MIDI signal
  //Debug.update();       
  
}

void Schalter (){
                                                             //der verwendete Kippschalter hat einen Eingang und 2 Pins zum auslesen.
  Switch = digitalRead(7);                                   //diese Kombination ergibt ein AN-AUS-AUS und AUS-AUS-AN Muster.
  Switch2 = digitalRead(9);                                  //Daraus lassen sich mit 2 Pins die 3 Stellungen des Schalters ablesen:

  if (Switch == 0 and Switch2 == 1){
    State = 1;
    }
  
  if (Switch == 0 and Switch2 == 0){
    State = 2;
    }
    
  if (Switch == 1 and Switch2 == 0){
    State = 3;
    }
}

void LEDanzeiger (){

  LEDspeed = (State * 2) - 1 ;                                //um die LED zu dimmen, wird sie einfach sehr schnell an und aus geschaltet.
                                                              //Die Helligkeit ergibt sich aus der Schalterstellung (1,2 oder 3)
  if (LEDcounter < LEDspeed){                                 //Die LED Bleibt so lange aus, bis ein Zähler im Millisekundentakt einmal bis 1, 3 oder 5 gezählt hat
    digitalWrite(11,0);                                       //Es ändert sich also die Blinkfrequenz der LED mit der stellung des Kippschalters
    LEDcounter = LEDcounter + 1 ;                             //Somit ist die LED bei Schalterstellung 1 am hellsten (Einmal AN und Aus in 2 Millisekuden)
    }
  else {
    digitalWrite(11,1);
    LEDcounter = 0;
    }
}

void writePotis() {
    
  //PotiW01 = analogRead(A1);                                 // alle 4 Potis einlesen, die Werte glätten und in 8Bit-Werte (0-127), die MIDI verwendet umwandeln
  SmoothW01 = (SmoothW01 * 10) + (analogRead(A1) * 6);
  SmoothW01 = (SmoothW01 + 8) >> 4;
  SmoothW01 = map(SmoothW01, 0, 462, 0, 127);
  
  //PotiW02 = analogRead(A2);
  SmoothW02 = (SmoothW02 * 10) + (analogRead(A2) * 6);
  SmoothW02 = (SmoothW02  + 8) >> 4;
  SmoothW02 = map(SmoothW02, 0, 462, 0, 127);
  
  //PotiW03 = analogRead(A3);
  SmoothW03 = (SmoothW03 * 10) + (analogRead(A3) * 6);
  SmoothW03 = (SmoothW03 + 8) >> 4;
  SmoothW03 = map(SmoothW03, 0, 462, 0, 127);

  //PotiW04 = analogRead(A4);
  SmoothW04 = (SmoothW04 * 10) + (analogRead(A4) * 6);
  SmoothW04 = (SmoothW04 + 8) >> 4;
  SmoothW04 = map(SmoothW04, 0, 462, 0, 127);

  if (State == 1){                                            //zuordnung der Werte je nach Schaltestellung
    W01 = SmoothW01;
    W02 = SmoothW02;
    W03 = SmoothW03;
    W04 = SmoothW04;
    }
    
  if (State == 2){
    W05 = SmoothW01 ;
    W06 = SmoothW02 ;
    W07 = SmoothW03 ;
    W08 = SmoothW04 ;
    }
    
  if (State == 3){
    W09 = SmoothW01 ;
    W10 = SmoothW02 ;
    W11 = SmoothW03 ;
    W12 = SmoothW04 ;
    }
  

  if (W01 != W01Alt){                                         //Sendet eine Midi-Nachricht, sobald sich ein Wert verändert.
    MIDI.sendControlChange(cc + 1, W01, midiCh);              //Da jeder Poti 3fach belegt ist, gibt es insgesamt 12 verschiedene Midi Signale 
    }
    
  if (W02 != W02Alt){
    MIDI.sendControlChange(cc + 2, W02, midiCh);
    }

  if (W03 != W03Alt){
    MIDI.sendControlChange(cc + 3, W03, midiCh);
    }
    
  if (W04 != W04Alt){
    MIDI.sendControlChange(cc + 4, W04, midiCh);
    }

  if (W05 != W05Alt){
    MIDI.sendControlChange(cc + 5, W05, midiCh);
    }
    
  if (W06 != W06Alt){
    MIDI.sendControlChange(cc + 6, W06, midiCh);
    }

  if (W07 != W07Alt){
    MIDI.sendControlChange(cc + 7, W07, midiCh);
    }
    
  if (W08 != W08Alt){
    MIDI.sendControlChange(cc + 8, W08, midiCh);
    }

  if (W09 != W09Alt){
    MIDI.sendControlChange(cc + 9, W09, midiCh);
    }
    
  if (W10 != W10Alt){
    MIDI.sendControlChange(cc + 10, W10, midiCh);
    }

  if (W11 != W11Alt){
    MIDI.sendControlChange(cc + 11, W11, midiCh);
    }
    
  if (W12 != W12Alt){
    MIDI.sendControlChange(cc + 12, W12, midiCh);
    }

W01Alt = W01;                                                 //aktualisiert den Vorherigen Wert
W02Alt = W02;
W03Alt = W03;
W04Alt = W04;
W05Alt = W05;
W06Alt = W06;
W07Alt = W07;
W08Alt = W08;
W09Alt = W09;
W10Alt = W10;
W11Alt = W11;
W12Alt = W12;

}

void debugWrite (){                                           //Debug Programm:  hier kann jede beliebige Variable überprüft werden.
  Serial.println(SmoothW01);
}
