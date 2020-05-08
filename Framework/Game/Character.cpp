#include "Framework.h"
#include "Character.h"

Character::Character(Vector3 position, Vector3 size)
{
	animRect = new AnimationRect(position, size);
	animator = new Animator();
}

Character::~Character()
{
	for (auto tex : textureMap)
		SAFE_DELETE(tex.second);

	SAFE_DELETE(animRect);
	SAFE_DELETE(animator);
}

void Character::Update()
{
	animator->Update();
	animRect->Update();
}

void Character::Render()
{
	animRect->Render();
}

void Character::GUI()
{
	animRect->GUI();
}
