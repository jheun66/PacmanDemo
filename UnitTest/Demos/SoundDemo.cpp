#include "stdafx.h"
#include "SoundDemo.h"

#include "Systems/SoundSystem.h"

void SoundDemo::Init()
{
	bgSound = new SoundSystem();
	bgSound->CreateBGSound("../_Resources/_Sounds/story.mp3", "BGM");
	bgSound->Play();

	effectSound = new SoundSystem();
	effectSound->CreateEffectSound("../_Resources/_Sounds/play_fire.wav", "Fire");
}

void SoundDemo::Destroy()
{
	SAFE_DELETE(bgSound);
	SAFE_DELETE(effectSound);
}

void SoundDemo::Update()
{
	bgSound->Update();
	effectSound->Update();

	if (Keyboard::Get()->Down(VK_SPACE))
		effectSound->Play();
}

void SoundDemo::GUI()
{
	ImGui::Begin("SoundSystem");
	{
		bgSound->GUI();
		effectSound->GUI();
	}
	ImGui::End();
}
