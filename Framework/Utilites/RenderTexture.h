#pragma once

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	void Init();
	void RenderToTexture();
	ID3D11ShaderResourceView* GetSRV() { return srv; }
	void SaveTexture(const wstring& path);

private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	Color clearColor = Color(163.0f / 255.0f, 204.0f / 255.0f, 163.0f / 255.0f, 1);
};