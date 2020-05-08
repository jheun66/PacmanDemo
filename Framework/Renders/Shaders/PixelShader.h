#pragma once


class PixelShader : public IShader
{
public:
	~PixelShader();

	void Create(wstring path, string entryName) override;
	void Clear() override;
	void SetShader() override;

	ID3DBlob* GetBlob() { return blob; }
	ID3D11PixelShader* GetResource() { return shader; }

private:
	ID3D11PixelShader* shader = nullptr;
	ID3DBlob* blob = nullptr;
};

