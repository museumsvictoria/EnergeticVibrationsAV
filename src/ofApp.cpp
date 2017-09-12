#include "ofApp.h"

// Here's the general idea:
//
// 1.    Draw a field of instanced boxes, 256 in x, and 256 in y direction
// 2.    Push boxes up in y direction based on a height value sampled from the texture
// 2.a   Scale boxes in y direction to create "skyscrapers" based on sampled value
// 3.    Modulate height map texture sampling to create a "radar scanline" effect
// 4.    Add some colour effects
//
// BONUS POINTS: use the camera live feed as your height map texture.


//--------------------------------------------------------------
void ofApp::setup(){

    isShaderDirty = true; // initialise dirty shader
    
    // step 1: load our height map image
    
    ofDisableArbTex(); 	///< we need normalised image coordinates
    ofLoadImage(mTex1, "elevation2.png");
    ofEnableArbTex();
    
    // step 2: we will want to draw our geometry as instances
    // therefore, we will use a vbo
    
    primitives.push_back(ofIcoSpherePrimitive(1,1));
    mMshBox = primitives[0].getMesh();// ofBoxPrimitive(1, 1, 1).getMesh();
    
    mMshOptimisedBox = optimisedBox.getOptimisedBox();
    
    // we will also need a camera, so we can move around in the scene
    mCam1.setupPerspective(false, 60, 0.1, 5000);
    mCam1.setDistance(20); // set default camera distance to 1000
    //mCam1.boom(5); // move camera up a little
    mCam1.lookAt(ofVec3f(0)); // look at centre of the world
    
    //required call
    gui.setup();
    

    //Post Processing
    post.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    float val = -0.5+(sin(ofGetElapsedTimef()*0.05)*0.5)*3.;
//    mCam1.truck(val);
//   mCam1.boom(val);
//    mCam1.dolly(val);
//    mCam1.tilt(val);
//    mCam1.pan(val);
//    mCam1.roll(val);

    params.time = ofGetElapsedTimef();

    //Post Processing
    post.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0.65); // pro app gray =)
    ofSetColor(ofColor::white);
 
    /*
    //required to call this at beginning
    gui.begin();
    
    ImGui::Text("Hello, world!");
    //ImGui::SliderFloat("Scale", &params.scale, 0.0f, 10.0f);
    
    //required to call this at end
    gui.end();
    */

    // let's do something slightly more funky with this.
    
    // let's optimise this:
    
    // we're going to use a flat shaded box, with only 8 vertices per box.
    // That's going to reduce the amount of geometry (and vertex calculations) massively.
    
    // We're going to use a shader which scales the boxes,
    // so we can get something like a skyline.
    
    if (isShaderDirty){
        // dirty shader pattern:
        shared_ptr<ofxUboShader> tmpShader = shared_ptr<ofxUboShader>(new ofxUboShader);
        
//        if (tmpShader->load("shaders/instanced-2.vert", "shaders/instanced-2.frag")){
        if (tmpShader->load("shaders/vertexshaderart.vert", "shaders/vertexshaderart.frag")){
            mShd1 = tmpShader;
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        
        isShaderDirty = false;
    }
    
    // begin scene to post process
    post.begin();
    mCam1.begin();
    
    // alpha blending is enabled by default,
    // let's see if disabling it will help us a little.
    ofDisableBlendMode();
    
    
    // also, let's get rid of the back faces.
    glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.
    glFrontFace(GL_CCW);
    // tell GLSL to use the first vertex for flat shading the whole triangle, instead
    // of the last one, as would be the default.
    
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    // and enable depth testing.
    
    ofEnableDepthTest();
    
    if (mShd1) {
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 0);
        mShd1->setUniform1f("tick_position", (int)(ofGetElapsedTimef() * 4.4) % NUM_INSTANCES);
        mShd1->setUniform1i("tile_length", TILE_LENGTH);
        mShd1->setUniformBuffer("ShaderParams", params);
        mShd1->setUniformTexture("tex_unit_0", mTex1, 0); // first texture unit has index 0, name is not that important!
        // draw lots of boxes
        mMshBox.drawInstanced(OF_MESH_WIREFRAME, NUM_INSTANCES);
        mShd1->end();
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 1);
        mMshOptimisedBox.drawInstanced(OF_MESH_WIREFRAME, NUM_INSTANCES);
        mShd1->end();
        
    }
    ofDisableDepthTest();
    
    glDisable(GL_CULL_FACE);
    ofEnableAlphaBlending();
    
    mCam1.end();
    
    post.end();

    post.draw();
    
    // draw our frame rate
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(ofGetFrameRate(),2,4,' '), ofGetWidth() - 4 * 8 - 50, 16 + 20);
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key) {
        case ' ':
            isShaderDirty = true;
            break;
        case 'r':
        for(int i = 0; i < NUM_INSTANCES; i++){
            int active;
            float speed;
            if(ofRandomuf() < 0.1) {
                active = 1;
                speed = ofRandom(20.0);
            }
            else {
                active = 0;
                speed = 0.0;
            }
            params.active_chair[i] = active;
            params.transducer_speed[i] = speed;
        }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
