#include "stdafx.h"
#include "TileMapDemo.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"

void TileMapDemo::Init()
{
	TileSet::Create();

	uint spacing = 40;
	uint width = WinMaxWidth / spacing;
	uint height = WinMaxHeight / spacing;
	tm = new TileMap(width, height, spacing);
	bkImage = new Texture2D(TexturePath + L"myMap.png");
	//tm->SetSRV(bkImage->GetSRV());
}

void TileMapDemo::Destroy()
{
	TileSet::Delete();
	SAFE_DELETE(tm);
}

void TileMapDemo::Update()
{
	tm->Update();
}

void TileMapDemo::Render()
{
	tm->Render();
}

void TileMapDemo::GUI()
{
	TileSet::Get()->GUI();
	tm->GUI();
}
