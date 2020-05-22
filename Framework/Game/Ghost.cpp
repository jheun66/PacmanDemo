#include "Framework.h"
#include "Game/Player.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "Ghost.h"

Ghost::Ghost(Vector3 position, Vector3 size)
	: Character(position, size)
{
	eyeFile = TexturePath + L"eyes.json";
	scaredFile = TexturePath + L"scared.json";

	vector<string> animERunDFile = { "d.png" };
	vector<string> animERunLFile = { "l.png" };
	vector<string> animERunUFile = { "u.png" };
	vector<string> animERunRFile = { "r.png" };
	vector<string> animScaredFile = { "scared0.png", "scared1.png" };

	eRunD = new AnimationClip(L"ERunD", eyeFile, animERunDFile);
	eRunL = new AnimationClip(L"ERunL", eyeFile, animERunLFile);
	eRunU = new AnimationClip(L"ERunU", eyeFile, animERunUFile);
	eRunR = new AnimationClip(L"ERunR", eyeFile, animERunRFile);
	scared = new AnimationClip(L"Scared", scaredFile, animScaredFile);

	animator->AddAnimClip(eRunD);
	animator->AddAnimClip(eRunL);
	animator->AddAnimClip(eRunU);
	animator->AddAnimClip(eRunR);
	animator->AddAnimClip(scared);

	mode = SCATTER;

	for (uint l = 0; l < 22; l++)
	{
		if (l == 0)
		{
			scatterTimes[l][0] = 7;
			scatterTimes[l][1] = 7;
			scatterTimes[l][2] = 5;
			scatterTimes[l][3] = 5;
			chaseTimes[l][0] = 20;
			chaseTimes[l][1] = 20;
			chaseTimes[l][2] = 20;
			chaseTimes[l][3] = FLT_MAX;
		}
		else if (l < 5)
		{
			scatterTimes[l][0] = 7;
			scatterTimes[l][1] = 7;
			scatterTimes[l][2] = 5;
			scatterTimes[l][3] = 0.01f;
			chaseTimes[l][0] = 20;
			chaseTimes[l][1] = 20;
			chaseTimes[l][2] = 1033;
			chaseTimes[l][3] = FLT_MAX;
		}
		else
		{
			scatterTimes[l][0] = 5;
			scatterTimes[l][1] = 5;
			scatterTimes[l][2] = 5;
			scatterTimes[l][3] = 0.01f;
			chaseTimes[l][0] = 20;
			chaseTimes[l][1] = 20;
			chaseTimes[l][2] = 1037;
			chaseTimes[l][3] = FLT_MAX;
		}

	}
}

Ghost::~Ghost()
{
	SAFE_DELETE(runD);
	SAFE_DELETE(runL);
	SAFE_DELETE(runU);
	SAFE_DELETE(runR);
	SAFE_DELETE(scared);

	SAFE_DELETE(eRunD);
	SAFE_DELETE(eRunL);
	SAFE_DELETE(eRunU);
	SAFE_DELETE(eRunR);

}

void Ghost::Update()
{
	__super::Update();
}

void Ghost::InitTimerS()
{
	elapsedS = 0;
	setTimerS = false;
}

void Ghost::InitTimerC()
{
	elapsedC = 0;
	setTimerC = false;
}

void Ghost::InitTimerF()
{
	elapsedF = 0;
	setTimerF = false;
}

void Ghost::ChangeDirection()
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
	else if (targetPos != Vector3(-1, -1, -1))
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
}







void Ghost::SetNextTile()
{
	nextTile = tm->GetNextTile(GetPosition(), dirVec[dir]);
}


// TODO : �������ͼ� �ؽ�Ʈ Ÿ�� ����
void Ghost::MoveInTunnel()
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
