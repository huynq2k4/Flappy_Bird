#include "TexturedRectangle.h"
#include "ResourceManager.h"

//Constructor
TexturedRectangle::TexturedRectangle(SDL_Renderer* renderer, std::string path)
{
	//Get surface through the resource manager
	SDL_Surface* surfaceRect = ResourceManager::GetInstance().GetSurface(path); 

	//Initialize texture
	mTexture = SDL_CreateTextureFromSurface(renderer, surfaceRect);

	SDL_FreeSurface(surfaceRect);
	

	//Set initial value for textured rectangle
	mRect.x = 200;
	mRect.y = 200;
	mRect.w = 10;
	mRect.h = 10;

	//Set normal value for extra properties
	mAngle = 0;
	mClip = NULL;
	mFlip = SDL_FLIP_NONE;
}

//Destructor
TexturedRectangle::~TexturedRectangle()
{
	SDL_DestroyTexture(mTexture);
}

//Set the coordinate of texture
void TexturedRectangle::SetPosition(double x, double y)
{
	mRect.x = x;
	mRect.y = y;
}

//Set width and height of texture
void TexturedRectangle::SetDimension(int w, int h)
{
	mRect.w = w;
	mRect.h = h;
}

//Set angle of texture
void TexturedRectangle::SetAngle(double angle)
{
	mAngle = angle;
}

//Set clip from images (render animated sprite)
void TexturedRectangle::SetClip(SDL_Rect* clip)
{
	mClip = clip;
}

//Set image flipping
void TexturedRectangle::SetFlip(SDL_RendererFlip flip)
{
	mFlip = flip;
}

//Get texture width
int TexturedRectangle::GetWidth()
{
	return mRect.w;
}

//Get texture height
int TexturedRectangle::GetHeight()
{
	return mRect.h;
}



//Render texture
void TexturedRectangle::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, mTexture, mClip, &mRect, mAngle, NULL, mFlip);
}

//Check collision
SDL_bool TexturedRectangle::IsColliding(TexturedRectangle& obj)
{
	SDL_Rect tmp = obj.GetRect();
	return SDL_HasIntersection(&mRect, &tmp);
}
