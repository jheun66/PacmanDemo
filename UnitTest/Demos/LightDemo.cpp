#include "stdafx.h"
#include "LightDemo.h"

#include "Utilites/RenderTexture.h"
#include "Geometries/LightMap.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"
#include "Geometries/AnimationRect.h"

void LightDemo::Init()
{
	TileSet::Create();

	tm = new TileMap(width, height, spacing);
	ar = new AnimationRect(Vector3(WinMaxWidth / 2.0f, WinMaxHeight / 2.0f, 0), Vector3(spacing, spacing, 1));

	rt = new RenderTexture();
	rt->Init();

	lm = new LightMap(Vector3(0, 0, 0), Vector3(WinMaxWidth, WinMaxHeight, 1));

	Camera::Get()->SetTileMapBoundary(tm->GetMapBoundary());
}

void LightDemo::Destroy()
{
	SAFE_DELETE(lm);
	SAFE_DELETE(rt);
	SAFE_DELETE(tm);
	SAFE_DELETE(ar);
	TileSet::Delete();
}

void LightDemo::Update()
{
	Camera::Get()->SetPosition(ar->GetCenterPosition());

	tm->Update();

	ar->Update();
	ar->Move();


	lm->SetLightPos(ar->GetCenterPositionInSS());
	lm->Update();
}

void LightDemo::PreRender()
{
	Graphics::Get()->InitViewport();
	rt->RenderToTexture();
	{
		tm->Render();
		ar->Render();
	}
}

void LightDemo::Render()
{
	Camera::Get()->SetPositionToOrigin();
	lm->SetSRV(rt->GetSRV());
	lm->Render();

}

void LightDemo::GUI()
{
	TileSet::Get()->GUI();
}
