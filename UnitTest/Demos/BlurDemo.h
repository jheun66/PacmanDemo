#pragma once

class BlurDemo : public IObject
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override {}

	void GUI() override;

private:
	class BlurredTexture* bt = nullptr;
	class AnimationRect* ar = nullptr;

};