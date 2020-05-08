#include "Framework.h"
#include "IShader.h"

void IShader::CompileShader(wstring path, string entryName, string profile, ID3DBlob ** blob)
{
	ID3DBlob* error = nullptr;		// error를 위한 블롭(이진 데이터)
	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		nullptr,
		entryName.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		blob,						// 처리 결과가 들어간 blob
		&error						// error를 위한 블롭(이진 데이터)
	);

	CheckShaderError(hr, error);
	SAFE_RELEASE(error);
}

void IShader::CheckShaderError(HRESULT hr, ID3DBlob* error)
{
	if (FAILED(hr))
	{
		if (error)
		{
			string str = (const char*)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		CHECK(false);
	}
}
