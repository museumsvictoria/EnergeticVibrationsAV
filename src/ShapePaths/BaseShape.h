
//
//  BaseShape.h
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

/*
 Shapes:
 - Rectangle
 - Ellipse
 - N Polygon
 - Star
 - 2d Super Formula
 - Triangle
 */

#pragma once 

#include "ofMain.h"

class BaseShape{
public:
    BaseShape(){
        BaseShape::init();
    }
    
    void init();
    virtual void draw();
    virtual void updateShape();
    virtual void setWidth(float width);
    virtual void setHeight(float height);
    virtual void setSize(int size);
    virtual void setRotateAmount(float rotationAmount);

    //------------------ Polyline Shape
    ofPolyline shape;
    
    //------------------ Rectangle
    int size;
    float width;
    float height;
    float rotationAmount;
    
    ofPoint center;
private:


};