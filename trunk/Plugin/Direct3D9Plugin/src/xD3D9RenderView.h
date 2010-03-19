#pragma  once 
#include <RenderAPI/xRenderView.h>
#include <RenderAPI/xBaseTexture.h>
#include "xDirect3D9API.h"
#define MAX_RENDER_TARGET 4
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3D9DepthBuffer;
class xD3D9RenderView : public IRenderView
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9RenderView);
    DECLARE_DEVICE_LOST(xD3D9RenderView);
protected:
	IDirect3DSurface9*   depthView();
    bool                 setupDepthView(int w , int h , bool bSetAsDepthBuffer);
public:
    xD3D9RenderView(xD3D9RenderApi* pRenderApi , const xRTSampleDesc& sampleDesc);
    virtual ~xD3D9RenderView();
	bool                 clear(xColor_4f& bkColor);
	bool                 clear(xColor_4f& bkColor, int nClear);
    bool                 clear(xColor_4f& bkColor , float z , unsigned int stencil) ;
    bool                 clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear);
	bool                 clearRenderTarget(xColor_4f& bkColor, int idx);
    bool                 install();
    virtual int          startIdx() { return 0 ; }
    int                  nRenderTargetView();
    bool                 createDepthView(int w , int h);
	bool                 desc(xTextureDesc& _desc){ _desc = m_TextureDesc ; return true;}
    bool                 setup();
	virtual bool         destory();
public:
	bool                 resize(int w , int h);
	bool                 resize(int w , int h  , bool destoryRT);
	bool                 createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture);
	bool                 setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx);
	bool                 setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx);
	bool                 setDepthBuffer(IRenderTarget* pDepthBuffer);
    bool                 setDepthBuffer(IBaseTexture* pDepthTexture);
	int                  addRenderTarget(IBaseTexture*  pTexture);
	int                  addRenderTarget(IRenderTarget* pRenderTarget);
	IRenderTarget*       depthBuffer();
    bool                 depthBufferDesc(D3DSURFACE_DESC& _desc );
	IRenderApiObject*    renderTarget(size_t rtIdx );
    void                 onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);

protected:
    IDirect3DSurface9*        m_RenderTargetsView[MAX_RENDER_TARGET];
    IRenderTarget*            m_RenderTargets[MAX_RENDER_TARGET];
    xD3D9RenderApi*           m_pD3D9RenderApi;
protected:
    xD3D9DepthBuffer*         m_DepthBuffer;
    IDirect3DSurface9*        m_pDepthStencilView;
    int                       m_widht;
    int                       m_height;
    xTextureDesc              m_TextureDesc;
};

class xD3D9WindowRenderTarget : public IRenderTarget
{
    friend class              xD3D9RenderWindow;
	xTextureDesc&             m_TexDesc;
    IDirect3DSurface9*        m_pRenderTargetView;
public:
	int                       RefCount(){return 1 ; }
	int                       AddRef() {return 1 ; }
	int                       ReleaseObject(){return 1 ; }
	int                       KillObject() {return 1 ; }
public:
	xD3D9WindowRenderTarget(IRenderApi* pRenderApi , xTextureDesc& _desc):IRenderTarget(pRenderApi) , m_TexDesc(_desc) {}
	~xD3D9WindowRenderTarget() {}
	IBaseTexture* toTexture() {return NULL ; }
	bool          desc(xTextureDesc& _desc){_desc = m_TexDesc ;  return true;};
	bool          grabRenderTagetData(void* pData , int x , int y , int w , int h ){return false;}
	bool          saveToFile(const wchar_t* fileName){ return false; }
    void*         handle(){ return m_pRenderTargetView ; }
};

class xD3D9RenderWindow : public xD3D9RenderView
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9RenderWindow)
public:
	HWND                     m_hWnd;
	xD3D9WindowRenderTarget  m_WindowRT;
    D3DPRESENT_PARAMETERS    m_d3dpp;
    IDirect3DSwapChain9*     m_pSwapChain;
    int                      m_iSwapChain;
public:
	xD3D9RenderWindow(int iSwapChain ,HWND hWnd , xD3D9RenderApi* pRenderApi);
	~xD3D9RenderWindow();
	bool                    desc(xTextureDesc& _desc);
	virtual bool            destory();
	bool                    create(int w , int h);
	bool                    resize(int w , int h);
    HRESULT                 Present(CONST RECT * pSourceRect,    CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags);
    bool                    NeedResize(int width , int height);
	int                     startIdx(){ return 1;}
	IDirect3DSurface9*      GetDepthStencilView();
    void                    onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
protected:
	bool                    _createRenderTargets();
};

END_NAMESPACE_XEVOL3D
