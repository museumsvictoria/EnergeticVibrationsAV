//
//  ofxCameraMove.cpp
//  emptyExample
//
//  Created by strimbob on 14/08/2013.
//  Adapted by Joshua Batty on 11/03/2018
// 

#include "ofxCameraMove.h"

//--------------------------------------------------------------
void ofxCameraMove::setup(ofEasyCam *_cam,string folder) {
    cam = _cam;
    destFolder = folder;
    current_camera = 0;
    getNumberOfCamFormXML(folder);
    start = 0;
    end = 1;
    pre = 0;
}

int ofxCameraMove::getNumCameras(){
    return numCameras-1;
}

//--------------------------------------------------------------
void  ofxCameraMove::getNumberOfCamFormXML(string folder){
    ofDirectory checkDir;
    checkDir.isDirectoryEmpty(folder);
    
    vector<string> xmlPath = loadString(folder);

    numCameras = xmlPath.size();
    cout << "NUM CAMERAS = " << numCameras << endl;
    for(int a = 0; a < xmlPath.size(); a++){

        ofQuaternion sQuat;
        ofVec3f sPos;
        ofxXmlSettings XML;
        XML.loadFile(xmlPath[a]);
        
        sPos.set(ofVec3f(XML.getValue("camera:getGlobalPositionX", 0.0),
                             XML.getValue("camera:getGlobalPositionY",0.0),
                             XML.getValue("camera:getGlobalPositionZ",  0.0)));

        startPositions.push_back(sPos);
        
        float angle =  XML.getValue("camera:angle", 0.0);
        float x =      XML.getValue("camera:x",  0.0);
        float y =      XML.getValue("camera:y",  0.0);
        float z =      XML.getValue("camera:z",  0.0);

        sQuat.makeRotate(angle, x,y,z);
        startQuaternions.push_back(sQuat);
    }
}
//--------------------------------------------------------------
vector<string> ofxCameraMove::loadString(string folder){
    vector<string> stringTemp;
    ofDirectory dir;
    dir.listDir(folder);
    dir.sort();
    
    for(int i = 0; i < (int)dir.size(); i++){
        stringTemp.push_back(dir.getPath(i));
        cout << " loading " << dir.getPath(i) << endl;
    }
    return stringTemp;
}

//--------------------------------------------------------------
void ofxCameraMove::update(){
    Tweener.update();
    if(start != pre){
        float tweenvalue = ofMap (start,0,1,0.,1.,true);
        ofQuaternion tweenedCameraQuaternion;
        tweenedCameraQuaternion.slerp(tweenvalue, startQuat, targetQuat);
        ofVec3f lerpPos;
        lerpPos = startPos + ((targetPos-startPos) * tweenvalue);
        cam->setOrientation(tweenedCameraQuaternion);
        cam->setGlobalPosition(lerpPos);
        pre = start;
    }
}

//--------------------------------------------------------------
void ofxCameraMove::cutNow(int cameraNumber){
    current_camera = cameraNumber;
    
    if(cameraNumber<startPositions.size()){
        cam->setGlobalOrientation(startQuaternions[current_camera]);
        cam->setGlobalPosition(startPositions[current_camera]);
    }
} 

//--------------------------------------------------------------
void ofxCameraMove::tweenNow(int cameraNumber,float time) {
    current_camera = cameraNumber;
    startPos.set(cam->getGlobalPosition());

    whater = cam->getOrientationQuat() ;
    float angle;
    float xa;
    float ya;
    float za;
    whater.getRotate(angle, xa,ya,za);
    startQuat.makeRotate(angle, xa,ya,za);

    targetPos.set(startPositions[current_camera]);
    targetQu = startQuaternions[current_camera]; ;
    
    float angleEnd;
    float x;
    float y;
    float z;
    targetQu.getRotate(angleEnd, x,y,z);
    targetQuat.makeRotate(angleEnd, x,y,z);
    start = 0;
    end = 1;
    pre = 0;
    if(tweenType == 0){
        Tweener.addTween(start, end, time,&ofxTransitions::easeOutExpo);
    }
    if(tweenType == 1){
        Tweener.addTween(start, end, time,&ofxTransitions::easeInOutExpo);
    }
}
//--------------------------------------------------------------
void ofxCameraMove::saveCameraPosition(int _camNum){
    string camNum = ofToString(_camNum);
    
    ofxXmlSettings XML;

    ofQuaternion startQuat;
    ofQuaternion targetQuat;
    ofVec3f startPos;
    ofVec3f targetPos;
    ofQuaternion whater;
    startPos.set(cam->getGlobalPosition());
    whater = cam->getOrientationQuat() ;
    float angle;
    float xb;
    float yb;
    float zb;
    whater.getRotate(angle, xb,yb,zb);
    startQuat.makeRotate(angle, xb,yb,zb);
    XML.setValue("camera:angle", angle);
    XML.setValue("camera:x",  xb);
    XML.setValue("camera:y",  yb);
    XML.setValue("camera:z",  zb);
    XML.setValue("camera:getGlobalPositionX",  cam->getGlobalPosition().x);
    XML.setValue("camera:getGlobalPositionY",  cam->getGlobalPosition().y);
    XML.setValue("camera:getGlobalPositionZ",  cam->getGlobalPosition().z);
    XML.saveFile(destFolder + "/camera" + camNum + ".xml");
    cout << "saved to " + destFolder + "/camera" + camNum + ".xml" << endl;
}

void ofxCameraMove::setTweenType(int _tweenType){
    tweenType = _tweenType;
}
