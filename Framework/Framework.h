#pragma once

#define NOMINMAX

// stdafx
// 미리컴파일된 헤더
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <functional>
#include <assert.h>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

// DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>		// 비동기 : 런타임중에 불러올 때, 쓰레드나 네트워크에도 사용  

//링크 연결
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")

//ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#pragma comment(lib, "ImGui/ImGui.lib")

//FMOD
#pragma comment(lib,"FMod/fmodex_vc.lib")

//DirectXTex
#include <DirectTex/DirectXTex.h>
#pragma comment(lib, "DirectTex/DirectXTex.lib")

//jsoncpp
#include <json/json.h>
#pragma comment(lib, "json/jsoncpp.lib")

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX	Matrix;
typedef D3DXCOLOR	Color;
typedef UINT		uint;

#define SAFE_RELEASE(p) {if(p) {p->Release(); p=nullptr;}}
#define SAFE_DELETE(p) {if(p) {delete(p); p = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); p = nullptr;}}

#define WinMaxWidth 1280
#define WinMaxHeight 720

const wstring ShaderPath = L"../_Shaders/";
const wstring TexturePath = L"../_Textures/";

const wstring TileMapDataPath = L"../_Resources/_TileMapData/";

// extern : 어딘가에 전역변수로 선언되어 있다.
extern HWND handle;

#include "Interfaces/IObject.h"

#include "Utilites/SingletonBase.h"
#include "Utilites/String.h"
#include "Utilites/Path.h"

#include "Systems/Graphics.h"
#include "Systems/Keyboard.h"
#include "Systems/Time.h"
#include "Systems/Mouse.h"


#define DEVICE Graphics::Get()->GetDevice()
#define DC Graphics::Get()->GetDC()

#define CHECK(p) assert(SUCCEEDED(p))


#include "Renders/Resources/VertexType.h"
#include "Renders/Resources/ShaderBuffer.h"
#include "Renders/Resources/GlobalBuffers.h"
#include "Renders/Resources/Texture2D.h"


#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"


#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"

#include "Renders/States.h"

#include "Utilites/DirectHelpers.h"
#include "Utilites/FileStream.h"
#include "Utilites/Json.h"

#include "Systems/Camera.h"