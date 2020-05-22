#include "Framework.h"
#include "Game/Player.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "Red.h"

Red::Red(Vector3 position, Vector3 size)
	:Ghost(position, size)
{
	// 랜덤 방향을 위해
	srand((unsigned int)time(NULL));

	ghostFile = TexturePath + L"red.json";

	vector<string> animRunDFile = { "d0.png", "d1.png" };
	vector<string> animRunLFile = { "l0.png", "l1.png" };
	vector<string> animRunUFile = { "u0.png", "u1.png" };
	vector<string> animRunRFile = { "r0.png", "r1.png" };

	
	runD = new AnimationClip(L"RunD", ghostFile, animRunDFile);
	runL = new AnimationClip(L"RunL", ghostFile, animRunLFile);
	runU = new AnimationClip(L"RunU", ghostFile, animRunUFile);
	runR = new AnimationClip(L"RunR", ghostFile, animRunRFile);

	
	animator->AddAnimClip(runD);
	animator->AddAnimClip(runL);
	animator->AddAnimClip(runU);
	animator->AddAnimClip(runR);


	animRect->SetAnimator(animator);
	animator->SetCurrentAnimClip(runL->GetClipName());


	dir = LEFT;
	startPositionInHome = Vector3(WinMaxWidth / 2, 296, 0);
}

Red::~Red()
{
	// ghost에서 delete

}

void Red::Update()
{	
	ChangeAnimClip();
	ChangeMode();
	__super::Update();
}

void Red::ChangeAnimClip()
{
	
	if (mode == SCATTER || mode == CHASE)// 정상
	{
		switch (dir)
		{
		case UP:
			animator->SetCurrentAnimClip(runU->GetClipName());
			break;
		case DOWN:
			animator->SetCurrentAnimClip(runD->GetClipName());
			break;
		case LEFT:
			animator->SetCurrentAnimClip(runL->GetClipName());
			break;
		case RIGHT:
			animator->SetCurrentAnimClip(runR->GetClipName());
			break;
		default:
			animator->SetCurrentAnimClip(runD->GetClipName());
			break;
		}
	}
	else if (mode == EATEN)// 죽었을 때
	{
		switch (dir)
		{
		case UP:
			animator->SetCurrentAnimClip(eRunU->GetClipName());
			break;
		case DOWN:
			animator->SetCurrentAnimClip(eRunD->GetClipName());
			break;
		case LEFT:
			animator->SetCurrentAnimClip(eRunL->GetClipName());
			break;
		case RIGHT:
			animator->SetCurrentAnimClip(eRunR->GetClipName());
			break;
		default:
			animator->SetCurrentAnimClip(eRunD->GetClipName());
			break;
		}
	}
	else// 겁먹은 상태
	{
		animator->SetCurrentAnimClip(scared->GetClipName());
	}


}

void Red::ChangeMode()
{
	if (mode == SCATTER)
	{
		exMode = SCATTER;

		// 타겟 타일 설정

		if (!setTimerS)
		{
			before = Time::Get()->GetBefore();
			setTimerS = true;
		}

		if (Time::Get()->Timer(before, scatterTimes[level-1][wave-1], false, elapsedS))
		{
			mode = CHASE;
			InitTimerS();
		}

	}
	else if (mode == CHASE)
	{
		exMode = CHASE;

		// 타겟 타일 설정

		if (!setTimerC)
		{
			before = Time::Get()->GetBefore();
			setTimerC = true;
		}

		if (Time::Get()->Timer(before, chaseTimes[level - 1][wave - 1], false, elapsedC))
		{
			mode = SCATTER;
			wave++;
			InitTimerC();
		}
	}
	else if (mode == FRIGHTEN)
	{
		if (setTimerS)
		{
			Time::Get()->Timer(before, scatterTimes[level - 1][wave - 1], true, elapsedS);
			InitTimerS();
		}
		else if (setTimerC)
		{
			Time::Get()->Timer(before, chaseTimes[level - 1][wave - 1], true, elapsedC);
			InitTimerC();
		}

		if (!setTimerF)
		{
			before = Time::Get()->GetBefore();
			setTimerF = true;
		}

		float f = 5;
		if (Time::Get()->Timer(before, f, false, elapsedF))
		{
			mode = exMode;
			InitTimerF();
		}
	}
	else
	{
		InitTimerF();

		if (ready)
		{
			mode = exMode;
			dir = UP;
			ready = false;
		}
		
	}

}

