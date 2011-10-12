#pragma once

#include "ofMain.h"

class Clone {
public:
	void setup(int width, int height);
	void setStrength(int strength);
	void update(ofTexture& src, ofTexture& dst,  ofTexture & dst2, float s, ofTexture& mask);
	void draw();
	
	ofPoint darkPoint;
	
protected:
	void maskedBlur(ofTexture& tex, ofTexture& mask,  ofFbo& result);
	ofFbo buffer, srcBlur, dstBlur;
	ofShader maskBlurShader, cloneShader;
	int strength;
};