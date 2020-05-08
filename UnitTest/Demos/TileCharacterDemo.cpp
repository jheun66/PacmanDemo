#include "stdafx.h"
#include "TileCharacterDemo.h"

#include "Game/Player.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"

void TileCharacterDemo::Init()
{
	TileSet::Create();

	uint spacing = 40;
	uint width = WinMaxWidth / spacing;
	uint height = WinMaxHeight / spacing;
	tm = new TileMap(width, height, spacing);

	player = new Player(Vector3(WinMaxWidth*0.5f, WinMaxHeight*0.5f, 0), Vector3(spacing, spacing, 1));
	destination = player->GetPosition();

}

void TileCharacterDemo::Destroy()
{
	TileSet::Delete();
	SAFE_DELETE(tm);
	SAFE_DELETE(player);
}

void TileCharacterDemo::Update()
{
	tm->Update();
	player->Update();
	PlayerMove();
}

void TileCharacterDemo::Render()
{
	tm->Render();
	player->Render();
}

void TileCharacterDemo::GUI()
{
	TileSet::Get()->GUI();
	tm->GUI();
	player->GUI();
}

void TileCharacterDemo::PlayerMove()
{
	if (KeyPressUp)
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(0, 1), bWalkable);
	else if(KeyPressDown)
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(0, -1), bWalkable);
	else if (KeyPressRight)
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(1, 0), bWalkable);
	else if (KeyPressLeft)
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(-1, 0), bWalkable);

	static Vector3 newPos = Values::ZeroVector;
	if (!Math::Approximation(player->GetPosition(), destination, 0.001f) && bWalkable) 
	{
		D3DXVec3Lerp(&newPos, &player->GetPosition(), &destination, 10 * Time::Delta());
		player->Move(newPos);
	}
	else if (!Math::Approximation(player->GetPosition(), destination, 40+0.001f) && !bWalkable)
	{
		D3DXVec3Lerp(&newPos, &player->GetPosition(), &destination, 10 * Time::Delta());
		player->Move(newPos);
	}

}
