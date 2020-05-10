#pragma once

class AnimationClip
{
	friend class Animator;
public:
	AnimationClip
	(
		wstring clipName,
		Texture2D* srcTex,
		UINT frameCount,
		D3DXVECTOR2 startPos,
		D3DXVECTOR2 endPos,
		bool bReversed = false
	);

	AnimationClip
	(
		wstring clipName,
		wstring jsonPath,
		vector<string> fileNames,
		bool bReversed = false
	);


	wstring GetClipName() { return clipName; }

private:
	wstring clipName = L"";
	vector<D3DXVECTOR2> keyframes;
	D3DXVECTOR2 texelFrameSize = D3DXVECTOR2(0, 0);
	UINT frameCount = 0;
	ID3D11ShaderResourceView* srv = nullptr;
	bool bReversed = false;
};

class Animator
{
public:
	Animator();
	~Animator();

	void Update();
	void AddAnimClip(AnimationClip* animClip);

	D3DXVECTOR2 GetCurrentFrame() { return currentFrame; }
	D3DXVECTOR2 GetTexelFrameSize() { return currentClip->texelFrameSize; }

	void SetCurrentAnimClip(wstring clipName);

	void SetSRVFunc(function<void(ID3D11ShaderResourceView*)> func)
	{
		this->func = func;
	}

	void SetMoveLeftClip(AnimationClip* clip)
	{
		moveLeft = clip;
		AddAnimClip(clip);
	}
	void SetMoveRightClip(AnimationClip* clip)
	{
		moveRight = clip;
		AddAnimClip(clip);
	}
	void SetMoveUpClip(AnimationClip* clip)
	{
		moveUp = clip;
		AddAnimClip(clip);
	}
	void SetMoveDownClip(AnimationClip* clip)
	{
		moveDown = clip;
		AddAnimClip(clip);
	}
	void SetIdleClip(AnimationClip* clip)
	{
		idle = clip;
		AddAnimClip(clip);
	}
	void SetDieClip(AnimationClip* clip)
	{
		idle = clip;
		AddAnimClip(clip);
	}

	void PlayMoveLeft() { SetCurrentAnimClip(moveLeft->clipName); }
	void PlayMoveRight() { SetCurrentAnimClip(moveRight->clipName); }
	void PlayMoveUp() { SetCurrentAnimClip(moveUp->clipName); }
	void PlayMoveDown() { SetCurrentAnimClip(moveDown->clipName); }
	void PlayIdle() { SetCurrentAnimClip(idle->clipName); }
	void PlayDie() { SetCurrentAnimClip(die->clipName); }

	bool CheckAnimClipDuplication(wstring clipName);
	
	void SetIsStop(bool isStop) { this->isStop = isStop; }

private:
	unordered_map<wstring, AnimationClip*> animClips;
	AnimationClip* currentClip = nullptr;
	UINT currentFrameIndex = 0;
	D3DXVECTOR2 currentFrame = D3DXVECTOR2(0, 0);

	AnimationClip* idle = nullptr;
	AnimationClip* moveLeft = nullptr;
	AnimationClip* moveRight = nullptr;
	AnimationClip* moveUp = nullptr;
	AnimationClip* moveDown = nullptr;
	AnimationClip* die = nullptr;

	float deltaTime = 0.0f;
	float playRate = 1.0f / 15.0f;

	bool isStop = false;

	function<void(ID3D11ShaderResourceView*)> func = nullptr;
};