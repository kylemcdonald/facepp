#pragma once

#include "ofMain.h"
#include "Mandala.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	Mandala mandala;
};
