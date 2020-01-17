
#include "ofxLaunchControl.h"


ofxLaunchControl::ofxLaunchControl(){
	name = "Launch Control";

    buttons.resize(8);
    buttons[0].controlNum = 9;
    buttons[1].controlNum = 10;
    buttons[2].controlNum = 11;
    buttons[3].controlNum = 12;

    buttons[4].controlNum = 25;
    buttons[5].controlNum = 26;
    buttons[6].controlNum = 27;
    buttons[7].controlNum = 28;

    knobsCC.resize(16);
    knobsCC[0]  = 21;
    knobsCC[1]  = 22;
    knobsCC[2]  = 23;
    knobsCC[3]  = 24;
    knobsCC[4]  = 25;
    knobsCC[5]  = 26;
    knobsCC[6]  = 27;
    knobsCC[7]  = 28;
    knobsCC[8]  = 41;
    knobsCC[9]  = 42;
    knobsCC[10] = 43;
    knobsCC[11] = 44;
    knobsCC[12] = 45;
    knobsCC[13] = 46;
    knobsCC[14] = 47;
    knobsCC[15] = 48;

}

void ofxLaunchControl::setup( int id, int channel ){
	const auto & ports = midiIn.getInPortList();
	if (id==-1){
		string target = "Launch Control";
		for( size_t i=0; i<ports.size(); ++i){
			string cut = ports[i].substr(0, target.length());
			if( target.compare(cut)==0 ){

				// now check that isn't the launch control xl
				string xl = "Launch Control XL";
				string recut = ports[i].substr(0, xl.length());
				if( xl.compare(recut)!=0 ){
					id = i;
					break;
				}
			}
		}
	}
	if(id>=0){
		ofxControllerBase::setup( id, channel );
	}else{
		ofLogError()<<"ofxLaunchControls: automatic setup error, Launch Control not found";
	}
}
