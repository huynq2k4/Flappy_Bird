#ifndef TEXTURED_RECTANGLE_H
#define TEXTURED_RECTANGLE_H

#include <SDL.h>
#include <string>


class TexturedRectangle {
private:
	SDL_Texture* mTexture = nullptr;
	SDL_Rect mRect;

	SDL_Rect* mClip;

	double mAngle;

	SDL_RendererFlip mFlip;

	//Get rectangle from texture
	inline SDL_Rect GetRect() { return mRect; }
	

public:
	//Constructor
	TexturedRectangle(SDL_Renderer* renderer, std::string path);

	//Destructor
	~TexturedRectangle();

	//Set the coordinate of texture
	void SetPosition(double x, double y);

	//Set width and height of texture
	void SetDimension(int w, int h);

	//Set angle of texture
	void SetAngle(double angle);

	//Set clip from images (render animated sprite)
	void SetClip(SDL_Rect* clip);

	//Set image flipping
	void SetFlip(SDL_RendererFlip flip);

	//Get texture width
	int GetWidth();

	//Get texture height
	int GetHeight();

	//Render texture
	void Render(SDL_Renderer* renderer);

	//Check collision
	SDL_bool IsColliding(TexturedRectangle& obj);


};

#endif
