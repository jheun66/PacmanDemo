#include "Framework.h"
#include "Player.h"

Player::Player(Vector3 position, Vector3 size)
	: Character(position, size)
{
	//Texture2D* atahoTex = new Texture2D(TexturePath + L"ataho.png");
	//textureMap.insert(make_pair("Ataho", atahoTex));

	//Vector2 textureSize = Vector2((float)atahoTex->GetWidth(), (float)atahoTex->GetHeight());

	//AnimationClip* runD = new AnimationClip(L"RunD", atahoTex, 5, Vector2(0,0), textureSize * 0.5f);
	//AnimationClip* runL = new AnimationClip(L"RunL", atahoTex, 5, Vector2(textureSize.x * 0.5f, 0), Vector2(textureSize.x, textureSize.y*0.5f));
	//AnimationClip* runU = new AnimationClip(L"RunU", atahoTex, 5, Vector2(0, textureSize.y * 0.5f), Vector2(textureSize.x*0.5f, textureSize.y));
	//AnimationClip* runR = new AnimationClip(L"RunR", atahoTex, 5, textureSize * 0.5f, textureSize);
	//
	//animator->SetMoveDownClip(runD);
	//animator->SetMoveUpClip(runU);
	//animator->SetMoveRightClip(runR);
	//animator->SetMoveLeftClip(runL);



	//animRect->SetAnimator(animator);
	//animator->SetCurrentAnimClip(runD->GetClipName());

	
	
	wstring file0 = TexturePath + L"pacman.json";
	wstring file1 = TexturePath + L"death.json";

	vector<string> animIdleFile = { "pacman.png" };
	vector<string> animRunDFile = { "d0.png", "d1.png" };
	vector<string> animRunLFile = { "l0.png", "l1.png" };
	vector<string> animRunUFile = { "u0.png", "u1.png" };
	vector<string> animRunRFile = { "r0.png", "r1.png" };
	vector<string> animDieFile = { "death0.png", "death1.png", "death2.png", "death3.png", 
								"death4.png", "death5.png", "death6.png", "death7.png", 
								"death8.png", "death9.png", "death10.png" };

	idle = new AnimationClip(L"Idle", file0, animIdleFile);
	runD = new AnimationClip(L"RunD", file0, animRunDFile);
	runL = new AnimationClip(L"RunL", file0, animRunLFile);
	runU = new AnimationClip(L"RunU", file0, animRunUFile);
	runR = new AnimationClip(L"RunR", file0, animRunRFile);
	die = new AnimationClip(L"Die", file1, animDieFile);


	animator->SetMoveDownClip(runD);
	animator->SetMoveUpClip(runU);
	animator->SetMoveRightClip(runR);
	animator->SetMoveLeftClip(runL);
	animator->SetIdleClip(idle);
	animator->SetDieClip(die);


	animRect->SetAnimator(animator);
	animator->SetCurrentAnimClip(idle->GetClipName());
	
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
