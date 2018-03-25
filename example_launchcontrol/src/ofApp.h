#pragma once

#include "ofMain.h"
#include "ofxLaunchControls.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxLaunchControl lc;
    
    ofxPanel gui;
    ofParameter<bool> testBool;
    ofParameter<float> testFloat0;
    ofParameter<float> testFloat1;
    ofParameter<int> testInt0;
    ofParameter<int> testInt1;
    ofParameter<int> testInt2;
    ofParameter<int> testInt3;                
};
