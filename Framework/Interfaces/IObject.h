#pragma once

class IObject 
{
public:
	virtual void Init() = 0;		// ������ ��ü
	virtual void Destroy() = 0;		// �Ҹ��� ��ü

	virtual void Update() = 0;		// �� �����Ӹ��� 
	
	virtual void PreRender() = 0;	// render to texture RTT
	virtual void Render() = 0;
	virtual void PostRender() = 0;	// post effect ��ó��

	virtual void GUI() = 0;		// UI ����

};

/* �̰� �����ذ���
public:
	void Init() override {}		
	 void Destroy() override {}		

	 void Update() override {}		

	 void PreRender() override {}	
	 void Render() override {}
	 void PostRender() override {}	

	 void GUI() override {}		
*/