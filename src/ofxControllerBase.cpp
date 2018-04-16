
#include "ofxControllerBase.h"

ofxControllerBase::ofxControllerBase(){
	name = "launch control";
}

void ofxControllerBase::listDevices() {
    midiIn.listPorts();
}


void ofxControllerBase::setup( int port, int channel ) {
    
    this->channel = channel;
    buttonsColor = ofxLCLeds::Red;
    
    knobs.resize(knobsCC.size());
    
    radios.clear();
	midiIn.openPort( port );
    
    if( midiIn.isOpen() ){
        midiIn.addListener(this);
        leds.openPort( port );       
        ofLogNotice() << "ofxLaunchControls: " << name << " activated!"; 
    }
    
    bEasing = false;
    easeAmount = 0.05f;
    
    bUpdate = false;

    clearLeds();
        
    ofAddListener( ofEvents().update, this, &ofxControllerBase::update);  
}


void ofxControllerBase::update( ofEventArgs & events ){
    cout<<"launch controls update is running\n";
    if(bUpdate){
		
		midilock.lock();
		
		bUpdate = false;
		
        // buttons update
        for( size_t b = 0; b<buttons.size(); ++b ){
            if( buttons[b].bUpdate ) {
                switch(buttons[b].typeCode){
                    case 1: // bool
                        *(buttons[b].pParamb) = buttons[b].bActive;
                    break;
                    
                    case 2: // float 
                        if( buttons[b].bActive ){
                            *(buttons[b].pParamf) = buttons[b].maxf;
                        }else{
                            *(buttons[b].pParamf) = buttons[b].minf;
                        }
                    break;
                    
                    case 3: // int
                       if( buttons[b].bActive ){
                            *(buttons[b].pParami) = buttons[b].maxi;
                        }else{
                            *(buttons[b].pParami) = buttons[b].mini;
                        }
                    break;
                }
                buttons[b].bUpdate = false;
            }
        }

        // radio buttons update
        for( size_t i = 0; i<radios.size(); ++i ){
            if ( radios[i].bUpdate ) {
                *(radios[i].pParami) = radios[i].value;
                radios[i].bUpdate = false;
            }
        }

		static const float stopvalue = 0.00001f;

        // knob update
        for( size_t i = 0; i<knobs.size(); ++i ){
            for( size_t k = 0; k<knobs[i].size(); k++ ) {
                if( knobs[i][k].bUpdate ) {
                    switch(knobs[i][k].typeCode){
                        case 2: // float 
							if( bEasing ){
								// lpf code
								float xn = (knobs[i][k].value*easeAmount + knobs[i][k].z1*(1.0f-easeAmount) );
								knobs[i][k].z1 = xn;
								if( abs(knobs[i][k].value - knobs[i][k].z1) < stopvalue ){
									knobs[i][k].z1 = knobs[i][k].value;
									knobs[i][k].bUpdate = false;
								}else{
									bUpdate = true; // keeps global update alive
								}
								*(knobs[i][k].pParamf) = knobs[i][k].z1;
							}else{
								// just set value
								*(knobs[i][k].pParamf) = knobs[i][k].value;
								knobs[i][k].bUpdate = false;
							}
                        break;
                        
                        case 3: // int
							if( bEasing ){
								// lpf code
								float xn = (knobs[i][k].value*easeAmount + knobs[i][k].z1*(1.0f-easeAmount) );
								knobs[i][k].z1 = xn;
								if( abs(knobs[i][k].value - knobs[i][k].z1) < stopvalue ){
									knobs[i][k].z1 = knobs[i][k].value;
									knobs[i][k].bUpdate = false;
								}else{
									bUpdate = true; // keeps global update alive
								}
								*(knobs[i][k].pParami) = (int) knobs[i][k].z1;
							}else{
								// just set value
								*(knobs[i][k].pParami) = (int) knobs[i][k].value;
								knobs[i][k].bUpdate = false;
							}
                        break;
                    }
                }                                
            }
        }

        midilock.unlock();		
        // update leds
        refreshLeds();
    }
    
}

