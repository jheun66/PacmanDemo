#pragma once

#include "Framework.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer()
		: ShaderBuffer(&data, sizeof(data))
	{
		D3DXMatrixIdentity(&data.world);
	}

	void SetWorld(D3DXMATRIX world)
	{
		D3DXMatrixTranspose(&data.world, &world);
	}

	struct Data
	{
		D3DXMATRIX world;
	};

private:
	Data data;
};

class VPBuffer : public ShaderBuffer
{
public:
	VPBuffer()
		: ShaderBuffer(&data, sizeof(data))
	{
		D3DXMatrixIdentity(&data.view);
		D3DXMatrixIdentity(&data.proj);
	}

	void SetView(D3DXMATRIX view)
	{
		D3DXMatrixTranspose(&data.view, &view);
	}

	void SetProj(D3DXMATRIX proj)
	{
		D3DXMatrixTranspose(&data.proj, &proj);
	}

	struct Data
	{
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};

private:
	Data data;
};