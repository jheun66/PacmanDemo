#include "Framework.h"
#include "Player.h"

#include "TileMap/TileMap.h"
#include "TileMap/Tile.h"

Player::Player(Vector3 position, Vector3 size)
	: Character(position, size)
{
	life = 3;
	state = IDLE;

	wstring file0 = TexturePath + L"pacman.json";
	wstring file1 = TexturePath + L"death.json";

	vector<string> animIdleFile = { "pacman.png" };
	vector<string> animRunDFile = { "d0.png", "d1.png" };
	vector<string> animRunLFile = { "l0.png", "l1.png" };
	vector<string> animRunUFile = { "u0.png", "u1.png" };
	vector<string> animRunRFile = { "r0.png", "r1.png" };
	vector<string> animDieFile = { "death0.png", "death1.png", "death2.png", "death3.png", 
								"death4.png", "death5.png", "death6.png", "death7.png", 
								"death8.png", "death9.png", "death10.png" };

	idle = new AnimationClip(L"Idle", file0, animIdleFile);
	runD = new AnimationClip(L"RunD", file0, animRunDFile);
	runL = new AnimationClip(L"RunL", file0, animRunLFile);
	runU = new AnimationClip(L"RunU", file0, animRunUFile);
	runR = new AnimationClip(L"RunR", file0, animRunRFile);
	die = new AnimationClip(L"Die", file1, animDieFile, false, true);


	animator->SetMoveDownClip(runD);
	animator->SetMoveUpClip(runU);
	animator->SetMoveRightClip(runR);
	animator->SetMoveLeftClip(runL);
	animator->SetIdleClip(idle);
	animator->SetDieClip(die);


	animRect->SetAnimator(animator);
	animator->PlayIdle();
	//animator->SetCurrentAnimClip(idle->GetClipName());
	
}

Player::~Player()
{
	SAFE_DELETE(idle);
	SAFE_DELETE(runD);
	SAFE_DELETE(runL);
	SAFE_DELETE(runU);
	SAFE_DELETE(runR);
	SAFE_DELETE(die);
}

void Player::Update()
{
	// 애니메이션에 대한 update
	__super::Update();


}

