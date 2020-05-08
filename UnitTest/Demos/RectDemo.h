#pragma once

// 전방선언(둘중 하나 쓰면됨, 밑에 class Rect* 하던지)
// forward declaration (스펠링 에러 안잡으니 조심)
//class Rect;

class RectDemo : public IObject
{
public:
	void Init() override;			// 생성자 대체
	void Destroy() override;		// 소멸자 대체

	void Update() override; 		// 매 프레임마다 

	void PreRender() override {}	// render to texture RTT
	void Render() override;
	void PostRender() override {}	// post effect 후처리

	void GUI() override;			// UI 관련

private:
	// 전방선언
	// forward declaration (스펠링 에러 안잡으니 조심)
	class Rect* r1 = nullptr;
	Rect* r2 = nullptr;

	vector<class Rect*> rects;
};