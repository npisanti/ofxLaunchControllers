
#include "ofxLaunchControlXL.h"


ofxLaunchControlXL::ofxLaunchControlXL(){

	name = "Launch Control XL";


    buttons.resize(16);

    buttons[0].controlNum = 41;
    buttons[1].controlNum = 42;
    buttons[2].controlNum = 43;
    buttons[3].controlNum = 44;

    buttons[4].controlNum = 57;
    buttons[5].controlNum = 58;
    buttons[6].controlNum = 59;
    buttons[7].controlNum = 60;

    buttons[8].controlNum = 73;
    buttons[9].controlNum = 74;
    buttons[10].controlNum = 75;
    buttons[11].controlNum = 76;

    buttons[12].controlNum = 89;
    buttons[13].controlNum = 90;
    buttons[14].controlNum = 91;
    buttons[15].controlNum = 92;


    knobsCC.resize(32);

    knobsCC[0]  = 13;
    knobsCC[1]  = 14;
    knobsCC[2]  = 15;
    knobsCC[3]  = 16;
    knobsCC[4]  = 17;
    knobsCC[5]  = 18;
    knobsCC[6]  = 19;
    knobsCC[7]  = 20;

    knobsCC[8]  = 29;
    knobsCC[9]  = 30;
    knobsCC[10] = 31;
    knobsCC[11] = 32;
    knobsCC[12] = 33;
    knobsCC[13] = 34;
    knobsCC[14] = 35;
    knobsCC[15] = 36;

    knobsCC[16] = 49;
    knobsCC[17] = 50;
    knobsCC[18] = 51;
    knobsCC[19] = 52;
    knobsCC[20] = 53;
    knobsCC[21] = 54;
    knobsCC[22] = 55;
    knobsCC[23] = 56;

    knobsCC[24] = 77;
    knobsCC[25] = 78;
    knobsCC[26] = 79;
    knobsCC[27] = 80;
    knobsCC[28] = 81;
    knobsCC[29] = 82;
    knobsCC[30] = 83;
    knobsCC[31] = 84;
}

void ofxLaunchControlXL::setup(  ){
	int id = -1;
	const auto & ports = midiIn.getInPortList();
	string target = "Launch Control XL";
	for( size_t i=0; i<ports.size(); ++i){
		string cut = ports[i].substr(0, target.length());
		if( target.compare(cut)==0 ){
			id = i;
			break;
		}
	}
	if(id>=0){
		ofxControllerBase::setup( id );
	}else{
		ofLogError()<<"ofxLaunchControls: automatic setup error, Launch Control XL not found";
	}
}

void ofxLaunchControlXL::fader( int index, ofParameter<float> & param, float min, float max ){
	if(index>=0 && index<8 ){
		knob( index+24, param, min, max );
	}else{
		ofLogError()<<"ofxLaunchControls: wrong index in fader() function, binding ignored";
	}
}

void ofxLaunchControlXL::fader( int index, ofParameter<int> & param, int min, int max ){
	if(index>=0 && index<8 ){
		knob( index+24, param, min, max );
	}else{
		ofLogError()<<"ofxLaunchControls: wrong index in fader() function, binding ignored";
	}
}

void ofxLaunchControlXL::fader( int index, ofParameter<float> & param ){
	if(index>=0 && index<8 ){
		knob( index+24, param );
	}else{
		ofLogError()<<"ofxLaunchControls: wrong index in fader() function, binding ignored";
	}
}

void ofxLaunchControlXL::fader( int index, ofParameter<int> & param ){
	if(index>=0 && index<8 ){
		knob( index+24, param);
	}else{
		ofLogError()<<"ofxLaunchControls: wrong index in fader() function, binding ignored";
	}
}
