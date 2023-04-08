#ifndef TEXT_H
#define TEXT_H


#include <SDL.h>
#include <SDL_ttf.h>
#include "TexturedRectangle.h"



class Text {
private:
	TexturedRectangle* mText;
	TTF_Font* mFont;
	SDL_Color mColor;
	SDL_Renderer* mRenderer;
	SDL_Rect mRect;

public:
	Text();
	Text(SDL_Renderer* renderer, std::string fontPath, std::string text, int fontSize, SDL_Color color);
	~Text();

	void SetSize(int x, int y, int w, int h);

	void ChangeText(std::string text);

	void Render();

};

#endif
