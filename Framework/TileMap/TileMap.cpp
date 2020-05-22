#include "Framework.h"
#include "TileMap.h"
#include "TileSet.h"
#include "Tile.h"
#include "Utilites/FileStream.h"

TileMap::TileMap(uint width, uint height, uint spacing)
	: width(width), height(height), spacing(spacing)
{
	uint mapSizeX = width * spacing;
	uint mapSizeY = height * spacing;
	mapBoundary.LT = Vector3(0, mapSizeY, 0);
	mapBoundary.RB = Vector3(mapSizeX, 0, 0);

	tileSet = TileSet::Get();
	PathFinder::Create();
	//srv = tileSet->tileSRV;

	GenerateTileMap();

	vertices.assign(4, VertexTile());

	vertices[0].position = Vector3(0, 0, 0);
	vertices[1].position = Vector3(0, (float)spacing, 0);
	vertices[2].position = Vector3((float)spacing, 0, 0);
	vertices[3].position = Vector3((float)spacing, (float)spacing, 0);

	vertices[0].uv2 = Vector2(0, 1);
	vertices[1].uv2 = Vector2(0, 0);
	vertices[2].uv2 = Vector2(1, 1);
	vertices[3].uv2 = Vector2(1, 0);

	indices = { 0,1,2,2,1,3 };
	
	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(ShaderPath + L"VertexTile.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(ShaderPath + L"VertexTile.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexTile::descs, VertexTile::count, VS->GetBlob());

	WB = new WorldBuffer();

	world = Values::Identity;

	tb = new TileBuffer();

}

TileMap::~TileMap()
{
	SAFE_DELETE(tb);
	PathFinder::Delete();

	for (uint y = 0; y < height; y++)
	{
		SAFE_DELETE_ARRAY(tiles[y]);
	}
	SAFE_DELETE_ARRAY(tiles);

	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(VS);
	SAFE_DELETE(PS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);
}

void TileMap::Update()
{
	//GetMousePos();
	//if (Mouse::Get()->Down(0))
	//{
	//	Tile* tile = GetTile(worldMpos);
	//	if (tile)
	//	{

	//		//tile->color = Values::Red;
	//		tile->SetStartUV(tileSet->SelectedStartUV);
	//		tile->SetEndUV(tile->GetStartUV() + tileSet->SelectedtexelTileSize);
	//		tile->SetSpriteName(tileSet->selectedSpriteName);
	//		if(tileSet->selectedSpriteName == "largepellet" || tileSet->selectedSpriteName == "smallpellet")
	//			tile->SetIsWalkable(true);
	//		else
	//			tile->SetIsWalkable(false);

	//		if (CheckDuplication(tileSet->selectedSpriteName) == false)
	//			usedDatas.push_back(tileSet->selectedSpriteName);
	//	}
	//}

	// 길찾기
	/*
	if (KeyDown(VK_SPACE))
	{
		Build();
		FindPath(Vector2(0, 0), Vector2(0, 0));
	}*/

	// 타일 지우기
	/*if (Mouse::Get()->Down(1))
	{
		Vector3 mPos = Mouse::Get()->GetPosition();
		Tile* tile = GetTile(mPos);
		if (tile)
		{
			tile->SetColor(Values::Black);
			tile->SetStartUV(Vector2(0, 0));
			tile->SetEndUV(Vector2(0, 0));

			tile->SetSpriteName("");
			tile->SetIsWalkable(true);
		}
	}
*/

}

void TileMap::Render()
{
	VB->SetIA();
	IB->SetIA();
	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	PS->SetShader();
	if (srv)
		DC->PSSetShaderResources(0, 1, &srv);
	tb->SetPSBuffer(0);

	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			Tile& tile = tiles[y][x];
			
			// 먹힌 팔렛이면 검은색으로
			if (tile.IsEaten())
			{
				tile.SetColor(Values::Black);
				tile.SetStartUV(Vector2(0, 0));
				tile.SetEndUV(Vector2(0, 0));

				tile.SetSpriteName("");
				tile.SetIsWalkable(true);
			}
			
			{
				D3D11_MAPPED_SUBRESOURCE subResource;
				DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

				for (VertexTile& v : vertices)
				{
					v.color = tile.GetColor();
					v.index = tile.GetIndex();
				}

				vertices[0].uv = Vector2(tile.GetStartUV().x, tile.GetEndUV().y);
				vertices[1].uv = tile.GetStartUV();
				vertices[2].uv = tile.GetEndUV();
				vertices[3].uv = Vector2(tile.GetEndUV().x, tile.GetStartUV().y);


				memcpy(subResource.pData, vertices.data(), sizeof(VertexTile) * VB->GetCount());
				DC->Unmap(VB->GetResource(), 0);
			}

			D3DXMatrixTranslation(&world, tile.GetPosition().x, tile.GetPosition().y, tile.GetPosition().z);
			WB->SetWorld(world);
			WB->SetVSBuffer(0);

			if (tileSet->spriteMap.size() > 0)
			{
				if (tileSet->spriteMap.find(tile.GetSpriteName()) != tileSet->spriteMap.end())
				{
					srv = tileSet->spriteMap.find(tile.GetSpriteName())->second.sprite->GetSRV();
					DC->PSSetShaderResources(0, 1, &srv);
				}
			}

			DC->DrawIndexed(IB->GetCount(), 0, 0);

			
		}
	}

}



