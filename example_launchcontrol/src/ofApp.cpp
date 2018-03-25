#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // gui setup -------------------------------------
    gui.setup("", "settings.xml", 20, 20 );
    gui.add( testBool.set("test bool", false) );
    gui.add( testFloat0.set( "test float 0 moment", 0.3f, 0.0f, 1.5f) );
    gui.add( testInt0.set( "test int 0 toggle", 2, 0, 150) );
    gui.add( testFloat1.set( "test float 1 knob", 0.3f, 0.0f, 1.5f) );
    gui.add( testInt1.set( "test int 1 knob",  5, 0, 100 ) );
    gui.add( testInt2.set( "test int 2 radio", 2, 0, 3   ) );
    gui.add( testInt3.set( "test int 3 knob again", 0, 0, 100   ) );


    // control bindings ------------------------------
    //lc.listDevices();
    //lc.setup(1);
    lc.setup(); // setup with automatic id finding   

    //lc.easing( 0.2f ); // slower easing 
    //lc.disableEasing(); // easing off
     
    lc.toggle( 0, testBool );
    
    // first value when on, second when off
    lc.momentary( 1, testFloat0, 0.2f, 1.1f ); 
    lc.toggle( 2, testInt0, 5, 42 );
    
    // index for the knobs are 0-15
    lc.knob( 0, testFloat1 ); // automatically mapped to ofParameter range
    lc.knob( 8, testInt1, 40, 80 ); // with value range
    lc.knob( 8, testInt3, 20, 90 ); // multiple 
    
    // radio buttons
    lc.radio( 4, 7, testInt2 );    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    lc.close(); // remember to close the midi ports
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
