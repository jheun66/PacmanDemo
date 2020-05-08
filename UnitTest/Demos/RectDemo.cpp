#include "stdafx.h"
#include "RectDemo.h"

// rect 헤더 추가
#include "Geometries/Rect.h"

void RectDemo::Init()
{
	r1 = new Rect(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(100, 100, 1), 45);

	r2 = new Rect(D3DXVECTOR3(300, 300, 0), D3DXVECTOR3(200, 100, 1), 0);
}

void RectDemo::Destroy()
{
	SAFE_DELETE(r1);
	SAFE_DELETE(r2);
}

void RectDemo::Update()
{
	r1->Update();
	r1->Move();

	r1->CheckIntersect(r2);

	r2->Update();

}

void RectDemo::Render()
{
	r1->Render();
	r2->Render();
}

void RectDemo::GUI()
{
	r2->GUI();

	//static bool bOpen = true;
	//ImGui::ShowDemoWindow(&bOpen);
}
