#include "Bird.h"

void Bird::SetDefaultStatus(double posX, double posY, double angle, double velFly, double velRotate)
{
	GetTexturedRectangle().SetPosition(posX, posY);
	GetTexturedRectangle().SetAngle(angle);
	mVelFly = velFly;
	mVelRotate = velRotate;
}

void Bird::SetDefaultFrame(int x, int y, int w, int h)
{
	mFrame.x = x;
	mFrame.y = y;
	mFrame.w = w;
	mFrame.h = h;
	mSpriteWidth = w;
	mSpriteHeight = h;
}

void Bird::DrawFrame(int frame, int speed)
{
	mFrame.x = mSpriteWidth * (int)(frame / speed);
	GetTexturedRectangle().SetClip(&mFrame);
}

void Bird::Fly(double velFly, double angle)
{
	mVelFly = velFly;
	GetTexturedRectangle().SetAngle(angle);
	mVelRotate = 0;
}

void Bird::FreeFall(double accelerationFly, double accelerationRotate)
{
	double posY = GetY();
	posY += mVelFly + accelerationFly;
	mVelFly += accelerationFly;

	double angle = GetAngle();
	angle += mVelRotate + accelerationRotate;
	mVelRotate += accelerationRotate;

	if (angle >= 90) angle = 90;
	GetTexturedRectangle().SetPosition(GetX(), posY);
	GetTexturedRectangle().SetAngle(angle);
}

void Bird::StopOnGround(double posY)
{
	GetTexturedRectangle().SetPosition(GetX(), posY);
}
