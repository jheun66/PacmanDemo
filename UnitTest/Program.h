#pragma once
class Program : public IObject
{
public:
	void Init() override; 			// ������ ��ü
	void Destroy()  override;		// �Ҹ��� ��ü

	void Update()  override;		// �� �����Ӹ��� 

	void PreRender()  override;		// render to texture RTT
	void Render()  override;
	void PostRender()  override;	// post effect ��ó��

	void GUI()  override;			// UI ����

private:
	void MainMenu();
	void PrintFPS();
	void PrintMousePos();
	void Push(IObject* obj);


	vector<IObject*> objs;

	bool bFps = true;
	bool bMouse = true;
};

