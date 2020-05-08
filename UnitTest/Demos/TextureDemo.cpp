#include "stdafx.h"

#include "TextureDemo.h"

#include "Geometries/TextureRect.h"
#include "Geometries/BlurredTexture.h"

void TextureDemo::Init()
{
	tr1 = new TextureRect({ WinMaxWidth / 2, WinMaxHeight / 2, 0 }, { 200, 200, 1 }, 0);
	tr2 = new BlurredTexture({ WinMaxWidth / 4, WinMaxHeight / 4, 0 }, { 200, 200, 1 });

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.Width = 512;
	loadInfo.Height = 512;
	texture = new Texture2D(TexturePath + L"face.png", &loadInfo);
	tr1->SetSRV(texture->GetSRV());
	tr2->SetSRV(texture->GetSRV());

	vector<D3DXCOLOR> pixels;
	D3D11_TEXTURE2D_DESC desc = texture->ReadPixel(texture->GetFormat(), &pixels);

	//texture->SaveTexture(L"./saved.png", desc, pixels);
	texture->SaveTexture(L"./saved.png");
}

void TextureDemo::Destroy()
{
	SAFE_DELETE(tr1);
	SAFE_DELETE(tr2);
}

void TextureDemo::Update()
{
	tr1->Update();
	tr1->Move();

	tr2->Update();
	tr2->Move();
}

void TextureDemo::Render()
{
	tr1->Render();

	tr2->Render();
}

void TextureDemo::GUI()
{
	tr1->GUI();
}

