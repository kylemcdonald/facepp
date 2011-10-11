
#pragma once


#include "ofMain.h"
#include "ofxEdsdk.h"
	
	
class inputManager  {
	
	public:
	
	
	enum InputType {
		file, webcam, edsdk
	};
	
	InputType inputType;
	
	
	void setup();
	void update();
	void draw();
	
	unsigned char * getTrackingPixels(){ return pixelsToTrack; }
		
	bool bSaveImages;
	bool bFrameNew;
		
	ofVideoGrabber cam;
	ofVideoPlayer video;
	ofxEdsdk::Camera camera;
	ofPixels tempToBeResized;
	
	void			preparePixelsToTrack(ofPixels & pix);
	void			preparePixelsToTrack(ofBaseHasPixels & pix);
	ofImage			pixelsSmaller;
	unsigned char * pixelsToTrack;		// 640x480;
	ofTexture		pixelsSmallerTex;
	
	bool isFrameNew(){return bFrameNew;};
	
	ofPixels smallAsOfPixels;
	
};
