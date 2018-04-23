//
//  ReceiverOSC.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 9/2/18.
//
//

#include "ReceiverOSC.h"

//-----------------------------------------------
ReceiverOSC::ReceiverOSC(){
    for(int i = 0; i < NUM_CHAIRS; i++){
        chair_states.push_back(1);
        vibration_hz.push_back(0.0);
        seat_kickers.push_back(1.0);
    }
    
    receiver.setup(OSC_PORT);
    
   
    original_combos.push_back({1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1});
    original_combos.push_back({1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0});
    original_combos.push_back({0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0});
    original_combos.push_back({0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0});
    original_combos.push_back({1,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,1});
    original_combos.push_back({1,1,0,1,0,0,0,1,1,0,1,0,0,0,1,1,0,1,0,0,0});
    
    seat_combos = original_combos;
    
    seconds_since_combo = 0;
    init_combo_time = (int)ofGetElapsedTimef();
}

//-----------------------------------------------
void ReceiverOSC::update(){
    
    seconds_since_combo = (int)ofGetElapsedTimef() - init_combo_time;
    seconds_since_last_seat = (int)ofGetElapsedTimef() - init_last_seat_time;
    
    //cout << "seconds since combo = " << seconds_since_combo;
    //cout << " -- seat_combos size = " << seat_combos.size() << endl;
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        int prev_chair_sum = 0;
        for(auto &chair: chair_states){
            prev_chair_sum += chair;
        }
        
        ///-------------- CHAIR TRIGGERS
        if(m.getAddress() == "/ev_chairs/group1/triggers"){
            for(int i = 0; i < 7; i++){
                chair_states[i] = m.getArgAsInt(i);
            }
        }
        if(m.getAddress() == "/ev_chairs/group2/triggers"){
            for(int i = 7; i < 14; i++){
                chair_states[i] = m.getArgAsInt(i-7);
            }
        }
        if(m.getAddress() == "/ev_chairs/group3/triggers"){
            for(int i = 14; i < 21; i++){
                chair_states[i] = m.getArgAsInt(i-14);
            }
        }
        
        int current_chair_sum = 0;
        for(auto &chair: chair_states){
            current_chair_sum += chair;
        }
        
        if(seconds_since_last_seat > TIME_SINCE_LAST_CHAIR_IN_SECONDS){
            // Check to see if new chairs have been sat on since the last loop
            if(current_chair_sum > prev_chair_sum){
                // initialise the count timer
                init_last_seat_time = (int)ofGetElapsedTimef();
                
                // Send an evvent to the ofApp to trigger a generative process
                bool combo = true;
                ofNotifyEvent(seat_triggered, combo, this);
            }
        }
        
        if(seconds_since_combo > 3){
            for(int i = 0; i < seat_combos.size(); i++){
                // Check to see is the current chair state matches any of the exisiting
                // seat combos
                if(seat_combos[i] == chair_states){
                    // Pop the combo from the seat_combos stack
                    // Return the result via an ofEvent to the ofApp to trigger generative event
                    bool combo = true;
                    ofNotifyEvent(combo_triggered, combo, this);

                    // remove the cmobo from the stack
                    seat_combos.erase(seat_combos.begin() + i);
                    
                    // initialise the count timer
                    init_combo_time = (int)ofGetElapsedTimef();
                    
                    // If we have run out of possible combos
                    if(seat_combos.size() == 0){
                        // Refill the availble seat combos with the originals
                        seat_combos = original_combos;
                    }
                }
            }
        }
//        cout << "chairs = ";
//        for(int i = 0; i < NUM_CHAIRS; i++){
//            cout << chair_states[i]  << "," ;
//        }
//        cout << endl;
        
        ///-------------- VIBRATION FREQUENCY
        if(m.getAddress() == "/ev_chairs/group1/rates"){
            for(int i = 0; i < 7; i++){
                vibration_hz[i] = m.getArgAsFloat(i);
            }
        }
        if(m.getAddress() == "/ev_chairs/group2/rates"){
            for(int i = 7; i < 14; i++){
                vibration_hz[i] = m.getArgAsFloat(i-7);
            }
        }
        if(m.getAddress() == "/ev_chairs/group3/rates"){
            for(int i = 14; i < 21; i++){
                vibration_hz[i] = m.getArgAsFloat(i-14);
            }
        }
        
        
        /////// TEMP DELETE ME EPIC HACK FOR ARUP
        if(m.getAddress() == "/ev_bass1"){
            seat_kickers[0] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }
        if(m.getAddress() == "/ev_bass2"){
            seat_kickers[7] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }
        if(m.getAddress() == "/ev_bass3"){
            seat_kickers[14] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }
        
        
        if(m.getAddress() == "/ev_drums1"){
            seat_kickers[1] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[2] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[3] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }
        if(m.getAddress() == "/ev_drums2"){
            seat_kickers[8] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[9] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[10] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }
        if(m.getAddress() == "/ev_drums3"){
            seat_kickers[15] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[16] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
            seat_kickers[17] = ofClamp(m.getArgAsFloat(0),0.0,1.0);
        }

    }
}

//-----------------------------------------------
vector<int> ReceiverOSC::get_chair_states(){
    return chair_states;
}

//-----------------------------------------------
vector<float> ReceiverOSC::get_vibration_speeds(){
    return vibration_hz;
}

//-----------------------------------------------
vector<float> ReceiverOSC::get_seat_kickers(){
    return seat_kickers;
}
