#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX11tex.h>

class xDirect3D11DllApi
{
public:
	xDirect3D11DllApi();
	~xDirect3D11DllApi();
	bool Validate();
public:

	typedef HRESULT     (WINAPI * LPCREATEDXGIFACTORY)(REFIID, void ** );
	typedef HRESULT     (WINAPI * LPD3D11CREATEDEVICE)( IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT32, D3D_FEATURE_LEVEL*, UINT, UINT32, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext** );
	typedef HRESULT     (WINAPI * LPD3D11CreateDeviceAndSwapChain)(IDXGIAdapter* ,D3D_DRIVER_TYPE ,HMODULE , UINT , CONST D3D_FEATURE_LEVEL* , UINT , UINT , CONST DXGI_SWAP_CHAIN_DESC* ,IDXGISwapChain** , ID3D11Device** , D3D_FEATURE_LEVEL* , ID3D11DeviceContext**  );
	LPD3D11CREATEDEVICE             D3D11CreateDevice;
	LPD3D11CreateDeviceAndSwapChain D3D11CreateDeviceAndSwapChain;
	LPCREATEDXGIFACTORY             CreateDXGIFactory1;
protected:
	HINSTANCE           m_hModD3D11;
	HINSTANCE           m_hModDXGI; 
};


struct XD3D11_TEXTURE2D_DESC : public D3D11_TEXTURE2D_DESC
{
    XD3D11_TEXTURE2D_DESC()
    {
       ZeroMemory(this , sizeof(D3D11_TEXTURE2D_DESC) );
    }

    explicit XD3D11_TEXTURE2D_DESC( const D3D11_TEXTURE2D_DESC& o ) :    D3D11_TEXTURE2D_DESC( o )
    {

    }
    explicit XD3D11_TEXTURE2D_DESC(
        DXGI_FORMAT format,
        UINT width,
        UINT height,
        UINT arraySize = 1,
        UINT mipLevels = 0,
        UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
        D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
        UINT cpuaccessFlags = 0,
        UINT sampleCount = 1,
        UINT sampleQuality = 0,
        UINT miscFlags = 0 )
    {
        Width = width;
        Height = height;
        MipLevels = mipLevels;
        ArraySize = arraySize;
        Format = format;
        SampleDesc.Count = sampleCount;
        SampleDesc.Quality = sampleQuality;
        Usage = usage;
        BindFlags = bindFlags;
        CPUAccessFlags = cpuaccessFlags;
        MiscFlags = miscFlags;
    }
    ~XD3D11_TEXTURE2D_DESC() {}
    operator const XD3D11_TEXTURE2D_DESC&() const { return *this; }
};

extern xDirect3D11DllApi* g_D3D11Dll;