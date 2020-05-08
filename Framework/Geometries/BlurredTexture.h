#pragma once

#include "TextureRect.h"

class BlurBuffer : public ShaderBuffer
{
public:
	BlurBuffer() : ShaderBuffer(&data, sizeof(Struct))
	{
		data.blurCount = 1;
		data.textureSize = Vector3(1,1,1);
	}

	int* GetBlurCountPointer() { return &data.blurCount; }
	void SetTextureSize(Vector3 size) { data.textureSize = size; }

	struct Struct
	{
		int blurCount;
		Vector3 textureSize;
	};
private:
	Struct data;
};

class BlurredTexture : public TextureRect
{
public:
	BlurredTexture(Vector3 position, Vector3 size);
	~BlurredTexture();

	void Render();
	void GUI();

private:
	//Texture2D* texture = nullptr;
	BlurBuffer* bb = nullptr;
};