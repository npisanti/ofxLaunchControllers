
#include "ofxLCLeds.h"

const int ofxLCLeds::colors [] = { 12, 13, 15, 29, 63, 62, 28, 60 };

ofxLCLeds::ofxLCLeds() {
    connected = false;
}

void ofxLCLeds::setup( int midiOutId, int midiChannel ) {
    midiOut.listPorts(); // via instance
    midiOut.openPort(midiOutId); // by number
    channel = midiChannel;
    if( midiOut.isOpen()){
        connected = true;
        ofAddListener( ofEvents().exit, this, &ofxLCLeds::exit );
    }
}

void ofxLCLeds::exit( ofEventArgs &args ) {
    midiOut.closePort();        
}

void ofxLCLeds::led( int i, int color ) {
    if( connected && i >=0 && i<16 && color>=0 && color<8){
        int note = 36+i;
        midiOut.sendNoteOn(channel, note, colors[color] );
    }
}

void ofxLCLeds::ledSwitch(int i, int color, bool active ) {
    if(active){
        led(i, color);
    }else{
        led(i, 0 );
    }
}

bool ofxLCLeds::press( const ofxMidiMessage & msg, int & indexToReturn  ) {
    if(msg.status == MIDI_NOTE_ON && msg.velocity==127 && msg.pitch>=36 && msg.pitch <60 ){
        indexToReturn = msg.pitch - 36;
        return true;
    }else{
        return false;
    }
}

void ofxLCLeds::combo( int active, int start, int stop, int color ) {
    for( int i=start; i<=stop; ++i ){
        if( i!=active ){
            led( i, ofxLCLeds::Off );            
        }else{
            led( i, color );
        }
    }
}

