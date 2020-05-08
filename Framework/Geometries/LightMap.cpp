#include "Framework.h"
#include "LightMap.h"

LightMap::LightMap(Vector3 position, Vector3 size)
	:TextureRect(position, size, 0)
{
	lightTex = new Texture2D(TexturePath + L"light.png");
	lightSRV = lightTex->GetSRV();
	
	SetShader(ShaderPath + L"VertexLighting.hlsl");

	lb = new LightBuffer();

}

LightMap::~LightMap()
{
	SAFE_DELETE(lb);
	SAFE_DELETE(lightTex);
}

void LightMap::Render()
{
	//DC->PSSetShaderResources(1, 1, &lightSRV);
	lb->SetPSBuffer(0);
	__super::Render();
}

void LightMap::Update()
{
	//lb->SetMPos(Mouse::Get()->GetPosition());
}
