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
	class Red* red = nullptr;

	Vector3 destination = Values::ZeroVector;
	bool bWalkable = true;

	GameState state;

	uint spacing;
	uint width;
	uint height;
};

/*
���� ����
linear interpolation
D3DXVecLerp
*/