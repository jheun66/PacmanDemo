#pragma once
#include "Ghost.h"

class Red : public Ghost
{
public:
	Red(Vector3 position, Vector3 size);
	~Red();

	void Update();
	void ChangeAnimClip() override;
	void ChangeMode() override;
	Vector3 ChangeDirection(Vector3 targetPos, OUT Direction &dir);
	void MoveToTarget(class Player* player);
	void MoveToHome();

	void SetNextTile();
	void MoveInTunnel();

	void SetSpeed(float speed) { animRect->SetSpeed(speed); }
	float GetSpeed() { return animRect->GetSpeed(); }

private:
	wstring ghostFile = L"";

	Vector3 nextTilePos;
	class Tile* currentTile = nullptr;
	Tile* nextTile = nullptr;

	// 죽었다가 목적지에 도달하면 준비
	Vector3 startPositionInHome;
	bool ready = false;
};

