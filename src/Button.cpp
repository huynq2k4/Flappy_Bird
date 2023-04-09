#include "Button.h"

Button::Button(SDL_Renderer* renderer, std::string path, int x, int y, int w, int h)
{
	mRenderer = renderer;
	mButton = new TexturedRectangle(mRenderer, path);
	mIsClicking = false;
	mRectButton.x = x;
	mRectButton.y = y;
	mRectButton.w = w;
	mRectButton.h = h;
}

Button::~Button()
{
	mRenderer = nullptr;
	mButton->~TexturedRectangle();
}

void Button::HandleButtonClicking(SDL_Event event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x >= mRectButton.x && x <= mRectButton.x + mRectButton.w && y >= mRectButton.y && y <= mRectButton.y + mRectButton.h) {
			mIsClicking = true;
		}
	}
	else if (event.type == SDL_MOUSEMOTION) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x >= mRectButton.x && x <= mRectButton.x + mRectButton.w && y >= mRectButton.y && y <= mRectButton.y + mRectButton.h) {
			mButton->SetPosition(mRectButton.x - mRectButton.w / 8, mRectButton.y - mRectButton.h / 8);
			mButton->SetDimension(5 * mRectButton.w / 4, 5 * mRectButton.h / 4);
		}
		else {
			mButton->SetPosition(mRectButton.x, mRectButton.y);
			mButton->SetDimension(mRectButton.w, mRectButton.h);
		}
	}
}

bool Button::IsClicking()
{
	return mIsClicking;
}

void Button::Render()
{
	if (mButton != nullptr) {
		mButton->Render(mRenderer);
	}
}
