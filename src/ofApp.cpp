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
    params.scale_speed = 1.0;
    params.rot_speed = 0.3;
    explode_amount = 0.0;
    
    toggle_post_processing = false;
    toggle_blending = false;
    toggle_backface_cull = true;
    
    //Geometry shader
    geom_lfo_offset = 0.58;
    geom_lfo_speed = 0.5;
    geom_lfo_amp = 1.0;
    geom_effect_mix = 1.0;
    geom_effect_lfo_offset = 0.2;
    geom_effect_lfo_speed = 0.1;
    geom_effect_lfo_amp = 1.0;
    
    //Fragment Shader
    xray_mix = 1.0;
    xray_lfo_offset = 0.0;
    xray_lfo_speed = 0.5;
    xray_lfo_amp = 1.0;
    
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
    mCam1.setDistance(50); // set default camera distance to 1000
    //mCam1.boom(5); // move camera up a little
    mCam1.lookAt(ofVec3f(0)); // look at centre of the world
    
    //Post Processing
    post.setup();

    init();
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    ofSetBackgroundColor(0);

    //load fonts first
    gui_theme.load_font();
    
    //required call
    gui.setup();
    
    //load theme
    gui_theme.init_theme();

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    //float orbit_x = 0.5+(saw(ofGetElapsedTimef()*0.4)*0.5)*360.;
    //float orbit_y = 0.5+(saw(ofGetElapsedTimef()*0.2)*0.5)*360.;
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
    if(toggle_post_processing){
        post.update();
    }
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
            ImGui::SliderFloat("Speed",&params.scale_speed,0.0,1.0);
            ImGui::SliderFloat("Rotation Speed",&params.rot_speed,0.0,1.0);
            ImGui::SliderFloat("Explode Scale",&explode_amount,0.0,20.0);

            if(ImGui::SmallButton("Random Primitive")){
                primitives.randomise_objects();
            }
            ImGui::SameLine();
            if(ImGui::SmallButton("Random Mesh Res")){
                primitives.randomise_mesh_resolution();
            }
            
            ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree("Geometry Shader", mainSettings)){
            ImGui::SliderFloat("Geom LFO Offset",&geom_lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Speed",&geom_lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Amp",&geom_lfo_amp,0.0,1.0);
            ImGui::Separator();
            ImGui::SliderFloat("Geom Efct Offset",&geom_effect_lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Speed",&geom_effect_lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Amp",&geom_effect_lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Mix",&geom_effect_mix,0.0,1.0);

            ofxImGui::EndTree(mainSettings);
        }

        if (ofxImGui::BeginTree("Fragment Shader", mainSettings)){
            ImGui::SliderFloat("Xray Offset",&xray_lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Xray Speed",&xray_lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Xray Amp",&xray_lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Xray Mix",&xray_mix,0.0,1.0);
            ImGui::Separator();
            
            ofxImGui::EndTree(mainSettings);
        }
        
        
        
        if (ofxImGui::BeginTree("DOF", mainSettings)){
            ImGui::SliderFloat("Blur Amount",&post.dof_blur_amount,0.0,3.0);
            ImGui::SliderFloat("Focal Distance",&post.dof_focal_distance,0.0,600.0);
            ImGui::SliderFloat("Focal Range",&post.dof_focal_range,0.0,500.0);
            
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("REACTION DIFFUSION", mainSettings)){
            ImGui::SliderFloat("Intensity",&post.reaction_diffusion.intensity,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("ALPHA TRAILS", mainSettings)){
            ImGui::SliderFloat("Delay Amount",&post.trail_delay,0.0,0.99);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("GL STATE", mainSettings)){
            ImGui::Checkbox("Post Processing", &toggle_post_processing);
            ImGui::Checkbox("GL Blending Add", &toggle_blending);
            ImGui::Checkbox("Cull Backface", &toggle_backface_cull);
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

        if (tmpShader->load("shaders/vertexshaderart.vert", "shaders/vertexshaderart.frag", "shaders/vertexshaderart.geom")){
            mShd1 = tmpShader;
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        
        isShaderDirty = false;
    }
    
    // begin scene to post process
    if(toggle_post_processing){
        post.dof_begin();
    }
    
    mCam1.begin();
    
    // alpha blending is enabled by default,
    // let's see if disabling it will help us a little.
    //ofDisableBlendMode();
    if(toggle_blending){
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    
    // also, let's get rid of the back faces.
    if(toggle_backface_cull){
        glEnable(GL_CULL_FACE); // wohooo! 200% performance boost.
        glFrontFace(GL_CCW);
    }
    
    // tell GLSL to use the first vertex for flat shading the whole triangle, instead
    // of the last one, as would be the default.
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    // and enable depth testing.
    ofEnableDepthTest();
    
    if (mShd1) {
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 0);
        mShd1->setUniform1f("tick_position", (int)(ofGetElapsedTimef() * 4.4) % NUM_INSTANCES);
        mShd1->setUniform1f("time", ofGetElapsedTimef());
        mShd1->setUniform1f("alpha", abs(sin(ofGetElapsedTimef())*255));
        mShd1->setUniform1f("explode_amount", explode_amount);
        mShd1->setUniformBuffer("ShaderParams", params);
        mShd1->setUniformTexture("tex_unit_0", mTex1, 0); // first texture unit has index 0, name is not that important!
        // draw lots of boxes
        
        
        //---- Geometry shader pass
        mShd1->setUniform1f("geom_lfo_offset", geom_lfo_offset);
        mShd1->setUniform1f("geom_lfo_speed", geom_lfo_speed);
        mShd1->setUniform1f("geom_lfo_amp", geom_lfo_amp);
        mShd1->setUniform1f("geom_effect_lfo_offset", geom_effect_lfo_offset);
        mShd1->setUniform1f("geom_effect_lfo_speed", geom_effect_lfo_speed);
        mShd1->setUniform1f("geom_effect_lfo_amp", geom_effect_lfo_amp);
        mShd1->setUniform1f("geom_effect_mix", geom_effect_mix);
        
        primitives.draw_idle_mesh();
        mShd1->end();
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 1);
        mShd1->setUniform1f("xray_lfo_offset", xray_lfo_offset);
        mShd1->setUniform1f("xray_lfo_speed", xray_lfo_speed);
        mShd1->setUniform1f("xray_lfo_amp", xray_lfo_amp);
        mShd1->setUniform1f("xray_mix", xray_mix);
        primitives.draw_active_mesh();
        mShd1->end();
    }
    
    ofDisableDepthTest();
    
    if(toggle_backface_cull){
        glDisable(GL_CULL_FACE);
    }
    
    ofEnableAlphaBlending();

    mCam1.end();
    
    if(toggle_post_processing){
        post.dof_end();
        post.draw();
        ofSetColor(255,50);
        post.depthOfField.getFbo().draw(0, 0);
    }
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
