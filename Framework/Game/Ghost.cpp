#include "Framework.h"
#include "Ghost.h"

Ghost::Ghost(Vector3 position, Vector3 size)
	: Character(position, size)
{
	eyeFile = TexturePath + L"eyes.json";
	scaredFile = TexturePath + L"scared.json";

	vector<string> animERunDFile = { "d.png" };
	vector<string> animERunLFile = { "l.png" };
	vector<string> animERunUFile = { "u.png" };
	vector<string> animERunRFile = { "r.png" };
	vector<string> animScaredFile = { "scared0.png", "scared1.png" };

	eRunD = new AnimationClip(L"ERunD", eyeFile, animERunDFile);
	eRunL = new AnimationClip(L"ERunL", eyeFile, animERunLFile);
	eRunU = new AnimationClip(L"ERunU", eyeFile, animERunUFile);
	eRunR = new AnimationClip(L"ERunR", eyeFile, animERunRFile);
	scared = new AnimationClip(L"Scared", scaredFile, animScaredFile);

	animator->AddAnimClip(eRunD);
	animator->AddAnimClip(eRunL);
	animator->AddAnimClip(eRunU);
	animator->AddAnimClip(eRunR);
	animator->AddAnimClip(scared);

	dir = DOWN;

}

Ghost::~Ghost()
{
	SAFE_DELETE(runD);
	SAFE_DELETE(runL);
	SAFE_DELETE(runU);
	SAFE_DELETE(runR);
	SAFE_DELETE(scared);

	SAFE_DELETE(eRunD);
	SAFE_DELETE(eRunL);
	SAFE_DELETE(eRunU);
	SAFE_DELETE(eRunR);

}

void Ghost::Update()
{
	__super::Update();
}
