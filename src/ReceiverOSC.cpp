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
        chair_states.push_back(0);
        vibration_hz.push_back(0.0);
    }
    
    receiver.setup(OSC_PORT);
}

//-----------------------------------------------
void ReceiverOSC::update(){
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        
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
        
        cout << "chairs = ";
        for(int i = 0; i < NUM_CHAIRS; i++){
            cout << chair_states[i] ;
        }
        cout << endl;
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
