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
	bool isDead = false;	// 일시적인 죽음, 상자안으로 돌아감.
	bool isScared = false;	// 팩맨이 큰 팰럿을 먹었는지 여부

	wstring ghostFile = L"";
	wstring eyeFile = L"";
	wstring scaredFile = L"";

	Direction dir;	// 0 : 시작상태 , 1 : 위, 2 : 아래, 3 : 왼쪽, 4 : 오른쪽

	// 애니메이션 클립들
	AnimationClip* runD = nullptr;
	AnimationClip* runL = nullptr;
	AnimationClip* runU = nullptr;
	AnimationClip* runR = nullptr;

	AnimationClip* scared = nullptr;

	// 죽고 난후
	AnimationClip* eRunD = nullptr;
	AnimationClip* eRunL = nullptr;
	AnimationClip* eRunU = nullptr;
	AnimationClip* eRunR = nullptr;


};