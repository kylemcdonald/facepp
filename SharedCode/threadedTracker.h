#pragma once


#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCv.h"

//using namespace ofxCv;


class threadedTracker : public ofThread{
	
public:
	
	
	ofPixels pixelForTracker;
	ofPixels pixelInThreadForTracker;
	ofxFaceTracker * tracker;
	ofxFaceTracker * trackerInThread;
	bool bHavePixels;
	
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
					pixelInThreadForTracker = pixelForTracker;
					unlock();
				}
				trackerInThread->update(ofxCv::toCv(pixelInThreadForTracker));
				if( lock() ){
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
	void copyPixels(ofPixels & input){
		
		if( lock() ){
			pixelForTracker = input;
			bHavePixels = true;
			unlock();
		}
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
