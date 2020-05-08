#pragma once

class VertexBuffer 
{
public:
	~VertexBuffer();

	template <typename T>
	void Create(const vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() {
		return buffer;
	}
	UINT GetStride() { return stride; }
	UINT GetOffset() { return offset; }
	UINT GetCount() { return count; }

	void SetIA();

private:
	ID3D11Buffer* buffer = nullptr;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;

};

template<typename T>
inline void VertexBuffer::Create(const vector<T>& vertices, const D3D11_USAGE & usage)
{
	stride = sizeof(T);
	count = (UINT)vertices.size();

	/*
	typedef struct D3D11_BUFFER_DESC
	{
	UINT ByteWidth;		//The size, in bytes, of the vertex buffer we are going to create.
	D3D11_USAGE Usage;
	UINT BindFlags;		//For a vertex buffer, specify D3D11_BIND_VERTEX_BUFFER.
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;
	} 	D3D11_BUFFER_DESC;
	*/
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
	subData.pSysMem = vertices.data();	// 벡터의 시작 주소

	HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);

}
