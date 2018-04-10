#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"


//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
	//settings.setSize(1920, 1080);
    settings.width = 1920;
    settings.height = 1080;
    settings.setPosition(ofVec2f(1920,0));
    settings.resizable = true;
    settings.windowMode = ofWindowMode::OF_FULLSCREEN;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
	//settings.setSize(850, 1000);
    settings.width = 850;
    settings.height = 1000;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
    // uncomment next line to share main's OpenGL resources with gui
    settings.windowMode = ofWindowMode::OF_WINDOW;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupGui();
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}
