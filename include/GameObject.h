#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "TexturedRectangle.h"
#include <vector>

class GameObject {
private:
	TexturedRectangle* mSprite;
	SDL_Renderer* mRenderer;
	

public:
	//Constructor
	GameObject();
	GameObject(SDL_Renderer* renderer, std::string path);

	//Destructor
	~GameObject();

	//Update properties
	void Update();

	//Get coordinate
	double GetX();
	double GetY();

	//Get angle
	double GetAngle();

	//Render object
	void Render();

	//Retrieve textured rectangle from object
	TexturedRectangle& GetTexturedRectangle();

	virtual SDL_bool IsColliding(GameObject& obj);
	

};



#endif