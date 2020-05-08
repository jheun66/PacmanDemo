#pragma once

class D3DEnumAdapterInfo;
class D3DEnumOutputInfo;

class Graphics : public SingletonBase<Graphics>
{
public:
	// �̱��濡���� ���Ŭ������ ����ϰ� �ϱ� ���� ����
	friend class SingletonBase<Graphics>;

	void Resize(const UINT& width, const UINT& height);
	void SetViewport(const UINT& width, const UINT& height);

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return deviceContext; }

	void Begin();
	void End();
	void GUI();

	D3DXCOLOR GetClearColor() { return clearColor; }
	void InitViewport();
private:
	void CreateSwapChain();
	void CreateRenderTargetView();
	void DeleteSurface();

	void EnumerateAdapters();
	bool EnumerateAdapterOutput(D3DEnumAdapterInfo* adapterInfo);
	void SetGPUInfo();

	
private:

	Graphics();
	~Graphics();

	// ID3D11 : com Interface , ���������� ���

	// device�� �ڿ��� buffer�� texture
	// buffer�� ��� RAM���� VRAM���� ����
	// Texture 1D,2D,3D, pixel,texel
	// �ؽ�ó�� �ִ� �ڿ���
	// 1. RenderTargetView		�׷��پ�, ��ȭ�� ������ (���� ����) : �׸� ���ִ¾�
	// 2. ShaderResourceView	ȿ��??, �׷��� ī�带 �̿��ؼ� �ټ��մ� ȿ��, �ڿ�?? (�б� ����): �ִ°� ������ ���ڴ�
	// 3. DepthStencilView		���̰����� �ִ� �ڿ� (3D�Ҷ� ���)
	// 4. UnorderedAccessView	����������� �����ϴ� �ڿ� (�б�,���� �Ѵ� ��) 

	// Device Context : DC
	// ������ �׸��� �ſ� ��������.
	// �� �ȿ� �ؿ��� ������� 
	// Rendering Pipeline : �׸��� �ܰ�??
	//						IA (Input Assembler(�Է� ������))
	//					 	�׸��� ���� �ʿ��� �������� �־��ִ� �ܰ� 
	//						VS (Vertex Shader)	����(Vertex)	Shader: �׷���ī�带 ������ �� �ִ� �Լ����� ����
	//						�������� ����						
	//						RS (Rasterizer Stage)
	//						3D ������ 2D(ȭ��)�� ǥ���ϱ� ���� �ȼ�ȭ??
	//						PS (Pixel Shader)
	//						�ȼ����� ���� ( ���� ���� ���� �ٲ�� �� )
	//						OM (Output Merger)
 	//						���������� RenderTagetView�� �׷��ִ� �ܰ�

	// �츮�� �ڵ��� ���ִ°� Shader ���� �ֵ�
	// �������� ������ �Է����ִ°� �ۿ� ����

	// DXGI ����Ʈ����� �ϵ����(�׷��� ī��) ���̿��� ������ , swapchain : ������۸��� ���� �ִ� ��, adapter : �׷��� ī��, output : �����
	// SwapChain : ��ȭ���� �ϳ��� ����� �� ���� ������ �׸������� �յڷ� ���̴°� �Ⱥ��̰� �ϴµ�
	//			   (����� ������ �Ⱥ���, �����̴°� �����) �̶� ��ӵ��ư��� ��
	
	// Viewport : �׸� �����ȿ��� ȭ�鿡 ������ ���� 

	// d3dxcolor : RGBA float 4��(16����Ʈ) ����ü (0 ���� 1������ ������ ���) ����ȭ����

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	D3D11_VIEWPORT viewport;
	D3DXCOLOR clearColor = 0xFF555566;			// ���ڸ��� ��� 1����Ʈ FF : 255 �� �ڴ� RGB 

	UINT numerator = 0;			// ����
	UINT denomiator = 1;		// �и�

	UINT gpuMemorySize = 0;
	wstring gpuDesc = L"";

	string gpuName = "";

	vector<D3DEnumAdapterInfo*> adapterInfos;
	int selectedAdapterIndex = 0;

	bool bVsync = false;
};

/*
DXGI �⺻���� ��Ȱ
�׷���ī�� ������ ����
���÷��� ����� ����
�����ġ�� ����
����ü�� ����(���� ���۸�)
*/

class D3DEnumAdapterInfo
{
public:
	~D3DEnumAdapterInfo();

	UINT adapterOrdinal = 0;		// Ordinal : ���� (ù��° �ι�°)		Cardinal : ��� (���̻��)
	IDXGIAdapter1* adapter = nullptr;
	DXGI_ADAPTER_DESC1 adapterDesc;

	class D3DEnumOutputInfo* outputInfo = nullptr;
private:
	// ���Կ����� �����Ǹ� ���� �ƹ��͵� ���ϰ� �� (���� ����, ���� ���� �ƹ� ���� ������)
	const D3DEnumAdapterInfo& operator =
		(const D3DEnumAdapterInfo& rhs);

};

class D3DEnumOutputInfo
{
public:
	~D3DEnumOutputInfo();

	IDXGIOutput* output = nullptr;
	DXGI_OUTPUT_DESC outputDesc;

	UINT numerator = 0;
	UINT denominator = 1;

private:
	const D3DEnumOutputInfo& operator =
		(const D3DEnumOutputInfo& rhs);
};