void TileMap::GenerateTileMap()
{
	if (width == 0 || height == 0 || spacing == 0)
	{
		assert(false);
		return;
	}

	uint index = 0;
	tiles = new Tile*[height];
	for (uint y = 0; y < height; y++)
	{
		tiles[y] = new Tile[width];
		for (uint x = 0; x < width; x++)
		{
			tiles[y][x].SetPosition(Vector3((float)(x*spacing), (float)(y*spacing), 0.0f));
			tiles[y][x].SetColor(Values::Black);
			tiles[y][x].SetIndex(index++);
		}
	}
}

Tile * TileMap::GetTile(Vector3 position)
{
	uint x = (int)position.x / spacing;
	uint y = (int)position.y / spacing;

	if (x > (int)width - 1 || x < 0 || y >(int)height - 1 || y < 0) {
		return nullptr;
	}

	return &tiles[y][x];
}

Tile * TileMap::GetNextTile(Vector3 position, Vector2 dir)
{
	int x = (int)(position.x / spacing) + dir.x;
	int y = (int)(position.y / spacing) + dir.y;

	if (x > (int)width - 1 || x < 0 || y >(int)height - 1 || y < 0) {
		return nullptr;
	}

	return &tiles[y][x];
}

Vector3 TileMap::GetNextTileCenterPos(Vector3 worldPos, Vector2 dir)
{
	int x = (int)(worldPos.x / spacing) + dir.x;
	int y = (int)(worldPos.y / spacing) + dir.y;

	if (x > (int)width - 1 || x < 0 || y >(int)height - 1 || y < 0) {
		// 고려 안해도 됨
	}

	Vector3 vec = tiles[y][x].GetPosition();

	vec.x += spacing * 0.5f;
	vec.y += spacing * 0.5f;

	return vec;
}


Vector3 TileMap::GetNextTileCenterPos(Vector3 worldPos, Vector2 dir, OUT bool & bwalkable)
{
	int x = (int)(worldPos.x / spacing) + dir.x;
	int y = (int)(worldPos.y / spacing) + dir.y;

	if (x > (int)width - 1 || x < 0 || y > (int)height - 1 || y < 0) {
		// 고려 안해도 됨
	}

	bwalkable = tiles[y][x].GetIsWalkable();

	Vector3 vec = tiles[y][x].GetPosition();

	vec.x += spacing * 0.5f;
	vec.y += spacing * 0.5f;

	return vec;
}

bool TileMap::IsOverCenter(Vector3 position, Vector2 dir)
{
	int x = (int)(position.x / spacing);
	int y = (int)(position.y / spacing);

	Vector3 vec = tiles[y][x].GetPosition();

	vec.x += spacing * 0.5f;
	vec.y += spacing * 0.5f;

	if (dir == Vector2(0, 1))
	{
		if (position.y >= vec.y)
			return true;
	}
	else if (dir == Vector2(0, -1))
	{
		if (position.y <= vec.y)
			return true;
	}
	else if (dir == Vector2(1, 0))
	{
		if (position.x >= vec.x)
			return true;
	}
	else if (dir == Vector2(-1, 0))
	{
		if (position.x <= vec.x)
			return true;
	}

	return false;

}

bool TileMap::InSlowZone(Vector3 position)
{
	int x = (int)(position.x / spacing);
	int y = (int)(position.y / spacing);

	if (y == 18)
	{
		if (x >= 0 && x < 5)
			return true;

		if (x < (int)width && x >= (int)width - 5)
			return true;

	}
	return false;
}

bool TileMap::InHomeGate(Vector3 position)
{
	int x = (int)(position.x / spacing);
	int y = (int)(position.y / spacing);

	if (y == 20 || y == 21)
	{
		if (x > 12 && x <= 14)
			return true;
	}
	return false;
}

bool TileMap::InHome(Vector3 position)
{
	int x = (int)(position.x / spacing);
	int y = (int)(position.y / spacing);

	if (y == 17 || y == 18 || y == 19)
	{
		if (x > 10 && x <= 16)
			return true;
	}
	return false;
}

bool TileMap::InSpecialZone(Vector3 position)
{
	int x = (int)(position.x / spacing);
	int y = (int)(position.y / spacing);

	if (y == 9)
	{
		if (x <= 17 && x >= 10)
			return true;

	}

	if( y == 21)
	{
		if (x <= 17 && x >= 10)
			return true;
	}
	return false;
}

