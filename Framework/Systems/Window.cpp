#include "Framework.h"
#include "Window.h"
#include "Utilites/Gui.h"

D2DDesc Window::desc;
IObject* Window::mainObj = nullptr;

Window::Window(D2DDesc desc)
{

	WNDCLASSEX wndClass;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// �ڿ� ��� ���� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);						// �����쿡 �ִ� Ŀ�� ��� ������ ����
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);						// ���α׷� ������
	wndClass.hIconSm = wndClass.hIcon;									// ���� ������
	wndClass.hInstance = desc.instance;									// â�� �ĺ���
	wndClass.lpfnWndProc = (WNDPROC)WndProc;							// �޼��� ó���Լ�
	wndClass.lpszMenuName = NULL;										// �޴� �̸�
	wndClass.lpszClassName = desc.AppName.c_str();						// �� �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// ��Ÿ�� �ɼ�
	wndClass.cbSize = sizeof(WNDCLASSEX);								// � ����ü�� ����ϴ��� 

	WORD wHr = RegisterClassExW(&wndClass);								// Ŭ���� ����� ���
	assert(wHr != 0);													

	// Ȯ�� ����� ���� �ְ�
	desc.handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, desc.AppName.c_str()
		, desc.AppName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW		// ������ ��Ÿ��
		, CW_USEDEFAULT													// �����찡 ������ ��ǥ
		, CW_USEDEFAULT													// ���γ���	
		, CW_USEDEFAULT													// ���γ���
		, CW_USEDEFAULT													
		, NULL															// �θ��� �ڵ�
		, (HMENU) NULL													// �ڽ� �ĺ���
		, desc.instance													// â �ν��Ͻ�
		, NULL															
	);

	// ����� â�� �۾� ����
	RECT rect{ 0,0, (LONG)desc.width, (LONG)desc.height };		// ���� ���� 0,0

	// ȭ���� �߽� ��ǥ,	�߾ӿ� ��ġ�ϵ���	
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - desc.width)/2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - desc.height)/2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);				// �簢�� ����
	
	MoveWindow(
		desc.handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE												// ��ġ�� �ű�� �ٽ� �׷���
	);


	ShowWindow(desc.handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.handle);
	SetFocus(desc.handle);					

	ShowCursor(true);

	Window::desc = desc;
}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(),desc.instance);
}

WPARAM Window::Run(IObject* mainObj)
{
	this->mainObj = mainObj;

	MSG msg = { 0 };

	Graphics::Create();
	Keyboard::Create();
	Time::Create();
	Mouse::Create();

	Gui::Create();

	this->mainObj->Init();

	Time::Get()->Start();
	
	while (true)
	{
		// �޽��� ���� ����, get�� ������ �����, peek�� ������ ����, �ٵ� pm_remove�Ἥ �����
		// get�� �ö����� ��ٸ�, peek�� �޽����� �ȵ����� �ٸ� �۾���
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);			// ��������
			
		}
		else
		{
			MainRender();
		}
	}
	this->mainObj->Destroy();

	Gui::Delete();
	Mouse::Delete();
	Time::Delete();
	Graphics::Delete();
	Keyboard::Delete();
	return msg.wParam;
}

void Window::MainRender()
{

	Time::Get()->Update();
	Gui::Get()->Update();

	if (ImGui::GetIO().WantCaptureMouse == false )
	{
		Keyboard::Get()->Update();
		Mouse::Get()->Update();
	}
	else
	{
		Keyboard::Get()->ResetKeyState();
	}

	mainObj->Update();

	mainObj->PreRender();
	Graphics::Get()->Begin();
	{
		mainObj->Render();
		mainObj->PostRender();

		mainObj->GUI();
		Graphics::Get()->GUI();
		Gui::Get()->Render();
	}
	Graphics::Get()->End();
}

// ���� ����
HWND handle;
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Gui::Get()->MsgProc(handle, message, wParam, lParam);
	Mouse::Get()->InputProc(message, wParam, lParam);
	//������ �ȵǸ� �ٲ㺸��
	//if (Gui::Get()->MsgProc(handle, message, wParam, lParam))
	//	return true;
	//if (Gui::Get() && ImGui::GetIO().WantCaptureMouse == false)
	//	Mouse::Get()->InputProc(message, wParam, lParam);	//Q. ���콺 ���� �Լ��ε� ���� ���ϱ�? A. 

	if (message == WM_CREATE)
		::handle = handle;

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
