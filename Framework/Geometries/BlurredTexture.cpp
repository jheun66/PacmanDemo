#include "Framework.h"
#include "BlurredTexture.h"

BlurredTexture::BlurredTexture(Vector3 position, Vector3 size)
	:TextureRect(position, size, 0)
{
	SetShader(ShaderPath + L"VertexTextureBlur.hlsl");

	/*texture = new Texture2D(TexturePath + L"butterfly.jpg");
	srv = texture->GetSRV();*/

	bb = new BlurBuffer();
	bb->SetTextureSize(size);
}

BlurredTexture::~BlurredTexture()
{
	//SAFE_DELETE(texture);
	SAFE_DELETE(bb);
}

void BlurredTexture::Render()
{
	bb->SetPSBuffer(0);
	TextureRect::Render();

}

void BlurredTexture::GUI()
{
	ImGui::Begin("Blur");
	{
		ImGui::SliderInt("BlurCount", bb->GetBlurCountPointer(), 1, 30);
		if (ImGui::SliderFloat2("TextureSize", size, 128, 1280, "%.f"))
		{
			bb->SetTextureSize(size);

			UpdateWorld();
		}
	}
	ImGui::End();
}
