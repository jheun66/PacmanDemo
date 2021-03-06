#pragma once

class TextureRect
{
public:
	TextureRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation);
	~TextureRect();

	void Update();
	void Render();
	void GUI();

	Vector3 GetPosition() { return position; }

	void Move();

	void SetShader(wstring shaderPath);

	// AABB 충돌 체크 방식
	void CheckIntersect(TextureRect* other);
	void SetSRV(wstring path);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	Vector3 GetCenterPosition() { return Vector3(position.x , position.y , 0.0f); }
	Vector3 GetCenterPositionInSS();

	void SetSpeed(float speed) { this->speed = speed; }
	float GetSpeed() { return speed; }

protected:
	void TransformedVertices();
	void UpdateWorld();

	vector<VertexTexture> vertices;
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

	ID3D11ShaderResourceView* srv = nullptr;

	RectEdge r;
	bool bIntersect = false;
	//ID3D11SamplerState* sampler = nullptr;

	float speed = 100.0f;

};