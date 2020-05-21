#include "Framework.h"
#include "AnimationRect.h"
#include "Utilites/Animator.h"

AnimationRect::AnimationRect(D3DXVECTOR3 position, D3DXVECTOR3 size)
	: TextureRect(position, size, 0)
{

	SetShader(ShaderPath + L"VertexAnimation.hlsl");
	
	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &samplerLinear);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &samplerPoint);
	}
	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &blendDisabled);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		States::CreateBlend(&desc, &blendEnabled);
	}
}

AnimationRect::~AnimationRect()
{
	SAFE_RELEASE(blendEnabled);
	SAFE_RELEASE(blendDisabled);

	SAFE_RELEASE(samplerPoint);
	SAFE_RELEASE(samplerLinear);

}

void AnimationRect::Update()
{
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		vertices[0].uv = D3DXVECTOR2(animator->GetCurrentFrame().x, animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y);
		vertices[1].uv = animator->GetCurrentFrame();
		vertices[2].uv = animator->GetCurrentFrame() + animator->GetTexelFrameSize();
		vertices[3].uv = D3DXVECTOR2(animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x, animator->GetCurrentFrame().y);

		memcpy(subResource.pData, vertices.data(), sizeof(VertexTexture) * vertices.size());

		DC->Unmap(VB->GetResource(), 0);
	}
}

void AnimationRect::Render()
{
	DC->PSSetSamplers(0,1, &samplerPoint);
	DC->OMSetBlendState(blendEnabled, nullptr, 0xffffffff);

	//TextureRect::Render();		부모클래스를 명시적으로
	__super::Render();			// 부모클래스의 render실행

	
	DC->PSSetSamplers(0, 1, &samplerLinear);
	DC->OMSetBlendState(blendDisabled, nullptr, 0xffffffff);

}

void AnimationRect::Move()
{
	if (Keyboard::Get()->Press(VK_LSHIFT)) return;

	//if (Keyboard::Get()->Press('A'))
	//	animator->SetCurrentAnimClip(L"RunL");
	//else if (Keyboard::Get()->Press('D'))
	//	animator->SetCurrentAnimClip(L"RunR");
	
	__super::Move();
}

void AnimationRect::Move(Vector3 destination)
{
	this->position = destination;
	UpdateWorld();
}

void AnimationRect::Move(Vector2 direction)
{
	if (direction == Vector2(0,1))
		position.y += speed * Time::Delta();
	else if (direction == Vector2(0, -1))
		position.y -= speed * Time::Delta();
	else if (direction == Vector2(-1, 0))
		position.x -= speed * Time::Delta();
	else if (direction == Vector2(1, 0))
		position.x += speed * Time::Delta();
	UpdateWorld();
}

void AnimationRect::SetAnimator(Animator * animator)
{
	this->animator = animator;
	function<void(ID3D11ShaderResourceView*)> func
		= bind(&AnimationRect::SetSRV, this, placeholders::_1);
	animator->SetSRVFunc(func);
}
