#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "TexturedRectangle.h"

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


	//Render object
	void Render();

	//Retrieve textured rectangle from object
	TexturedRectangle& GetTexturedRectangle();
};



#endif