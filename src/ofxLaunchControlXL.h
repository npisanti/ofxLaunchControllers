
#pragma once

#include "ofxControllerBase.h"

class ofxLaunchControlXL : public ofxControllerBase {
public:
	ofxLaunchControlXL();
	void setup();
	
    void fader( int index, ofParameter<float> & param, float min, float max );
    void fader( int index, ofParameter<int> & param, int min, int max );
    void fader( int index, ofParameter<float> & param );
    void fader( int index, ofParameter<int> & param );
};
