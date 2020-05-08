#pragma once

// ���漱��(���� �ϳ� �����, �ؿ� class Rect* �ϴ���)
// forward declaration (���縵 ���� �������� ����)
//class Rect;

class RectDemo : public IObject
{
public:
	void Init() override;			// ������ ��ü
	void Destroy() override;		// �Ҹ��� ��ü

	void Update() override; 		// �� �����Ӹ��� 

	void PreRender() override {}	// render to texture RTT
	void Render() override;
	void PostRender() override {}	// post effect ��ó��

	void GUI() override;			// UI ����

private:
	// ���漱��
	// forward declaration (���縵 ���� �������� ����)
	class Rect* r1 = nullptr;
	Rect* r2 = nullptr;

	vector<class Rect*> rects;
};