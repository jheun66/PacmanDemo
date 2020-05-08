#pragma once

struct TextureDesc
{
	wstring filePath = L"";
	UINT width = 0, height = 0;
	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::Image* image;

	bool operator ==(const TextureDesc& desc)
	{
		bool b = true;
		b &= filePath == desc.filePath;
		b &= width == desc.width;
		b &= height == desc.height;

		return b;
	}
};

// Texture2D
// srv path, (MetaData : width, hieght etc..)

class Texture2D
{
public:
	friend class Textures;
	Texture2D(wstring filePath, D3DX11_IMAGE_LOAD_INFO* loadInfo = nullptr);

	D3D11_TEXTURE2D_DESC ReadPixel(DXGI_FORMAT format, vector<D3DXCOLOR>* pixels);
	static D3D11_TEXTURE2D_DESC ReadPixel(ID3D11Texture2D* texture, DXGI_FORMAT format, vector<D3DXCOLOR>* pixels);

	void SaveTexture(wstring filePath, D3D11_TEXTURE2D_DESC srcDesc, vector<D3DXCOLOR>& pixels);
	void SaveTexture(wstring filePath);

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	DXGI_FORMAT GetFormat() { return metaData.format; }
	
	UINT GetWidth() { return metaData.width; }
	UINT GetHeight() { return metaData.height; }

	wstring GetFilePath() { return filePath; }

private:
	wstring filePath = L"";
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::TexMetadata metaData;
	TextureDesc desc;

};

//Textures
// TextureDesc 
// 경로를 가지고 이미지를 읽어오는 과정

class Textures
{
public:
	static void Delete();

private:
	friend class Texture2D;
	static void Load(Texture2D* texture, D3DX11_IMAGE_LOAD_INFO* loadInfo = nullptr);
	static vector<TextureDesc> descs;

};