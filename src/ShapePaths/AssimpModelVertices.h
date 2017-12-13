//
//  AssimpModelVertices.h
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 13/12/17.
//
//

#pragma once

#include "ofMain.h"
#include "BaseShape.h"
#include "ofxAssimpModelLoader.h"

class AssimpModelVertices : public BaseShape{
    
public:
    AssimpModelVertices();
    ofxAssimpModelLoader model;
};
