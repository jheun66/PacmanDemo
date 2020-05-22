#include "Framework.h"
#include "Game/Player.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "Orange.h"

Orange::Orange(Vector3 position, Vector3 size)
	:Ghost(position, size)
{
	// 랜덤 방향을 위해
	srand((unsigned int)time(NULL));

	ghostFile = TexturePath + L"orange.json";

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


	dir = DOWN;
	startPositionInHome = Vector3(WinMaxWidth / 2, 288, 0);
}

Orange::~Orange()
{
	// ghost에서 delete

}

void Orange::Update()
{
	ChangeAnimClip();
	ChangeMode();
	tm->DisplayOrangeTargetTile(targetPos);
	__super::Update();
}

void Orange::ChangeAnimClip()
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

void Orange::ChangeMode()
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

		if (Time::Get()->Timer(before, scatterTimes[level - 1][wave - 1], false, elapsedS))
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

		if (isNearPac)
		{
			mode = SCATTER;
			InitTimerC();
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




void Orange::SetTargetPos(Player* player)
{
	if (mode == SCATTER)
	{
		Tile** tiles = tm->GetTiles();
		targetPos = tiles[0][0].GetPosition();
		targetPos.x += tm->GetSpacing() * 0.5f;
		targetPos.y += tm->GetSpacing() * 0.5f;
	}
	else if (mode == CHASE)
	{
		targetPos = player->GetPosition();
		
		if (D3DXVec3Length(&(targetPos - GetPosition())) < 8 * tm->GetSpacing())
		{
			isNearPac = true;
		}
		else
		{
			isNearPac = false;
		}
	}
	else if (mode == FRIGHTEN)
	{
		targetPos = Vector3(-1, -1, -1);
	}
	else
	{
		targetPos = startPositionInHome;
	}
}

void Orange::MoveToTarget()
{

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
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				ChangeDirection();
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
			MoveInTunnel();
		}
		else if (tm->InHome(GetPosition()))
		{
			MoveInHome();
		}
		else
		{
			currentTile = tm->GetTile(GetPosition());


			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				ChangeDirection();
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
			MoveInTunnel();
		}
		else if (tm->InHome(GetPosition()))
		{
			MoveInHome();
		}
		else
		{
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				ChangeDirection();
			}

			Move(dirVec[dir]);
		}
	}
	else
	{
		SetSpeed(100);
		if (tm->InHomeGate(GetPosition())|| tm->InHome(GetPosition()))
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
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				ChangeDirection();
			}

			Move(dirVec[dir]);

		}

	}

}

void Orange::MoveToHome()
{
	if (already)
	{
		Vector3 tmp = startPositionInHome;
		tmp.x += 16 * 2;

		if (GetPosition().x < tmp.x)
		{
			Move(dirVec[RIGHT]);
			
		}
		else
		{
			ready = true;
			already = false;
		}
	}
	else
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
					already = true;
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
					already = true;
				}
			}
		}
		nextTile = tm->GetNextTile(GetPosition(), dirVec[UP]);
	}

}



void Orange::MoveInHome()
{
	if (Math::Approximation(GetPosition().x, WinMaxWidth / 2, 3.0f))
	{
		dir = UP;
		nextTile = tm->GetNextTile(GetPosition(), dirVec[UP]);
		Move(dirVec[dir]);
	}
	else
	{
		dir = LEFT;
		Move(dirVec[dir]);
	}
}

