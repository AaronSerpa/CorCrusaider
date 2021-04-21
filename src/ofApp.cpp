/*
Author: Aaron Serpa
Class: CS134
3/29/2021
*/
#include "ofApp.h"

//--------------------------------------------------------------------------------------------------------------------
ofSoundPlayer radio;
ofSoundPlayer radio2;
ofSoundPlayer hurt;
ofSoundPlayer powerup;
ofSoundPlayer gamedone;
ofSoundPlayer bgm;
BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rotation = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 60;
	height = 80;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}


//  Render the sprite
//
void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), glm::vec3(pos));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));

	glm::mat4 T = translation * rot;

	ofPushMatrix();
	ofMultMatrix(T);

	if (haveImage) {
		image.draw(-width / 2.0 + trans.x, -height / 2.0 + trans.y);
	}
	else {

		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
	
	ofPopMatrix();
}

void SpriteSystem::add(Sprite s) {
	ofImage image;
	if (play == true)
	{
		radio.play();
	}
	
	else
	{
		max++;
		s.setImage(image);
			
		s.trueSize = scaleSize;
		s.size = scaleSize;
		s.pointYield = 100;
		s.enemyLife = 1;
		if (scaleSize >= 1.66 && scaleSize < 2.33)
		{
			image.load("images/commit2.png");
			s.setImage(image);
			s.pointYield += 100;
			s.enemyLife += 1;
		}
		else if (scaleSize >= 2.33)
		{
			image.load("images/commit3.png");
			s.setImage(image);
			s.pointYield += 200;
			s.enemyLife += 2;
		}
		s.image.resize(s.image.getWidth()*scaleSize, s.image.getHeight()*scaleSize);
	}

	sprites.push_back(s);
}

//--------------------------------------------------------------------------------------------------------------------
void TriangleShape::draw() {

	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);

	glm::mat4 T = trans * rot * scale;

	ofPushMatrix();
	ofMultMatrix(T);
	ofSetColor(0.0, 0.0, 0.0, 0.0);
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();


}

void TriangleShape::integrate() {
	float dt = 1.0 / ofGetFrameRate();
	ofVec3f accel = acceleration;
	float rotAccel = rotAcceleration;

	if (velocity.x > 0 && velocity.y > 0 && velocity.z > 0)
	{
		accel -= 10 * (1.0 / mass);
		velocity += accel * dt;
		velocity *= damping;
		pos += glm::vec3(velocity * dt)* heading;
	}
	if (velocity.x < 0 && velocity.y < 0 && velocity.z < 0)
	{
		accel += 10 * (1.0 / mass);
		velocity += accel * dt;
		velocity *= damping;
		pos += glm::vec3(velocity * dt)* heading;
	}
	if (rotVel > 0)
	{
		rotAccel -= 10 * (1.0 / mass);
		rotVel += rotAccel * dt;
		rotVel *= damping;
		rotation += rotVel * dt;
	}
	if (rotVel < 0)
	{
		rotAccel += 10 * (1.0 / mass);
		rotVel += rotAccel * dt;
		rotVel *= damping;
		rotation += rotVel * dt;
	}
	if (pos.y - 40 < 0.0)
	{
		pos = glm::vec3(pos.x, pos.y + 1, 0);
	}
	else if (pos.y + 40 > ofGetHeight())
	{
		pos = glm::vec3(pos.x, pos.y - 1, 0);
	}
	else if (pos.x + 40 > ofGetWidth())
	{
		pos = glm::vec3(pos.x - 1, pos.y, 0);
	}
	else if (pos.x - 40 < 0.0)
	{
		pos = glm::vec3(pos.x + 1, pos.y, 0);
	}
}
//--------------------------------------------------------------
void ofApp::setup() {
	tri.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 1);
	radio.load("sounds/sf_laser_13.mp3");//https://www.soundfishing.eu/sound/laser-gun
	radio2.load("sounds/mixkit-short-explosion-1694.wav");//https://mixkit.co/free-sound-effects/boom/
	hurt.load("sounds/mixkit-arcade-mechanical-bling-210.wav");//https://mixkit.co/free-sound-effects/game/
	powerup.load("sounds/mixkit-arcade-retro-changing-tab-206.wav");//https://mixkit.co/free-sound-effects/game/
	gamedone.load("sounds/mixkit-ominous-drums-227.wav");//https://mixkit.co/free-sound-effects/game/
	bgm.load("sounds/Interplanetary Odyssey.ogg");//https://patrickdearteaga.com/arcade-music/
	bgm.setVolume(0.4);
	bgm.setLoop(true);
	bgm.play();
	newGame = true;
	gameOver = false;
	spinning = false;
	hpupPres = false;
	linVel = 200;
	ofImage img;
	ofSetVerticalSync(true);

	img.load("images/gameOverScreen.png");
	gameOverScreen.setImage(img);
	gameOverScreen.setPosition(glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 1));

	img.load("images/title.png");
	Title.setImage(img);
	Title.setPosition(glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 1));

	img.load("images/ship.png");
	player.setImage(img);
	player.pos = ofVec3f(tri.pos.x, tri.pos.y, tri.pos.z);

	img.load("images/bg.png");
	background.setImage(img);
	background.setPosition(glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 1));
	
	img.load("images/health.png");
	h1.setImage(img);
	h1.setPosition(glm::vec3(20, 15, 1));
	h2.setImage(img);
	h2.setPosition(glm::vec3(40, 15, 1));
	h3.setImage(img);
	h3.setPosition(glm::vec3(60, 15, 1));
	health = { h1, h2, h3 };

	img.load("images/hpup.png");
	hpPickup.setImage(img);

	

	if (defaultImage.load("images/core.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/core.png");
		ofExit();
	}
	if (enemy.load("images/commit.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/commit.png");
		ofExit();
	}


	radialForce = new ImpulseRadialForce(0.5, 1);
	partEmit.setOneShot(true);
	partEmit.sys->addForce(radialForce);
	partEmit.setEmitterType(RadialEmitter);
	partEmit.setGroupSize(5);
	partEmit.setVelocity(ofVec3f(100, 100,50));
	partEmit.setLifespan(1);
	partEmit.setParticleRadius(25);
	

	turret = new Emitter(new SpriteSystem());
	turret->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
	turret->drawable = true;
	turret->setChildImage(defaultImage);
	turret->setLifespan(2000);    // convert to milliseconds 
	turret->setRate(2);
	turret->start();
	turret->setSound();

	enemyLauncher1 = new Emitter(new SpriteSystem());
	enemyLauncher1->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, 0,0));//top middle
	enemyLauncher1->drawable = true;
	enemyLauncher1->setChildImage(enemy);
	enemyLauncher1->lifespan = -1;
	enemyLauncher1->sys->bounce = true;
	enemyLauncher1->start();

	enemyLauncher2 = new Emitter(new SpriteSystem());
	enemyLauncher2->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight(), 0));//bottom middle
	enemyLauncher2->drawable = true;
	enemyLauncher2->setChildImage(enemy);
	enemyLauncher2->lifespan = -1;
	enemyLauncher2->sys->bounce = true;
	enemyLauncher2->start();

	enemyLauncher3 = new Emitter(new SpriteSystem());
	enemyLauncher3->setPosition(ofVec3f(0, ofGetWindowHeight() / 2.0, 0));//left middle
	enemyLauncher3->drawable = true;
	enemyLauncher3->setChildImage(enemy);
	enemyLauncher3->lifespan = -1;
	enemyLauncher3->sys->bounce = true;
	enemyLauncher3->start();

	enemyLauncher4 = new Emitter(new SpriteSystem());
	enemyLauncher4->setPosition(ofVec3f(ofGetWindowWidth(), ofGetWindowHeight() / 2.0, 0));//right middle
	enemyLauncher4->drawable = true;
	enemyLauncher4->setChildImage(enemy);
	enemyLauncher4->lifespan = -1;
	enemyLauncher4->sys->bounce = true;
	enemyLauncher4->start();

	verdana14.load("verdana.ttf", 14);
	verdana14.setLineHeight(18.0f);
	verdana14.setLetterSpacing(1.037);
	bHide = false;

}

