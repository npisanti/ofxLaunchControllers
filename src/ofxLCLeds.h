
#pragma once

#include "ofxMidi.h"
    
class ofxLCLeds {
  
public:
    static const int Off = 0;
    static const int DarkRed = 1;
    static const int Red = 2;
    static const int DarkAmber = 3;
    static const int Amber = 4;
    static const int Yellow = 5;
    static const int DarkGreen = 28;
    static const int Green = 60;
    static const int colors [];
    
    ofxLCLeds();
    void setup( int midiOutId, int midiChannel = 1);

    void led( int i, int color );
    void ledSwitch(int i, int color, bool active );
    bool press( const ofxMidiMessage & msg, int & indexToReturn );

    void combo( int active, int start, int stop, int color );

private:
    void exit( ofEventArgs &args );
    
    bool connected; 
	ofxMidiOut midiOut;
	int channel;
};
    
