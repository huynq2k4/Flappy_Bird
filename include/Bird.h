#ifndef BIRD_H
#define BIRD_H

#include "GameObject.h"
#include "TexturedRectangle.h"


class Bird : public GameObject {
private:
	double mVelFly;
	double mVelRotate;
	SDL_Rect mFrame;
	int mSpriteWidth;
	int mSpriteHeight;

	
public:
	using GameObject::GameObject;

	void SetDefaultStatus(double posX, double posY, double angle, double velFly, double velRotate);

	void SetDefaultFrame(int x, int y, int w, int h);

	void DrawFrame(int frame, int speed);

	void Fly(double velFly, double angle);
	void FreeFall(double accelerationFly, double accelerationRotate);
	void StopOnGround(double posY);


};


#endif