//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {
	
	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	

	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan || 
			((s->trans.x < 0.0 || s->trans.y < 0.0 || s->trans.x >ofGetWidth() || s->trans.y > ofGetHeight())&&bounce == false)) {
			tmp = sprites.erase(s);
			s = tmp;
		}

		else s++;

	}
	for (int i = 0; i < sprites.size(); ++i) 
	{
		if ((sprites[i].trans.x <= 0.0 || sprites[i].trans.x >= ofGetWidth())  && bounce == true)//sides of screen
		{
			sprites[i].velocity = ofVec3f(-sprites[i].velocity.x, sprites[i].velocity.y, 0);
		}
		
		if ((sprites[i].trans.y >= ofGetHeight() || sprites[i].trans.y <= 0.0) && bounce == true)// top and bot of screen
		{
			sprites[i].velocity = ofVec3f(sprites[i].velocity.x, -sprites[i].velocity.y, 0);
		}

	}
	
	scaleSize = ofRandom(1.0, 3.0);
	for (int i = 0; i < sprites.size(); ++i) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
		
	}
	
}

void SpriteSystem::setSound()
{
	play = true;
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	
	for (int i = 0; i < sprites.size(); i++) {
		
		sprites[i].draw();
	}
}

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;
	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, 0, 0);
		}
	}

	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		
		sprite.birthtime = time;
		sys->add(sprite);
		lastSpawned = time;
	}
	sys->update();
}

void Emitter::setSound()
{
	sys->setSound();
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}


