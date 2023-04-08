#include "Pipe.h"
#include <random>
#include "CheckCollision.h"

Pipe::Pipe()
{

	mPipeHigh = nullptr;
	mPipeLow = nullptr;
	mSpriteWidth = 0;
	mSpriteHeight = 0;
	mIsMoving = true;
	mIsMovingDown = false;
	mGetScore = { 0, 0, 0, 0 };
	mDistance = 0;
	mIsScoring = false;
}

Pipe::Pipe(SDL_Renderer* renderer, std::string path, int w, int h)
{
	
	mPipeHigh = new GameObject(renderer, path);
	mPipeHigh->GetTexturedRectangle().SetFlip(SDL_FLIP_VERTICAL);
	mPipeLow = new GameObject(renderer, path);

	mPipeHigh->GetTexturedRectangle().SetDimension(w, h);
	mPipeLow->GetTexturedRectangle().SetDimension(w, h);
	mSpriteHeight = h;
	mSpriteWidth = w;
	mIsMoving = true;
	mIsMovingDown = false;
	mGetScore = { 0, 0, 0, 0 };
	mDistance = 0;
	mIsScoring = false;

}

GameObject& Pipe::GetPipeUp()
{
	return *mPipeHigh;
}

GameObject& Pipe::GetPipeDown()
{
	return *mPipeLow;
}


void Pipe::SetDimension(int w, int h) {
	mPipeHigh->GetTexturedRectangle().SetDimension(w, h);
	mPipeLow->GetTexturedRectangle().SetDimension(w, h);
	mSpriteHeight = h;
	mSpriteWidth = w;
}

void Pipe::SetStatus(int posX, int posYMax, int posYMin, int distance)
{
	
	mPipeLow->GetTexturedRectangle().SetPosition(posX, rand() % (posYMax - posYMin + 1) + posYMin);
	mPipeHigh->GetTexturedRectangle().SetPosition(posX, mPipeLow->GetY() - distance - mSpriteHeight);
	mGetScore.x = mPipeLow->GetX() + mSpriteWidth;
	mGetScore.y = mPipeLow->GetY() - distance;
	mGetScore.h = distance;
	mDistance = distance;
}

void Pipe::MoveHorizontal(int speed)
{
	mPipeHigh->GetTexturedRectangle().SetPosition(mPipeHigh->GetX() - speed, mPipeHigh->GetY());
	mPipeLow->GetTexturedRectangle().SetPosition(mPipeLow->GetX() - speed, mPipeLow->GetY());
	mGetScore.x = mPipeLow->GetX() + mSpriteWidth;

	
}

void Pipe::MoveVertical(int speed, bool isMovingDown)
{
	if (isMovingDown) {
		mPipeHigh->GetTexturedRectangle().SetPosition(mPipeHigh->GetX(), mPipeHigh->GetY() + speed);
		mPipeLow->GetTexturedRectangle().SetPosition(mPipeLow->GetX(), mPipeLow->GetY() + speed);
	}
	else {
		mPipeHigh->GetTexturedRectangle().SetPosition(mPipeHigh->GetX(), mPipeHigh->GetY() - speed);
		mPipeLow->GetTexturedRectangle().SetPosition(mPipeLow->GetX(), mPipeLow->GetY() - speed);
	}
	mGetScore.y = mPipeLow->GetY() - mDistance;
}

int Pipe::GetPipeWidth()
{
	return mSpriteWidth;
}

int Pipe::GetPipeHeight()
{
	return mSpriteHeight;
}

int Pipe::GetPipeX()
{
	return mPipeLow->GetX();
}

void Pipe::SetMode()
{
	int tmp = rand() % 2;
	if (tmp == 0) mIsMoving = true;
	else mIsMoving = false;
}



bool Pipe::IsMoving()
{
	return mIsMoving;
}



void Pipe::Render()
{
	if (mPipeHigh != nullptr && mPipeLow != nullptr) {
		mPipeHigh->Render();
		mPipeLow->Render();
	}
}

void Pipe::SetMoveDown(bool goDown)
{
	mIsMovingDown = goDown;
}

bool Pipe::DetectScoring(Bird* bird)
{
	Point center = { bird->GetTexturedRectangle().GetRect().x + 1.0 * bird->GetTexturedRectangle().GetRect().w / 2, bird->GetTexturedRectangle().GetRect().y + 1.0 * bird->GetTexturedRectangle().GetRect().h / 2 };
	vector<Point> collisionShape = bird->GetCollisionShape();
	if (CheckCollision2(collisionShape, bird->GetAngle(), center, &mGetScore) == SDL_TRUE && mIsScoring == false) {
		mIsScoring = true;
		return true;
	}
	else if (CheckCollision2(collisionShape, bird->GetAngle(), center, &mGetScore) == SDL_FALSE && mIsScoring == true) {
		mIsScoring = false;
	}
	return false;
}

int Pipe::GetPipeY()
{
	return mPipeLow->GetY();
}

bool Pipe::IsMovingDown()
{
	return mIsMovingDown;
}
