#include "Framework.h"
#include "Game/Player.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "Red.h"

Red::Red(Vector3 position, Vector3 size)
	:Ghost(position, size)
{
	// ���� ������ ����
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
	// ghost���� delete

}

void Red::Update()
{	
	ChangeAnimClip();
	ChangeMode();
	__super::Update();
}

void Red::ChangeAnimClip()
{
	
	if (mode == SCATTER || mode == CHASE)// ����
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
	else if (mode == EATEN)// �׾��� ��
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
	else// �̸��� ����
	{
		animator->SetCurrentAnimClip(scared->GetClipName());
	}


}

void Red::ChangeMode()
{
	if (mode == SCATTER)
	{
		exMode = SCATTER;

		// Ÿ�� Ÿ�� ����

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

		// Ÿ�� Ÿ�� ����

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


	// �ӽ� ����
	Direction tempDir = dir;

	// next�� ��ΰ� �ϳ��� ���� ���� �ӽ� ����
	Direction NextDir;
	
	// ������ ������ �ݴ���� �����ϰ� 1���� �׹������� (1�ڳ� �����̿���)
	for (int i = 0; i < 4; i++)
	{
		if (bWalkable[i] == true && i != (3 - tempDir))
		{
			possiblePath++;

			// next�� ��ΰ� �ϳ��� ���� ���� �ӽ� ����
			NextDir = (Direction)i;
		}
	}

	if (possiblePath < 2)
	{
		dir = NextDir;
	}
	else if( targetPos != Vector3(-1,-1,-1) )
	{
		// �ݴ����, �Ұ����� ���� ���� => �ݴ���� �����ϰ� �Ÿ� ª�� �� ( ������ ���� )
		for (int i = 0; i < 4; i++)
		{
			if (bWalkable[i] == true && i != (3 - tempDir))
			{
				// �Ÿ� ���
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
		// �����ο��� ������ ���� ����
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
		// �ͳ� �Ա� + �ͳ�
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(5);
			// �ͳο��� 
			MoveInTunnel();
		}
		else
		{
			SetSpeed(10);
			Tile** tiles = tm->GetTiles();
			targetPos = tiles[35][25].GetPosition();
			targetPos.x += tm->GetSpacing() * 0.5f;
			targetPos.y += tm->GetSpacing() * 0.5f;


			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile)
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				SetNextTile();
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Vector3 newPos = Values::ZeroVector;
			if (!Math::Approximation(GetPosition(), nextTilePos, 0.001f))
			{
				D3DXVec3Lerp(&newPos, &GetPosition(), &nextTilePos, GetSpeed() * Time::Delta());
				Move(newPos);
			}

		}

		
	}
	else if (mode == CHASE)
	{
		
		// �ͳ� �Ա� + �ͳ�
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(5);
			// �ͳο��� 
			MoveInTunnel();
		}
		else
		{
			SetSpeed(10);

			targetPos = player->GetPosition();

			currentTile = tm->GetTile(GetPosition());


			if (currentTile == nextTile)
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				SetNextTile();
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Vector3 newPos = Values::ZeroVector;
			if (!Math::Approximation(GetPosition(), nextTilePos, 0.001f))
			{
				D3DXVec3Lerp(&newPos, &GetPosition(), &nextTilePos, GetSpeed() * Time::Delta());
				Move(newPos);
			}

		}
		
	}
	else if (mode == FRIGHTEN)
	{
		SetSpeed(8);
		// �ͳ� �Ա� + �ͳ�
		if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(5);
			// �ͳο��� 
			MoveInTunnel();
		}
		else
		{
			targetPos = Vector3(-1, -1, -1);
			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile)
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				SetNextTile();
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Vector3 newPos = Values::ZeroVector;
			if (!Math::Approximation(GetPosition(), nextTilePos, 0.001f))
			{
				D3DXVec3Lerp(&newPos, &GetPosition(), &nextTilePos, GetSpeed() * Time::Delta());
				Move(newPos);
			}
		}
	}
	else
	{

		if (tm->InHomeGate(GetPosition()) || tm->InHome(GetPosition()))
		{
			MoveToHome();
		}
		else if (tm->InSlowZone(GetPosition()) || tm->OutOfMap(GetPosition()))
		{
			SetSpeed(5);
			MoveInTunnel();
		}
		else
		{
			targetPos = startPositionInHome;
			targetPos.y += 48;

			currentTile = tm->GetTile(GetPosition());

			if (currentTile == nextTile)
			{
				nextTilePos = ChangeDirection(targetPos, dir);
			}
			else
			{
				SetNextTile();
				nextTilePos = tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]);
			}

			Vector3 newPos = Values::ZeroVector;
			if (!Math::Approximation(GetPosition(), nextTilePos, 0.001f))
			{
				D3DXVec3Lerp(&newPos, &GetPosition(), &nextTilePos, GetSpeed() * Time::Delta());
				Move(newPos);
			}

		}

	}
	
	tm->DisplayRedTargetTile(targetPos);
	
}

void Red::MoveToHome()
{
	Vector3 targetPos = startPositionInHome;
	targetPos.y += 48;
	Vector3 newPos = Values::ZeroVector;
	if (!Math::Approximation(GetPosition(), startPositionInHome, 0.001f))
	{
		D3DXVec3Lerp(&newPos, &GetPosition(), &startPositionInHome, GetSpeed() * Time::Delta());
		Move(newPos);
	}
	else
	{
		ready = true;
	}
}

void Red::SetNextTile()
{
	nextTile = tm->GetTile(tm->GetNextTileCenterPos(GetPosition(), dirVec[dir]));
}

void Red::MoveInTunnel()
{
	Vector3 pos = GetPosition();
	if (dir == LEFT)
	{
		nextTilePos = pos;
		nextTilePos.x -= float(tm->GetSpacing());
	}
	else if (dir == RIGHT)
	{
		nextTilePos = pos;
		nextTilePos.x += float(tm->GetSpacing());
	}

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
	else
	{
		Vector3 newPos = Values::ZeroVector;
		if (!Math::Approximation(pos, nextTilePos, 0.1f))
		{
			D3DXVec3Lerp(&newPos, &pos, &nextTilePos, GetSpeed() * Time::Delta());
			Move(newPos);
		}
	}

}
