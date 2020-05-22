#pragma once

#include "Character.h"

enum GhostMode
{
	SCATTER,
	CHASE,
	FRIGHTEN,
	EATEN
};

class Ghost : public Character
{
public:
	Ghost(Vector3 position, Vector3 size);
	virtual ~Ghost();

	void Update();
	virtual void ChangeAnimClip() = 0;
	virtual void ChangeMode() = 0;

	void SetDirection(Direction dir) { this->dir = dir; }
	Direction GetDirection() { return dir; }

	GhostMode GetMode() { return mode; }
	void SetMode(GhostMode mode) { this->mode = mode; }
	GhostMode GetExMode() { return exMode; }
	void SetExMode(GhostMode exMode) { this->exMode = exMode; }

	void InitTimerS();
	void InitTimerC();
	void InitTimerF();

	void SetLevel(uint level) { this->level = level; }
	uint GetLevel() { return level; }

	void SetWave(uint wave) { this->wave = wave; }
	uint GetWave() { return wave; }

	void SetTileMap(class TileMap* tm) { this->tm = tm; }
	

	void ChangeDirection();
	void MoveToTarget();


	void MoveInTunnel();

	void SetNextTile();

	void SetSpeed(float speed) { animRect->SetSpeed(speed); }
	float GetSpeed() { return animRect->GetSpeed(); }

protected:

	
	wstring eyeFile = L"";
	wstring scaredFile = L"";

	Direction dir;	// 0 : ���ۻ��� , 1 : ��, 2 : �Ʒ�, 3 : ����, 4 : ������

	GhostMode mode;
	GhostMode exMode;

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


	// Ÿ�̸� ����
	INT64 before;

	float elapsedS = 0;
	float elapsedC = 0;
	float elapsedF = 0;

	bool setTimerS = false;
	bool setTimerC = false;
	bool setTimerF = false;

	// level ����
	uint level = 1;
	uint wave = 1;

	float scatterTimes[21][4];
	float chaseTimes[21][4];

	class TileMap* tm = nullptr;

	class Tile* currentTile = nullptr;
	Tile* nextTile = nullptr;

	Vector3 targetPos;

	// �׾��ٰ� �������� �����ϸ� �غ�
	Vector3 startPositionInHome;
	bool ready = false;

};