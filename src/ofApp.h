#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxUbo.h"
#include "ofxImGui.h"
#include "PostProcessing.h"
#include "Primitives.h"



struct ShaderParams {
    float time;
    float speed;
    float transducer_speed[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
    float shape_morph;
    float circle_motion;
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
        ofxImGui::Gui gui;
};
