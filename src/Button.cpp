#include "Button.h"

Button::Button(SDL_Renderer* renderer, std::string path, int x, int y, int w, int h)
{
	mRenderer = renderer;
	mButton = new TexturedRectangle(mRenderer, path);
	mButton->SetPosition(x, y);
	mButton->SetDimension(w, h);
	mIsClicking = false;
	mRectButton.x = x;
	mRectButton.y = y;
	mRectButton.w = w;
	mRectButton.h = h;
}

Button::~Button()
{
	mRenderer = nullptr;
	delete mButton;
}

void Button::HandleMouseDown()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x >= mRectButton.x && x <= mRectButton.x + mRectButton.w && y >= mRectButton.y && y <= mRectButton.y + mRectButton.h) {
		mIsClicking = true;
	}
}

void Button::HandleMouseOver() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x >= mRectButton.x && x <= mRectButton.x + mRectButton.w && y >= mRectButton.y && y <= mRectButton.y + mRectButton.h) {
		mButton->SetPosition(mRectButton.x - mRectButton.w / 12, mRectButton.y - mRectButton.h / 12);
		mButton->SetDimension(7 * mRectButton.w / 6, 7 * mRectButton.h / 6);
	}
	else {
		mButton->SetPosition(mRectButton.x, mRectButton.y);
		mButton->SetDimension(mRectButton.w, mRectButton.h);
	}
}

bool Button::IsClicking()
{
	if (mIsClicking) {
		mIsClicking = false;
		return true;
	}
	return false;
}

void Button::Render()
{
	if (mButton != nullptr) {
		mButton->Render(mRenderer);
	}
}
