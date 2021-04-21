#pragma once
//  Aaron Serpa - CS 134 SJSU

#include "ofMain.h"
#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};



// Some convenient built-in forces
//


class ImpulseRadialForce : public ParticleForce {
	float magnitude, height;
public:
	ImpulseRadialForce(float magnitude, float height);
	void updateForce(Particle *);
	void set(float m)
	{
		magnitude = m;
	}
	void setHeight(float h)
	{
		height = h;
	}
};

