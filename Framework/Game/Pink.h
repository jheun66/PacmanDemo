#pragma once
#include "Ghost.h"

class Pink : public Ghost
{
public:
	Pink(Vector3 position, Vector3 size);
	~Pink();

	void Update();
	void ChangeAnimClip() override;
	void ChangeMode() override;

	void MoveToHome();
	void MoveInHome();

	void SetTargetPos(class Player* player);

	void MoveToTarget();

private:
	wstring ghostFile = L"";

};