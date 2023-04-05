#include "Pipe.h"
#include <random>

Pipe::Pipe()
{

	mPipeHigh = nullptr;
	mPipeLow = nullptr;
	mSpriteWidth = 0;
	mSpriteHeight = 0;
	mIsMoving = true;
	mIsMovingDown = false;
	//mRenderer = nullptr;
}

Pipe::Pipe(SDL_Renderer* renderer, std::string path, int w, int h)
{
	//mRenderer = renderer;
	mPipeHigh = new GameObject(renderer, path);
	mPipeHigh->GetTexturedRectangle().SetFlip(SDL_FLIP_VERTICAL);
	mPipeLow = new GameObject(renderer, path);

	mPipeHigh->GetTexturedRectangle().SetDimension(w, h);
	mPipeLow->GetTexturedRectangle().SetDimension(w, h);
	mSpriteHeight = h;
	mSpriteWidth = w;
	mIsMoving = true;
	mIsMovingDown = false;

}

GameObject& Pipe::GetPipe()
{
	return *(mPipe.second);
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
}

void Pipe::MoveHorizontal(int speed)
{
	mPipeHigh->GetTexturedRectangle().SetPosition(mPipeHigh->GetX() - speed, mPipeHigh->GetY());
	mPipeLow->GetTexturedRectangle().SetPosition(mPipeLow->GetX() - speed, mPipeLow->GetY());
	
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

int Pipe::GetPipeY()
{
	return mPipeLow->GetY();
}

bool Pipe::IsMovingDown()
{
	return mIsMovingDown;
}
