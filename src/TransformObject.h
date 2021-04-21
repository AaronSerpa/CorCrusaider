#pragma once
#include "ofMain.h"

//  Aaron Serpa - CS 134 SJSU
//

//  Base class for any object that needs a transform.
//
class TransformObject {
public:
	TransformObject();
	ofVec3f position, scale;
	float	rotation;
	bool	bSelected;
	void setPosition(ofVec3f p);
};