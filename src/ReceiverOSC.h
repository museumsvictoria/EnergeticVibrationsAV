//
//  ReceiverOSC.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 9/2/18.
//
//

#pragma once 

#include "ofMain.h"
#include "ofxOsc.h"
#include "Constants.h"

#define NUM_CHAIRS 21
#define OSC_PORT 9001

class ReceiverOSC{
public:
    
    ReceiverOSC();
    void update();
    vector<int> get_chair_states();
    vector<float> get_vibration_speeds();
    vector<float> get_seat_kickers();
    
    // Secret Combos
    vector<vector<int> > seat_combos;
    vector<vector<int> > original_combos;

    // Timers for keeping track how long
    // it has been since a seat combo has occured
    // So we only pass on events if the amount of seconds
    // ellapsed is greater than a set period
    ofEvent<bool> combo_triggered;
    int seconds_since_combo;
    int init_combo_time;
    
    // Timers for keeping track how long
    // it has been since a seat has been sat on
    // So we only pass on events if the amount of seconds
    // ellapsed is greater than a set period
    ofEvent<bool> seat_triggered;
    int seconds_since_last_seat;
    int init_last_seat_time;
    
private:
    ofxOscReceiver receiver;
    vector<int> chair_states;
    vector<float> vibration_hz;
    vector<float> seat_kickers;
};