//--------------------------------------------------------------
void ofApp::update() {
	Title.image.resize(ofGetWidth(), ofGetHeight());
	gameOverScreen.image.resize(ofGetWidth(), ofGetHeight());
	if (newGame == false)
	{
		background.image.resize(ofGetWidth(), ofGetHeight());
		tri.integrate();
		
		if (score > highScore)
		{
			highScore = score;
		}
		if (curHealth == 0)
		{
			bgm.stop();
			gameOver = true;
			newGame = true;
			gamedone.play();

		}
		int r = ofRandom(0, 1000);
		if (r == 5 && hpupPres == false)
		{
			hpPickup.lifespan = -1;
			hpPickup.setPosition(ofVec3f(ofRandom(150, ofGetWidth() - 150), ofRandom(150, ofGetHeight() - 150), 0));
			
			hpupPres = true;
		}
		if (spacePressed == true)
		{
			turret->setRate(2);
		}
		else
		{
			turret->setRate(0);
		}


		
		turret->setVelocity(linVel * tri.heading);// ofVec3f(velocity->x, velocity->y, velocity->z));
		turret->update();
		
		if (enemyLauncher1->sys->max >= 3)
		{
			enemyLauncher1->setRate(0);
		}
		else
		{
			enemyLauncher1->setRate(3);
		}
		enemyLauncher1->setVelocity(ofVec3f(ofRandom(-45, 45), ofRandom(50,110), 0));
		enemyLauncher1->setPosition(ofVec3f(ofRandom(200, ofGetWidth()-200), 10, 0));
		enemyLauncher1->update();
		

		if (enemyLauncher2->sys->max >= 3)
		{
			enemyLauncher2->setRate(0);
		}
		else 
		{
			enemyLauncher2->setRate(3);
		}
		enemyLauncher2->setVelocity(ofVec3f(ofRandom(-45, 45), ofRandom(-50, -110), 0));
		enemyLauncher2->setPosition(ofVec3f(ofRandom(200, ofGetWidth()-200), ofGetHeight()-10, 0));
		enemyLauncher2->update();

		if (enemyLauncher3->sys->max >= 3)
		{
			enemyLauncher3->setRate(0);
		}
		else 
		{
			enemyLauncher3->setRate(3);
		}
		enemyLauncher3->setVelocity(ofVec3f(ofRandom(50, 110), ofRandom(-45, 45), 0));
		enemyLauncher3->setPosition(ofVec3f(10, ofRandom(200, ofGetHeight()-200), 0));
		enemyLauncher3->update();

		if (enemyLauncher4->sys->max >= 3)
		{
			enemyLauncher4->setRate(0);
		}
		else
		{
			enemyLauncher4->setRate(3);
		}
		enemyLauncher4->setVelocity(ofVec3f(ofRandom(-50, -110), ofRandom(-45, 45), 0));
		enemyLauncher4->setPosition(ofVec3f(ofGetWidth()-10, ofRandom(200, ofGetHeight()-200), 0));
		enemyLauncher4->update();

		glm::vec3 p1 = tri.pos + glm::vec3(-20, -20, 1);
		glm::vec3 p2 = tri.pos + glm::vec3(0, 40, 1);
		glm::vec3 p3 = tri.pos + glm::vec3(20, -20, 1);
		


		for (int j = 0; j < enemyLauncher1->sys->sprites.size(); ++j)
		{
			
			if (iFrames ==0 && (tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x, enemyLauncher1->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top left
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x + enemyLauncher1->sys->sprites[j].width * enemyLauncher1->sys->sprites[j].trueSize / 2, enemyLauncher1->sys->sprites[j].trans.y, 0), p1, p2, p3) == true ||//top right
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x + enemyLauncher1->sys->sprites[j].width* enemyLauncher1->sys->sprites[j].trueSize / 2, enemyLauncher1->sys->sprites[j].trans.y + enemyLauncher1->sys->sprites[j].height* enemyLauncher1->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom right
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x, enemyLauncher1->sys->sprites[j].trans.y + enemyLauncher1->sys->sprites[j].height* enemyLauncher1->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom left


				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x + (enemyLauncher1->sys->sprites[j].width / 2) * (enemyLauncher1->sys->sprites[j].trueSize / 2), enemyLauncher1->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top 
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x + enemyLauncher1->sys->sprites[j].width * enemyLauncher1->sys->sprites[j].trueSize / 2, enemyLauncher1->sys->sprites[j].trans.y + (enemyLauncher1->sys->sprites[j].height / 2) * (enemyLauncher1->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true ||//right
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x + (enemyLauncher1->sys->sprites[j].width / 2) * (enemyLauncher1->sys->sprites[j].trueSize / 2), enemyLauncher1->sys->sprites[j].trans.y + enemyLauncher1->sys->sprites[j].height* enemyLauncher1->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom 
				tri.inside(glm::vec3(enemyLauncher1->sys->sprites[j].trans.x, enemyLauncher1->sys->sprites[j].trans.y + (enemyLauncher1->sys->sprites[j].height / 2) * (enemyLauncher1->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true//left
				))
			{
				enemyLauncher1->sys->sprites[j].lifespan = 0;
				enemyLauncher1->sys->max--;
				hurt.play();
				iFrames = 300;
				if (curHealth <= 3 && curHealth > 0)
				{
					curHealth--;

				}
				
			}

		}
		for (int j = 0; j < enemyLauncher2->sys->sprites.size(); ++j)
		{

			if (iFrames == 0 && (tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x, enemyLauncher2->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top left
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x + enemyLauncher2->sys->sprites[j].width * enemyLauncher2->sys->sprites[j].trueSize / 2, enemyLauncher2->sys->sprites[j].trans.y, 0), p1, p2, p3) == true ||//top right
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x + enemyLauncher2->sys->sprites[j].width* enemyLauncher2->sys->sprites[j].trueSize / 2, enemyLauncher2->sys->sprites[j].trans.y + enemyLauncher2->sys->sprites[j].height* enemyLauncher2->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom right
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x, enemyLauncher2->sys->sprites[j].trans.y + enemyLauncher2->sys->sprites[j].height* enemyLauncher2->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom left


				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x + (enemyLauncher2->sys->sprites[j].width / 2) * (enemyLauncher2->sys->sprites[j].trueSize / 2), enemyLauncher2->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top 
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x + enemyLauncher2->sys->sprites[j].width * enemyLauncher2->sys->sprites[j].trueSize / 2, enemyLauncher2->sys->sprites[j].trans.y + (enemyLauncher2->sys->sprites[j].height / 2) * (enemyLauncher2->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true ||//right
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x + (enemyLauncher2->sys->sprites[j].width / 2) * (enemyLauncher2->sys->sprites[j].trueSize / 2), enemyLauncher2->sys->sprites[j].trans.y + enemyLauncher2->sys->sprites[j].height* enemyLauncher2->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom 
				tri.inside(glm::vec3(enemyLauncher2->sys->sprites[j].trans.x, enemyLauncher2->sys->sprites[j].trans.y + (enemyLauncher2->sys->sprites[j].height / 2) * (enemyLauncher2->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true//left
				))
			{
				enemyLauncher2->sys->sprites[j].lifespan = 0;
				enemyLauncher2->sys->max--;
				hurt.play();
				iFrames = 300;
				if (curHealth <= 3 && curHealth > 0)
				{
					curHealth--;

				}

			}

		}
		for (int j = 0; j < enemyLauncher3->sys->sprites.size(); ++j)
		{

			if (iFrames == 0 && (tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x, enemyLauncher3->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top left
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x + enemyLauncher3->sys->sprites[j].width * enemyLauncher3->sys->sprites[j].trueSize / 2, enemyLauncher3->sys->sprites[j].trans.y, 0), p1, p2, p3) == true ||//top right
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x + enemyLauncher3->sys->sprites[j].width* enemyLauncher3->sys->sprites[j].trueSize / 2, enemyLauncher3->sys->sprites[j].trans.y + enemyLauncher3->sys->sprites[j].height* enemyLauncher3->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom right
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x, enemyLauncher3->sys->sprites[j].trans.y + enemyLauncher3->sys->sprites[j].height* enemyLauncher3->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom left


				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x + (enemyLauncher3->sys->sprites[j].width / 2) * (enemyLauncher3->sys->sprites[j].trueSize / 2), enemyLauncher3->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top 
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x + enemyLauncher3->sys->sprites[j].width * enemyLauncher3->sys->sprites[j].trueSize / 2, enemyLauncher3->sys->sprites[j].trans.y + (enemyLauncher3->sys->sprites[j].height / 2) * (enemyLauncher3->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true ||//right
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x + (enemyLauncher3->sys->sprites[j].width / 2) * (enemyLauncher3->sys->sprites[j].trueSize / 2), enemyLauncher3->sys->sprites[j].trans.y + enemyLauncher3->sys->sprites[j].height* enemyLauncher3->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom 
				tri.inside(glm::vec3(enemyLauncher3->sys->sprites[j].trans.x, enemyLauncher3->sys->sprites[j].trans.y + (enemyLauncher3->sys->sprites[j].height / 2) * (enemyLauncher3->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true//left
				))
			{
				enemyLauncher3->sys->sprites[j].lifespan = 0;
				enemyLauncher3->sys->max--;
				hurt.play();
				iFrames = 300;
				if (curHealth <= 3 && curHealth > 0)
				{
					curHealth--;

				}

			}

		}
		for (int j = 0; j < enemyLauncher4->sys->sprites.size(); ++j)
		{

			if (iFrames == 0 && (tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x, enemyLauncher4->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top left
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x + enemyLauncher4->sys->sprites[j].width * enemyLauncher4->sys->sprites[j].trueSize / 2, enemyLauncher4->sys->sprites[j].trans.y, 0), p1, p2, p3) == true ||//top right
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x + enemyLauncher4->sys->sprites[j].width* enemyLauncher4->sys->sprites[j].trueSize / 2, enemyLauncher4->sys->sprites[j].trans.y + enemyLauncher4->sys->sprites[j].height* enemyLauncher4->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom right
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x, enemyLauncher4->sys->sprites[j].trans.y + enemyLauncher4->sys->sprites[j].height* enemyLauncher4->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true || //bottom left


				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x + (enemyLauncher4->sys->sprites[j].width / 2) * (enemyLauncher4->sys->sprites[j].trueSize / 2), enemyLauncher4->sys->sprites[j].trans.y, 0), p1, p2, p3) == true || //top 
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x + enemyLauncher4->sys->sprites[j].width * enemyLauncher4->sys->sprites[j].trueSize / 2, enemyLauncher4->sys->sprites[j].trans.y + (enemyLauncher4->sys->sprites[j].height / 2) * (enemyLauncher4->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true ||//right
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x + (enemyLauncher4->sys->sprites[j].width / 2) * (enemyLauncher4->sys->sprites[j].trueSize / 2), enemyLauncher4->sys->sprites[j].trans.y + enemyLauncher4->sys->sprites[j].height* enemyLauncher4->sys->sprites[j].trueSize / 2, 0), p1, p2, p3) == true ||//bottom 
				tri.inside(glm::vec3(enemyLauncher4->sys->sprites[j].trans.x, enemyLauncher4->sys->sprites[j].trans.y + (enemyLauncher4->sys->sprites[j].height / 2) * (enemyLauncher4->sys->sprites[j].trueSize / 2), 0), p1, p2, p3) == true//left
				))
			{
				enemyLauncher4->sys->sprites[j].lifespan = 0;
				enemyLauncher4->sys->max--;
				hurt.play();
				iFrames = 300;
				if (curHealth <= 3 && curHealth > 0)
				{
					curHealth--;

				}

			}

		}

		if (iFrames >= 200)
		{
			player.rotation += 18;
			spinning = true;
			iFrames--;
		}
		else if (iFrames < 200 && iFrames > 0)
		{
			spinning = false;
			iFrames--;
		}
		if (spinning == false)
		{
			player.rotation = tri.rotation;
		}
		if (tri.inside(glm::vec3(hpPickup.trans.x, hpPickup.trans.y, 0), p1, p2, p3) == true || //middle
			tri.inside(glm::vec3(hpPickup.trans.x + hpPickup.width / 4, hpPickup.trans.y, 0), p1, p2, p3) == true ||//right
			tri.inside(glm::vec3(hpPickup.trans.x - hpPickup.width / 4, hpPickup.trans.y, 0), p1, p2, p3) == true ||//left
			tri.inside(glm::vec3(hpPickup.trans.x, hpPickup.trans.y + hpPickup.height / 4, 0), p1, p2, p3) == true ||//bottom
			tri.inside(glm::vec3(hpPickup.trans.x, hpPickup.trans.y - hpPickup.height / 4, 0), p1, p2, p3) == true)//top))
		{
			hpPickup.lifespan = 0;
			hpupPres = false;
			powerup.play();
			hpPickup.setPosition(ofVec3f(-10000, -10000, 0));
			if (curHealth == 3)
			{
				score += 1000;
			}
			else
			{
				curHealth++;
			}
			
		}
		for (int i = 0; i < turret->sys->sprites.size(); ++i) {
			bool xCol = false;
			bool yCol = false;
			

			for (int j = 0; j < enemyLauncher1->sys->sprites.size(); ++j)
			{
				if (
					turret->sys->sprites[i].trans.x + turret->sys->sprites[i].width >= enemyLauncher1->sys->sprites[j].trans.x &&//left
					turret->sys->sprites[i].trans.x <= enemyLauncher1->sys->sprites[j].trans.x + enemyLauncher1->sys->sprites[j].width* enemyLauncher1->sys->sprites[j].trueSize//right side
					)
				{xCol = true;}
				if (turret->sys->sprites[i].trans.y + turret->sys->sprites[i].height >= enemyLauncher1->sys->sprites[j].trans.y  &&//bottom side
					turret->sys->sprites[i].trans.y<= enemyLauncher1->sys->sprites[j].trans.y + enemyLauncher1->sys->sprites[j].height* enemyLauncher1->sys->sprites[j].trueSize)//top side
				{yCol = true;}
				if (xCol == true && yCol == true)
				{
					radio2.play();
					partEmit.setPos(ofVec3f(enemyLauncher1->sys->sprites[j].trans.x+ enemyLauncher1->sys->sprites[j].width* enemyLauncher1->sys->sprites[j].trueSize/3, enemyLauncher1->sys->sprites[j].trans.y + enemyLauncher1->sys->sprites[j].height* enemyLauncher1->sys->sprites[j].trueSize/3));
					partEmit.sys->reset();
					partEmit.start();
					turret->sys->sprites[i].lifespan = 0;
					enemyLauncher1->sys->sprites[j].enemyLife--;
					enemyLauncher1->sys->sprites[j].takenDamage++;
					if (enemyLauncher1->sys->sprites[j].enemyLife ==0)
					{
						enemyLauncher1->sys->sprites[j].lifespan = 0;
						score += enemyLauncher1->sys->sprites[j].pointYield;
						enemyLauncher1->sys->max--;
					}
					else if (enemyLauncher1->sys->sprites[j].takenDamage == 1 && enemyLauncher1->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit2d.png");
						enemyLauncher1->sys->sprites[j].setImage(damage);
						enemyLauncher1->sys->sprites[j].image.resize(enemyLauncher1->sys->sprites[j].image.getWidth()*enemyLauncher1->sys->sprites[j].trueSize, enemyLauncher1->sys->sprites[j].image.getHeight()*enemyLauncher1->sys->sprites[j].trueSize);

					}
					else if (enemyLauncher1->sys->sprites[j].takenDamage == 1 && enemyLauncher1->sys->sprites[j].enemyLife == 2)
					{
						damage.load("images/commit3d.png");
						enemyLauncher1->sys->sprites[j].setImage(damage);
						enemyLauncher1->sys->sprites[j].image.resize(enemyLauncher1->sys->sprites[j].image.getWidth()*enemyLauncher1->sys->sprites[j].trueSize, enemyLauncher1->sys->sprites[j].image.getHeight()*enemyLauncher1->sys->sprites[j].trueSize);
					}
					else if (enemyLauncher1->sys->sprites[j].takenDamage == 2 && enemyLauncher1->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit3d2.png");
						enemyLauncher1->sys->sprites[j].setImage(damage);
						enemyLauncher1->sys->sprites[j].image.resize(enemyLauncher1->sys->sprites[j].image.getWidth()*enemyLauncher1->sys->sprites[j].trueSize, enemyLauncher1->sys->sprites[j].image.getHeight()*enemyLauncher1->sys->sprites[j].trueSize);
					}
				}
				xCol = false;
				yCol = false;
			}


			for (int j = 0; j < enemyLauncher2->sys->sprites.size(); ++j)
			{
				if (turret->sys->sprites[i].trans.x + turret->sys->sprites[i].width >= enemyLauncher2->sys->sprites[j].trans.x &&//left
					turret->sys->sprites[i].trans.x<= enemyLauncher2->sys->sprites[j].trans.x + enemyLauncher2->sys->sprites[j].width* enemyLauncher2->sys->sprites[j].trueSize)//right side
				{
					xCol = true;
				}
				if (turret->sys->sprites[i].trans.y + turret->sys->sprites[i].height >= enemyLauncher2->sys->sprites[j].trans.y  &&//bottom side
					turret->sys->sprites[i].trans.y<= enemyLauncher2->sys->sprites[j].trans.y + enemyLauncher2->sys->sprites[j].height* enemyLauncher2->sys->sprites[j].trueSize)//top side
				{
					yCol = true;
				}
				if (xCol == true && yCol == true)
				{
					radio2.play();
					partEmit.setPos(ofVec3f(enemyLauncher2->sys->sprites[j].trans.x + enemyLauncher2->sys->sprites[j].width* enemyLauncher2->sys->sprites[j].trueSize / 3, enemyLauncher2->sys->sprites[j].trans.y + enemyLauncher2->sys->sprites[j].height* enemyLauncher2->sys->sprites[j].trueSize / 3));
					partEmit.sys->reset();
					partEmit.start();
					turret->sys->sprites[i].lifespan = 0;
					enemyLauncher2->sys->sprites[j].enemyLife--;
					enemyLauncher2->sys->sprites[j].takenDamage++;
					if (enemyLauncher2->sys->sprites[j].enemyLife == 0)
					{
						enemyLauncher2->sys->sprites[j].lifespan = 0;
						score += enemyLauncher2->sys->sprites[j].pointYield;
						enemyLauncher2->sys->max--;
					}
					else if (enemyLauncher2->sys->sprites[j].takenDamage == 1 && enemyLauncher2->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit2d.png");
						enemyLauncher2->sys->sprites[j].setImage(damage);
						enemyLauncher2->sys->sprites[j].image.resize(enemyLauncher2->sys->sprites[j].image.getWidth()*enemyLauncher2->sys->sprites[j].trueSize, enemyLauncher2->sys->sprites[j].image.getHeight()*enemyLauncher2->sys->sprites[j].trueSize);

					}
					else if (enemyLauncher2->sys->sprites[j].takenDamage == 1 && enemyLauncher2->sys->sprites[j].enemyLife == 2)
					{
						damage.load("images/commit3d.png");
						enemyLauncher2->sys->sprites[j].setImage(damage);
						enemyLauncher2->sys->sprites[j].image.resize(enemyLauncher2->sys->sprites[j].image.getWidth()*enemyLauncher2->sys->sprites[j].trueSize, enemyLauncher2->sys->sprites[j].image.getHeight()*enemyLauncher2->sys->sprites[j].trueSize);
					}
					else if (enemyLauncher2->sys->sprites[j].takenDamage == 2 && enemyLauncher2->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit3d2.png");
						enemyLauncher2->sys->sprites[j].setImage(damage);
						enemyLauncher2->sys->sprites[j].image.resize(enemyLauncher2->sys->sprites[j].image.getWidth()*enemyLauncher2->sys->sprites[j].trueSize, enemyLauncher2->sys->sprites[j].image.getHeight()*enemyLauncher2->sys->sprites[j].trueSize);
					}
				}
				xCol = false;
				yCol = false;
			}


			for (int j = 0; j < enemyLauncher3->sys->sprites.size(); ++j)
			{
				if (turret->sys->sprites[i].trans.x + turret->sys->sprites[i].width >= enemyLauncher3->sys->sprites[j].trans.x &&//left
					turret->sys->sprites[i].trans.x <= enemyLauncher3->sys->sprites[j].trans.x + enemyLauncher3->sys->sprites[j].width* enemyLauncher3->sys->sprites[j].trueSize)//right side
				{
					xCol = true;
				}
				if (turret->sys->sprites[i].trans.y + turret->sys->sprites[i].height >= enemyLauncher3->sys->sprites[j].trans.y  &&//bottom side
					turret->sys->sprites[i].trans.y<= enemyLauncher3->sys->sprites[j].trans.y + enemyLauncher3->sys->sprites[j].height* enemyLauncher3->sys->sprites[j].trueSize)//top side
				{
					yCol = true;
				}
				if (xCol == true && yCol == true)
				{
					radio2.play();
					partEmit.setPos(ofVec3f(enemyLauncher3->sys->sprites[j].trans.x + enemyLauncher3->sys->sprites[j].width* enemyLauncher3->sys->sprites[j].trueSize / 3, enemyLauncher3->sys->sprites[j].trans.y + enemyLauncher3->sys->sprites[j].height* enemyLauncher3->sys->sprites[j].trueSize / 3));
					partEmit.sys->reset();
					partEmit.start();
					turret->sys->sprites[i].lifespan = 0;
					enemyLauncher3->sys->sprites[j].enemyLife--;
					enemyLauncher3->sys->sprites[j].takenDamage++;
					if (enemyLauncher3->sys->sprites[j].enemyLife == 0)
					{
						enemyLauncher3->sys->sprites[j].lifespan = 0;
						score += enemyLauncher3->sys->sprites[j].pointYield;
						enemyLauncher3->sys->max--;
					}
					else if (enemyLauncher3->sys->sprites[j].takenDamage == 1 && enemyLauncher3->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit2d.png");
						enemyLauncher3->sys->sprites[j].setImage(damage);
						enemyLauncher3->sys->sprites[j].image.resize(enemyLauncher3->sys->sprites[j].image.getWidth()*enemyLauncher3->sys->sprites[j].trueSize, enemyLauncher3->sys->sprites[j].image.getHeight()*enemyLauncher3->sys->sprites[j].trueSize);

					}
					else if (enemyLauncher3->sys->sprites[j].takenDamage == 1 && enemyLauncher3->sys->sprites[j].enemyLife == 2)
					{
						damage.load("images/commit3d.png");
						enemyLauncher3->sys->sprites[j].setImage(damage);
						enemyLauncher3->sys->sprites[j].image.resize(enemyLauncher3->sys->sprites[j].image.getWidth()*enemyLauncher3->sys->sprites[j].trueSize, enemyLauncher3->sys->sprites[j].image.getHeight()*enemyLauncher3->sys->sprites[j].trueSize);
					}
					else if (enemyLauncher3->sys->sprites[j].takenDamage == 2 && enemyLauncher3->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit3d2.png");
						enemyLauncher3->sys->sprites[j].setImage(damage);
						enemyLauncher3->sys->sprites[j].image.resize(enemyLauncher3->sys->sprites[j].image.getWidth()*enemyLauncher3->sys->sprites[j].trueSize, enemyLauncher3->sys->sprites[j].image.getHeight()*enemyLauncher3->sys->sprites[j].trueSize);
					}
				}
				xCol = false;
				yCol = false;
			}

			for (int j = 0; j < enemyLauncher4->sys->sprites.size(); ++j)
			{
				if (turret->sys->sprites[i].trans.x + turret->sys->sprites[i].width >= enemyLauncher4->sys->sprites[j].trans.x &&//left
					turret->sys->sprites[i].trans.x <= enemyLauncher4->sys->sprites[j].trans.x + enemyLauncher4->sys->sprites[j].width* enemyLauncher4->sys->sprites[j].trueSize)//right side
				{
					xCol = true;
				}
				if (turret->sys->sprites[i].trans.y + turret->sys->sprites[i].height >= enemyLauncher4->sys->sprites[j].trans.y  &&//bottom side
					turret->sys->sprites[i].trans.y<= enemyLauncher4->sys->sprites[j].trans.y + enemyLauncher4->sys->sprites[j].height* enemyLauncher4->sys->sprites[j].trueSize)//top side
				{
					yCol = true;
				}
				if (xCol == true && yCol == true)
				{
					radio2.play();
					partEmit.setPos(ofVec3f(enemyLauncher4->sys->sprites[j].trans.x + enemyLauncher4->sys->sprites[j].width* enemyLauncher4->sys->sprites[j].trueSize / 3, enemyLauncher4->sys->sprites[j].trans.y + enemyLauncher4->sys->sprites[j].height* enemyLauncher4->sys->sprites[j].trueSize / 3));
					partEmit.sys->reset();
					partEmit.start();
					turret->sys->sprites[i].lifespan = 0;
					enemyLauncher4->sys->sprites[j].enemyLife--;
					enemyLauncher4->sys->sprites[j].takenDamage++;
					if (enemyLauncher4->sys->sprites[j].enemyLife == 0)
					{
						enemyLauncher4->sys->sprites[j].lifespan = 0;
						score += enemyLauncher4->sys->sprites[j].pointYield;
						enemyLauncher4->sys->max--;
					}
					else if(enemyLauncher4->sys->sprites[j].takenDamage == 1 && enemyLauncher4->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit2d.png");
						enemyLauncher4->sys->sprites[j].setImage(damage);
						enemyLauncher4->sys->sprites[j].image.resize(enemyLauncher4->sys->sprites[j].image.getWidth()*enemyLauncher4->sys->sprites[j].trueSize, enemyLauncher4->sys->sprites[j].image.getHeight()*enemyLauncher4->sys->sprites[j].trueSize);

					}
					else if (enemyLauncher4->sys->sprites[j].takenDamage == 1 && enemyLauncher4->sys->sprites[j].enemyLife == 2)
					{
						damage.load("images/commit3d.png");
						enemyLauncher4->sys->sprites[j].setImage(damage);
						enemyLauncher4->sys->sprites[j].image.resize(enemyLauncher4->sys->sprites[j].image.getWidth()*enemyLauncher4->sys->sprites[j].trueSize, enemyLauncher4->sys->sprites[j].image.getHeight()*enemyLauncher4->sys->sprites[j].trueSize);
					}
					else if (enemyLauncher4->sys->sprites[j].takenDamage == 2 && enemyLauncher4->sys->sprites[j].enemyLife == 1)
					{
						damage.load("images/commit3d2.png");
						enemyLauncher4->sys->sprites[j].setImage(damage);
						enemyLauncher4->sys->sprites[j].image.resize(enemyLauncher4->sys->sprites[j].image.getWidth()*enemyLauncher4->sys->sprites[j].trueSize, enemyLauncher4->sys->sprites[j].image.getHeight()*enemyLauncher4->sys->sprites[j].trueSize);
					}
				}
				xCol = false;
				yCol = false;
			}
		}
		
		tri.heading = glm::vec3(glm::cos(glm::radians(tri.rotation + 90)), glm::sin(glm::radians(tri.rotation + 90)), 0);
		turret->setPosition(tri.pos);
		player.pos = tri.pos;
		partEmit.update();
	}
	if (gameOver == true)
	{
		tri.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 1);
		tri.rotation = 0;
		tri.heading = glm::vec3(glm::cos(glm::radians(tri.rotation + 90)), glm::sin(glm::radians(tri.rotation + 90)), 0);
		turret->setPosition(tri.pos);
		hpupPres = false;
		spinning = false;
		player.pos = tri.pos;
		player.rotation = tri.rotation;
		iFrames = 0;
		score = 0;
		for (int j = 0; j < enemyLauncher1->sys->sprites.size(); ++j)
		{
			enemyLauncher1->sys->sprites[j].lifespan = 0;
			enemyLauncher1->sys->max = 0;
		}
		for (int j = 0; j < enemyLauncher2->sys->sprites.size(); ++j)
		{
			enemyLauncher2->sys->sprites[j].lifespan = 0;
			enemyLauncher2->sys->max = 0;
		}
		for (int j = 0; j < enemyLauncher3->sys->sprites.size(); ++j)
		{
			enemyLauncher3->sys->sprites[j].lifespan = 0;
			enemyLauncher3->sys->max = 0;
		}
		for (int j = 0; j < enemyLauncher4->sys->sprites.size(); ++j)
		{
			enemyLauncher4->sys->sprites[j].lifespan = 0;
			enemyLauncher4->sys->max = 0;
		}
		for (int i = 0; i < turret->sys->sprites.size(); ++i)
		{
			turret->sys->sprites[i].lifespan = 0;
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	gameOverScreen.draw();
	
	if (gameOver == false)
	{

		background.draw();
		tri.draw();
		if (iFrames == 0 || iFrames % 2 == 0)
		{
			player.draw();
		}
		turret->draw();
		enemyLauncher1->draw();
		enemyLauncher2->draw();
		enemyLauncher3->draw();
		enemyLauncher4->draw();

		if (hpupPres == true)
		{
			hpPickup.draw();
		}
		
		string s;
		s += "Score: " + std::to_string(score);
		verdana14.drawString(s, ofGetWindowWidth() - 150, 35);


		string st;
		st += "High Score: " + std::to_string(highScore);
		verdana14.drawString(st, ofGetWindowWidth() - 202, 55);
		for (int i = 0; i < curHealth; i++)
		{
			health[i].draw();
		}
		partEmit.draw();
		if (newGame == true)
		{
			Title.draw();
		}
		
		
		
	}
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);
	
}

bool TriangleShape::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	glm::vec3 v1 = glm::normalize(p1 - p);
	glm::vec3 v2 = glm::normalize(p2 - p);
	glm::vec3 v3 = glm::normalize(p3 - p);
	float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
	float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
	float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
	if (a1 < 0 && a2 < 0 && a3 < 0) return true;
	else return false;
}
//--------------------------------------------------------------
//
void ofApp::keyPressed(int key) {
	ofVec3f accel = tri.acceleration;
	float rotAccel = tri.rotAcceleration;
	switch (key) {
	case OF_KEY_LEFT:
	case 'a':
	case 'A':
		if (spinning == false)
		{
			rotAccel = tri.rotAcceleration;
			rotAccel -= (tri.thrust * (1.0 / tri.mass));
			tri.rotVel += rotAccel * (1.0/ ofGetFrameRate());
			tri.rotVel *= tri.damping;
			tri.rotation += (tri.rotVel * (1.0 / ofGetFrameRate()))*2;
			player.rotation = tri.rotation;
		}
		
		break;
	case OF_KEY_RIGHT:
	case 'd':
	case 'D':
		if (spinning == false)
		{
			rotAccel = tri.rotAcceleration;
			rotAccel += (tri.thrust * (1.0 / tri.mass));
			tri.rotVel += rotAccel * (1.0 / ofGetFrameRate());
			tri.rotVel *= tri.damping;
			tri.rotation += tri.rotVel * (1.0 / ofGetFrameRate());
			player.rotation = tri.rotation;
		}

		break;
	case OF_KEY_UP:
	case 'w':
	case 'W':
		if (spinning == false)
		{
			if (tri.pos.y-40 < 0.0)
			{tri.pos = glm::vec3(tri.pos.x, tri.pos.y + 1, 0);}
			else if (tri.pos.y + 40 > ofGetHeight())
			{tri.pos = glm::vec3(tri.pos.x, tri.pos.y - 1, 0);}
			else if (tri.pos.x + 40 > ofGetWidth())
			{tri.pos = glm::vec3(tri.pos.x-1, tri.pos.y, 0);}
			else if (tri.pos.x - 40 < 0.0)
			{tri.pos = glm::vec3(tri.pos.x + 1, tri.pos.y, 0);}
			else 
			{
				accel = tri.acceleration;
				accel += (tri.thrust * (1.0 / tri.mass));
				tri.velocity += accel * (1.0 / ofGetFrameRate());
				tri.velocity *= tri.damping;
				tri.pos += (glm::vec3((tri.velocity *  (1.0 / ofGetFrameRate()))) * tri.heading);
			}
		}

		
		break;
	case OF_KEY_DOWN:
	case 's':
	case 'S':
		if (spinning == false)
		{
			if (tri.pos.y - 40 < 0.0)
			{tri.pos = glm::vec3(tri.pos.x, tri.pos.y + 1, 0);}
			else if (tri.pos.y + 40 > ofGetHeight())
			{tri.pos = glm::vec3(tri.pos.x, tri.pos.y - 1, 0);}
			else if (tri.pos.x + 40 > ofGetWidth())
			{tri.pos = glm::vec3(tri.pos.x - 1, tri.pos.y, 0);}
			else if (tri.pos.x - 40 < 0.0)
			{tri.pos = glm::vec3(tri.pos.x + 1, tri.pos.y, 0);}
			else
			{
				accel = tri.acceleration;
				accel -= (tri.thrust * (1.0 / tri.mass));
				tri.velocity += accel * (1.0 / ofGetFrameRate());
				tri.velocity *= tri.damping;
				tri.pos += glm::vec3((tri.velocity * (1.0 / ofGetFrameRate()))) * tri.heading;
			}
		}

		break;
	case ' ':
		if (spinning == false)
		{
			if(newGame == false && gameOver == false)
			{
				spacePressed = true;
			}
			if (newGame == true && gameOver == false)
			{
				newGame = false;
				
			}
			if (gameOver == true)
			{
				curHealth = 3;
				gameOver = false;
				newGame = true;
				bgm.play();
			}
		

		}

		break;

	default:
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		spacePressed = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (spinning == false)
	{
		if (tri.stored == true)
		{
			if (tri.pos.y - 40 < 0.0)
			{
				tri.pos = glm::vec3(tri.pos.x, tri.pos.y + 1, 0);
				tri.stored = false;
			}
			else if (tri.pos.y + 40 > ofGetHeight())
			{
				tri.pos = glm::vec3(tri.pos.x, tri.pos.y - 1, 0);
				tri.stored = false;
			}
			else if (tri.pos.x + 40 > ofGetWidth())
			{
				tri.pos = glm::vec3(tri.pos.x - 1, tri.pos.y, 0);
				tri.stored = false;
			}
			else if (tri.pos.x - 40 < 0.0)
			{
				tri.pos = glm::vec3(tri.pos.x + 1, tri.pos.y, 0);
				tri.stored = false;
			}
			else 
			{
				tri.pos = glm::vec3(x, y, 0);
				turret->setPosition(ofVec3f(x, y, 0));
			}
			player.pos = tri.pos;
		}
	}


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (spinning == false)
	{
		glm::vec3 p1 = tri.pos + glm::vec3(-20, -20, 1);
		glm::vec3 p2 = tri.pos + glm::vec3(0, 40, 1);
		glm::vec3 p3 = tri.pos + glm::vec3(20, -20, 1);

		if (tri.inside(glm::vec3(x, y, 0), p1, p2, p3) == true)
		{
			tri.stored = true;
		}
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	tri.stored = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	inside = true;
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	inside = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
