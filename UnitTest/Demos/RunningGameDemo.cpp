#include "stdafx.h"
#include "RunningGameDemo.h"

#include "Geometries/RunningRect.h"
#include "Geometries/BlurredTexture.h"
#include "Utilites/RenderTexture.h"

void RunningGameDemo::Init()
{
	//srand((unsigned int)time(NULL));

	player = new RunningRect(D3DXVECTOR3(100, 0, 0), D3DXVECTOR3(100, 100, 1));

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 size = D3DXVECTOR3(100, 100, 1);
	for (UINT i = 0; i < maxObjCount; i++)
	{
		//size.y = rand() % 251 + 50;
		position.x = WinMaxWidth + spacing * i;
		objs.push_back(new RunningRect(position, size));

	}

	rt = new RenderTexture();
	rt->Init();
	bt = new BlurredTexture(Vector3(0, 0, 0), Vector3(WinMaxWidth, WinMaxHeight, 1));
}

void RunningGameDemo::Destroy()
{
	SAFE_DELETE(rt);
	SAFE_DELETE(bt);

	SAFE_DELETE(player);
	for (RunningRect* obj : objs)
		SAFE_DELETE(obj);
}

void RunningGameDemo::Update()
{
	for (RunningRect* obj : objs)
	{
		obj->AutoMove();
		if (obj->CheckOut(-100))
		{
			float newPosX = objs.back()->GetPositionX() + spacing;
			obj->Reposition(newPosX);
			rotate(objs.begin(), objs.begin() + 1, objs.end());
		}
	}
}

void RunningGameDemo::PreRender()
{
	Graphics::Get()->InitViewport();
	rt->RenderToTexture();
	{
		for (RunningRect* obj : objs)
			obj->Render();
		player->Render();
	}
}

void RunningGameDemo::Render()
{
	bt->SetSRV(rt->GetSRV());
	bt->Render();
}

void RunningGameDemo::GUI()
{
	player->Update();

	if (Keyboard::Get()->Down(VK_SPACE))
		player->Jump();

	ImGui::Begin("RunningGame");
	{
		ImGui::SliderFloat("Speed", &RunningRect::speed, 0, 1000, "%.f");
	}
	ImGui::End();

	for (RunningRect* obj : objs)
		obj->GUI();

	bt->GUI();
}
