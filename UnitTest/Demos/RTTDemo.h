#pragma once

class RTTDemo :public IObject
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
	class RenderTexture* rt = nullptr;
	class TextureRect* rtt = nullptr;

	class TextureRect* tr1 = nullptr;
	class TextureRect* tr2 = nullptr;
};