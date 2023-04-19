#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedRectangle.h"

class Button {
private:
	TexturedRectangle* mButton;
	SDL_Renderer* mRenderer;
	bool mIsClicking;
	SDL_Rect mRectButton;
	SDL_Rect mClip;
public:
	Button(SDL_Renderer* renderer, std::string path, int x, int y, int w, int h);
	~Button();

	void SetClip(int x, int y, int w, int h);

	SDL_Rect GetClip();

	void HandleMouseDown();
	void HandleMouseOver();
	bool IsClicking();
	void Render();
};

#endif
