#pragma once

#include "ofMain.h"

inline void randomize(ofVec2f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.normalize();
}

inline ofVec2f intersectPointLine(ofVec2f point, ofVec2f lineStart, ofVec2f lineEnd) {
	ofVec2f ray = lineEnd - lineStart;
	float u = (point - lineStart).dot(ray) / ray.dot(ray);
	return lineStart + u * ray;
}

class Force {
public:
	ofVec2f position;
	float magnitude;
	Force(ofVec2f position, float magnitude) {
		this->position = position;
		this->magnitude = magnitude;
	}
};

class NoiseParticle {
public:
	static ofVec2f centeringForce, globalOffset, externalForce;
	static float speed, spread, viscosity, independence, rebirthRadius, neighborhood, confusion, indecision, age, globalDirection;
	static vector<NoiseParticle> particles;
	static vector<Force> forces;
	static void setup(int n, ofVec2f radius, float radius);
  static void drawAllPoints();
  static void drawAllLines();
	static void addForce(ofVec2f position, float magnitude);
  static void updateAll(float dt = 1. / 60.);
	static void setLeaving(bool leaving);

	bool visible, leaving;
  ofVec2f position, velocity, force, localOffset;
	list<ofVec2f> trail;
	static const int maxTrailSize = 32;
  NoiseParticle(ofVec2f origin, float radius) {
    randomize(localOffset);
  	randomize(position);
  	position = position * radius + origin;
		leaving = false;
  }
  inline void applyFlockingForce() {
		float basex = position.x / neighborhood;
		float basey = position.y / neighborhood;
    force.x +=
      ofSignedNoise(
				basex + globalOffset.x + localOffset.x * independence,
				basey);
		force.y +=
      ofSignedNoise(
				basex,
				basey + globalOffset.y  + localOffset.y * independence);
  }
  inline void applyViscosityForce() {
    force += velocity * -viscosity;
  }
  inline void applyCenteringForce() {
    centeringForce = position;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
		float magnitude = distanceToCenter / (spread * spread);
    centeringForce *= leaving ? magnitude : -magnitude;
    force += centeringForce;
  }
	inline void applyExternalForce() {
		for(int i = 0; i < forces.size(); i++) {
			externalForce = position - forces[i].position;
			float distanceToForce = externalForce.length();
			externalForce.normalize();
			externalForce *= forces[i].magnitude / distanceToForce;
			force += externalForce;		
		}
	}
	inline void applyLeavingForce() {
	}
  inline void update(float dt) {
    force.set(0, 0);
    applyFlockingForce();
    applyViscosityForce();
		applyCenteringForce();
		applyExternalForce();
    velocity += force * dt; // mass = 1, acceleration = force
    position += velocity * speed * dt;
		
		trail.push_back(position);
		if(trail.size() > maxTrailSize) {
			trail.pop_front();
		}
		ofRectangle screen(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth(), ofGetHeight());
		visible = true;
		if(trail.size() > 2) {
			visible = false;
			list<ofVec2f>::iterator it;
			ofVec2f front = trail.front();
			ofVec2f back = trail.back();
			for(it = trail.begin(); it != trail.end(); it++) {
				ofVec2f& cur = *it;
				ofVec2f projection = intersectPointLine(cur, front, back);
				cur.interpolate(projection, .01); // describes how fast it straightens out
				visible |= screen.inside(cur);
			}
		}
  }
};
