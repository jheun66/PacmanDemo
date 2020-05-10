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

	int life = 1;			// ���ſ� ���� ����� ���߿� , ���� ���� ����
	bool isdead = false;	// ��� �Ͻ����� ����
	bool isPower = false;	// ū �ӷ��� �Ծ����� ����
	Direction dir = NONE;	// 0 : ���ۻ��� , 1 : ��, 2 : �Ʒ�, 3 : ����, 4 : ������

	// �ִϸ��̼� Ŭ����
	AnimationClip* idle = nullptr;
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;
	AnimationClip* die = nullptr;
};