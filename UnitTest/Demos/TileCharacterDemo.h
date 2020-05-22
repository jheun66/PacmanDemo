#pragma once

enum GameState {
	MENU,
	GAMESTART,
	GAMEPLAY,
	GAMEWIN,
	PACMANDEATH,
	GAMEOVER
};

class TileCharacterDemo	: public IObject
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void GUI() override;


private:


	class TileMap* tm = nullptr;
	class Player* player = nullptr;

	vector<class Ghost*> ghosts;
	class Red* red = nullptr;
	class Pink* pink = nullptr;
	class Orange* orange = nullptr;
	class Cyan* cyan = nullptr;

	Vector3 destination = Values::ZeroVector;
	bool bWalkable = true;

	GameState state;

	uint spacing;
	uint width;
	uint height;

	INT64 before; // 죽었을 때의 타이머
	float elapsed = 0;
};

/*
선형 보간
linear interpolation
D3DXVecLerp
*/