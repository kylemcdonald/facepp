#pragma once

#include "ofxOsc.h"
#include "ofxXmlSettings.h"
/*
 At first, call ofxDaito::setup("setting.xml").
 To trigger a sound event, call:
 ofxDaito::bang(eventName, energy, pan)
 energy is 0.0f - 1.0f, by default 1.0f
 pan is 0.0f - 1.0f, by defualt 0.5f
 */

class ofxDaito {
private:
	static ofxXmlSettings xml;
	static vector<ofxOscSender> senders;
	static void send(ofxOscMessage& msg);
public:
	static void setup(string settings, bool verbose = true);
	static void bang(string eventName, float energy = 1.0f, float pan = 0.5f);
	static void bangVec3f(string eventName, ofVec3f _vec);
	static void bang(string eventName, int _val);	
	static void bang(string eventName);
	static void sendCustom(ofxOscMessage &msg);
};

