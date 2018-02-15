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
        
        if(m.getAddress() == "/ev_chairs/group1"){
            for(int i = 0; i < 6; i++){
                chair_states[i] = m.getArgAsInt(i);
            }
        }
        if(m.getAddress() == "/ev_chairs/group2"){
            for(int i = 6; i < 12; i++){
                chair_states[i] = m.getArgAsInt(i-6);
            }
        }
        if(m.getAddress() == "/ev_chairs/group3"){
            for(int i = 12; i < 18; i++){
                chair_states[i] = m.getArgAsInt(i-12);
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
