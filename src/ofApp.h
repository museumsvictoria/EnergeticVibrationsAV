#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxUbo.h"
#include "ofxImGui.h"
#include "GuiTheme.h"
#include "PostProcessing.h"
#include "Primitives.h"



struct ShaderParams {
    float scale_speed;
    float rot_speed;
    float transducer_speed[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
};

class ofApp : public ofBaseApp{

        
	public:
        void init();
		void setup();
		void update();
		void draw();
    
        void setupGui();
        void drawGui(ofEventArgs & args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Primitives primitives;
    
        ofEasyCam mCam1;
        
        shared_ptr<ofxUboShader> mShd1;
        ShaderParams params;
        bool isShaderDirty;
        ofTexture mTex1;

        PostProcessing post;
    
        ///------------- GUI
        ofxImGui::Gui gui;
        GuiTheme gui_theme;
    
    
        // Shader Params
        float explode_amount;
    
        // Camera
        float cam_near_clip;
        float cam_far_clip;
    
        // OPEN GL STATE
    bool toggle_post_processing;
    bool toggle_blending;
    bool toggle_backface_cull;
};
