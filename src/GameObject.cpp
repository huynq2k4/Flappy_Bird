#include "GameObject.h"
#include <iostream>

//Constructor
GameObject::GameObject()
{
	mSprite = nullptr;
	mRenderer = nullptr;
}

//Constructor with parameters
GameObject::GameObject(SDL_Renderer* renderer, std::string path)
{
	mRenderer = renderer;
	mSprite = new TexturedRectangle(mRenderer, path);
}

//Destructor
GameObject::~GameObject()
{
	mSprite->~TexturedRectangle();

	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;
}

//Update properties
void GameObject::Update()
{
}

//Render object
void GameObject::Render()
{
	if (mSprite != nullptr) {
		mSprite->Render(mRenderer);
	}
}

//Retrieve textured rectangle from object
TexturedRectangle& GameObject::GetTexturedRectangle()
{
	return *mSprite;
}
