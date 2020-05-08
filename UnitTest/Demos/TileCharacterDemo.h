#pragma once

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

	void PlayerMove();

private:

	class TileMap* tm = nullptr;
	class Player* player = nullptr;

	Vector3 destination = Values::ZeroVector;
	bool bWalkable = true;
};

/*
선형 보간
linear interpolation
D3DXVecLerp
*/