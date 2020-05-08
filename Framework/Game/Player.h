#pragma once

#include "Character.h"

class Player : public Character
{
public:
	Player(Vector3 position, Vector3 size);
	~Player();

	void Update();
	void PlayerMove();

private:

};