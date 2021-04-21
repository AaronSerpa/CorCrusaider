/*
Author: Aaron Serpa
Class: CS134
3/29/2021
*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"
#include "ParticleEmitter.h"

//  Shape base class
//
class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside() { return false; }
	//          VVVVVVVVVVVVVVVVVVVV
	glm::vec3 pos = glm::vec3(0,0,0);
	float rotation = 0.0;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 heading = glm::vec3(glm::cos(glm::radians(rotation + 90)), glm::sin(glm::radians(rotation + 90)), 0);
	vector<glm::vec3> verts;
};

//  TriangleShape
//
class TriangleShape : public Shape {
public:
	TriangleShape(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
		
	}
	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	bool stored = false;
	void draw();
	//  Integrator Function;
	//
	void integrate();

	// Physics data goes here  (for integrate() );
	//
	ofVec3f velocity = ofVec3f(0, 0, 0);
	ofVec3f acceleration = ofVec3f(0, 0, 0);
	float	damping = 0.99;
	float mass = 1;
	float rotVel = 0;
	float rotAcceleration = 0;
	float thrust = 500;
};


//---------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------------
class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	
	TriangleShape tri = TriangleShape(glm::vec3(-20, -20, 0), glm::vec3(0, 40, 0), glm::vec3(20, -20, 0));
	bool inside = false;
	
	//ofxPanel gui;
	ofxLabel screenSize;

	ofEasyCam    cam;
	bool spacePressed = false;
	Sprite Title;
	Sprite gameOverScreen;
	Sprite player;
	Sprite background;
	Sprite hpPickup;
	vector<Sprite> health;
	Sprite h1;
	Sprite h2;
	Sprite h3;
	bool spinning;
	bool hpupPres;
	bool gameOver;
	int iFrames = 0;
	int curHealth = 3;
	ofImage enemy;
		vector<Emitter *> emitters;
		int numEmitters;

	Emitter  *turret = NULL;
	Emitter *enemyLauncher1 = NULL;
	Emitter *enemyLauncher2 = NULL;
	Emitter *enemyLauncher3 = NULL;
	Emitter *enemyLauncher4 = NULL;
	ParticleEmitter partEmit;
	ImpulseRadialForce *radialForce;

	ofTrueTypeFont  verdana14;
	ofImage damage;
	ofImage defaultImage;
	ofVec3f mouse_last;
	int linVel;
	int score = 0;
	int highScore = 0;
	bool imageLoaded;
	bool newGame;
	bool bHide;
};