void ofxControllerBase::newMidiMessage( ofxMidiMessage& msg ) {
    
    midilock.lock();
    bUpdate = true;
    
    switch( msg.status ){
        case MIDI_CONTROL_CHANGE:
            for ( size_t i=0; i<knobs.size(); ++i ) {
                for( size_t k = 0; k<knobs[i].size(); k++ ) {
                    if( msg.control==knobs[i][k].controlNum && knobs[i][k].typeCode>0 ) {
						switch(knobs[i][k].typeCode){
							case 2: // float 
								knobs[i][k].value = ofMap( msg.value, 0, 127, knobs[i][k].minf, knobs[i][k].maxf);
							break;
							
							case 3: // int
								knobs[i][k].value = ofMap( msg.value, 0, 127, knobs[i][k].mini, knobs[i][k].maxi);
							break;
						}
                        knobs[i][k].bUpdate = true;
                    }
                }
            }
        break;
                
        case MIDI_NOTE_ON: 
            for( size_t b = 0; b<buttons.size(); ++b ){
                if( msg.pitch == buttons[b].controlNum && buttons[b].typeCode>0 ){
                    switch( buttons[b].buttonMode ){
                        case 0: // toggle
                            buttons[b].bActive = buttons[b].bActive ? false : true;
                            buttons[b].bUpdate = true;
                        break;
                        
                        case 1: // momentary
                            buttons[b].bActive = true;
                            buttons[b].bUpdate = true;
                        break;
                        
                        case 2: // radio
                            radios[buttons[b].radioGroup].value = buttons[b].radioValue;
                            radios[buttons[b].radioGroup].bUpdate = true;
                        break;
                    }                    
                }
            }
        break;
        
        case MIDI_NOTE_OFF: 
            for( size_t b = 0; b<buttons.size(); ++b ){
                if( msg.pitch == buttons[b].controlNum && buttons[b].typeCode>0 ){
                    if( buttons[b].buttonMode == 1 ) { // momentary
                        buttons[b].bActive = false;
                        buttons[b].bUpdate = true;
                    }
                }
            }
        break;
        
        default: break;
    }
    
    midilock.unlock();
}


// init values 
ofxControllerBase::Binding::Binding() {
    controlNum = -1;
    typeCode = 0;
    bUpdate = false;
    buttonMode = 0;
    pParamf = nullptr;
    pParami = nullptr;
    pParamb = nullptr;
    value = 0;
    minf = 0.0f;
    maxf = 0.0f;
    mini = 0;
    maxi = 0;
    bActive = false;
    radioGroup = -1;
    radioValue = -1;
    z1 = 0.0f;
}

ofxControllerBase::Binding::Binding(const Binding & other) {
    controlNum = other.controlNum;
    typeCode = other.typeCode;
    bUpdate.store( other.bUpdate );
    buttonMode = other.buttonMode;
    pParamf = other.pParamf;
    pParami = other.pParami;
    pParamb = other.pParamb;
    value.store( other.value );
    minf = other.minf;
    maxf = other.maxf;
    mini = other.mini;
    maxi = other.maxi;
    bActive.store(other.bActive);
    radioGroup = other.radioGroup;
    radioValue = other.radioValue;
    z1 = other.z1;
}

void ofxControllerBase::button( int index, ofParameter<bool> & param, bool momentary ) {
    if( midiIn.isOpen() ){
        if(index>=0 && index<(int)buttons.size() ){
            buttons[index].typeCode = 1; 
            buttons[index].pParamb = &param;
            if( momentary ){
                buttons[index].buttonMode = 1;
            }else{
                buttons[index].buttonMode = 0;
            }
            param.addListener( this, &ofxControllerBase::buttonChangedB );
            refreshLeds();
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for button(), momentary() or toggle() function, binding ignored";
        }
    }
}

void ofxControllerBase::button( int index, ofParameter<float> & param, float min, float max, bool momentary ) {
    if( midiIn.isOpen() ){
        if(index>=0 && index<(int)buttons.size()){
            buttons[index].typeCode = 2; 
            buttons[index].pParamf = &param;
            buttons[index].maxf = max;
            buttons[index].minf = min;
            if( momentary ){
                buttons[index].buttonMode = 1;
            }else{
                buttons[index].buttonMode = 0;
            }
            
            param.addListener( this, &ofxControllerBase::buttonChangedF );
            refreshLeds();
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for button(), momentary() or toggle() function, binding ignored";
        }
    }
}

