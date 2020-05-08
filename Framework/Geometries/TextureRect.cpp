#include "Framework.h"
#include "TextureRect.h"

TextureRect::TextureRect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation)
	:position(position), size(size), rotation(rotation)
{
	vertices.resize(4, VertexTexture());
	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0);
	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0);
	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0);
	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0);

	vertices[0].uv = D3DXVECTOR2(0, 1);
	vertices[1].uv = D3DXVECTOR2(0, 0);
	vertices[2].uv = D3DXVECTOR2(1, 1);
	vertices[3].uv = D3DXVECTOR2(1, 0);

	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices.resize(6, UINT());
	indices = { 0,1,2,2,1,3 };

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(ShaderPath + L"VertexTexture.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(ShaderPath + L"VertexTexture.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexTexture::descs, VertexTexture::count, VS->GetBlob());

	WB = new WorldBuffer();

	UpdateWorld();

}

TextureRect::~TextureRect()
{
	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(VS);
	SAFE_DELETE(PS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);
}

void TextureRect::Update()
{
	UpdateWorld();
}

void TextureRect::Render()
{
	VB->SetIA();
	IB->SetIA();
	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	PS->SetShader();
	if (srv)
		DC->PSSetShaderResources(0, 1, &srv);
	

	WB->SetVSBuffer(0);

	DC->DrawIndexed(IB->GetCount(), 0, 0);
}

void TextureRect::GUI()
{

}

void TextureRect::Move()
{
	if (Keyboard::Get()->Press('W'))
		position.y += speed * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		position.y -= speed * Time::Delta();
	else if (Keyboard::Get()->Press('A'))
		position.x -= speed * Time::Delta();
	else if (Keyboard::Get()->Press('D'))
		position.x += speed * Time::Delta();

	UpdateWorld();
}

void TextureRect::SetShader(wstring shaderPath)
{
	VS->Clear();
	PS->Clear();
	VS->Create(shaderPath, "VS");
	PS->Create(shaderPath, "PS");
}

void TextureRect::CheckIntersect(TextureRect * other)
{

}

void TextureRect::SetSRV(wstring path)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		DEVICE,
		path.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	);
	CHECK(hr);
}

//SS screenspace
Vector3 TextureRect::GetCenterPositionInSS()
{
	Vector3 centerPos;
	centerPos.x = position.x + size.x / 2.0f;
	centerPos.y = position.y + size.y / 2.0f;

	Matrix vp = Camera::Get()->GetView() * Camera::Get()->GetProjection();
	D3DXVec3TransformCoord(&centerPos, &centerPos, &vp);
	// NDC -1~1 ÁÂÇ¥·Î º¯È¯µÊ

	centerPos.x = (centerPos.x + 1) * WinMaxWidth * 0.5f;
	centerPos.y = (centerPos.y + 1) * WinMaxHeight * 0.5f;
	centerPos.z = 0.0f;

	return centerPos;
}

void TextureRect::TransformedVertices()
{
	D3DXVec3TransformCoord(&r.LT, &vertices[1].position, &world);
	D3DXVec3TransformCoord(&r.RB, &vertices[2].position, &world);
}

void TextureRect::UpdateWorld()
{
	D3DXMatrixScaling(&S, size.x, size.y, 1);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	world = S * R * T;
	WB->SetWorld(world);

	TransformedVertices();
}
