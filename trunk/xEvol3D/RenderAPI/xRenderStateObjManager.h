#ifndef _XRENDERERSAMPLER_MANGER_H_
#define _XRENDERERSAMPLER_MANGER_H_
#include <vector>

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xResPackageMgr.h"

#include "xRenderApiObject.h"


BEGIN_NAMESPACE_XEVOL3D

class  _XEVOL_BASE_API_  xRenderStateObjectLoader : public TResPkgLoader2<IRenderApiState,std::ds_wstring,TPureVirtualResCreator<IRenderApiState> >
{
	IRenderApi*    m_pRenderApi;
public:	
	xRenderStateObjectLoader();
	void           setRenderApi(IRenderApi* pRenderApi);
protected:
	IRenderApiState*  newInstance(const wchar_t* strResName, unsigned long arg);
	void           deleteInstance(IRenderApiState*& pRes);
};

typedef TResHandle   <IRenderApiState , std::ds_wstring , xRenderStateObjectLoader> HRenderState;
typedef TSimpleResMgr<IRenderApiState , std::ds_wstring , xRenderStateObjectLoader> xRenderStateObjMgr;


END_NAMESPACE_XEVOL3D
#endif
