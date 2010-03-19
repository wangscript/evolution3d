#ifndef _XEVOL_DIRECT3D10_RAZ_STATE_H_
#define _XEVOL_DIRECT3D10_RAZ_STATE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3D9RazState : public IRasterizerState
{
	IMPL_BASE_OBJECT_INTERFACE(xD3D9RazState);
	xD3D9RenderApi*   m_pD3D9RenderApi;
	std::wstring      m_name;

    D3DFILLMODE       m_FillMode      ;// D3DRS_FILLMODE           ;
    D3DSHADEMODE      m_ShadeMode     ;// D3DRS_SHADEMODE          ;
    D3DCULL           m_CullMode      ;// D3DRS_CULLMODE           ;
    DWORD             m_SlopScaleZbias;// D3DRS_SLOPESCALEDEPTHBIAS;   bias = (max * D3DRS_SLOPESCALEDEPTHBIAS) + D3DRS_DEPTHBIAS.
    float             m_ZBias         ;// D3DRS_DEPTHBIAS          ;
    D3DCMPFUNC        m_AlphaFunc     ;// D3DRS_ALPHAFUNC          ;
    DWORD             m_LineAntiAlias ;// D3DRS_ANTIALIASEDLINEENABLE


	bool                      _destory() ; 
	bool                      _load(xXmlNode* node);
public:
	void*         handle(){ return NULL; }
	xD3D9RazState(xD3D9RenderApi* pAPI  );
	~xD3D9RazState();
	bool          load(const wchar_t* fileName , unsigned long  arg);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
	unsigned long memUsage();
    void          Install(xD3D9RazState* pRazState);
};

END_NAMESPACE_XEVOL3D

#endif