void TileMap::Build()
{
	for (uint y = 0; y < height; y++)
		for (uint x = 0; x < width; x++)
			tiles[y][x].Release();


	int newX = 0, newY = 0;		// 오프셋
	Tile* child = nullptr;		// 자식노드를 위한 임시변수
	Tile* current = nullptr;	// 현재노드를 위한 임시변수
	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			current = &tiles[y][x];
			for (int i = -1; i < 2; i++)
			{
				newY = y + i;
				for (int j = -1; j < 2; j++)
				{
					newX = x + j;
					if (newY > -1 && newY < (int)height && newX > -1 && newX < (int)width)
					{
						child = &tiles[newY][newX];
						if (child->GetIsWalkable() == true && current != child && (newX == x || newY == y))		// 상하좌우, 자기 자신 빼고
						{
							tiles[y][x].AddChild(make_pair(child, (float)spacing));
						
						}
					}//if(newY..)
				}//for(j)
			}//for(i)
		}//for(x)
	}//for(y)
}

void TileMap::FindPath(Vector2 start, Vector2 end)
{
	vector<Tile*> path;

	bool result = PathFinder::Get()->GetPath(&tiles[0][0], &tiles[height - 1][width - 1], path);

	if (result)
	{
		for (Tile* node : path)
			node->SetColor(Values::Green);
	}
}

void TileMap::GUI()
{
	if (ImGui::Button("Save", ImVec2(100, 40)))
		SaveTileDatas();
	if (ImGui::Button("Load", ImVec2(100, 40)))
		LoadTileDatas();
}

// call back 함수
void TileMap::SaveTileDatas(wstring filePath)
{
	if (filePath.length() < 1)
	{
		function<void(wstring)> func = bind(&TileMap::SaveTileDatas, this, placeholders::_1);
		Path::SaveFileDialog(L"", Path::TileMapDataFilter, L".\\", func, handle);
	}
	else
	{
		FileStream* write = new FileStream(String::ToString(filePath), FILE_STREAM_WRITE);

		write->Write(usedDatas.size());
		for (string str : usedDatas)
			write->WriteString(str);

		for (uint y = 0; y < height; y++)
			for (uint x = 0; x < width; x++)
				write->Write(&tiles[y][x]);
			
		SAFE_DELETE(write);
	}
}

void TileMap::LoadTileDatas(wstring filePath)
{
	if (filePath.length() < 1)
	{
		function<void(wstring)> func = bind(&TileMap::LoadTileDatas, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::TileMapDataFilter, L".\\", func, handle);
	}
	else
	{
		FileStream* read = new FileStream(String::ToString(filePath), FILE_STREAM_READ);

		uint size = read->Read<uint>();
		usedDatas.clear();
		for (int i = 0; i < size; i++) {
			usedDatas.push_back(read->ReadString());
			wstring tempPath = TileMapDataPath + String::ToWstring(usedDatas.back()) + L".sif";
			tileSet->OpenSpriteInfos(tempPath);
		}

		for (uint y = 0; y < height; y++)
		{
			for (uint x = 0; x < width; x++)
			{
				tiles[y][x] = read->ReadTile();
			}
		}

		SAFE_DELETE(read);
	}

}

void TileMap::GetMousePos()
{
	Vector3 mPos = Mouse::Get()->GetPosition();
	Camera::Get()->UnProjection(&worldMpos, mPos, Values::Identity);
}

bool TileMap::CheckDuplication(string str)
{
	vector<string>::iterator iter = usedDatas.begin();
	iter = std::find(usedDatas.begin(), usedDatas.end(), str);

	return iter != usedDatas.end();
	// 찾으면 true 못찾으면 false
}

void TileMap::DisplayRedTargetTile(Vector3 position)
{
	if (position == Vector3(-1, -1, -1) || OutOfMap(position))
	{
		tb->SetTileIndex(-1);
	}
	else
	{
		tb->SetTileIndex(GetTile(position)->GetIndex());
	}
}

void TileMap::DisplayPinkTargetTile(Vector3 position)
{
	if (position == Vector3(-1, -1, -1) || OutOfMap(position))
	{
		tb->SetTileIndex2(-1);
	}
	else
	{
		tb->SetTileIndex2(GetTile(position)->GetIndex());
	}
}

void TileMap::DisplayOrangeTargetTile(Vector3 position)
{
	if (position == Vector3(-1, -1, -1) || OutOfMap(position))
	{
		tb->SetTileIndex3(-1);
	}
	else
	{
		tb->SetTileIndex3(GetTile(position)->GetIndex());
	}
}

void TileMap::DisplayCyanTargetTile(Vector3 position)
{
	if (position == Vector3(-1, -1, -1) || OutOfMap(position))
	{
		tb->SetTileIndex4(-1);
	}
	else
	{
		tb->SetTileIndex4(GetTile(position)->GetIndex());
	}
}

bool TileMap::OutOfMap(Vector3 position)
{
	if (position.x <= 0 || position.x >= WinMaxWidth
		|| position.y <= 0 || position.y >= WinMaxHeight)
		return true;
	else
		return false;
}
