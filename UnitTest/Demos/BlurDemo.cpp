#include "stdafx.h"
#include "BlurDemo.h"

#include "Geometries/BlurredTexture.h"
#include "Geometries/AnimationRect.h"
#include "Utilites/RenderTexture.h"

void BlurDemo::Init()
{
	bt = new BlurredTexture(Vector3(0, 0, 0), Vector3(WinMaxWidth, WinMaxHeight, 1));
}

void BlurDemo::Destroy()
{
	SAFE_DELETE(bt);
}

void BlurDemo::Update()
{
	bt->Update();
}


void BlurDemo::PreRender()
{
	
}

void BlurDemo::Render()
{
	bt->Render();
}

void BlurDemo::GUI()
{
	bt->GUI();
}
