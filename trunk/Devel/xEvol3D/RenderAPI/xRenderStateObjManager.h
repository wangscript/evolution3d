#ifndef _XRENDERERSAMPLER_MANGER_H_
#define _XRENDERERSAMPLER_MANGER_H_
#include <vector>

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xResPackageMgr.h"

#include "xRenderObject.h"


BEGIN_NAMESPACE_XEVOL3D

class  _XEVOL_BASE_API_  xRenderStateObjectLoader : public TResPkgLoader2<IRenderState,std::ds_wstring,TPureVirtualResCreator<IRenderState> >
{
	IRenderApi*    m_pRenderApi;
public:	
	xRenderStateObjectLoader();
	void           setRenderApi(IRenderApi* pRenderApi);
protected:
	IRenderState*  newInstance(const wchar_t* strResName, unsigned long arg);
	void           deleteInstance(IRenderState*& pRes);
};

typedef TResHandle   <IRenderState , std::ds_wstring , xRenderStateObjectLoader> HRenderState;
typedef TSimpleResMgr<IRenderState , std::ds_wstring , xRenderStateObjectLoader> xRenderStateObjMgr;


END_NAMESPACE_XEVOL3D
#endif
