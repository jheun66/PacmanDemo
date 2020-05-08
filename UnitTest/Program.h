#pragma once
class Program : public IObject
{
public:
	void Init() override; 			// 생성자 대체
	void Destroy()  override;		// 소멸자 대체

	void Update()  override;		// 매 프레임마다 

	void PreRender()  override;		// render to texture RTT
	void Render()  override;
	void PostRender()  override;	// post effect 후처리

	void GUI()  override;			// UI 관련

private:
	void MainMenu();
	void PrintFPS();
	void PrintMousePos();
	void Push(IObject* obj);


	vector<IObject*> objs;

	bool bFps = true;
	bool bMouse = true;
};

