#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedRectangle.h"

class Button {
private:
	TexturedRectangle* mButton;
	SDL_Renderer* mRenderer;
	bool mIsClicking;
	SDL_Rect mRectButton;
public:
	Button(SDL_Renderer* renderer, std::string path, int x, int y, int w, int h);
	~Button();

	void HandleButtonClicking(SDL_Event event);

	bool IsClicking();
	void Render();
};

#endif
