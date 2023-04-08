#ifndef PIPE_H
#define PIPE_H

#include "GameObject.h"
#include <iostream>
#include "Bird.h"

class Pipe{
private:
	std::pair<GameObject*, GameObject*> mPipe;
	GameObject* mPipeHigh;
	GameObject* mPipeLow;
	int mSpriteWidth, mSpriteHeight;
	bool mIsMoving;
	bool mIsMovingDown;
	SDL_Rect mGetScore;
	int mDistance;
	bool mIsScoring;
public:
	Pipe();

	Pipe(SDL_Renderer* renderer, std::string path, int w, int h);

	GameObject& GetPipeUp();
	GameObject& GetPipeDown();

	void SetDimension(int w, int h);

	void SetStatus(int posX, int posYMax, int posYMin, int distance);

	void SetMode();
	void SetMoveDown(bool goDown);
	bool IsMoving();
	bool IsMovingDown();
	int GetPipeX();
	int GetPipeY();
	int GetPipeWidth();
	int GetPipeHeight();

	void MoveHorizontal(int speed);

	void MoveVertical(int speed, bool isMovingDown);

	void Render();

	bool DetectScoring(Bird* bird);

};

#endif
