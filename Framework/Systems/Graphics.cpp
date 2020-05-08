#include "Framework.h"
#include "Graphics.h"

Graphics::Graphics()
{
	SetGPUInfo();
	CreateSwapChain();
}

Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

void Graphics::Resize(const UINT & width, const UINT & height)
{
	DeleteSurface();
	{
		HRESULT hr = swapChain->ResizeBuffers
		(
			0,
			width,
			height,
			DXGI_FORMAT_UNKNOWN,
			0
		);
	}
	CreateRenderTargetView();
	SetViewport(width, height);
}

void Graphics::SetViewport(const UINT & width, const UINT & height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void Graphics::Begin()
{
	InitViewport();
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
	deviceContext->ClearRenderTargetView(rtv, clearColor);
}

void Graphics::End()
{
	HRESULT hr = swapChain->Present(bVsync == true ?  1 : 0, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::InitViewport()
{
	deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::CreateSwapChain()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(swapChain);

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	if (bVsync)
	{
		desc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
		desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
	}
	else
	{
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = handle;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	vector<D3D_FEATURE_LEVEL> featureLevel
	{// 기능 수준, 무슨 버전 쓸지
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};


	UINT maxVideoMemory = 0;
	for (int i = 0; i < adapterInfos.size(); i++)
	{
		if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory)
		{
			maxVideoMemory = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
			selectedAdapterIndex = i;
		}
	}

	// adapterInfos[0]는 내장그래픽카드
	numerator = adapterInfos[0]->outputInfo->numerator;
	denomiator = adapterInfos[0]->outputInfo->denominator;

	gpuMemorySize = adapterInfos[selectedAdapterIndex]->adapterDesc.DedicatedVideoMemory;
	gpuDesc = adapterInfos[selectedAdapterIndex]->adapterDesc.Description;

	//  directx 2d와 3d 리소스의 상호운영을 가능하게하는 옵션
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3DCOMPILE_PREFER_FLOW_CONTROL
		| D3DCOMPILE_DEBUG
		| D3DCOMPILE_SKIP_OPTIMIZATION;
#endif


	// 원래는 분리해서 만들어야함.
	// Hresult 실패할경우 왜 실패했는지 숫자로 나옴
	// featureLevel 넣어주고 사이즈
	// 
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		adapterInfos[selectedAdapterIndex]->adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		creationFlags,
		featureLevel.data(),
		featureLevel.size(),
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);

	assert(SUCCEEDED(hr));

	Resize(WinMaxWidth, WinMaxHeight);

	gpuName.assign(gpuDesc.begin(), gpuDesc.end());
}

void Graphics::CreateRenderTargetView()
{
	// com interface는 *접근 인지하기
	ID3D11Texture2D *backbuffer = nullptr;

	HRESULT hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backbuffer
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		backbuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(backbuffer);
}

void Graphics::DeleteSurface()
{
	SAFE_RELEASE(rtv);
}

void Graphics::EnumerateAdapters()
{
	IDXGIFactory1* factory;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
		return;

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter = nullptr;
		HRESULT hr = factory->EnumAdapters1(index, &adapter);
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		CHECK(hr);

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		adapterInfo->adapterOrdinal = index;
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->adapter = adapter;

		EnumerateAdapterOutput(adapterInfo);
		adapterInfos.push_back(adapterInfo);

		index++;
	}

	SAFE_RELEASE(factory);
}

bool Graphics::EnumerateAdapterOutput(D3DEnumAdapterInfo * adapterInfo)
{

	IDXGIOutput* output = nullptr;
	HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);
	if (hr == DXGI_ERROR_NOT_FOUND)
		return false;

	/*
		해상도
		감마설정
	*/

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();
	ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));

	hr = output->GetDesc(&outputInfo->outputDesc);
	CHECK(hr);

	outputInfo->output = output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = output->GetDisplayModeList
	(	format,	// 설정한 포맷만 열거
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,	// 모드가 몇갠지 넘어옴
		nullptr
	);
	CHECK(hr);

	displayModes = new DXGI_MODE_DESC[numModes];
	hr = output->GetDisplayModeList
	(
		format,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModes
	);
	CHECK(hr);

	// 맞는 해상도 가져와줌
	for (UINT i = 0; i < numModes; i++)
	{
		bool bCheck = true;
		bCheck &= displayModes[i].Width == WinMaxWidth;
		bCheck &= displayModes[i].Height == WinMaxHeight;

		if (bCheck)
		{
			// 수직 동기화
			// (모니터의 주사율이 60hz이면 프레임도 60으로 144hz면 144로)
			// 갱신 주기 (refreshrate)
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}
	adapterInfo->outputInfo = outputInfo;
	SAFE_DELETE_ARRAY(displayModes);

	return true;
}

void Graphics::SetGPUInfo()
{
	EnumerateAdapters();
}

void Graphics::GUI()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("SystemInfo"))
		{
			// 왼쪽에 체크박스
			ImGui::Checkbox("VSync", &bVsync);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), gpuName.c_str());
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
	SAFE_RELEASE(adapter);
	SAFE_DELETE(outputInfo);
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
	SAFE_RELEASE(output);
}
