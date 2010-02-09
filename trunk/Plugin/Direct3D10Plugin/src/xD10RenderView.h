#pragma  once 
#include <RenderAPI/xRenderView.h>
#include <D3D10.h>
#include <d3d10shader.h>
#include <RenderAPI/xBaseTexture.h>
#define MAX_RENDER_TARGET 8
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10DepthTexture;
class xD10RenderView : public IRenderView
{
	IMPL_BASE_OBJECT_INTERFACE(xD10RenderView)
protected:
	ID3D10DepthStencilView*        depthView();
    bool                           setupDepthView(int w , int h , bool bSetAsDepthBuffer);
public:
    xD10RenderView(xD3D10RenderApi* pRenderApi , const xRTSampleDesc& desc);
    virtual ~xD10RenderView();
	bool               clear(xColor_4f& bkColor);
	bool               clear(xColor_4f& bkColor, int nClear);
	bool               clearRenderTarget(xColor_4f& bkColor, int idx);
    bool               clear(xColor_4f& bkColor , float z , unsigned int stencil) ;
    bool               clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear);
    bool               install();
    virtual int        startIdx() { return 0 ; }
    int                nRenderTargetView();
    bool               createDepthView(int w , int h );
	bool               desc(xTextureDesc& _desc){ _desc = m_TextureDesc ; return true;}
    bool               setup();
	virtual bool       destory();
public:
	bool              resize(int w , int h);
	bool              resize(int w , int h  , bool destoryRT);
	bool              createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture);
	bool              setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx);
	bool              setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx);
	bool              setDepthBuffer(IRenderTarget* pDepthBuffer);
    bool              setDepthBuffer(IBaseTexture* pDepthTexture);
	int               addRenderTarget(IBaseTexture*  pTexture);
	int               addRenderTarget(IRenderTarget* pRenderTarget);
	IRenderTarget*    depthBuffer();
    bool                 depthBufferDesc(D3D10_TEXTURE2D_DESC& desc );
	IRenderApiObject*    renderTarget(size_t rtIdx );

protected:
    ID3D10RenderTargetView*        m_RenderTargetsView[MAX_RENDER_TARGET];
    IRenderTarget*                 m_RenderTargets[MAX_RENDER_TARGET];
    xD3D10RenderApi*               m_pD10Api;
protected:
    xD10DepthTexture*              m_DepthTexture;
    IRenderTarget*                 m_DepthBuffer;
    ID3D10DepthStencilView*        m_pDepthStencilView;
    int                            m_widht;
    int                            m_height;
    xTextureDesc                   m_TextureDesc;
	DXGI_SAMPLE_DESC               m_DXGISampleDesc;
};

class xD10WindowRenderTarget : public IRenderTarget
{
    friend class              xD10RenderWindow;
	xTextureDesc&             m_TexDesc;
public:
	int                       RefCount(){return 1 ; }
	int                       AddRef() {return 1 ; }
	int                       ReleaseObject(){return 1 ; }
	int                       KillObject() {return 1 ; }
public:
	xD10WindowRenderTarget(IRenderApi* pRenderApi , xTextureDesc& _desc):IRenderTarget(pRenderApi) , m_TexDesc(_desc) {}
	~xD10WindowRenderTarget() {}
	IBaseTexture* toTexture() {return NULL ; }
	bool          desc(xTextureDesc& _desc){_desc = m_TexDesc ;  return true;};
	bool          grabRenderTagetData(int x , int y , int w , int h , void* pData){return false;}
	bool          saveToFile(const wchar_t* fileName){ return false; }
};

class xD10RenderWindow : public xD10RenderView
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10RenderWindow)
public:
	HWND                    m_hWnd;
	IDXGISwapChain*         m_pSwapChain;
	xD10WindowRenderTarget  m_WindowRT;
public:
	xD10RenderWindow(HWND hWnd , xD3D10RenderApi* pRenderApi);
	~xD10RenderWindow();
	bool                    desc(xTextureDesc& _desc);
	virtual bool            destory();
	bool                    create(IDXGISwapChain* pSwapChain , int w , int h);
	bool                    resize(int w , int h);
	int                     startIdx(){ return 1;}
	ID3D10DepthStencilView* GetDepthStencilView(){ return m_pDepthStencilView ; }
protected:
	bool                    _createRenderTargets();
};

END_NAMESPACE_XEVOL3D