void Player::PlayerMove(TileMap * tileMap)
{
	if (state == IDLE)
	{
		animator->SetCurrentAnimClip(idle->GetClipName());
		// 방향키를 누르면 방향이 정해지면서 상태 변경
		if (KeyDownUp)
		{
			dir = UP;
			exDir = dir;
			state = MOVE;
		}
		else if (KeyDownDown)
		{
			dir = DOWN;
			exDir = dir;
			state = MOVE;
		}
		else if (KeyDownRight)
		{
			dir = RIGHT;
			exDir = dir;
			state = MOVE;
		}
		else if (KeyDownLeft)
		{
			dir = LEFT;
			exDir = dir;
			state = MOVE;
		}
	}

	if (state == MOVE)
	{
		Vector3 pacmanPos = GetPosition();
		
		// 터널에서는 안바뀌게
		if (!tileMap->OutOfMap(pacmanPos))
		{
			// 방향 변경
			if (KeyDownUp)
			{
				dir = UP;
			}
			else if (KeyDownDown)
			{
				dir = DOWN;
			}
			else if (KeyDownRight)
			{
				dir = RIGHT;
			}
			else if (KeyDownLeft)
			{
				dir = LEFT;
			}
		}
	

		Vector3 destination = GetPosition();
		Tile * nextTile = nullptr;
		bool bWalkable; 
		Tile * nextDiagonalTile = tileMap->GetNextTile(pacmanPos, dirVec[exDir] + dirVec[dir]);

		if (dir == exDir)
		{
			nextTile = tileMap->GetNextTile(pacmanPos, dirVec[dir]);
			if (nextTile != nullptr)
			{
				bWalkable = nextTile->GetIsWalkable();
				if (bWalkable)
				{
					animator->SetIsStop(false);
					destination = tileMap->GetNextTileCenterPos(pacmanPos, dirVec[dir]);
					switch (dir)
					{
					case UP:
						animator->PlayMoveUp();
						break;
					case DOWN:
						animator->PlayMoveDown();
						break;
					case LEFT:
						animator->PlayMoveLeft();
						break;
					case RIGHT:
						animator->PlayMoveRight();
						break;
					default:
						break;
					}

				}
				else
				{
					animator->SetIsStop(true);
					destination = tileMap->GetNextTileCenterPos(pacmanPos, Vector2(0, 0));
				}
			}
			else
			{
				// 터널에서
				if (dir == LEFT)
				{
					destination = pacmanPos;
					destination.x -= float(tileMap->GetSpacing());
				}
				else if (dir == RIGHT)
				{
					destination = pacmanPos;
					destination.x += float(tileMap->GetSpacing());
				}
			}

			if (pacmanPos.x > WinMaxWidth + (float(tileMap->GetSpacing()) * 3))
			{
				pacmanPos.x = -(float(tileMap->GetSpacing()) * 3);
				Move(pacmanPos);
			}
			else if (pacmanPos.x < -(float(tileMap->GetSpacing()) * 3))
			{
				pacmanPos.x = WinMaxWidth + (float(tileMap->GetSpacing()) * 3);
				Move(pacmanPos);
			}
			else
			{
				Vector3 newPos = Values::ZeroVector;
				if (!Math::Approximation(pacmanPos, destination, 0.1f))
				{
					D3DXVec3Lerp(&newPos, &pacmanPos, &destination, 10 * Time::Delta());
					Move(newPos);
				}
			}
			
		}
		else if ((3 - dir) == exDir)
		{
			// 방향이 반대일 때
			exDir = dir;
		}
		else
		{
			nextTile = tileMap->GetNextTile(pacmanPos, dirVec[exDir]);

			// 벽에 부딫친 경우
			if (!nextTile->GetIsWalkable())
			{
				exDir = dir;
			}
			else
			{
				nextTile = tileMap->GetNextTile(pacmanPos, dirVec[dir]);
				bool bWalkable = nextTile->GetIsWalkable();

				bool overCenter = tileMap->IsOverCenter(pacmanPos, dirVec[exDir]);
				
				if (bWalkable)
				{
					destination = tileMap->GetNextTileCenterPos(pacmanPos, dirVec[dir]);
					switch (dir)
					{
					case UP:
						animator->PlayMoveUp();
						break;
					case DOWN:
						animator->PlayMoveDown();
						break;
					case LEFT:
						animator->PlayMoveLeft();
						break;
					case RIGHT:
						animator->PlayMoveRight();
						break;
					default:
						break;
					}
					Vector3 newPos = Values::ZeroVector;

					if (!Math::Approximation(pacmanPos, destination, 0.001f))
					{
						D3DXVec3Lerp(&newPos, &pacmanPos, &destination, 10 * Time::Delta());
						Move(newPos);
					}
					else
					{
						exDir = dir;
					}
				}
				else if (nextDiagonalTile->GetIsWalkable() && overCenter)
				{

					destination = tileMap->GetNextTileCenterPos(pacmanPos, dirVec[exDir] + dirVec[dir]);
					switch (dir)
					{
					case UP:
						animator->PlayMoveUp();
						break;
					case DOWN:				
						animator->PlayMoveDown();
						break;
					case LEFT:
						animator->PlayMoveLeft();
						break;
					case RIGHT:
						animator->PlayMoveRight();
						break;
					default:
						break;
					}
					Vector3 newPos = Values::ZeroVector;

					if (!Math::Approximation(pacmanPos, destination, 0.001f))
					{
						D3DXVec3Lerp(&newPos, &pacmanPos, &destination, 10 * Time::Delta());
						Move(newPos);
					}
					else
					{
						exDir = dir;
					}

				}
				else
				{
					dir = exDir;
				}


			}

		}
	}
}

void Player::PlayerIdle()
{
	animator->SetCurrentAnimClip(idle->GetClipName());
}

void Player::PlayerDeath()
{
	animator->SetCurrentAnimClip(die->GetClipName());
}
