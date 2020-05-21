#include "stdafx.h"
#include "TileCharacterDemo.h"

#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Game/Red.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"

#include "Game/Collision.h"


void TileCharacterDemo::Init()
{
	TileSet::Create();

	spacing = 16;
	width = WinMaxWidth / spacing;
	height = WinMaxHeight / spacing;

	state = GAMESTART;
	
	tm = new TileMap(width, height, spacing);
	tm->LoadTileDatas(TileMapDataPath + L"map10.tilemap");

	Tile** tiles = tm->GetTiles();

	player = new Player(Vector3(WinMaxWidth / 2 , WinMaxHeight / 4 + spacing/2 , 0), Vector3(spacing*2, spacing*2, 1));
	red = new Red(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7*spacing/2 , 0), Vector3(spacing*2, spacing*2, 1));
	red->SetTileMap(tm);
	red->SetNextTile();

	ghosts.push_back(red);

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
		red->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7 * spacing / 2, 0));

		// �⺻���ΰ� ������ �� ���� �÷��̷� ����
		state = GAMEPLAY;
	}

	if (state == GAMEPLAY)
	{
		// �Ѹ�
		player->PlayerMove(tm);
		player->Update();

		
		Tile* tile = tm->GetTile(player->GetPosition());
		Collision::WithLPellet(player, ghosts, tm);
		Collision::WithSPellet(player, tm);
		Collision::WithGhost(player, red, tm);

		if (Collision::IsWin())
			state = GAMEWIN;
		
		if (player->GetState() == DEATH)
		{
			before = Time::Get()->GetBefore();
			elapsed = 0;
			state = PACMANDEATH;
		}

		// ���� 
		red->MoveToTarget(player);
		red->Update();
	}
	if (state == GAMEWIN)
	{
		player->SetState(IDLE);
		player->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 4 + spacing / 2, 0));
		player->PlayerIdle();
		player->Update();
		tm->LoadTileDatas(TileMapDataPath + L"map10.tilemap");
	}

	if (state == PACMANDEATH)
	{
		float f = 2;
		if (Time::Get()->Timer(before, f, false, elapsed))
		{
			// �Ѹ� ��ġ �ʱ�ȭ
			player->SetState(IDLE);
			player->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 4 + spacing / 2, 0));
			player->PlayerIdle();

			// ���� ��ġ �ʱ�ȭ
			red->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7 * spacing / 2, 0));
			state = GAMEPLAY;

		}
		
		player->PlayerDeath();
		player->Update();
		
	}
}

void TileCharacterDemo::Render()
{
	tm->Render();
	if(state == GAMESTART || state == GAMEPLAY)
	{
		red->Render();
	}
	player->Render();
}

void TileCharacterDemo::GUI()
{
	//tm->GUI();
	//TileSet::Get()->GUI();
}
