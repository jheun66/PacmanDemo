#pragma once

#include "Character.h"

enum PlayerState
{
	IDLE,
	MOVE,
	DEATH
};

class Player : public Character
{
public:
	Player(Vector3 position, Vector3 size);
	~Player();

	void Update();
	void PlayerMove(class TileMap* tileMap);

	void SetDirection(Direction dir) { this->dir = dir; }
	Direction GetDirection() { return dir; }

private:

	int life;			// 여거에 대한 제어는 나중에 , 게임 오버 조건
	

	PlayerState state;

	// 애니메이션 클립들
	AnimationClip* idle = nullptr;
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;
	AnimationClip* die = nullptr;
};