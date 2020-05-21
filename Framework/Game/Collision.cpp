#include "Framework.h"
#include "TileMap/Tile.h"
#include "TileMap/TileMap.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Game/Red.h"

#include "Collision.h"

uint Collision::pelletCount = 0;

void Collision::WithSPellet(Player* player, TileMap* tm)
{
	Vector3 pacmanPos = player->GetPosition();
	if (tm->OutOfMap(pacmanPos))
	{
		return;
	}
	else
	{
		Tile* tile = tm->GetTile(pacmanPos);
		if (tile->GetSpriteName() == "smallpellet" && !tile->IsEaten())
		{
			tile->EatPallet();

			// ������ ������
			pelletCount++;
		}

	}
	
}

void Collision::WithLPellet(Player* player, vector<Ghost*> ghosts, TileMap* tm)
{
	Vector3 pacmanPos = player->GetPosition();
	if (tm->OutOfMap(pacmanPos))
	{
		return;
	}
	else
	{
		Tile* tile = tm->GetTile(pacmanPos);
		if (tile->GetSpriteName() == "largepellet" && !tile->IsEaten())
		{
			tile->EatPallet();

			// ������ ������ + ���� ����
			pelletCount++;

			for (Ghost* ghost : ghosts)
			{
				if (ghost->GetMode() == SCATTER || ghost->GetMode() == CHASE)
					ghost->SetMode(FRIGHTEN);
			}

		}
	}
	
}

void Collision::WithGhost(Player * player, Ghost * ghost, TileMap* tm)
{
	Vector3 pos = player->GetPosition();
	Vector3 ghostPos = ghost->GetPosition();

	// �� �ȿ����� �浹 üũ
	if (!tm->OutOfMap(pos) && !tm->OutOfMap(ghostPos))
	{
		// �浹 ���� Ȯ��
		if (tm->GetTile(pos) == tm->GetTile(ghostPos))
		{
			GhostMode mode = ghost->GetMode();
			if (mode == SCATTER || mode == CHASE)
			{
				ghost->SetDirection(LEFT);
				
				//((Red*)ghost)->SetNextTile(tm);
				// setNextTile�� Ghost�� �����ص� ������ �׳� ��ĳ����
				Red* tmp = dynamic_cast<Red*>(ghost);
				if (tmp != nullptr)
					tmp->SetNextTile();

				player->SetState(DEATH);
			}
			else if (mode == FRIGHTEN)
			{
				ghost->SetMode(EATEN);
				
				
			}
		}
	}


}


bool Collision::IsWin()
{
	if (pelletCount == 244)
	{
		// �ʱ�ȭ
		pelletCount = 0;
		return true;
	}
	else
		return false;
}
