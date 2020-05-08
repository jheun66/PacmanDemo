#pragma once

class Rect
{
public:
	Rect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation);
	virtual ~Rect();

	void Update();
	void Render();
	virtual void GUI();

	void Move();

	// AABB 충돌 체크 방식
	void CheckIntersect(Rect* other);


protected:
	void TransformedVertices();
	void UpdateWorld();
	void UpdateColor(D3DXCOLOR color);
	
	vector<VertexColor> vertices;
	VertexBuffer* VB = nullptr;

	vector<UINT> indices;
	IndexBuffer* IB = nullptr;

	VertexShader* VS = nullptr;
	PixelShader* PS = nullptr;

	InputLayout* IL = nullptr;

	WorldBuffer* WB = nullptr;
	D3DXMATRIX world, S, R, T;

	D3DXVECTOR3 position;
	D3DXVECTOR3 size;
	float rotation;

	RectEdge r;
	bool bIntersect = false;
	//ID3D11SamplerState* sampler = nullptr;

	float speed = 100.0f;

	TransformData transform;

	class OBB* obb = nullptr;
};