void ofxControllerBase::button( int index, ofParameter<int> & param, int min, int max, bool momentary ) {
    if( midiIn.isOpen() ){
        if(index>=0 && index<(int)buttons.size() ){
            buttons[index].typeCode = 3; 
            buttons[index].pParami = &param;
            buttons[index].maxi = max;
            buttons[index].mini = min;
            if( momentary ){
                buttons[index].buttonMode = 1;
            }else{
                buttons[index].buttonMode = 0;
            }
            param.addListener( this, &ofxControllerBase::buttonChangedI );
            refreshLeds();
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for button(), momentary() or toggle() function, binding ignored";
        }
    }
}

// remember to change midi channel 
void ofxControllerBase::refreshLeds() {
    
    for( size_t b = 0; b<buttons.size(); ++b ){
        
        if( buttons[b].typeCode>0 && buttons[b].typeCode!=4 ){
            
            switch(buttons[b].typeCode){
                case 1: // bool
                    buttons[b].bActive = *(buttons[b].pParamb);
                break;
                
                case 2: // float 
                    if( *(buttons[b].pParamf) > buttons[b].minf ){
                        buttons[b].bActive = true;
                    }else{
                        buttons[b].bActive = false;
                    }
                break;
                
                case 3: // int
                    if( *(buttons[b].pParami) > buttons[b].mini ){
                        buttons[b].bActive = true;
                    }else{
                        buttons[b].bActive = false;
                    }
                break;
            }

            if( buttons[b].bActive ){
                leds.sendNoteOn( channel, buttons[b].controlNum, ofxLCLeds::colors[buttonsColor] );
            }else{
                leds.sendNoteOn( channel, buttons[b].controlNum, ofxLCLeds::colors[ 0 ] );
            }            

            buttons[b].bUpdate = false;
        }
    }
    
    for( size_t i = 0; i<radios.size(); ++i ){
        for ( int k=radios[i].min; k<=radios[i].max; ++k ) {
            if( k== *(radios[i].pParami) + radios[i].min ){
                leds.sendNoteOn( channel, buttons[k].controlNum, ofxLCLeds::colors[radios[i].color] );
            }else{
                leds.sendNoteOn( channel, buttons[k].controlNum, ofxLCLeds::colors[0] );
            }
        }
    }
    
}

void ofxControllerBase::clearLeds() {
    for( size_t b = 0; b<buttons.size(); ++b ){
		leds.sendNoteOn( channel, buttons[b].controlNum, ofxLCLeds::colors[ 0 ] );
    }    
}

void ofxControllerBase::radio( int indexMin, int indexMax, ofParameter<int> & param, int color ) {
    if( midiIn.isOpen() ){
        if ( indexMin >=0 && indexMax<=(int)buttons.size() && indexMin<=indexMax ) {

            radios.emplace_back();
            radios.back().min = indexMin;
            radios.back().max = indexMax;

            radios.back().pParami = &param;
            radios.back().bUpdate = false;
            radios.back().color = color; 
            
            int val = param;
            if(val<0) val = 0;
            int max = indexMax-indexMin;
            if(val>max) val = max; 

            radios.back().value = val;
            
            for( int b = indexMin; b<=indexMax; ++b ){
                buttons[b].buttonMode = 2;
                buttons[b].typeCode = 4;
                buttons[b].radioValue = b-indexMin; // radio value that the button will set
                buttons[b].radioGroup = radios.size()-1;
            }        
            param.addListener( this, &ofxControllerBase::radioChanged );

            refreshLeds();
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for radio() function, binding ignored";
        }
    }
}

ofxControllerBase::RadioGroup::RadioGroup( ) { 
    min = 0;
    max = 0;
    value = 0;
    pParami = nullptr;
    bUpdate = false;
    color = 0;
}