Vector3 Red::ChangeDirection(Vector3 targetPos, OUT Direction &dir)
{
	Vector3 tilePos[4];
	bool bWalkable[4];
	float distance = FLT_MAX;
	int possiblePath = 0;
	tilePos[UP] = tm->GetNextTileCenterPos(GetPosition(), dirVec[UP], bWalkable[UP]);
	tilePos[DOWN] = tm->GetNextTileCenterPos(GetPosition(), dirVec[DOWN], bWalkable[DOWN]);
	tilePos[LEFT] = tm->GetNextTileCenterPos(GetPosition(), dirVec[LEFT], bWalkable[LEFT]);
	tilePos[RIGHT] = tm->GetNextTileCenterPos(GetPosition(), dirVec[RIGHT], bWalkable[RIGHT]);

	if (tm->InSpecialZone(GetPosition()))
	{
		bWalkable[UP] = false;
	}


	// 임시 방향
	Direction tempDir = dir;

	// next는 경로가 하나일 때를 위한 임시 방향
	Direction NextDir;
	
	// 가능한 방향이 반대방향 제외하고 1개면 그방향으로 (1자나 모퉁이에서)
	for (int i = 0; i < 4; i++)
	{
		if (bWalkable[i] == true && i != (3 - tempDir))
		{
			possiblePath++;

			// next는 경로가 하나일 때를 위한 임시 방향
			NextDir = (Direction)i;
		}
	}

	if (possiblePath < 2)
	{
		dir = NextDir;
	}
	else if( targetPos != Vector3(-1,-1,-1) )
	{
		// 반대방향, 불가능한 방향 제외 => 반대방향 제외하고 거리 짧을 쪽 ( 교차로 에서 )
		for (int i = 0; i < 4; i++)
		{
			if (bWalkable[i] == true && i != (3 - tempDir))
			{
				// 거리 계산
				float temp = D3DXVec3Length(&(tilePos[i] - targetPos));
				if (temp < distance)
				{
					distance = temp;
					dir = (Direction)i;
				}
			}
		}
	}
	else
	{
		// 교차로에서 가능한 랜덤 방향
		while (1)
		{
			tempDir = (Direction)(rand() % 4);
			if (bWalkable[tempDir])
				break;
		}
		dir = tempDir;
	}

	nextTile = tm->GetTile(tilePos[dir]);

	return tilePos[dir];

}


void Red::MoveToTarget(Player* player)
{
	Vector3 targetPos;
	
	if (mode == SCATTER)
	{

		SetSpeed(100);
		// 터널 입구 + 터널
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(60);
			// 터널에서 
			MoveInTunnel();
		}
		else if (tm->InHome(GetPosition()))
		{
			// 터널에서 
			MoveInHome();
		}
		else
		{
			Tile** tiles = tm->GetTiles();
			targetPos = tiles[35][25].GetPosition();
			targetPos.x += tm->GetSpacing() * 0.5f;
			targetPos.y += tm->GetSpacing() * 0.5f;

			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}


			Move(dirVec[dir]);

		}

		
	}
	else if (mode == CHASE)
	{
		SetSpeed(100);
		// 터널 입구 + 터널
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(60);
			// 터널에서 
			MoveInTunnel();
		}
		else if (tm->InHome(GetPosition()))
		{
			// 터널에서 
			MoveInHome();
		}
		else
		{
			
			targetPos = player->GetPosition();

			currentTile = tm->GetTile(GetPosition());


			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Move(dirVec[dir]);

		}
		
	}
	else if (mode == FRIGHTEN)
	{
		SetSpeed(60);
		// 터널 입구 + 터널
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(60);
			// 터널에서 
			MoveInTunnel();
		}
		else if (tm->InHome(GetPosition()))
		{
			// 터널에서 
			MoveInHome();
		}
		else
		{
			targetPos = Vector3(-1, -1, -1);
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Move(dirVec[dir]);
		}
	}
	else
	{
		SetSpeed(100);
		if (tm->InHomeGate(GetPosition()) || tm->InHome(GetPosition()))
		{
			MoveToHome();
		}
		else if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(60);
			MoveInTunnel();
		}
		else
		{
			targetPos = startPositionInHome;
			//targetPos.y += 48;

			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Move(dirVec[dir]);

		}

	}
	
	tm->DisplayRedTargetTile(targetPos);
	
}

void Red::MoveToHome()
{
	if (dir == LEFT)
	{
		if (GetPosition().x > startPositionInHome.x)
		{
			Move(dirVec[dir]);
		}
		else
		{
			Direction tmp = DOWN;
			Move(dirVec[tmp]);
			if (GetPosition().y < startPositionInHome.y)
			{
				Move(startPositionInHome);
				ready = true;
			}
		}
	}
	else if (dir == RIGHT)
	{
		if (GetPosition().x < startPositionInHome.x)
		{
			Move(dirVec[dir]);
		}
		else
		{
			Direction tmp = DOWN;
			Move(dirVec[tmp]);
			if (GetPosition().y < startPositionInHome.y)
			{
				Move(startPositionInHome);
				ready = true;
			}
		}
	}
	nextTile = tm->GetNextTile(GetPosition(), dirVec[UP]);
}

void Red::MoveInHome()
{
	dir = UP;
	nextTile = tm->GetNextTile(GetPosition(), dirVec[UP]);
	Move(dirVec[dir]);
}

void Red::SetNextTile()
{
	nextTile = tm->GetNextTile(GetPosition(), dirVec[dir]);
}


// TODO : 빠져나와서 넥스트 타일 수정
void Red::MoveInTunnel()
{
	Vector3 pos = GetPosition();
	
	if (pos.x > WinMaxWidth + (float(tm->GetSpacing()) * 3))
	{
		pos.x = -(float(tm->GetSpacing()) * 3);
		Move(pos);
	}
	else if (pos.x < -(float(tm->GetSpacing()) * 3))
	{
		pos.x = WinMaxWidth + (float(tm->GetSpacing()) * 3);
		Move(pos);
	}
	Move(dirVec[dir]);

	nextTile = tm->GetNextTile(pos, dirVec[dir]);
}
