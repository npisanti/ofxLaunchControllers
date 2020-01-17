
#pragma once

#include "ofxControllerBase.h"

class ofxLaunchControl : public ofxControllerBase {
public:
	ofxLaunchControl();
	void setup( int id=-1, int channel=9 );
};
