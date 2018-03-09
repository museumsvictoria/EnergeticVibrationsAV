#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::init(){
    //params.scale_speed = 1.0;
    //params.rot_speed = 0.3;
    explode_amount = 0.0;
    
    toggle_post_processing = true;
    toggle_blending = true;
    toggle_backface_cull = true;
    
    //Geometry shader
    geom_max_height = 4.0;
    geom_num_copies = 1;
    
    geom.lfo_type1 = (int)ofRandom(33);
    geom.lfo_offset = 0.58;
    geom.lfo_speed = 0.02;
    geom.lfo_amp = 1.0;
    geom_effect.lfo_type1 = (int)ofRandom(33);
    geom_effect.mix = 0.0;
    geom_effect.lfo_offset = 0.2;
    geom_effect.lfo_speed = 0.1;
    geom_effect.lfo_amp = 1.0;
    
    //Fragment Shader
    xray.lfo_type1 = (int)ofRandom(33);
    xray.lfo_type2 = (int)ofRandom(33);
    xray.mix = 1.0;
    xray.lfo_offset = 0.0;
    xray.lfo_speed = 0.5;
    xray.lfo_amp = 1.0;

    
    for(int i = 0; i < NUM_INSTANCES; i++){
        //params.transducer_speed[i] = 0.0;
        //params.active_chair[i] = 0;
        params.instance_model_grid[i] = glm::vec3(0.0,0.0,0.0);
        
        if(i % 7 == 0) params.object_size[i] = 50.0; // large seat
        else if((i % 7) < 4) params.object_size[i] = 25.0; // middle size seats
        else params.object_size[i] = 12.5; // small seats
        
        params.vibration_hz[i] = 0.0;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){

    isShaderDirty = true; // initialise dirty shader
    
    //Post Processing
    post.setup();
    textures.setup();
    primitives.setup();
    
    toggle_camera_automation = false;
    cam_near_clip = 1.0;
    cam_far_clip = 1000.0;
    
    // we will also need a camera, so we can move around in the scene
    mCam1.setupPerspective(false, 50, 0.001, 0);
    mCam1.setDistance(330); // set default camera distance to 1000
    mCam1.lookAt(ofVec3f(0)); // look at centre of the world
    
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

    osc.update();
    for(int i = 0; i < osc.get_chair_states().size(); i++){
        params.active_chair[i] = osc.get_chair_states()[i];
        params.vibration_hz[i] = osc.get_vibration_speeds()[i];
    }
    
    if(toggle_camera_automation){
        if(ofGetFrameNum() % 200 == 0){
            cam_tweens.randomise_distance();
            cam_tweens.trigger();
            vector<float> speeds;
            for(int i = 0; i < cam_tweens.size(); i++){
                speeds.push_back(ofRandom(1,3));
            }
            cam_tweens.set_duration(speeds);
            for(int i = 0; i < cam_tween_types.size(); i++){
                cam_tween_types[i] = (int)ofRandom(cam_tweens.size());
            }
        }
        
        float orbit_x = ofMap(cam_tweens.get_value()[cam_tween_types[0]],0.0,1.0,-90,90);
        float orbit_y = ofMap(cam_tweens.get_value()[cam_tween_types[1]],0.0,1.0,-90,90);
        float orbit_z = ofMap(cam_tweens.get_value()[cam_tween_types[2]],0.0,1.0,450,150);
        mCam1.orbitDeg(orbit_x, orbit_y, orbit_z);
    }

    mCam1.setNearClip(cam_near_clip);
    mCam1.setFarClip(cam_far_clip);
    
    //Post Processing
    if(toggle_post_processing){
        post.update();
    }
    
    paths.update();
    paths.set_model_path(params.instance_model_grid);

    
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
        
        shared_ptr<ofxUboShader> tmpShader2 = shared_ptr<ofxUboShader>(new ofxUboShader);
        tmpShader2->setGeometryInputType(GL_TRIANGLES);
        tmpShader2->setGeometryOutputType(GL_TRIANGLE_STRIP);
        tmpShader2->setGeometryOutputCount(4);
        
        if (tmpShader2->load("shaders/idle_mesh.vert", "shaders/idle_mesh.frag", "shaders/vertexshaderart.geom")){
            mShd2 = tmpShader2;
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        isShaderDirty = false;
    }
    
 /*
    if(ofGetFrameNum() % 100 == 0){
        geom.lfo_type1 = (int)ofRandom(34);
        geom_effect.lfo_type1 = (int)ofRandom(34);
        xray.lfo_type1 = (int)ofRandom(34);
        xray.lfo_type2 = (int)ofRandom(34);

        geom.lfo_offset = ofRandomuf();
        geom.lfo_speed = ofRandomuf();
        geom.lfo_amp = ofRandomuf();
        geom_effect.mix = ofRandomuf();
        geom_effect.lfo_offset = ofRandomuf();
        geom_effect.lfo_speed = ofRandomuf();
        geom_effect.lfo_amp = ofRandomuf();
        
        //Fragment Shader
        xray.mix = ofRandomuf();
        xray.lfo_offset = ofRandomuf();
        xray.lfo_speed = ofRandomuf();
        xray.lfo_amp = ofRandomuf();
        
        primitives.randomise_mesh_resolution();
    }
   
    float speed = ofGetElapsedTimef() * 0.01;
    geom.lfo_offset = ofNoise(speed);
    geom.lfo_speed = ofNoise(speed+1);
    geom.lfo_amp = ofNoise(speed+2);
    geom_effect.mix = ofNoise(speed+3);
    geom_effect.lfo_offset = ofNoise(speed+4);
    geom_effect.lfo_speed = ofNoise(speed+5);
    geom_effect.lfo_amp = ofNoise(speed+6);
    
    //Fragment Shader
    xray.mix = ofNoise(speed+7);
    xray.lfo_offset = ofNoise(speed+8);
    xray.lfo_speed = ofNoise(speed+10);
    xray.lfo_amp = ofNoise(speed+11);
    */
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0.65); // pro app gray =)
    ofSetColor(ofColor::white);

    //----------------------------------------------------------------
    //----------------- ACTIVE MESH MODE DRAWING
    
    // begin scene to post process
    if(toggle_post_processing){
        post.dof_begin();
    }
    
    mCam1.begin();
    
    // we need to flip the view port if we are doing postprocessing
    if(toggle_post_processing){
        ofScale (1,-1,1);
    }

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
    
    ofTexture tex = textures.getActiveTexture();
    tex.bind();
    
    if (mShd1) {
        
        mShd1->begin();
        for(int i = 0; i < 2; i++){
            
            mShd1->setUniform1i("is_active", i); // this is used in the frag shader
            mShd1->setUniform1f("time", ofGetElapsedTimef());
            mShd1->setUniform1f("alpha", abs(sin(ofGetElapsedTimef())*255));
            mShd1->setUniformBuffer("ShaderParams", params);
            
            //---- Geometry shader pass
            mShd1->setUniform1i("active_idx", primitives.get_index());
            mShd1->setUniform1i("geom_num_copies", geom_num_copies);
            mShd1->setUniform1f("geom_max_height", geom_max_height);

            mShd1->setUniform1i("geom_lfo_type", geom.lfo_type1);
            mShd1->setUniform1f("geom_lfo_offset", geom.lfo_offset);
            mShd1->setUniform1f("geom_lfo_speed", geom.lfo_speed);
            mShd1->setUniform1f("geom_lfo_amp", geom.lfo_amp);
            mShd1->setUniform1i("geom_effect_lfo_type", geom_effect.lfo_type1);
            mShd1->setUniform1f("geom_effect_lfo_offset", geom_effect.lfo_offset);
            mShd1->setUniform1f("geom_effect_lfo_speed", geom_effect.lfo_speed);
            mShd1->setUniform1f("geom_effect_lfo_amp", geom_effect.lfo_amp);
            mShd1->setUniform1f("geom_effect_mix", geom_effect.mix);
            
            if(i ==0) primitives.draw_filled_mesh();
            else primitives.draw_wireframe_mesh();

            mShd1->setUniformTexture("tex_unit_0", tex, 0); // first texture unit has index 0, name is not that important!
            mShd1->setUniform1i("fill_lfo_type", xray.lfo_type1);
            mShd1->setUniform1i("wireframe_lfo_type", xray.lfo_type2);
            mShd1->setUniform1f("xray_lfo_offset", xray.lfo_offset);
            mShd1->setUniform1f("xray_lfo_speed", xray.lfo_speed);
            mShd1->setUniform1f("xray_lfo_amp", xray.lfo_amp);
            mShd1->setUniform1f("xray_mix", xray.mix);
        }
        mShd1->end();
    }
    tex.unbind();

    ofDisableDepthTest();
    
    if(toggle_backface_cull){
        glDisable(GL_CULL_FACE);
    }
    mCam1.end();

    //----------------------------------------------------------------
    //-----------------  DRAW POST PROCESSING
    if(toggle_post_processing){
        post.dof_end();
        post.draw();
    }

    //----------------------------------------------------------------
    //----------------- IDLE MESH MODE DRAWING
    mCam1.begin();
    
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
    
    tex = textures.getIdleTexture();
    tex.bind();
    
    if (mShd2) {
        mShd2->begin();
        for(int i = 0; i < 2; i++){
            
            mShd2->setUniform1i("is_active", i);
            mShd2->setUniform1f("time", ofGetElapsedTimef());
            mShd2->setUniform1f("alpha", abs(sin(ofGetElapsedTimef())*255));
            mShd2->setUniformBuffer("ShaderParams", params);
            
            //---- Geometry shader pass
            mShd2->setUniform1i("active_idx", primitives.get_index());
            mShd2->setUniform1i("geom_num_copies", geom_num_copies);
            mShd2->setUniform1f("geom_max_height", geom_max_height);
            
            mShd2->setUniform1i("geom_lfo_type", geom.lfo_type1);
            mShd2->setUniform1f("geom_lfo_offset", geom.lfo_offset);
            mShd2->setUniform1f("geom_lfo_speed", geom.lfo_speed);
            mShd2->setUniform1f("geom_lfo_amp", geom.lfo_amp);
            mShd2->setUniform1i("geom_effect_lfo_type", geom_effect.lfo_type1);
            mShd2->setUniform1f("geom_effect_lfo_offset", geom_effect.lfo_offset);
            mShd2->setUniform1f("geom_effect_lfo_speed", geom_effect.lfo_speed);
            mShd2->setUniform1f("geom_effect_lfo_amp", geom_effect.lfo_amp);
            mShd1->setUniform1f("geom_effect_mix", geom_effect.mix);
            
            if(i ==0) primitives.draw_idle_filled_box();
            else primitives.draw_idle_wireframe_box();
            
            mShd2->setUniformTexture("tex_unit_0", tex, 0); // first texture unit has index 0, name is not that important!
            mShd2->setUniform1i("fill_lfo_type", xray.lfo_type1);
            mShd2->setUniform1i("wireframe_lfo_type", xray.lfo_type2);
            mShd2->setUniform1f("xray_lfo_offset", xray.lfo_offset);
            mShd2->setUniform1f("xray_lfo_speed", xray.lfo_speed);
            mShd2->setUniform1f("xray_lfo_amp", xray.lfo_amp);
            mShd2->setUniform1f("xray_mix", xray.mix);
        }
        mShd2->end();
    }
    tex.unbind();
    ofDisableDepthTest();
    
    if(toggle_backface_cull){
        glDisable(GL_CULL_FACE);
    }
    mCam1.end();
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key) {
        case ' ':
            isShaderDirty = true;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 't':
            textures.load_random_idle_texture();
            textures.load_random_active_texture();
            break;
        case 'e':
            geom.lfo_type1 = (int)ofRandom(34);
            geom_effect.lfo_type1 = (int)ofRandom(34);
            xray.lfo_type1 = (int)ofRandom(34);
            xray.lfo_type2 = (int)ofRandom(34);
            break;
        case 'g':
            geom.lfo_offset = ofRandomuf();
            geom.lfo_speed = ofRandomuf();
            geom.lfo_amp = ofRandomuf();
            geom_effect.mix = ofRandomuf();
            geom_effect.lfo_offset = ofRandomuf();
            geom_effect.lfo_speed = ofRandomuf();
            geom_effect.lfo_amp = ofRandomuf();
            
            //Fragment Shader
            xray.mix = ofRandomuf();
            xray.lfo_offset = ofRandomuf();
            xray.lfo_speed = ofRandomuf();
            xray.lfo_amp = ofRandomuf();
            break;
        case 'p':
            primitives.randomise_objects();
            break;
        case 'm':
            primitives.randomise_mesh_resolution();
            break;
        case 'z':
        {
            cam_tweens.randomise_distance();
            cam_tweens.trigger();
            vector<float> speeds;
            for(int i = 0; i < cam_tweens.size(); i++){
                speeds.push_back(ofRandom(1,3));
            }
            cam_tweens.set_duration(speeds);
            for(int i = 0; i < cam_tween_types.size(); i++){
                cam_tween_types[i] = (int)ofRandom(cam_tweens.size());
            }
            break;
        }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
    this->gui.begin();
    
    auto mainSettings = ofxImGui::Settings();
    mainSettings.windowPos = ofVec2f(0, 0);
    
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
    const char* items[] = { "Sine", "Triangle", "Saw", "Pulse", "Noise", "Exponential In", "Exponential Out", "Exponential In Out", "Sine In", "Sine Out", "Sine In Out", "Qintic In", "Qintic Out", "Qintic In Out", "Quartic In", "Quartic Out", "Quartic In Out", "Quadratic In", "Quadratic Out", "Quadratic In Out", "Cubic In", "Cubic Out", "Cubic In Out", "Elastic In", "Elastic Out", "Elastic In Out", "Circular In", "Circular Out", "Circular In Out", "Bounce In", "Bounce Out", "Bounce In Out", "Back In", "Back Out", "Back In Out" };
    
    if (ofxImGui::BeginWindow("Layer Assignments", mainSettings, false))
    {
        if (ofxImGui::BeginTree("Active Shader", mainSettings)){
            ImGui::SliderInt("Num Copies",&geom_num_copies,1,15);
            ImGui::SliderFloat("Max Height",&geom_max_height,0.0,10.0);
            //ImGui::SliderFloat("Speed",&params.scale_speed,0.0,1.0);
            //ImGui::SliderFloat("Rotation Speed",&params.rot_speed,0.0,1.0);
            
            ImGui::Combo("Geom LFO", &geom.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Geom LFO Offset",&geom.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Speed",&geom.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Amp",&geom.lfo_amp,0.0,1.0);

            ImGui::Combo("Efct LFO", &geom_effect.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Geom Efct Offset",&geom_effect.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Speed",&geom_effect.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Amp",&geom_effect.lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Mix",&geom_effect.mix,0.0,1.0);
            
            ImGui::Combo("Fill LFO", &xray.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::Combo("Wireframe LFO", &xray.lfo_type2, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Xray Offset",&xray.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Xray Speed",&xray.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Xray Amp",&xray.lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Xray Mix",&xray.mix,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree("Idle Shader", mainSettings)){
            ImGui::SliderInt("Num Copies",&geom_num_copies,1,15);
            ImGui::SliderFloat("Max Height",&geom_max_height,0.0,10.0);
            //ImGui::SliderFloat("Speed",&params.scale_speed,0.0,1.0);
            //ImGui::SliderFloat("Rotation Speed",&params.rot_speed,0.0,1.0);
            
            ImGui::Combo("Geom LFO", &geom.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Geom LFO Offset",&geom.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Speed",&geom.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom LFO Amp",&geom.lfo_amp,0.0,1.0);
            
            ImGui::Combo("Efct LFO", &geom_effect.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Geom Efct Offset",&geom_effect.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Speed",&geom_effect.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Amp",&geom_effect.lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Geom Efct Mix",&geom_effect.mix,0.0,1.0);
            
            ImGui::Combo("Fill LFO", &xray.lfo_type1, items, IM_ARRAYSIZE(items));
            ImGui::Combo("Wireframe LFO", &xray.lfo_type2, items, IM_ARRAYSIZE(items));
            ImGui::SliderFloat("Xray Offset",&xray.lfo_offset,0.0,1.0);
            ImGui::SliderFloat("Xray Speed",&xray.lfo_speed,0.0,1.0);
            ImGui::SliderFloat("Xray Amp",&xray.lfo_amp,0.0,1.0);
            ImGui::SliderFloat("Xray Mix",&xray.mix,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }

    }
    ofxImGui::EndWindow(mainSettings);
    
    //-------------- POST PROCESSING WINDOW
    mainSettings.windowPos = ofVec2f(440, 0);
    if (ofxImGui::BeginWindow("Post Processing", mainSettings, false)) {
        if (ofxImGui::BeginTree("GL STATE", mainSettings)){
            ImGui::Checkbox("Post Processing", &toggle_post_processing);
            
            static bool vid_actvie_toggle = false;
            ImGui::SameLine();
            ImGui::Checkbox("Toggle Active Play", &vid_actvie_toggle);
            if(vid_actvie_toggle == true) {
                textures.vid_active.setPaused(false);
            } else {
                textures.vid_active.setPaused(true);
            }
            ImGui::Checkbox("GL Blending Add", &toggle_blending);
            
            static bool vid_idle_toggle = true;
            ImGui::SameLine();
            ImGui::Checkbox("Toggle Idle Play", &vid_idle_toggle);
            if(vid_idle_toggle == true) {
                textures.vid_idle.setPaused(false);
            } else {
                textures.vid_idle.setPaused(true);
            }
            ImGui::Checkbox("Cull Backface", &toggle_backface_cull);
            ImGui::SameLine();
            ImGui::Checkbox("Automate Cam", &toggle_camera_automation);
            
            if(ImGui::SmallButton("Random Primitive")){
                primitives.randomise_objects();
            }
            ImGui::SameLine();
            if(ImGui::SmallButton("Random Mesh Res")){
                primitives.randomise_mesh_resolution();
            }
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("DOF", mainSettings)){
            ImGui::SliderFloat("Blur Amount",&post.dof_blur_amount,0.0,3.0);
            ImGui::SliderFloat("Focal Distance",&post.dof_focal_distance,20.0,200.0);
            ImGui::SliderFloat("Focal Range",&post.dof_focal_range,0.0,200.0);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("PIXELATE", mainSettings)){
            ImGui::SliderFloat("dry_wet",&post.downsample.dry_wet,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("ALPHA TRAILS", mainSettings)){
            ImGui::SliderFloat("Delay Amount",&post.alpha_trails.delay,0.0,0.99);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("FEEDBACK", mainSettings)){
            ImGui::SliderFloat("strength",&post.feedback.strength,0.0,1.0);
            ImGui::SliderFloat("zoom",&post.feedback.zoom,-1.0,1.0);
            ImGui::SliderFloat("x_mult",&post.feedback.x_mult,1.0,32.0);
            ImGui::SliderFloat("y_mult",&post.feedback.y_mult,1.0,32.0);
            ImGui::SliderFloat("x_amp",&post.feedback.x_amp,0.0,1.0);
            ImGui::SliderFloat("y_amp",&post.feedback.y_amp,0.0,1.0);
            ImGui::SliderFloat("x_speed",&post.feedback.x_speed,0.0,1.0);
            ImGui::SliderFloat("y_speed",&post.feedback.y_speed,0.0,1.0);
            ImGui::SliderFloat("rotate_amp",&post.feedback.rotate_amp,0.0,1.0);
            ImGui::SliderFloat("rotate_speed",&post.feedback.rotate_speed,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }
        if (ofxImGui::BeginTree("REACTION DIFFUSION", mainSettings)){
            ImGui::SliderFloat("dry_wet",&post.reaction_diffusion.dry_wet,0.0,1.0);
            ImGui::SliderFloat("Intensity",&post.reaction_diffusion.intensity,0.0,1.0);
            ImGui::SliderFloat("BleedAmount",&post.reaction_diffusion.bleed_amount,0.0,1.0);
            ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree("Camera", mainSettings)){
            ImGui::SliderFloat("Near Clip",&cam_near_clip,0.0,1000.0);
            ImGui::SliderFloat("Far Clip",&cam_far_clip,0.0,1000.0);
            ofxImGui::EndTree(mainSettings);
        }
    }
    ofxImGui::EndWindow(mainSettings);
    this->gui.end();
}

