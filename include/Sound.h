#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Sound {
private:
	Mix_Chunk* mSound;
public:
	Sound(std::string path);
	~Sound();

	void PlaySound(int loop);

	void StopSound();


};


#endif
