#pragma once

class AnimationDemo : public IObject
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void GUI() override;
private:
	class AnimationRect* ar = nullptr;

	Texture2D* bkImage = nullptr;
	class TextureRect* bkRect = nullptr;

};