//
//  Grid.h
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//


#pragma once

#include "ofMain.h"
#include "BaseShape.h"

class Grid : public BaseShape{
public:
    Grid();
    
    void draw();
    void updateShape();
    void setWidth(float width);
    void setHeight(float height);
    void setSize(int size);
    void setRotateAmount(float rotationAmount);
    
private:
};
