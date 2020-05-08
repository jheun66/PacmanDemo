#pragma once

#include "TextureRect.h"

class LightBuffer :public ShaderBuffer
{
public:
	LightBuffer() : ShaderBuffer(&data, sizeof(Struct))
	{
		data.mPos = Vector3(-1, -1, -1);
	}

	void SetMPos(Vector3 mPos) 
	{
		this->data.mPos.x = mPos.x / (float)WinMaxWidth; 
		this->data.mPos.y = mPos.y / (float)WinMaxHeight;
	}

	struct Struct
	{
		Vector3 mPos;
		int dummy;
	};

private:
	Struct data;
};

class LightMap : public TextureRect
{
public:
	LightMap(Vector3 position, Vector3 size);
	~LightMap();

	void Render();
	void Update();

	void SetLightPos(Vector3 pos) { lb->SetMPos(pos); }

private:
	Texture2D* lightTex = nullptr;
	ID3D11ShaderResourceView* lightSRV = nullptr;

	LightBuffer* lb = nullptr;


};