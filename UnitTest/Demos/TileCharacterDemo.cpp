#include "stdafx.h"
#include "TileCharacterDemo.h"

#include "Game/Player.h"
#include "Game/Red.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"



void TileCharacterDemo::Init()
{
	TileSet::Create();

	spacing = 16;
	width = WinMaxWidth / spacing;
	height = WinMaxHeight / spacing;

	state = GAMESTART;
	
	tm = new TileMap(width, height, spacing);
	tm->LoadTileDatas(TileMapDataPath + L"map8.tilemap");
	Tile** tiles = tm->GetTiles();
	player = new Player(Vector3(WinMaxWidth / 2 , WinMaxHeight / 4 + spacing/2 , 0), Vector3(spacing*2, spacing*2, 1));
	red = new Red(tiles[height / 4][width / 4].GetPosition(), Vector3(spacing*2, spacing*2, 1));

	destination = player->GetPosition();

}

void TileCharacterDemo::Destroy()
{
	TileSet::Delete();
	SAFE_DELETE(tm);
	SAFE_DELETE(player);
	SAFE_DELETE(red);
}

void TileCharacterDemo::Update()
{
	// 게임 내에서 타일은 안바꿈, 그냥 위에 덧칠
	tm->Update();

	if (state == GAMESTART)
	{
		// 팩맨 위치 초기화
		player->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 4 + spacing / 2, 0));
		
		// 유령 위치 초기화

		// 기본적인거 세팅한 후 게임 플레이로 변경
		state = GAMEPLAY;
	}

	if (state == GAMEPLAY)
	{
		// 팩맨
		player->PlayerMove(tm);
		player->Update();

		
		// 유령 
		red->Update();
	}


}

void TileCharacterDemo::Render()
{
	tm->Render();
	player->Render();
	//red->Render();
}

void TileCharacterDemo::GUI()
{

}
