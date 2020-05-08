#pragma once

class GUIDemo : public IObject
{
public:
	void Init() override {}
	void Destroy() override {}

	void Update() override {}

	void PreRender() override {}
	void Render() override {}
	void PostRender() override {}

	void GUI() override;

private:

};