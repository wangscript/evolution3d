#pragma  once 
#include <RenderAPI/xRenderView.h>
#include <RenderAPI/xBaseTexture.h>
#include "../xDirect3D11DllApi.h"
#define MAX_RENDER_TARGET 8
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
class xD11DepthTexture;
class xD11RenderView : public IRenderView
{
	IMPL_BASE_OBJECT_INTERFACE(xD11RenderView)
protected:
	ID3D11DepthStencilView*        depthView();
    bool                           setupDepthView(int w , int h , bool bSetAsDepthBuffer);
public:
    xD11RenderView(xD3D11RenderApi* pRenderApi , const xRTSampleDesc& sampleDesc);
    virtual ~xD11RenderView();
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
	bool                 setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx, size_t iSlice = 0 , size_t iMipMapLevel =0 );
	bool                 setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx);
	bool                 setDepthBuffer(IRenderTarget* pDepthBuffer);
    bool                 setDepthBuffer(IBaseTexture* pDepthTexture);
	int                  addRenderTarget(IBaseTexture*  pTexture, size_t iSlice , size_t iMipMapLevel);
	int                  addRenderTarget(IRenderTarget* pRenderTarget);
	IRenderTarget*       depthBuffer();
    bool                 depthBufferDesc(XD3D11_TEXTURE2D_DESC& _desc );
	IRenderApiObject*    renderTarget(size_t rtIdx );

protected:
    ID3D11RenderTargetView*        m_RenderTargetsView[MAX_RENDER_TARGET];
    IRenderTarget*                 m_RenderTargets[MAX_RENDER_TARGET];
    xD3D11RenderApi*               m_pD11RenderApi;
protected:
    xD11DepthTexture*              m_DepthTexture;
    IRenderTarget*                 m_DepthBuffer;
    ID3D11DepthStencilView*        m_pDepthStencilView;
    int                            m_widht;
    int                            m_height;
    xTextureDesc                   m_TextureDesc;
	DXGI_SAMPLE_DESC               m_DXGISampleDesc;
};

class xD11WindowRenderTarget : public IRenderTarget
{
    friend class              xD11RenderWindow;
	xTextureDesc&             m_TexDesc;
public:
	int                       RefCount(){return 1 ; }
	int                       AddRef() {return 1 ; }
	int                       ReleaseObject(){return 1 ; }
	int                       KillObject() {return 1 ; }
public:
	xD11WindowRenderTarget(IRenderApi* pRenderApi , xTextureDesc& _desc):IRenderTarget(pRenderApi) , m_TexDesc(_desc) {}
	~xD11WindowRenderTarget() {}
	IBaseTexture* toTexture() {return NULL ; }
	bool          desc(xTextureDesc& _desc){_desc = m_TexDesc ;  return true;};
	bool          grabRenderTagetData(void* pData , int x , int y , int w , int h ){return false;}
	bool          saveToFile(const wchar_t* fileName){ return false; }
};

class xD11RenderWindow : public xD11RenderView
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11RenderWindow)
public:
	HWND                    m_hWnd;
	IDXGISwapChain*         m_pSwapChain;
    DXGI_SWAP_CHAIN_DESC    m_SwapChainDesc;
	xD11WindowRenderTarget  m_WindowRT;
public:
	xD11RenderWindow(HWND hWnd , xD3D11RenderApi* pRenderApi);
	~xD11RenderWindow();
	bool                    desc(xTextureDesc& _desc);
	virtual bool            destory();
	bool                    create(IDXGISwapChain* pSwapChain , int w , int h);
	bool                    resize(int w , int h);
	int                     startIdx(){ return 1;}
    bool                    Present(UINT syncInterval , UINT Flags);
    bool                    NeedResize(int width , int height );
    bool                    GetSwapChainDesc( DXGI_SWAP_CHAIN_DESC& swap_desc);
    DXGI_SWAP_CHAIN_DESC&   SwapChainDesc();
	ID3D11DepthStencilView* GetDepthStencilView(){ return m_pDepthStencilView ; }
protected:
	bool                    _createRenderTargets();
};

END_NAMESPACE_XEVOL3D
