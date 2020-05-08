#pragma once

class TileMapDemo : public IObject
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
	class TileMap* tm = nullptr;
	Texture2D* bkImage = nullptr;
};