#pragma once


#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCv.h"

//using namespace ofxCv;

// to do, make this thread only run when the testApp tracker's thread doesn't find faces. 


class threadedTracker : public ofThread{
	
public:
	
	
	unsigned char * pixelForTracker;
	unsigned char * pixelInThreadForTracker;
	
	ofxFaceTracker * trackerInThread;
	
	ofPixels colorPix;
	
	
	bool bHavePixels;
	bool bResetTracker;
	float scale;
	
	int nFramesGotOne;
	
	
	void setup(){
		
		// having this in the constructor failed because ofToDataPath() fails pre setup();
		// why?
		
		bHavePixels = false;
		bResetTracker = false;
		colorPix.allocate(640, 480, OF_PIXELS_RGB);
		pixelForTracker = new unsigned char[640*480*3];
		pixelInThreadForTracker = new unsigned char[640*480*3];
		scale = 0.5;
		nFramesGotOne = 0;
		
	}
	//--------------------------
	threadedTracker(){
		// nada
	}
	~threadedTracker(){
		stop();
		ofSleepMillis(50);
	}
	
	void setScale(float _scaleIn){
		scale = _scaleIn;	
	}
	
	void start(){
		startThread(true, false);   // blocking, verbose
	}
	
	void stop(){
		stopThread();
	}
	
	//--------------------------
	void threadedFunction(){

		// make the tracker in this thread, helpful?
		trackerInThread = new ofxFaceTracker();
		trackerInThread->setup();
		trackerInThread->setRescale(scale);
		float myScale = scale;
		
		
		while( isThreadRunning() != 0 ){
			if (bHavePixels == true){
				
				
				//-------------------------- copy the pixels in, via a lock
				
				if( lock() ){
					memcpy(pixelInThreadForTracker, pixelForTracker, 640*480*3);
					unlock();
				}
				
				//-------------------------- put them in a toCv friendly format, then update the tracker
				
				colorPix.setFromExternalPixels(pixelInThreadForTracker, 640, 480, 3);
				trackerInThread->update(ofxCv::toCv(colorPix));
				
				//-------------------------- keep track of how much we've found, so the main app can track or not				
				
				if (trackerInThread->getFound()){
					nFramesGotOne++;
				} else {
					nFramesGotOne = 0;	
				}
				
				//-------------------------- reset if we need to 
				
				if (bResetTracker == true){
					trackerInThread->reset();
					bResetTracker = false;
				}

				
				//-------------------------- if our scale has changed, reallocate. 
				
				if (fabs(myScale - scale) > 0.05){
					delete trackerInThread;
					trackerInThread = new ofxFaceTracker();
					trackerInThread->setup();
					trackerInThread->setRescale(scale);
					myScale = scale;
				}
				
				//-------------------------- sleep for a while, doesn't need to be fast
				
				ofSleepMillis(80);
		
			} else {
				ofSleepMillis(10);
			}
		}
		
		delete trackerInThread;
	}
	
	//--------------------------
	void copyPixels(unsigned char * input){
		if( lock() ){
			memcpy(pixelForTracker, input, 640*480*3);
			bHavePixels = true;
			unlock();
		}
	}
	
	void setReset(){
		bResetTracker = true;	
	}
	
	
		
	
	
};
