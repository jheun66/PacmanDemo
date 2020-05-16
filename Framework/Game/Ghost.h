#pragma once

#include "Character.h"

class Ghost : public Character
{
public:
	Ghost(Vector3 position, Vector3 size);
	virtual ~Ghost();

	void Update();
	virtual void ChangeAnimClip() = 0;

	void SetDirection(Direction dir) { this->dir = dir; }
	Direction GetDirection() { return dir; }

protected:
	bool isDead = false;	// �Ͻ����� ����, ���ھ����� ���ư�.
	bool isScared = false;	// �Ѹ��� ū �ӷ��� �Ծ����� ����

	wstring ghostFile = L"";
	wstring eyeFile = L"";
	wstring scaredFile = L"";

	Direction dir;	// 0 : ���ۻ��� , 1 : ��, 2 : �Ʒ�, 3 : ����, 4 : ������

	// �ִϸ��̼� Ŭ����
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;

	AnimationClip* scared = nullptr;

	// �װ� ����
	AnimationClip* eRunD = nullptr;
	AnimationClip* eRunL = nullptr;
	AnimationClip* eRunU = nullptr;
	AnimationClip* eRunR = nullptr;


};