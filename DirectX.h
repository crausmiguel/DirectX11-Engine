#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class DirectX {
public:
	DirectX();
	DirectX(const DirectX&);
	~DirectX();

	bool Initialize(int height, int width, HWND hwnd, bool vsync, bool fullscreen,
		float screenDepth, float screenNear);
	void Shutdow();

	//IGNORE for now
	/*
	void BeginScene(float x, float y, float z, float w);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void SetBackBufferRenderTarget();
	void ResetViewport();
	*/
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	/*
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_wolrdMatrix;
	XMMATRIX m_othorMatrix;
	D3D11_VIEWPORT m_viewport;*/
	
};