#include "stdafx.h"
#include "TileCharacterDemo.h"

#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Game/Red.h"
#include "Game/Pink.h"
#include "Game/Orange.h"
#include "Game/Cyan.h"

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
	red = new Red(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7 * spacing / 2 , 0), Vector3(spacing*2, spacing*2, 1));
	red->SetTileMap(tm);
	red->SetNextTile();

	pink = new Pink(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + spacing / 2, 0), Vector3(spacing * 2, spacing * 2, 1));
	pink->SetTileMap(tm);
	pink->SetNextTile();

	orange = new Orange(Vector3(WinMaxWidth / 2 + 2*spacing, WinMaxHeight / 2 + spacing / 2, 0), Vector3(spacing * 2, spacing * 2, 1));
	orange->SetTileMap(tm);
	orange->SetNextTile();

	cyan = new Cyan(Vector3(WinMaxWidth / 2 - 2 * spacing, WinMaxHeight / 2 + spacing / 2, 0), Vector3(spacing * 2, spacing * 2, 1));
	cyan->SetTileMap(tm);
	cyan->SetNextTile();

	ghosts.push_back(red);
	ghosts.push_back(pink);
	ghosts.push_back(orange);
	ghosts.push_back(cyan);

	destination = player->GetPosition();

}

void TileCharacterDemo::Destroy()
{
	TileSet::Delete();
	SAFE_DELETE(tm);
	SAFE_DELETE(player);
	SAFE_DELETE(red);
	SAFE_DELETE(pink);
	SAFE_DELETE(orange);
	SAFE_DELETE(cyan);
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
		red->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7 * spacing / 2, 0));
		pink->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + spacing / 2, 0));
		orange->Move(Vector3(WinMaxWidth / 2 + 2*spacing, WinMaxHeight / 2 + spacing / 2, 0));
		cyan->Move(Vector3(WinMaxWidth / 2 - 2 * spacing, WinMaxHeight / 2 + spacing / 2, 0));
		// 기본적인거 세팅한 후 게임 플레이로 변경
		state = GAMEPLAY;
	}

	if (state == GAMEPLAY)
	{
		// 팩맨
		player->PlayerMove(tm);
		player->Update();

		
		Tile* tile = tm->GetTile(player->GetPosition());
		Collision::WithLPellet(player, ghosts, tm);
		Collision::WithSPellet(player, tm);
		for (Ghost* g : ghosts)
		{
			Collision::WithGhost(player, g, tm);
		}
		

		if (Collision::IsWin())
			state = GAMEWIN;
		
		if (player->GetState() == DEATH)
		{
			before = Time::Get()->GetBefore();
			elapsed = 0;
			state = PACMANDEATH;
		}

		// 유령 
		red->SetTargetPos(player);
		red->MoveToTarget();
		pink->SetTargetPos(player);
		pink->MoveToTarget();
		orange->SetTargetPos(player);
		orange->MoveToTarget();
		cyan->SetTargetPos(player, red);
		cyan->MoveToTarget();

		red->Update();
		pink->Update();
		orange->Update();
		cyan->Update();
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
			// 팩맨 위치 초기화
			player->SetState(IDLE);
			player->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 4 + spacing / 2, 0));
			player->PlayerIdle();

			// 유령 위치 초기화
			red->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + 7 * spacing / 2, 0));
			red->SetNextTile();
			// 유령 위치 초기화
			pink->Move(Vector3(WinMaxWidth / 2, WinMaxHeight / 2 + spacing / 2, 0));
			pink->SetNextTile();
			// 유령 위치 초기화
			orange->Move(Vector3(WinMaxWidth / 2+2*spacing, WinMaxHeight / 2 + spacing / 2, 0));
			orange->SetNextTile();

			// 유령 위치 초기화
			cyan->Move(Vector3(WinMaxWidth / 2 - 2 * spacing, WinMaxHeight / 2 + spacing / 2, 0));
			cyan->SetNextTile();


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
		pink->Render();
		orange->Render();
		cyan->Render();
	}
	player->Render();
}

void TileCharacterDemo::GUI()
{
	//tm->GUI();
	//TileSet::Get()->GUI();
}
