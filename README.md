ofxLaunchControls
=====================================

This addon binds ofParameters floats, ints or bools to a Novation Launch Control or Launch Control XL midi controller. By having a `ofxLaunchControl lc` object binding the first knob to control one of yours parameter isn't harder than:   
```cpp
    lc.setup();
    lc.knob( 0, yourParameter);
```
now the first upper knob of the Launch Control controls your ofParameter using its min and max range.   
You can specify minimum and maximum ranges:   
```cpp
    lc.knob( 0, yourFloatParameter, 0.1f, 23.5f);
```
now values ranging from 0.1 to 23.5 are assigned to your parameter when you turn the knob.   
   
On the Launch Control XL you also have a `fader` method, that works in the same way as `knob`:   
```cpp
    lc.fader( 3, yourParameter ); // maps the fourth fader to your parameter
```   
   
You can also bind a knob to multiple values:   
```cpp
    lc.knob( 0, yourFloatParameter, 0.1f, 23.5f);
    lc.knob( 0, yourIntParameter, 20, 35 );
    lc.knob( 0, stillAnotherParameter );
```
   
You can map buttons to floats, int or bool and  they will lit to give you visual feedback. Here some useful methods:   
```cpp
    lc.momentary( 1, yourFloatParameter, 0.2f, 1.1f ); // second button, 1.1 when pressed, 0.2 when released
    lc.toggle( 2, yourBoolParameter );  // third button, true/false toggle
    lc.radio( 4, 7, yourIntValue );   // button from the fifth to the last mapped as radio button to an int value
```   

The movement of the knobs is also slewed using a simple easing algorhytm. By default the easing value is 0.5, you can also set it or deactivate it:   
```cpp
    lc.easing( 0.2f ); // slower easing 
    lc.disableEasing(); // easing off
```   
   
There's nothing harder then that. Have fun and remember to close you midi controllers when your applications quits:   
```cpp
   lc.close();
```  
   

Requires [ofxMidi](https://github.com/danomatika/ofxMidi).    
   
Nicola Pisanti, MIT License 2018.   


