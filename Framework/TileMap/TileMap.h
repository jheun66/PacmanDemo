#pragma once

class Tile;

class TileBuffer : public ShaderBuffer
{
public:
	TileBuffer() 
		: ShaderBuffer(&data, sizeof(Struct))
	{

	}
	void SetTileIndex(uint index) {
		data.tileIndex = index;
	}
	void SetTileIndex2(uint index) {
		data.tileIndex2 = index;
	}
	void SetTileIndex3(uint index) {
		data.tileIndex3 = index;
	}
	void SetTileIndex4(uint index) {
		data.tileIndex4 = index;
	}
	struct Struct
	{
		uint tileIndex = -1;
		uint tileIndex2 = -1;
		uint tileIndex3 = -1;
		uint tileIndex4 = -1;
	};

private:
	Struct data;
};

class TileMap
{
public:
	TileMap(uint width, uint height, uint spacing);
	~TileMap();

	void Update();
	void Render();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	
	void GenerateTileMap();
	Tile* GetTile(Vector3 position);
	Tile * GetNextTile(Vector3 position, Vector2 dir);
	Vector3 GetNextTileCenterPos(Vector3 worldPos, Vector2 dir);
	Vector3 GetNextTileCenterPos(Vector3 worldPos, Vector2 dir, OUT bool &bwalkable);
	bool IsOverCenter(Vector3 position, Vector2 dir);
	bool InSlowZone(Vector3 position);
	bool InHomeGate(Vector3 position);
	bool InHome(Vector3 position);
	bool InSpecialZone(Vector3 position);


	void Build();	// 자식 노드 추가해주는 함수
	void FindPath(Vector2 start, Vector2 end);	// 길찾는 함수


	void GUI();

	void SaveTileDatas(wstring filePath = L"");
	void LoadTileDatas(wstring filePath = L"");

	RectEdge* GetMapBoundary() { return &mapBoundary; }

	void GetMousePos();

	bool CheckDuplication(string str);

	Tile** GetTiles() { return this->tiles; }

	uint GetSpacing() { return spacing; }

	void DisplayRedTargetTile(Vector3 position);

	void DisplayPinkTargetTile(Vector3 position);

	void DisplayOrangeTargetTile(Vector3 position);

	void DisplayCyanTargetTile(Vector3 position);

	bool OutOfMap(Vector3 position);

private:
	uint width = 0;
	uint height = 0;
	uint spacing = 0;

	uint vertexCount = 0;
	uint indexCount = 0;

	vector<VertexTile> vertices;
	VertexBuffer* VB = nullptr;

	vector<UINT> indices;
	IndexBuffer* IB = nullptr;

	VertexShader* VS = nullptr;
	PixelShader* PS = nullptr;

	InputLayout* IL = nullptr;

	WorldBuffer* WB = nullptr;
	D3DXMATRIX world, S, R, T;

	D3DXVECTOR3 position;
	D3DXVECTOR3 size;
	float rotation;

	ID3D11ShaderResourceView* srv = nullptr;
	class Tile** tiles = nullptr;

	class TileSet* tileSet = nullptr;

	RectEdge mapBoundary;
	Vector3 worldMpos = Values::ZeroVector;

	vector<string> usedDatas;

	TileBuffer *tb = nullptr;
};