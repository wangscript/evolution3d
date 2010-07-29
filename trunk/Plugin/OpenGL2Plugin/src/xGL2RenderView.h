#pragma  once 
#include "../GL2.h"
#include <RenderAPI/xRenderView.h>
#include <RenderAPI/xBaseTexture.h>
#define MAX_RENDER_TARGET 8
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
class xGL2DepthTexture;
class xGL2RenderView : public IRenderView
{
	IMPL_BASE_OBJECT_INTERFACE(xGL2RenderView)
public:
    xGL2RenderView(IRenderApi* pRenderApi);
    virtual ~xGL2RenderView();
	bool               clear(xColor_4f& bkColor);
	bool               clear(xColor_4f& bkColor, int nClear);
    bool               clear(xColor_4f& bkColor , float z , unsigned int stencil) ;
    bool               clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear);
    bool               install();
    virtual int        startIdx() { return 0 ; }
    int                nRenderTargetView();
    bool               createDepthView(int w , int h);
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
    bool              depthBufferDesc(xTextureDesc& _desc );
	IRenderApiObject*    renderTarget(size_t rtIdx );

protected:
    IRenderTarget*               m_RenderTargets[MAX_RENDER_TARGET];
    xGL2RenderApi*               m_pGL2Api;
protected:
    xGL2DepthTexture*            m_DepthTexture;
    IRenderTarget*               m_DepthBuffer;
    int                          m_widht;
    int                          m_height;
    xTextureDesc                 m_TextureDesc;
};

class xGL2WindowRenderTarget : public IRenderTarget
{
    friend class              xGL2RenderWindow;
	xTextureDesc&             m_TexDesc;
public:
	int                       RefCount(){return 1 ; }
	int                       AddRef() {return 1 ; }
	int                       ReleaseObject(){return 1 ; }
	int                       KillObject() {return 1 ; }
public:
	xGL2WindowRenderTarget(IRenderApi* pRenderApi , xTextureDesc& _desc):IRenderTarget(pRenderApi) , m_TexDesc(_desc) {}
	~xGL2WindowRenderTarget() {}
	IBaseTexture* toTexture() {return NULL ; }
	bool          desc(xTextureDesc& _desc){_desc = m_TexDesc ;  return true;};
	bool          grabRenderTagetData(int x , int y , int w , int h , void* pData){return false;}
};

class xGL2RenderWindow : public xGL2RenderView
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2RenderWindow)
public:
	xGL2RenderWindow(HWND hWnd , IRenderApi* pRenderApi);
	~xGL2RenderWindow();
	bool                    desc(xTextureDesc& _desc);
	virtual bool            destory();
	bool                    resize(int w , int h);
	int                     startIdx();
    void                    makeCurrent();
    void                    swapBuffer();
protected:
	bool                    _createRenderTargets();

public:
    xGL2WindowRenderTarget  m_WindowRT;
    xTextureDesc            m_TexDesc;
    HWND                    m_hWnd;
#ifdef _WIN32    
    HDC                     m_hDC;
    HGLRC                   m_hRC;
#endif
    
};

END_NAMESPACE_XEVOL3D
