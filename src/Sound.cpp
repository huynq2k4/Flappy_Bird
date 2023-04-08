#include "Sound.h"

Sound::Sound(std::string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	mSound = Mix_LoadWAV(path.c_str());
}

Sound::~Sound()
{
	Mix_FreeChunk(mSound);
}

void Sound::PlaySound(int loop)
{
	if (mSound != nullptr) {
		Mix_PlayChannel(-1, mSound, loop);
	}
}

void Sound::StopSound()
{
	Mix_HaltChannel(-1);
}


