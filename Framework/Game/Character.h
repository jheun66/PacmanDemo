#pragma once

#include "Geometries/AnimationRect.h"
#include "Utilites/Animator.h"

class Character
{
public:
	Character(Vector3 position, Vector3 size);
	virtual ~Character();

	void Update();
	void Render();

	void GUI();	
	Vector3 GetCenterPosition() { return animRect->GetCenterPosition(); }
	Vector3 GetPosition() { return animRect->GetPosition(); }
	void Move(Vector3 destination) { animRect->Move(destination); }

	Animator* GetAnimator() { return animator; }

protected:
	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
	unordered_map<string, Texture2D*> textureMap;

};