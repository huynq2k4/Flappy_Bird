#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>


class SoundEffect {
private:
	Mix_Chunk* mSoundEffect;
public:
	SoundEffect(std::string path);
	~SoundEffect();
	void PlaySound(int loop);
	void SetVolume();
	void StopSound();
};

class Music{
private:
	Mix_Music* mMusic;
public:
	Music(std::string path);
	~Music();
	void PlaySound(int loop);
	void PauseSound();
	void SetVolume(int volume);
	void StopSound();
};


#endif
