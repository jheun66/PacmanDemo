#pragma once

#include "Geometries/AnimationRect.h"
#include "Utilites/Animator.h"

enum Direction
{
	UP = 0,
	LEFT = 1,
	RIGHT = 2,
	DOWN = 3
};

class Character
{
public:
	Character(Vector3 position, Vector3 size);
	virtual ~Character();

	void Update();
	void Render();

	void GUI();	
	
	Vector3 GetPosition() { return animRect->GetPosition(); }
	void Move(Vector3 destination) { animRect->Move(destination); }
	void Move(Vector2 direction) { animRect->Move(direction); }

	Animator* GetAnimator() { return animator; }

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
	unordered_map<string, Texture2D*> textureMap;

	// ���� ����� ��������
	Direction dir;
	Direction exDir;	

	// ���⿡ ���� ����
	Vector2 dirVec[4] =
	{
		Vector2(0,1),
		Vector2(-1,0),
		Vector2(1,0),
		Vector2(0,-1)
	};

};