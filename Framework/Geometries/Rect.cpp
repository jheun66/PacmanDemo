#include "Framework.h"

#include "Rect.h"
#include "BoundingBox/OBB.h"

Rect::Rect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation)
	: position(position), size(size), rotation(rotation)
{
	vertices.resize(4, VertexColor());
	vertices[0].position = D3DXVECTOR3(0.0f,0.0f, 0);
	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0);
	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0);
	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0);

	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices.resize(6, UINT());
	indices = { 0,1,2,2,1,3 };

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(ShaderPath + L"VertexColor.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(ShaderPath + L"VertexColor.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexColor::descs, VertexColor::count, VS->GetBlob());

	WB = new WorldBuffer();

	obb = new OBB(&transform);

	UpdateWorld();


	// 바꾸고 싶을땐 이런식으로
	//D3D11_SAMPLER_DESC desc;
	//States::GetSamplerDesc(&desc);
	//desc.MaxAnisotropy = 8;
	//States::CreateSampler(&desc, &sampler);
}

Rect::~Rect()
{
	SAFE_DELETE(obb);
	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(VS);
	SAFE_DELETE(PS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);
}

void Rect::Update()
{
	UpdateWorld();

	if (bIntersect)
		UpdateColor(D3DXCOLOR(0, 1, 0, 1));
	else
		UpdateColor(D3DXCOLOR(0, 0, 0, 0));
	
}

void Rect::Render()
{
	VB->SetIA();
	IB->SetIA();
	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	PS->SetShader();

	WB->SetVSBuffer(0);

	DC->DrawIndexed(IB->GetCount(), 0, 0);
}

void Rect::GUI()
{
	
}

void Rect::Move()
{
	if (Keyboard::Get()->Press('W'))
		position.y += speed * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		position.y -= speed * Time::Delta();

	if (Keyboard::Get()->Press('A'))
		position.x -= speed * Time::Delta();
	else if (Keyboard::Get()->Press('D'))
		position.x += speed * Time::Delta();

	UpdateWorld();
}

void Rect::CheckIntersect(Rect * other)
{
	// AABB
	//if (this->r.RB.x >= other->r.LT.x
	//	&& this->r.LT.x <= other->r.RB.x
	//	&& this->r.LT.y >= other->r.RB.y
	//	&& this->r.RB.y <= other->r.LT.y)
	//	other->bIntersect = true;
	//else
	//	other->bIntersect = false;
	if (OBB::CheckIntersect(obb, other->obb))
		other->bIntersect = true;
	else
		other->bIntersect = false;
}

void Rect::TransformedVertices()
{
	D3DXVec3TransformCoord(&r.LT, &vertices[1].position, &world);
	D3DXVec3TransformCoord(&r.RB, &vertices[2].position, &world);
}

void Rect::UpdateWorld()
{
	D3DXMatrixScaling(&S, size.x, size.y, 1);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	world = S * R * T;
	WB->SetWorld(world);

	TransformedVertices();

	transform.edge = r;
	transform.position = position;
	transform.size = size;
	transform.rotation = rotation;

	obb->Calculation();
}

void Rect::UpdateColor(D3DXCOLOR color)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(
		VB->GetResource(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&subResource
	);
	// Range based for
	for (VertexColor& v : vertices)
		v.color = color;
	memcpy(subResource.pData, vertices.data(), VB->GetStride()* VB->GetCount());
	DC->Unmap(VB->GetResource(), 0);
}
