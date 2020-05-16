#include "stdafx.h"
#include "Program.h"
#include "Systems/Window.h"

#include "Demos/GUIDemo.h"
#include "Demos/RectDemo.h"
#include "Demos/TextureDemo.h"
#include "Demos/RunningGameDemo.h"
#include "Demos/SoundDemo.h"
#include "Demos/RTTDemo.h"
#include "Demos/AnimationDemo.h"
#include "Demos/TileMapDemo.h"
#include "Demos/JsonDemo.h"
#include "Demos/BlurDemo.h"
#include "Demos/LightDemo.h"
#include "Demos/TileCharacterDemo.h"

void Program::Init()
{
	States::Create();
	Camera::Create();
	
	//Push(new RectDemo());
	//Push(new GUIDemo);
	//Push(new TextureDemo());
	//Push(new RunningGameDemo());
	//Push(new SoundDemo());
	//Push(new RTTDemo());
	//Push(new AnimationDemo());
	//Push(new TileMapDemo());
	//Push(new JsonDemo());
	//Push(new BlurDemo());
	//Push(new LightDemo());
	Push(new TileCharacterDemo());
}

void Program::Destroy()
{
	Camera::Delete();
	for (IObject* obj : objs)
	{
		obj->Destroy();
		SAFE_DELETE(obj);
	}
}

void Program::Update()
{
	Camera::Get()->Update();
	for (IObject* obj : objs)
	{
		obj->Update();
	}
}

void Program::PreRender()
{
	//Camera::Get()->Render();
	for (IObject* obj : objs)
	{
		obj->PreRender();
	}
}

void Program::Render()
{
	Camera::Get()->Render();
	for (IObject* obj : objs)
	{
		obj->Render();
	}
}

void Program::PostRender()
{
	for (IObject* obj : objs)
	{
		obj->PostRender();
	}
}

void Program::GUI()
{
	//MainMenu();

	
	/*for (IObject* obj : objs)
	{
		obj->GUI();
	}*/
}

void Program::MainMenu()
{
	ImGui::BeginMainMenuBar();
	{
	
		if (ImGui::BeginMenu("Option"))
		{
			// 왼쪽에 체크박스
			ImGui::Checkbox("FPS", &bFps);
			ImGui::Checkbox("MousePos", &bMouse);

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (bFps)
		PrintFPS();
	if (bMouse)
		PrintMousePos();

}

void Program::PrintFPS()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ WinMaxWidth - 100 ,15 });
	ImGui::Begin("FPS"
		, &bOpen
		, ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
	);
	{
		string frame = "FPS : " + to_string((int)ImGui::GetIO().Framerate);
		ImGui::TextColored(ImVec4(1, 1, 1, 1), frame.c_str());
	}
	ImGui::End();

}

void Program::PrintMousePos()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ WinMaxWidth - 100 , 30 });
	ImGui::SetNextWindowSize(ImVec2(200, 60));
	ImGui::Begin("MousePos"
		, &bOpen
		, ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
	);
	{
		string x = "X : " + to_string((int)Mouse::Get()->GetPosition().x);
		string y = "Y : " + to_string((int)Mouse::Get()->GetPosition().y);
		ImGui::TextColored(ImVec4(1, 1, 1, 1), x.c_str());
		ImGui::TextColored(ImVec4(1, 1, 1, 1), y.c_str());
	}
	ImGui::End();
}

void Program::Push(IObject* obj)
{
	objs.push_back(obj);
	obj->Init();
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance,
	LPSTR param, int command)
{
	// instance : 창의 식별자
	// handle : 프로그램의 식별자
	D2DDesc desc;
	desc.AppName = L"D2DGame";
	desc.instance = instance;
	desc.handle = NULL;
	desc.width = 448;
	desc.height = 576;

	Program* program = new Program();

	Window* window = new Window(desc);
	WPARAM wParam = window->Run(program);
	SAFE_DELETE(window);

	return wParam;
}