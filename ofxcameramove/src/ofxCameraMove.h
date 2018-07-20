//
//  ofxCameraMove.h
//  emptyExample
//
//  Created by strimbob on 14/08/2013.
//  Adapted by Joshua Batty on 11/03/2018
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxTweener.h"


class ofxCameraMove {
	
public:
	void setup(ofEasyCam *_cam,string folder);
    void update();    
    void tweenNow(int cameraNumber,float time);
    void cutNow(int cameraNumber);
    void getNumberOfCamFormXML(string folder);
    void setTweenType(int _tweenType); // not good how to change???
    vector<string> loadString(string path);
    void saveCameraPosition(int _camNum);
    
    int getNumCameras();
        
    float start;
    float end;
    float pre;
private:
    int tweenType;
	ofEasyCam *cam;
    vector<ofVec3f> startPositions;
    vector<ofQuaternion> startQuaternions;
    ofVec3f startPos;
    ofQuaternion startQuat;
    ofQuaternion targetQuat;
    ofQuaternion whater;
    ofQuaternion targetQu;
    ofVec3f targetPos;

    string destFolder;
    int numCameras;
    int current_camera;
};

