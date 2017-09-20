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
void ofApp::init(){
    params.shape_morph = 0.0;
    params.circle_motion = 0.0;
    params.speed = 1.0;
    explode_amount = 0.0;
    
    for(int i = 0; i < NUM_INSTANCES; i++){
        params.transducer_speed[i] = 0.0;
        params.active_chair[i] = 0;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){

    isShaderDirty = true; // initialise dirty shader
    
    // step 1: load our height map image
    
    ofDisableArbTex(); 	///< we need normalised image coordinates
    ofLoadImage(mTex1, "elevation2.png");
    ofEnableArbTex();
    
    // step 2: we will want to draw our geometry as instances
    // therefore, we will use a vbo
    primitives.setup();
    
    cam_near_clip = 0.0;
    cam_far_clip = 0.0;
    
    // we will also need a camera, so we can move around in the scene
    mCam1.setupPerspective(false, 60, 0.1, 5000);
    mCam1.setDistance(20); // set default camera distance to 1000
    //mCam1.boom(5); // move camera up a little
    mCam1.lookAt(ofVec3f(0)); // look at centre of the world
    
    //Post Processing
    post.setup();

    init();
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    ofSetBackgroundColor(0);

    //required call
    gui.setup();
}

//--------- LFO's --------------
float fract(float f) {
    float temp;
    return modf(f, &temp);
}

// FLOATS
float tri(float x) {
    return asin(sin(x))/(PI/2.);
}
float puls(float x) {
    return (floor(sin(x))+0.5)*2.;
}
float saw(float x) {
    return (fract((x/2.)/PI)-0.5)*2.;
}
float noise(float x) {
    return (fract(sin((x*2.) *(12.9898+78.233)) * 43758.5453)-0.5)*2.;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    float orbit_x = 0.5+(saw(ofGetElapsedTimef()*0.4)*0.5)*360.;
    float orbit_y = 0.5+(saw(ofGetElapsedTimef()*0.2)*0.5)*360.;
//    mCam1.orbitDeg(orbit_x, orbit_y, 10);
//    mCam1.truck(val);
//   mCam1.boom(val);
//    mCam1.dolly(val);
//    mCam1.tilt(val);
//    mCam1.pan(val);
//    mCam1.roll(val);

    mCam1.setNearClip(cam_near_clip);
    mCam1.setFarClip(cam_far_clip);
    
    //Post Processing
    post.update();
    
    
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
    this->gui.begin();

    auto mainSettings = ofxImGui::Settings();
    mainSettings.windowPos = ofVec2f(0, 0);


    if (ofxImGui::BeginWindow("Layer Assignments", mainSettings, false))
    {
        // Basic columns
        if (ofxImGui::BeginTree("Geometry", mainSettings)){
            ImGui::SliderFloat("Speed",&params.speed,0.0,1.0);
            ImGui::SliderFloat("Shape Morph",&params.shape_morph,0.0,1.0);
            ImGui::SliderFloat("Circle Motion",&params.circle_motion,0.0,1.0);
            ImGui::SliderFloat("Explode Scale",&explode_amount,0.0,20.0);

            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("DOF", mainSettings)){
            ImGui::SliderFloat("Blur Amount",&post.dof_blur_amount,0.0,3.0);
            ImGui::SliderFloat("Focal Distance",&post.dof_focal_distance,0.0,600.0);
            ImGui::SliderFloat("Focal Range",&post.dof_focal_range,0.0,500.0);
            
            ofxImGui::EndTree(mainSettings);
        }
    }
    ofxImGui::EndWindow(mainSettings);
    
    this->gui.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0.65); // pro app gray =)
    ofSetColor(ofColor::white);
 
    // let's do something slightly more funky with this.
    
    // let's optimise this:
    
    // we're going to use a flat shaded box, with only 8 vertices per box.
    // That's going to reduce the amount of geometry (and vertex calculations) massively.
    
    // We're going to use a shader which scales the boxes,
    // so we can get something like a skyline.
    
    if (isShaderDirty){
        // dirty shader pattern:
        shared_ptr<ofxUboShader> tmpShader = shared_ptr<ofxUboShader>(new ofxUboShader);
        
        tmpShader->setGeometryInputType(GL_TRIANGLES);
        tmpShader->setGeometryOutputType(GL_TRIANGLE_STRIP);
        tmpShader->setGeometryOutputCount(4);
        
//        if (tmpShader->load("shaders/instanced-2.vert", "shaders/instanced-2.frag")){
        if (tmpShader->load("shaders/vertexshaderart.vert", "shaders/vertexshaderart.frag", "shaders/vertexshaderart.geom")){
            mShd1 = tmpShader;
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        
        isShaderDirty = false;
    }
    
    // begin scene to post process
    post.dof_begin();
    mCam1.begin();
    
    // alpha blending is enabled by default,
    // let's see if disabling it will help us a little.
    //ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
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
        mShd1->setUniform1f("time", ofGetElapsedTimef());
        mShd1->setUniform1f("explode_amount", explode_amount);
        mShd1->setUniformBuffer("ShaderParams", params);
        mShd1->setUniformTexture("tex_unit_0", mTex1, 0); // first texture unit has index 0, name is not that important!
        // draw lots of boxes
        primitives.draw_idle_mesh();
        mShd1->end();
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 1);
        primitives.draw_active_mesh();
        mShd1->end();
        
    }
    ofDisableDepthTest();
    
    glDisable(GL_CULL_FACE);
    ofEnableAlphaBlending();
    
    mCam1.end();
    
    post.dof_end();

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
            break;
        case 'p':
            primitives.randomise_objects();
            break;
        case 'm':
            primitives.randomise_mesh_resolution();
            break;
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
    post.init();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
