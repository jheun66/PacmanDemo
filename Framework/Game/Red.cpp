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
	startPositionInHome = Vector3(WinMaxWidth / 2, 288, 0);
}

Red::~Red()
{
	// ghost에서 delete

}

void Red::Update()
{	
	ChangeAnimClip();
	ChangeMode();
	tm->DisplayRedTargetTile(targetPos);

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




void Red::SetTargetPos(Player* player)
{
	if (mode == SCATTER)
	{
		Tile** tiles = tm->GetTiles();
		targetPos = tiles[35][25].GetPosition();
		targetPos.x += tm->GetSpacing() * 0.5f;
		targetPos.y += tm->GetSpacing() * 0.5f;
	}
	else if (mode == CHASE)
	{
		targetPos = player->GetPosition();
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

void Red::MoveToTarget()
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
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile && tm->IsOverCenter(GetPosition(), dirVec[dir]))
			{
				ChangeDirection();
			}

			Move(dirVec[dir]);

		}

	}

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

