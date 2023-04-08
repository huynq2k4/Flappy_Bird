#ifndef TEXTURED_RECTANGLE_H
#define TEXTURED_RECTANGLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "CheckCollision.h"
#include <iostream>
#include <vector>

using namespace std;

class TexturedRectangle {
private:
	SDL_Texture* mTexture = nullptr;
	SDL_Rect mRect;

	SDL_Rect* mClip;

	double mAngle;

	SDL_RendererFlip mFlip;

	



	
	

public:
	//Image constructor
	TexturedRectangle(SDL_Renderer*, std::string);

	//Text constructor
	TexturedRectangle(SDL_Renderer*, TTF_Font*, std::string, SDL_Color);

	//Destructor
	~TexturedRectangle();

	//Set the coordinate of texture
	void SetPosition(int x, int y);

	//Set width and height of texture
	void SetDimension(int w, int h);

	//Set angle of texture
	void SetAngle(double angle);

	//Set clip from images (render animated sprite)
	void SetClip(SDL_Rect* clip);

	//Set image flipping
	void SetFlip(SDL_RendererFlip flip);

	//Get x-coordinate
	double GetX();

	//Get y-coordinate
	double GetY();

	//Get texture width
	int GetWidth();

	//Get texture height
	int GetHeight();

	//Get texture angle
	double GetAngle();

	//Render texture
	void Render(SDL_Renderer* renderer);

	//Check collision
	SDL_bool IsColliding(TexturedRectangle& obj);

	void SetAlpha(int alpha);

	//Get rectangle from texture
	inline SDL_Rect GetRect() { return mRect; }

	

	

};

#endif
