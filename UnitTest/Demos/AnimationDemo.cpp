#include "stdafx.h"
#include "AnimationDemo.h"
#include "Geometries/AnimationRect.h"
#include "Geometries/TextureRect.h"

void AnimationDemo::Init()
{
	ar = new AnimationRect(D3DXVECTOR3(300, 300, 0), D3DXVECTOR3(100, 100, 1));
	bkImage = new Texture2D(TexturePath + L"grass.jpg");
	bkRect = new TextureRect(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(WinMaxWidth*4, WinMaxHeight*4, 1),0);
	bkRect->SetSRV(bkImage->GetSRV());

}

void AnimationDemo::Destroy()
{
	SAFE_DELETE(bkImage);
	SAFE_DELETE(bkRect);
	SAFE_DELETE(ar);
}

void AnimationDemo::Update()
{
	Camera::Get()->SetPosition(ar->GetPosition());
	ar->Update();
	ar->Move();
}

void AnimationDemo::Render()
{
	bkRect->Render();
	ar->Render();
}

void AnimationDemo::GUI()
{
}
