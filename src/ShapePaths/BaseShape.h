
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
 - N Polygon
 - Wave

 Shapes are defined in screen coordinates and then normalised in the vertex 
 shader.
 */

#pragma once
#include "ofMain.h"
#include "Constants.h"



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
    vector<glm::vec3> get_path();

    //------------------ Rectangle
    int size;
    float width;
    float height;
    float rotationAmount;
    
    ofPoint center;
private:


};
