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
	if (KeyDownUp)
		player->SetDirection(UP);
	else if (KeyDownDown)
		player->SetDirection(DOWN);
	else if (KeyDownRight)
		player->SetDirection(RIGHT);
	else if (KeyDownLeft)
		player->SetDirection(LEFT);

	switch (player->GetDirection())
	{
	case NONE:
		break;
	case UP:
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(0, 1), bWalkable);
		break;
	case DOWN:
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(0, -1), bWalkable);
		break;
	case LEFT:
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(-1, 0), bWalkable);
		break;
	case RIGHT:
		destination = tm->GetNextTilePos(player->GetCenterPosition(), Vector2(1, 0), bWalkable);
		break;
	default:
		break;
	}
		

	static Vector3 newPos = Values::ZeroVector;
	if (!Math::Approximation(player->GetPosition(), destination, 0.001f) && bWalkable) 
	{
		player->GetAnimator()->SetIsStop(false);
		D3DXVec3Lerp(&newPos, &player->GetPosition(), &destination, 10 * Time::Delta());
		player->Move(newPos);
	}
	else if (!Math::Approximation(player->GetPosition(), destination, 40 + 0.001f) && !bWalkable)
	{
		player->GetAnimator()->SetIsStop(false);
		D3DXVec3Lerp(&newPos, &player->GetPosition(), &destination, 10 * Time::Delta());
		player->Move(newPos);
	}
	else
	{
		// 애니메이션 멈추기
		player->GetAnimator()->SetIsStop(true);
	}

}
