#pragma once

class TextureDemo : public IObject
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
	class TextureRect* tr1 = nullptr;
	class BlurredTexture* tr2 = nullptr;
	Texture2D* texture = nullptr;
};