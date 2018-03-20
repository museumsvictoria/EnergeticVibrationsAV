#include "TomsVideo.hpp"

bool TomsVideo::load(std::string name) {
#ifdef WINVID
	auto v = video.loadMovie(name);
	std::cout << "video loaded: " << v << endl;
	return v;
#else
	return video.load(name);
#endif
}

ofTexture &	TomsVideo::getTexture() {
	return video.getTexture();

}

void TomsVideo::setPaused(bool bPause) {
#ifdef WINVID
	if (bPause) {
		video.pause();
	}
	else {
		video.play();
	}
#else
	video.setPaused(bPause);
#endif
}

void TomsVideo::play() {
	video.play();
	std::cout << "is playing: " << video.isPlaying() << endl;
}

void TomsVideo::update() {
	video.update();
	auto t = video.getTexture();
	t.draw(0,0);
	//std::cout << "is playing: " << video.isPlaying() << endl;
}
