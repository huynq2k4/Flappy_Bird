#ifndef SCORE_H
#define SCORE_H


#include <SDL.h>
#include <SDL_ttf.h>
#include "TexturedRectangle.h"



class Score {
private:
	TexturedRectangle* mScore;
	TTF_Font* mFont;

public:
	Score();
	Score(std::string fontPath);


};

#endif
