#pragma once

#include "Character.h"

enum Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player : public Character
{
public:
	Player(Vector3 position, Vector3 size);
	~Player();

	void Update();
	void PlayerMove();

	void SetDirection(Direction dir) { this->dir = dir; }
	Direction GetDirection() { return dir; }

private:

	int life = 1;			// 여거에 대한 제어는 나중에 , 게임 오버 조건
	bool isdead = false;	// 요건 일시적인 죽음
	bool isPower = false;	// 큰 팰럿을 먹었는지 여부
	Direction dir = NONE;	// 0 : 시작상태 , 1 : 위, 2 : 아래, 3 : 왼쪽, 4 : 오른쪽

	// 애니메이션 클립들
	AnimationClip* idle = nullptr;
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;
	AnimationClip* die = nullptr;
};