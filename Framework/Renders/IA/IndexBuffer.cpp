#include "Framework.h"
#include "IndexBuffer.h"

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Create(const vector<UINT>& indices, const D3D11_USAGE & usage)
{
	stride = sizeof(UINT);
	count = (UINT)indices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = stride * count;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		break;
	case D3D11_USAGE_IMMUTABLE:
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = indices.data();	// 벡터의 시작 주소

	HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);

}

void IndexBuffer::SetIA()
{
	DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
