#include "Framework.h"
#include "Player.h"

Player::Player(Vector3 position, Vector3 size)
	: Character(position, size)
{
	Texture2D* atahoTex = new Texture2D(TexturePath + L"ataho.png");
	textureMap.insert(make_pair("Ataho", atahoTex));

	Vector2 textureSize = Vector2((float)atahoTex->GetWidth(), (float)atahoTex->GetHeight());

	AnimationClip* runD = new AnimationClip(L"RunD", atahoTex, 5, Vector2(0,0), textureSize * 0.5f);
	AnimationClip* runL = new AnimationClip(L"RunL", atahoTex, 5, Vector2(textureSize.x * 0.5f, 0), Vector2(textureSize.x, textureSize.y*0.5f));
	AnimationClip* runU = new AnimationClip(L"RunU", atahoTex, 5, Vector2(0, textureSize.y * 0.5f), Vector2(textureSize.x*0.5f, textureSize.y));
	AnimationClip* runR = new AnimationClip(L"RunR", atahoTex, 5, textureSize * 0.5f, textureSize);

	animator->SetMoveDownClip(runD);
	animator->SetMoveUpClip(runU);
	animator->SetMoveRightClip(runR);
	animator->SetMoveLeftClip(runL);

	animRect->SetAnimator(animator);
	animator->SetCurrentAnimClip(runD->GetClipName());


}

Player::~Player()
{
}

void Player::Update()
{
	__super::Update();
	PlayerMove();
	//animRect->Move();
}

void Player::PlayerMove()
{
	if (KeyPressUp)
		animator->PlayMoveUp();
	else if (KeyPressDown)
		animator->PlayMoveDown();
	else if (KeyPressLeft)
		animator->PlayMoveLeft();
	else if (KeyPressRight)
		animator->PlayMoveRight();
}
