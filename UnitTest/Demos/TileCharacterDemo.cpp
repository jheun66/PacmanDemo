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
	// ���� ������ Ÿ���� �ȹٲ�, �׳� ���� ��ĥ
	tm->Update();

	if (state == GAMESTART)
	{
		// �Ѹ� ��ġ �ʱ�ȭ
		player->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 4 + spacing / 2, 0));
		
		// ���� ��ġ �ʱ�ȭ

		// �⺻���ΰ� ������ �� ���� �÷��̷� ����
		state = GAMEPLAY;
	}

	if (state == GAMEPLAY)
	{
		// �Ѹ�
		player->PlayerMove(tm);
		player->Update();

		
		// ���� 
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
