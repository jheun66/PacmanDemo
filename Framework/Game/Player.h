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

	int life;			// ���ſ� ���� ����� ���߿� , ���� ���� ����
	

	PlayerState state;

	// �ִϸ��̼� Ŭ����
	AnimationClip* idle = nullptr;
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;
	AnimationClip* die = nullptr;
};