#pragma once

struct VertexColor
{
	VertexColor() : position(0, 0, 0), color(0, 0, 0, 0) {}
	VertexColor(const D3DXVECTOR3& position, const D3DXCOLOR& color)
		: position(position), color(color)
	{

	}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;
};

struct VertexTexture
{
	VertexTexture() : position(0, 0, 0), uv(0, 0) {}
	VertexTexture(const D3DXVECTOR3& position, const D3DXVECTOR2& color)
		: position(position), uv(uv)
	{

	}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 2;
};

struct VertexTextureColor
{
	VertexTextureColor() : position(0, 0, 0), uv(0, 0), color(0,0,0,1){}
	VertexTextureColor(const Vector3& position, const Vector2& uv, const Color& color)
		: position(position), uv(uv), color(color)
	{

	}

	Vector3 position;
	Vector2 uv;
	Color color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 3;
};

struct VertexTile
{
	VertexTile() : position(0, 0, 0), uv(0, 0),uv2(0,0), color(0, 0, 0, 1), index(-1) {}
	VertexTile(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Color& color, const uint& index)
		: position(position), uv(uv), uv2(uv2), color(color), index(index)
	{

	}

	Vector3 position;
	Vector2 uv;
	Vector2 uv2;
	Color color;
	uint index;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const UINT count = 5;
};