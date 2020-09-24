
#pragma once

#include "ofxMidi.h"
    
class ofxLCLeds {
  
public:
    /*
     * https://fael-downloads-prod.focusrite.com/customer/prod/s3fs-public/novation/downloads/6958/launch-control-programmers-reference-guide.pdf
     */

    static int CustomColor(int r, int g) {
        return 12 + g * 16 + r;
    }
    static const int Off = 0;
    static const int DarkRed = 1;
    static const int Red = 2;
    static const int DarkAmber = 29;
    static const int Amber = 63;
    static const int Yellow = 62;
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
    
