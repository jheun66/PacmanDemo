#pragma once

#include "Rect.h"
class RunningRect : public Rect
{
public:
	RunningRect(D3DXVECTOR3 position, D3DXVECTOR3 size);
	~RunningRect();

	void Update();

	void GUI() override;
	void AutoMove();
	bool CheckOut(float outPos);
	void Reposition(float xPos);
	float GetPositionX() { return position.x; }

	void Jump();

	static float speed;

private:
	static UINT objCount;
	UINT index = 0;

	string label = "";

	bool bJump = false;
	float vy = 0.0f;
	float jumpAccel = -600.0f;
	float jumpSpeed = 600.0f;
	float groundY = 0.0f;
};