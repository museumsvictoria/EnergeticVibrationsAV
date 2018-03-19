#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"


//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.width = 1920;
    settings.height = 1080;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 880;
    settings.height = 1000;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
    // uncomment next line to share main's OpenGL resources with gui
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupGui();
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}