ofxControllerBase::RadioGroup::RadioGroup( const RadioGroup & other) { 
    min = other.min;
    max = other.max;
    value.store(other.value);
    pParami = other.pParami;
    bUpdate.store( other.bUpdate );
    color = other.color;
}


void ofxControllerBase::knob( int index, ofParameter<float> & param, float min, float max ) {
    if( midiIn.isOpen() ){
        if(index>=0 && index<(int)knobs.size() ){
            knobs[index].emplace_back();
            knobs[index].back().controlNum = knobsCC[index];
            knobs[index].back().typeCode = 2; 
            knobs[index].back().pParamf = &param;
            knobs[index].back().maxf = max;
            knobs[index].back().minf = min;
            knobs[index].back().value = param;
            knobs[index].back().z1 = param;
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for knob() function, binding ignored";
        }
    }
}

void ofxControllerBase::knob( int index, ofParameter<int> & param, int min, int max ) {
    if( midiIn.isOpen() ){
        if(index>=0 && index<(int)knobs.size() ){
            knobs[index].emplace_back();
            knobs[index].back().controlNum = knobsCC[index];
            knobs[index].back().typeCode = 3;
            knobs[index].back().pParami = &param;
            knobs[index].back().maxi = max;
            knobs[index].back().mini = min;
            knobs[index].back().value = (float) param;
            knobs[index].back().z1 = (float) param;
        }else{
            ofLogError() << "ofxLaunchControls: wrong indices for knob() function, binding ignored";
        }     
    }
}


void ofxControllerBase::knob( int index, ofParameter<float> & param ) {
    knob( index, param, param.getMin(), param.getMax() );
}

void ofxControllerBase::knob( int index, ofParameter<int> & param ) {
    knob( index, param, param.getMin(), param.getMax() );
}

void ofxControllerBase::button( int index, ofParameter<float> & param, bool momentary ) {
    button( index, param, param.getMin(), param.getMax(), momentary );
}

void ofxControllerBase::button( int index, ofParameter<int> & param, bool momentary ) {
    button( index, param, param.getMin(), param.getMax(), momentary );
}

void ofxControllerBase::momentary( int index, ofParameter<bool> & param ) {
	button( index, param, true );
}
void ofxControllerBase::momentary( int index, ofParameter<float> & param, float min, float max ){
	button( index, param, min, max, true );
}
void ofxControllerBase::momentary( int index, ofParameter<int> & param, int min, int max ){
	button( index, param, min, max, true );
}
void ofxControllerBase::momentary( int index, ofParameter<float> & param ){
	button( index, param, true );
}
void ofxControllerBase::momentary( int index, ofParameter<int> & param ){
	button( index, param, true );
}
void ofxControllerBase::toggle( int index, ofParameter<bool> & param ){
	button( index, param, false );
}
void ofxControllerBase::toggle( int index, ofParameter<float> & param, float min, float max ){
	button( index, param, false );
}
void ofxControllerBase::toggle( int index, ofParameter<int> & param, int min, int max ){
	button( index, param, false );
}
void ofxControllerBase::toggle( int index, ofParameter<float> & param ){
	button( index, param, false );
}
void ofxControllerBase::toggle( int index, ofParameter<int> & param ){
	button( index, param, false );
}

void ofxControllerBase::radioChanged( int & value ) {
    bUpdate = true;
}

void ofxControllerBase::buttonChangedB( bool & value ) {
    bUpdate = true;    
}

void ofxControllerBase::buttonChangedI( int & value ) {
    bUpdate = true;    
}

void ofxControllerBase::buttonChangedF( float & value ) {
    bUpdate = true;    
}

void  ofxControllerBase::enableEasing( float speed ){
	this->easeAmount = speed;
	bEasing = true;
}
void  ofxControllerBase::disableEasing(){
	bEasing = false;
}

void ofxControllerBase::easing( float speed ) {
	if( speed>0.0f ){
		speed = (speed>1.0f) ? 1.0f : speed;
		enableEasing( easeAmount );
	}else{
		disableEasing();
	}
}


void ofxControllerBase::close(){
	clearLeds();
    midiIn.closePort();
	midiIn.removeListener(this);
}
