
#pragma once

#include "ofMain.h"
#include "ofxLCLeds.h"
#include <atomic>

// class for easily mapping with Novation Nocturn protocol
// uses the first fixed presets, that defaults to midi channel 9
// other template presets have the same cc and notes with different channels

class ofxControllerBase : public ofxMidiListener {

    private:
        struct Binding;
        struct RadioGroup;

    public:
        enum TypeCode {
            LC_TYPECODE_UNASSIGNED = 0,
            LC_TYPECODE_BOOL = 1,
            LC_TYPECODE_FLOAT = 2,
            LC_TYPECODE_INT = 3,
            LC_TYPECODE_RADIO = 4,
            LC_TYPECODE_VECTOR3 = 5
        };

        ofxControllerBase();
        void listDevices();
        void setup(int port, int channel = 9);


        void button(int index, ofParameter <bool> & param, bool momentary = false);
        void button(int index, ofParameter <float> & param, float min, float max, bool momentary = false);
        void button(int index, ofParameter <int> & param, int min, int max, bool momentary = false);
        void button(int index, ofParameter <float> & param, bool momentary = false);
        void button(int index, ofParameter <int> & param, bool momentary = false);

        void momentary(int index, ofParameter <bool> & param);
        void momentary(int index, ofParameter <float> & param, float min, float max);
        void momentary(int index, ofParameter <int> & param, int min, int max);
        void momentary(int index, ofParameter <float> & param);
        void momentary(int index, ofParameter <int> & param);
        void toggle(int index, ofParameter <bool> & param);
        void toggle(int index, ofParameter <float> & param, float minp, float max);
        void toggle(int index, ofParameter <int> & param, int min, int max);
        void toggle(int index, ofParameter <float> & param);
        void toggle(int index, ofParameter <int> & param);

        void radio(int indexMin, int indexMax, ofParameter <int> & param, int color = ofxLCLeds::Red, int offColor = ofxLCLeds::Off);

        void knob(int index, ofParameter <float> & param, float min, float max);
        void knob(int index, ofParameter <int> & param, int min, int max);
        void knob(int index, ofParameter <float> & param);
        void knob(int index, ofParameter <int> & param);

        void knob3(int index, ofParameter <glm::vec3> & param, glm::vec3 min, glm::vec3 max);
        void knob3(int index, ofParameter <glm::vec3> & param);

        void enableEasing(float speed = 0.5f);
        void easing(float speed = 0.5f);
        void disableEasing();

        void close();

        void refreshLeds(); // update leds from parameter values

    protected:
        vector <Binding> buttons;
        vector <int>     knobsCC;
        string name;

        ofxMidiOut leds;
        ofxMidiIn midiIn;

    private:
        void update(ofEventArgs & events); // update writes changes, use atomics for thread safe control

        void newMidiMessage(ofxMidiMessage & msg);

        void processMessage(const ofxMidiMessage & msg);

        std::atomic <bool> buttonLedsEnabled;

        std::vector <ofxMidiMessage> buffer;
        std::atomic <int> written;
        int lastRead;

        std::atomic <bool> bUpdate;

        std::vector <vector <Binding> > knobs;
        std::vector <RadioGroup>    radios;

        int buttonsColor;
        int channel;

        void radioChanged(int & value);
        void buttonChangedB(bool & value);
        void buttonChangedF(float & value);
        void buttonChangedI(int & value);
        void knob3Changed(glm::vec3 & value);

        void clearLeds(); // all leds off

        bool bEasing;
        float easeAmount;

        struct Binding {
            Binding();
            Binding(const Binding & other);

            int controlNum;
            TypeCode typeCode;            // 0 for unassigned, 1 for bool, 2 for float, 3 for int, 4 for radio
            atomic <bool> bUpdate;
            int buttonMode;                 // 0 toggle, 1 momentary, 2 radio

            ofParameter <glm::vec3> * pParamv3;
            ofParameter <float> * pParamf;
            ofParameter <int> * pParami;
            ofParameter <bool> * pParamb;

            std::atomic <float> value;
            std::atomic <float> values[3];

            std::atomic <bool> bActive;
            float minf;
            float maxf;
            int mini;
            int maxi;
            glm::vec3 minv3;
            glm::vec3 maxv3;

            int radioGroup;
            int radioValue;

            float z1;
            glm::vec3 z3;
        };

        struct RadioGroup {
            RadioGroup();
            RadioGroup(const RadioGroup & other);

            int min;
            int max;
            std::atomic <int>         value;
            ofParameter <int> * pParami;
            std::atomic <bool>        bUpdate;
            int color;
            int offColor;
        };

};
