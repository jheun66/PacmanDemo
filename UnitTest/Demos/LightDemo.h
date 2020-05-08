#pragma once
class LightDemo :public IObject
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

	uint spacing = 80;
	uint width = WinMaxWidth / spacing * 2;
	uint height = WinMaxHeight / spacing * 2;


	class TileMap* tm = nullptr;
	class AnimationRect* ar = nullptr;

	class RenderTexture* rt = nullptr;
	class LightMap* lm = nullptr;

};