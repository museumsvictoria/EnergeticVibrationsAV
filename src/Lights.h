//
//  Lights.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 8/12/17.
//
//

/* This is not implemented yet. I think I need to read 
   more about how to properly implement lighting passes first
*/

#pragma once

#include "ofMain.h"

class Lights{
public:
  
    void setup();
    void update();
    
    void enable();
    void disable();
    
    ofLight pointLight;
    ofLight spotLight;
    ofLight directionalLight;
    
    ofMaterial material;

};
