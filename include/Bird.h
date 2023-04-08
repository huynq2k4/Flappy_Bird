#ifndef BIRD_H
#define BIRD_H

#include "GameObject.h"
#include "TexturedRectangle.h"
#include "CheckCollision.h"


class Bird : public GameObject {
private:
	double mVelFly;
	double mVelRotate;
	SDL_Rect mFrame;
	int mSpriteWidth;
	int mSpriteHeight;
	vector<Point> mCollisionInit;
	vector<Point> mCollisionShape;

	
public:
	using GameObject::GameObject;

	void SetDefaultStatus(double posX, double posY, double angle, double velFly, double velRotate);

	void SetDefaultFrame(int x, int y, int w, int h);

	void DrawFrame(int frame, int speed);

	void Fly(double velFly, double angle);
	void FreeFall(double accelerationFly, double accelerationRotate);
	void StopOnGround(double posY);

	void CreateCollisionShape(vector<Point> collisionShape);

	vector<Point>& GetCollisionShape();

	void ShiftColliders();
	SDL_bool IsColliding(GameObject& obj);

};


#endif
