#include "Sound.h"

SoundEffect::SoundEffect(std::string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	mSoundEffect = Mix_LoadWAV(path.c_str());
}

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(mSoundEffect);
}

void SoundEffect::PlaySound(int loop)
{
	if (mSoundEffect != nullptr) {
		Mix_PlayChannel(-1, mSoundEffect, loop);
	}
}

void SoundEffect::SetVolume(int volume)
{
	Mix_VolumeChunk(mSoundEffect, volume);
}

void SoundEffect::StopSound()
{
	Mix_HaltChannel(-1);
}

Music::Music(std::string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	mMusic = Mix_LoadMUS(path.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(mMusic);
}

void Music::PlaySound(int loop)
{
	if (mMusic != nullptr) {
		
		Mix_PlayMusic(mMusic, loop);
	}
}

void Music::PauseSound()
{
	Mix_PauseMusic();
}

void Music::ResumeSound()
{
	Mix_ResumeMusic();
}

void Music::SetVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

void Music::StopSound()
{
	Mix_HaltMusic();
}
