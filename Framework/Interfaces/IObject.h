#pragma once

class IObject 
{
public:
	virtual void Init() = 0;		// 생성자 대체
	virtual void Destroy() = 0;		// 소멸자 대체

	virtual void Update() = 0;		// 매 프레임마다 
	
	virtual void PreRender() = 0;	// render to texture RTT
	virtual void Render() = 0;
	virtual void PostRender() = 0;	// post effect 후처리

	virtual void GUI() = 0;		// UI 관련

};

/* 이거 복사해가기
public:
	void Init() override {}		
	 void Destroy() override {}		

	 void Update() override {}		

	 void PreRender() override {}	
	 void Render() override {}
	 void PostRender() override {}	

	 void GUI() override {}		
*/