#include "Framework.h"
#include "Animator.h"

AnimationClip::AnimationClip(wstring clipName, Texture2D * srcTex, UINT frameCount, D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, bool bReversed)
	: clipName(clipName), frameCount(frameCount), bReversed(bReversed)
{
	srv = srcTex->GetSRV();

	float imageWidth = (float)srcTex->GetWidth();
	float imageHeight = (float)srcTex->GetHeight();

	D3DXVECTOR2 clipSize = endPos - startPos;
	D3DXVECTOR2 frameSize;
	frameSize.x = clipSize.x / frameCount;
	frameSize.y = clipSize.y;

	D3DXVECTOR2 texelSize = D3DXVECTOR2(1.0f / imageWidth, 1.0f / imageHeight);
	texelFrameSize = D3DXVECTOR2(frameSize.x * texelSize.x, frameSize.y * texelSize.y);
	D3DXVECTOR2 texelStartPos = D3DXVECTOR2(startPos.x * texelSize.x, startPos.y * texelSize.y);

	for (UINT i = 0; i < frameCount; i++)
	{
		D3DXVECTOR2 keyframe;
		keyframe.x = texelStartPos.x + texelFrameSize.x *i;
		keyframe.y = texelStartPos.y;
		keyframes.push_back(keyframe);
	}
}

Animator::Animator()
{
}

Animator::~Animator()
{
	for (auto clip : animClips)
		SAFE_DELETE(clip.second);
	
}

void Animator::Update()
{
	if (deltaTime > playRate)
	{
		if (currentClip->bReversed == false)
		{
			currentFrame = currentClip->keyframes[currentFrameIndex];
			currentFrameIndex++;
			if (currentFrameIndex == currentClip->frameCount)
				currentFrameIndex = 0;
		}
		else
		{
			currentFrame = currentClip->keyframes[currentFrameIndex];
			currentFrameIndex--;
			if (currentFrameIndex == -1)
				currentFrameIndex = currentClip->frameCount - 1;
		}
		deltaTime = 0.0f;
	}
	else
	{
		deltaTime += Time::Delta();
	}
}

void Animator::AddAnimClip(AnimationClip * animClip)
{
	animClips.insert(make_pair(animClip->clipName, animClip));
}

void Animator::SetCurrentAnimClip(wstring clipName)
{
	if (currentClip == nullptr)
		currentClip = animClips[clipName];
	else if (clipName == currentClip->clipName) 
		return;

	if (animClips.find(clipName) != animClips.end())
	{
		currentClip = animClips.find(clipName)->second;
		deltaTime = 0.0f;

		if (currentClip->bReversed == true)
			currentFrameIndex = currentClip->frameCount - 1;
		else
			currentFrameIndex = 0;

		currentFrame = currentClip->keyframes[currentFrameIndex];

		if (func != nullptr)
			func(currentClip->srv);
	}
}

// ¿÷¿∏∏È true
bool Animator::CheckAnimClipDuplication(wstring clipName)
{
	return animClips.find(clipName) != animClips.end();
}
