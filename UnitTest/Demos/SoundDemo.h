#pragma once
class SoundDemo :public IObject
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override {}
	void Render() override {}
	void PostRender() override {}

	void GUI() override;

private:
	class SoundSystem* bgSound = nullptr;
	class SoundSystem* effectSound = nullptr;
};