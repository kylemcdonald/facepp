#pragma once


#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCv.h"

//using namespace ofxCv;


class threadedTracker : public ofThread{
	
public:
	
	
	unsigned char * pixelForTracker;
	unsigned char * pixelInThreadForTracker;
	
	ofxFaceTracker * tracker;
	ofxFaceTracker * trackerInThread;
	
	ofPixels colorPix;
	
	
	bool bHavePixels;
	bool bResetTracker;
	float scale;
	
	
	void setup(){
		
		// having this in the constructor failed because ofToDataPath() fails pre setup();
		// why?
		
		bHavePixels = false;
		tracker = new ofxFaceTracker();
		tracker->setup();
		tracker->setRescale(.5);
		trackerInThread = new ofxFaceTracker();
		trackerInThread->setup();
		trackerInThread->setRescale(.5);
		bResetTracker = false;
		
		colorPix.allocate(640, 480, OF_PIXELS_RGB);
		pixelForTracker = new unsigned char[640*480*3];
		pixelInThreadForTracker = new unsigned char[640*480*3];
		
		//pixelInThreadForTracker.allocate(640, 480, OF_PIXELS_RGB);
		//trackerInThread->setIterations(6);
		//tracker->setIterations(6);
	}
	//--------------------------
	threadedTracker(){
		// nada
	}
	~threadedTracker(){
		stop();
		//delete tracker;
		//delete trackerInThread;
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

		while( isThreadRunning() != 0 ){
			if (bHavePixels == true){
				if( lock() ){
					memcpy(pixelInThreadForTracker, pixelForTracker, 640*480*3);
					
					//pixelInThreadForTracker = pixelForTracker;
					unlock();
					
				}
				
				colorPix.setFromExternalPixels(pixelInThreadForTracker, 640, 480, 3);
				
				trackerInThread->update(ofxCv::toCv(colorPix));
				if (bResetTracker == true){
					trackerInThread->reset();
					bResetTracker = false;
				}
				
				if( lock() ){
					
					trackerInThread->setRescale(scale);
					tracker->setRescale(scale);
					
					memcpy(tracker, trackerInThread, sizeof(ofxFaceTracker));
					unlock();
				}
				ofSleepMillis(50);
				//printf("-- tracked --\n");
			} else {
				ofSleepMillis(10);
			}
		}
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
	
	void copyFaceTracker(ofxFaceTracker & trackerOut){
		// could this be a different lock?
		if (lock()){
			//memcpy(&trackerOut, tracker); 
			memcpy(&trackerOut, tracker, sizeof(ofxFaceTracker));
			unlock();
		}
	}
	
	
	
};
