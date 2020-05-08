#pragma once

struct D2DDesc		// description
{
	wstring AppName;
	HINSTANCE instance;
	HWND handle;
	UINT width;
	UINT height;
};

class Window
{
public:
	Window(D2DDesc desc);
	~Window();

	WPARAM Run(IObject* mainObj);

	static void MainRender();
private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static D2DDesc desc;


	static IObject* mainObj;
};