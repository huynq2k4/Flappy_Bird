#include "Text.h"

Text::Text()
{
	mText = nullptr;
	mFont = nullptr;
	mRenderer = nullptr;
	mColor = { 0, 0, 0 };
	mRect = { 0, 0, 0, 0 };
}

Text::Text(SDL_Renderer* renderer, std::string fontPath, std::string text, int fontSize, SDL_Color color)
{
	mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
	mRenderer = renderer;
	mText = new TexturedRectangle(mRenderer, mFont, text, color);
	mColor = color;
	mRect.x = 0;
	mRect.y = 0;
	mRect.w = 0;
	mRect.h = 0;
}

Text::~Text()
{
	mText->~TexturedRectangle();
	TTF_CloseFont(mFont);
	mRenderer = nullptr;
}

void Text::SetSize(int x, int y, int w, int h)
{
	if (mText != nullptr) {
		mText->SetPosition(x, y);
		mText->SetDimension(w, h);
	}
	mRect.x = x;
	mRect.y = y;
	mRect.w = w;
	mRect.h = h;
}

void Text::ChangeText(std::string text)
{
	mText->~TexturedRectangle();
	mText = new TexturedRectangle(mRenderer, mFont, text, mColor);
	SetSize(mRect.x, mRect.y, mRect.w, mRect.h);
}

void Text::Render()
{
	if (mText != nullptr) {
		mText->Render(mRenderer);
	}
}
