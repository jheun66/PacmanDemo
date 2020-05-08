#include "Framework.h"
#include "Texture2D.h"

#include <wincodec.h>

using namespace DirectX;

vector<TextureDesc> Textures::descs;

Texture2D::Texture2D(wstring filePath, D3DX11_IMAGE_LOAD_INFO* loadInfo)
	: filePath(filePath)
{
	Textures::Load(this, loadInfo);
}

D3D11_TEXTURE2D_DESC Texture2D::ReadPixel(DXGI_FORMAT format, vector<D3DXCOLOR>* pixels)
{
	ID3D11Texture2D* texture;
	srv->GetResource((ID3D11Resource**)&texture);


	return ReadPixel(texture, format, pixels);
}

D3D11_TEXTURE2D_DESC Texture2D::ReadPixel(ID3D11Texture2D * texture, DXGI_FORMAT format, vector<D3DXCOLOR>* pixels)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	texture->GetDesc(&srcDesc);

	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));

	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = format;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.Usage = D3D11_USAGE_STAGING;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Texture2D* destTexture;

	HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, &destTexture);
	CHECK(hr);
	D3DX11LoadTextureFromTexture(DC, texture, nullptr, destTexture);

	UINT* colors = new UINT[destDesc.Width * destDesc.Height];
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(destTexture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(colors, subResource.pData, sizeof(UINT) * destDesc.Width * destDesc.Height);
	}
	DC->Unmap(destTexture, 0);

	for (UINT y = 0; y < destDesc.Height; y++)
	{
		for (UINT x = 0; x < destDesc.Width; x++)
		{
			UINT index = destDesc.Width * y + x;

			float f = 1.0f / 255.0f;

			float a = f * (float)((0xFF000000 & colors[index]) >> 24);
			float r = f * (float)((0x00FF0000 & colors[index]) >> 16);
			float g = f * (float)((0x0000FF00 & colors[index]) >> 8);
			float b = f * (float)((0x000000FF & colors[index]) >> 0);

			pixels->push_back(D3DXCOLOR(r, g, b, a));
		}
	}


	SAFE_DELETE_ARRAY(colors);
	SAFE_RELEASE(destTexture);

	return destDesc;
}

void Texture2D::SaveTexture(wstring filePath, D3D11_TEXTURE2D_DESC srcDesc, vector<D3DXCOLOR>& pixels)
{
	ID3D11Texture2D* texture;

	UINT* rawPixels = new UINT[pixels.size()];
	for (UINT i = 0; i < pixels.size(); i++)
		rawPixels[i] = pixels[i];

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = rawPixels;
	subData.SysMemPitch = srcDesc.Width * sizeof(UINT);
	subData.SysMemSlicePitch = srcDesc.Width * srcDesc.Height * sizeof(UINT);

	HRESULT hr = DEVICE->CreateTexture2D(&srcDesc, &subData, &texture);
	CHECK(hr);

	//bmp png
	wstring ext = Path::GetExtension(filePath);
	if (ext == L"bmp")
		hr = D3DX11SaveTextureToFile(DC, texture, D3DX11_IFF_BMP, filePath.c_str());
	else if (ext == L"png")
		hr = D3DX11SaveTextureToFile(DC, texture, D3DX11_IFF_PNG, filePath.c_str());
	else
		return;

	CHECK(hr);

	SAFE_DELETE_ARRAY(rawPixels);
}

void Texture2D::SaveTexture(wstring filePath)
{
	const Image* image = desc.image;
	//bbp bit per pixel - > ÇÑÇÈ¼¿´ç 32 bit
	HRESULT hr = SaveToWICFile(*image, WIC_FLAGS_NONE, GUID_ContainerFormatPng, filePath.c_str(), &GUID_WICPixelFormat32bppBGRA);
	CHECK(hr);
}

void Textures::Delete()
{
	for (TextureDesc desc : descs)
	{
		SAFE_DELETE_ARRAY(desc.image->pixels);
		SAFE_DELETE(desc.image);
		SAFE_RELEASE(desc.srv);
	}
}

void Textures::Load(Texture2D * texture, D3DX11_IMAGE_LOAD_INFO* loadInfo)
{
	HRESULT hr;
	TexMetadata metaData;
	wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
		CHECK(hr);
	}
	else if (ext == L"dds")
	{
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_FORCE_RGB, metaData);
		CHECK(hr);
	}
	else if (ext == L"hdr")
	{
		CHECK(false);
	}
	else
	{
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_FORCE_RGB, metaData);
		CHECK(hr);
	}

	UINT width = metaData.width;
	UINT height = metaData.height;

	if (loadInfo != nullptr)
	{
		width = loadInfo->Width;
		height = loadInfo->Height;

		metaData.width = loadInfo->Width;
		metaData.height = loadInfo->Height;
	}

	TextureDesc desc;
	desc.filePath = texture->filePath;
	desc.width = width;
	desc.height = height;

	TextureDesc exist;
	bool bExist = false;
	for (TextureDesc temp : descs)
	{
		if (desc == temp)
		{
			bExist = true;
			exist = temp;

			break;
		}
	}

	if (bExist == true)
	{
		texture->metaData = exist.metaData;
		texture->srv = exist.srv;
	}
	else
	{
		ScratchImage image;

		if (ext == L"tga")
		{
			hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
			CHECK(hr);
		}
		else if (ext == L"dds")
		{
			hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_FORCE_RGB, &metaData, image);
			CHECK(hr);
		}
		else if (ext == L"hdr")
		{
			CHECK(false);
		}
		else
		{
			hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_FORCE_RGB, &metaData, image);
			CHECK(hr);
		}

		ID3D11ShaderResourceView* srv = nullptr;
		hr = CreateShaderResourceView
		(
			DEVICE,
			image.GetImages(),
			image.GetImageCount(),
			metaData,
			&srv
		);
		CHECK(hr);

		desc.srv = srv;
		desc.metaData = metaData;
		
		Image* temp = const_cast<Image*>(image.GetImage(0, 0, 0));
		desc.image = new Image();
		memcpy(desc.image, temp, sizeof(Image));
		desc.image->pixels = new uint8_t[image.GetPixelsSize()];
		memcpy(desc.image->pixels, temp->pixels, sizeof(uint8_t) * image.GetPixelsSize());

		texture->srv = srv;
		texture->metaData = metaData;
		texture->desc = desc;

		descs.push_back(desc);
	}
}
