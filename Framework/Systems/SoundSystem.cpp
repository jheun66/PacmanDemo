#include "Framework.h"
#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
	FMOD_RESULT result = FMOD_System_Create(&system);
	if (result != FMOD_OK)
		CHECK(false);

	FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
}



SoundSystem::~SoundSystem()
{
	FMOD_Sound_Release(sound);

	FMOD_System_Close(system);
	FMOD_System_Release(system);
}

void SoundSystem::CreateBGSound(const string& fileName, string soundName)
{
	FMOD_RESULT result = FMOD_System_CreateSound(system, fileName.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);
	if (result != FMOD_OK)
		CHECK(false);

	this->soundName = soundName;
	bLoop = true;
}

void SoundSystem::CreateEffectSound(const string& fileName, string soundName)
{
	FMOD_RESULT result = FMOD_System_CreateSound(system, fileName.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK)
		CHECK(false);

	this->soundName = soundName;
}

void SoundSystem::Play()
{
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, false, &channel);
}

void SoundSystem::Stop()
{
	FMOD_Channel_Stop(channel);
}

void SoundSystem::Pause()
{
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, true, &channel);
}


void SoundSystem::Update()
{
	FMOD_System_Update(system);

	FMOD_Channel_SetVolume(channel, volume);
	FMOD_MODE mode = bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_Channel_SetMode(channel, mode);
	FMOD_Channel_SetMute(channel, bMute);

}

void SoundSystem::GUI()
{
	if(ImGui::BeginMenu(soundName.c_str()))
	{
		ImGui::SliderFloat("Volume", &volume, 0, 1.0f, "%.1f");
		ImGui::Checkbox("Mute", &bMute);
		ImGui::Checkbox("Loop", &bLoop);

		ImGui::EndMenu();
	}
}
