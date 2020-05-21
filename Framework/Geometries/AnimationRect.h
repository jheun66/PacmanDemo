#pragma once

#include "TextureRect.h"
class AnimationRect :public TextureRect
{
public:
	AnimationRect(D3DXVECTOR3 position, D3DXVECTOR3 size);
	~AnimationRect();

	void Update();
	void Render();

	void Move();
	void Move(Vector3 destination);
	void Move(Vector2 direction);
	void SetAnimator(class Animator* animator);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

private:
	class Animator* animator = nullptr;

	ID3D11SamplerState* samplerLinear = nullptr;
	ID3D11SamplerState* samplerPoint = nullptr;
	ID3D11BlendState* blendEnabled = nullptr;
	ID3D11BlendState* blendDisabled = nullptr;

};