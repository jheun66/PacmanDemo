#include "stdafx.h"
#include "RTTDemo.h"

#include "Utilites/RenderTexture.h"
#include "Geometries/TextureRect.h"

void RTTDemo::Init()
{
	rt = new RenderTexture();
	rt->Init();

	//rtt = new TextureRect(D3DXVECTOR3(WinMaxWidth - (400 * WinMaxWidth / WinMaxHeight), WinMaxHeight - 400, 1), D3DXVECTOR3(400 * WinMaxWidth / WinMaxHeight, 400, 1), 0);
	rtt = new TextureRect(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(WinMaxWidth, WinMaxHeight, 1), 0);

	rtt->SetShader(ShaderPath + L"VertexRTT.hlsl");

	tr1 = new TextureRect(D3DXVECTOR3(100, 100, 1), D3DXVECTOR3(100, 100, 1), 0);
	tr1->SetSRV(TexturePath + L"butterfly.jpg");
	tr2 = new TextureRect(D3DXVECTOR3(400, 400, 1), D3DXVECTOR3(200, 200, 1), 0);
	tr2->SetSRV(TexturePath + L"grass.jpg");
}

void RTTDemo::Destroy()
{
	SAFE_DELETE(rt);
	SAFE_DELETE(rtt);
	SAFE_DELETE(tr1);
	SAFE_DELETE(tr2);

}

void RTTDemo::Update()
{
	tr1->Move();

	if (Keyboard::Get()->Down(VK_SPACE))
		rt->SaveTexture(L"./test.png");
}

void RTTDemo::PreRender()
{
	Graphics::Get()->InitViewport();
	rt->RenderToTexture();
	{
		tr2->Render();
		tr1->Render();
	}
}

void RTTDemo::Render()
{
	
	tr2->Render();
	tr1->Render();

	rtt->SetSRV(rt->GetSRV());
	rtt->Render();
}

void RTTDemo::GUI()
{
}
