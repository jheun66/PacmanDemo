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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 뒤에 배경 색을 설정
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);						// 윈도우에 있는 커서 모양 가져와 설정
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);						// 프로그램 아이콘
	wndClass.hIconSm = wndClass.hIcon;									// 작은 아이콘
	wndClass.hInstance = desc.instance;									// 창의 식별자
	wndClass.lpfnWndProc = (WNDPROC)WndProc;							// 메세지 처리함수
	wndClass.lpszMenuName = NULL;										// 메뉴 이름
	wndClass.lpszClassName = desc.AppName.c_str();						// 앱 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// 스타일 옵션
	wndClass.cbSize = sizeof(WNDCLASSEX);								// 어떤 구조체를 사용하는지 

	WORD wHr = RegisterClassExW(&wndClass);								// 클래스 만든걸 등록
	assert(wHr != 0);													

	// 확장 기능을 쓸수 있게
	desc.handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, desc.AppName.c_str()
		, desc.AppName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW		// 윈도우 스타일
		, CW_USEDEFAULT													// 윈도우가 생성될 좌표
		, CW_USEDEFAULT													// 가로넓이	
		, CW_USEDEFAULT													// 세로넓이
		, CW_USEDEFAULT													
		, NULL															// 부모의 핸들
		, (HMENU) NULL													// 자식 식별자
		, desc.instance													// 창 인스턴스
		, NULL															
	);

	// 사용할 창의 작업 영역
	RECT rect{ 0,0, (LONG)desc.width, (LONG)desc.height };		// 왼쪽 위가 0,0

	// 화면의 중심 좌표,	중앙에 배치하도록	
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - desc.width)/2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - desc.height)/2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);				// 사각형 조절
	
	MoveWindow(
		desc.handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE												// 위치를 옮긴뒤 다시 그려라
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
		// 메시지 꺼내 보기, get은 꺼내면 사라짐, peek은 꺼내고 넣음, 근데 pm_remove써서 사라짐
		// get은 올때까지 기다림, peek은 메시지가 안들어오면 다른 작업함
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);			// 밝혀내다
			
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

// 전역 변수
HWND handle;
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Gui::Get()->MsgProc(handle, message, wParam, lParam);
	Mouse::Get()->InputProc(message, wParam, lParam);
	//위에꺼 안되면 바꿔보자
	//if (Gui::Get()->MsgProc(handle, message, wParam, lParam))
	//	return true;
	//if (Gui::Get() && ImGui::GetIO().WantCaptureMouse == false)
	//	Mouse::Get()->InputProc(message, wParam, lParam);	//Q. 마우스 안의 함수인데 무슨 뜻일까? A. 

	if (message == WM_CREATE)
		::handle = handle;

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
