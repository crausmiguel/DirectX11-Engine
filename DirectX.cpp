#include "DirectX.h"

DirectX::DirectX() {
	/*
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	*/
}

DirectX::DirectX(const DirectX&) {}

DirectX::~DirectX() {}

bool DirectX::Initialize(int height, int width, HWND hwnd, bool vsync, bool fullscreen,
	float screenDepth, float screenNear) {
	
	int error;
	float filedOfView, screenAscpect;
	unsigned long long stringLength;


	HRESULT result;
	DXGI_ADAPTER_DESC adapterDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;


	m_vsync_enabled = vsync;
	

	//Create and DirectX graphic interface factory
	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) {
		return false;
	}

	//Use the factory to create an adapter for the primary graphics interface (video card)
	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) {
		return false;
	}

	//Enumerate the primary adapter output (monitor)
	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) {
		return false;
	}

	//Get the number of modes (list size) that fit the DXGI_FORMAT display format 
	//for the adapter output (monitor)
	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) {
		return false;
	}

	//Create a list to hold all the possible display modes for this 
	//monitor/video card combination
	DXGI_MODE_DESC* displayModeList;
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) {
		return false;
	}

	//Now fill the display mode list structure
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) {
		return false;
	}
	// Now go through all the display modes and find the one that matches the screen 
	// width and height.
	// When a match is found store the numerator and denominator of the refresh rate for 
	// that monitor.
	unsigned int i, numerator, denominator;
	for (i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (unsigned int)width) {
			if (displayModeList[i].Height == (unsigned int)height) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//Get video card description
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) {
		return false;
	}

	//Store the dedicated video card memory in MB
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//Convert the name of the video card to a character array and store it
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) {
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	//Initialize the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	//Set to single back buffer
	swapChainDesc.BufferCount = 1;

	//Set the width and height
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	//Set regular 32 bit surface
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Set the refresh rate of the back buffer
	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	}

	//Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//Set the handle for the window to render to;
	swapChainDesc.OutputWindow = hwnd;

	//Turn multisampling off;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen) {
		swapChainDesc.Windowed = false;
	}
	else {
		swapChainDesc.Windowed = true;
	}

	//Set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Discard the back buffer contents after presenting 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Dont set the advanced flags
	swapChainDesc.Flags = 0;

	//Set directx version
	D3D_FEATURE_LEVEL featureLevel;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create the swap chain, Direct3d device and Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, 
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, 
		&m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(result)) {
		return false;
	}

	//Get the pointer to the back buffer
	ID3D11Texture2D* backBufferPtr;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	if (FAILED(result)) {
		return false;
	}

	//Create the render target view with the back buffer pointer
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result)) {
		return false;
	}

	//Release pointer to the back buffer as we no longer need
	backBufferPtr->Release();
	backBufferPtr = 0;
	

	//Initialize the description of the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };

	//Set the description of the depth buffer
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the texture for the depth buffer
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);

	if (FAILED(result)) {
		return false;
	}

	//Initialize and setup the descrption of the stencil
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Stencil operation if pixel is front facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;




}
