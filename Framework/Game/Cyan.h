#pragma once
#include "Ghost.h"

class Cyan :public Ghost
{
public:
	Cyan(Vector3 position, Vector3 size);
	~Cyan();

	void Update();
	void ChangeAnimClip() override;
	void ChangeMode() override;

	void MoveToHome();

	void MoveInHome();

	void SetTargetPos(class Player* player, class Red* red);

	void MoveToTarget();
private:
	wstring ghostFile = L"";
	bool already = false;

};