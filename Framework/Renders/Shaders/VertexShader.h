#pragma once
#include "IShader.h"

class VertexShader : public IShader
{
public:
	~VertexShader();

	void Create(wstring path, string entryName) override;
	void Clear() override;
	void SetShader() override;

	ID3DBlob* GetBlob() { return blob; }
	ID3D11VertexShader* GetResource() { return shader; }

private:
	ID3D11VertexShader* shader = nullptr;
	ID3DBlob* blob = nullptr;

};