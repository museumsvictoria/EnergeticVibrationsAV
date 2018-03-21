#pragma once
#include <string>
#include "ofVideoPlayer.h"
#include "ofTexture.h"

//Define for windows video
#define WINVID 1
#ifdef WINVID
#include "ofxWMFVideoPlayer.h"
#endif

class TomsVideo {
#ifdef WINVID
	ofxWMFVideoPlayer video;
#else
	ofVideoPlayer video;
#endif
public:
	bool load(std::string name);
	ofTexture &	getTexture();
	void setPaused(bool bPause);
	void play();
	void update();
};