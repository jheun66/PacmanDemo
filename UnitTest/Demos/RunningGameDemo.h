#pragma once
class RunningGameDemo :public IObject
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override {}

	void GUI() override;

private:
	class RunningRect* player = nullptr;

	vector<class RunningRect*> objs;
	float spacing = 600;
	const UINT maxObjCount = 5;

	class BlurredTexture* bt = nullptr;
	class RenderTexture* rt = nullptr;
};