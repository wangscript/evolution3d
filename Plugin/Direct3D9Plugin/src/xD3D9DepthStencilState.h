#ifndef _XEVOL_DIRECT3D10_STENCIL_DEPTH_STATE_H_
#define _XEVOL_DIRECT3D10_STENCIL_DEPTH_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
struct xStencilOp
{
    D3DSTENCILOP m_Fail ; //D3DRS_STENCILFAIL ;     D3DRS_CCW_STENCILFAIL  ;
    D3DSTENCILOP m_zFail; //D3DRS_STENCILZFAIL;     D3DRS_CCW_STENCILZFAIL ;
    D3DSTENCILOP m_Pass ; //D3DRS_STENCILPASS ;     D3DRS_CCW_STENCILPASS  ;
    D3DCMPFUNC   m_Func ; //D3DRS_STENCILFUNC ;     D3DRS_CCW_STENCILFUNC  ;

    xStencilOp()
    {
        m_Fail  =  D3DSTENCILOP_KEEP;
        m_zFail =  D3DSTENCILOP_KEEP;
        m_Pass  =  D3DSTENCILOP_KEEP;
        m_Func  =  D3DCMP_ALWAYS;
    }
};

struct xStencilArg
{
    BOOL       m_Enable ;  //D3DRS_STENCILENABLE   ;
    DWORD      m_Ref    ;  //D3DRS_STENCILREF      ;
    DWORD      m_Mask   ;  //D3DRS_STENCILMASK     ;
    DWORD      m_WriteMask;//D3DRS_STENCILWRITEMASK;
    xStencilOp m_OpCW;
    xStencilOp m_OpCCW;

    xStencilArg()
    {
        m_Enable    = FALSE;
        m_Ref       = 0;
        m_Mask      = 0xFFFFFFFF;
        m_WriteMask = 0xFFFFFFFF;
    }
};

class xD3D9ZStencilState : public IDepthStencilState
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9ZStencilState);
	xD3D9RenderApi*          m_pD3D9RenderApi;
	std::wstring             m_name;



    DWORD       m_zEnable       ; //D3DRS_ZENABLE;
    BOOL        m_zWriteable    ; //D3DRS_ZWRITEENABLE;
    D3DCMPFUNC  m_zFunc         ; //D3DRS_ZFUNC ;
    xStencilArg m_Stencil       ; //


	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void         stencil_ref(unsigned int ref) {m_Stencil.m_Ref = (DWORD)ref;}
	unsigned int stencil_ref(){return m_Stencil.m_Ref;}
	void*        handle(){ return NULL ; }
	xD3D9ZStencilState(xD3D9RenderApi* pAPI  );
	~xD3D9ZStencilState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
public:
    void Install(xD3D9ZStencilState* pOldStencil);

};

END_NAMESPACE_XEVOL3D

#endif

