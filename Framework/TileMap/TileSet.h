#pragma once
struct TileData
{
	wstring tileName = L"";
	Vector2 tilePos = Vector2(0, 0);
	Vector2 tileSize = Vector2(0, 0);
};

struct SpriteData
{
	Texture2D* sprite = nullptr;
	int tileXCount = 1;
	int tileYCount = 1;
	Vector2 texelTileSize = Vector2(1, 1);
	vector<TileData> tileDatas;

	void CalculateTexelTileSize()
	{
		texelTileSize.x = 1 / (float)tileXCount;
		texelTileSize.y = 1 / (float)tileYCount;
	}

};

class TileSet : public SingletonBase<TileSet>
{
	friend class SingletonBase<TileSet>;
	friend class TileMap;
public:
	void GUI();

private:
	void CreateImageButton(pair<string, SpriteData> sprite);
	void OpenSpriteImage(wstring filePath = L"");
	void SaveSpriteInfos();

	void OpenSpriteInfos(wstring filePath = L"");


	TileSet();
	~TileSet();

private:
	unordered_map<string, SpriteData> spriteMap;
	
	Vector2 SelectedStartUV = Vector2(0, 0);
	Vector2 SelectedtexelTileSize = Vector2(0, 0);
	string selectedSpriteName = "";

};