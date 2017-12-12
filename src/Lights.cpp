//
//  Lights.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 8/12/17.
//
//

#include "Lights.h"


void Lights::setup(){
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    
    // specular color, the highlight/shininess color //
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    pointLight.setPointLight();
    
    
    
    spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
    spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    // turn the light into spotLight, emit a cone of light //
    spotLight.setSpotlight();
    
    // size of the cone of emitted light, angle between light axis and side of cone //
    // angle range between 0 - 90 in degrees //
    spotLight.setSpotlightCutOff( 50 );
    
    // rate of falloff, illumitation decreases as the angle from the cone axis increases //
    // range 0 - 128, zero is even illumination, 128 is max falloff //
    spotLight.setSpotConcentration( 45 );
    
    
    // Directional Lights emit light based on their orientation, regardless of their position //
    directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    
    // set the direction of the light
    // set it pointing from left to right -> //
    directionalLight.setOrientation( ofVec3f(0, 90, 0) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    //material.setShininess( 120 );
    // the light highlight of the material //
    //material.setSpecularColor(ofColor(255, 255, 255, 255));
}

void Lights::update(){
    float radius  = 180.0;
    ofPoint center;
    center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);

    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                           sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
    spotLight.setPosition( ofGetMouseX(), ofGetMouseY(), 200);
}

void Lights::enable(){
    ofEnableLighting();

    // enable the material, so that it applies to all 3D objects before material.end() call //
    //material.begin();
    
    pointLight.enable();
    spotLight.enable();
    directionalLight.enable();
    

}

void Lights::disable(){
    pointLight.disable();
    spotLight.disable();
    directionalLight.disable();
    
    //material.end();

    ofDisableLighting();

}
