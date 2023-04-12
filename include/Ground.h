#ifndef GROUND_H
#define GROUND_H

#include "GameObject.h"

class Ground :public GameObject {
public:
	using GameObject::GameObject;
	int mScrollingGround = 0;
	void Scroll(int speedScrolling);

	void RenderScrolling();
};

#endif
