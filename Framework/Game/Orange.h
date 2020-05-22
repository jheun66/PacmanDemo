#pragma once
#include "Ghost.h"

class Orange :public Ghost
{
public:
	Orange(Vector3 position, Vector3 size);
	~Orange();

	void Update();
	void ChangeAnimClip() override;
	void ChangeMode() override;

	void MoveToHome();

	void MoveInHome();

	void SetTargetPos(class Player* player);

	void MoveToTarget();
private:
	wstring ghostFile = L"";
	bool isNearPac = false;
	bool already = false;
};