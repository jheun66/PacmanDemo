#pragma once

#include <FMod/fmod.h>

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void CreateBGSound(const string& fileName, string soundName);
	void CreateEffectSound(const string& fileName, string soundName);
	void Play();
	void Stop();
	void Pause();
	void Update();

	void GUI();
private:
	FMOD_SYSTEM* system = nullptr;
	FMOD_SOUND* sound = nullptr;
	FMOD_CHANNEL* channel = nullptr;

	float volume = 1.0f;
	string soundName = "";
	bool bMute = false;
	bool bLoop = false;
};