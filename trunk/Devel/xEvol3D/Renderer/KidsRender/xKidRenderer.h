#ifndef __XEVOL3D_KIDS_RENDERER_H__
#define __XEVOL3D_KIDS_RENDERER_H__
#include "../xRenderer.h"
/*********************************************

*********************************************/
BEGIN_NAMESPACE_XEVOL3D
#define KID_RENDERER L"KidRenderer"
class _XEVOL_BASE_API_ xKidBasicRenderTech;
class _XEVOL_BASE_API_ xKidRenderer : public IBaseRenderer
{
	DECL_BASE_OBJECT_DLLSAFE(xKidRenderer);
public:
	xKidRenderer(IRenderApi* pRenderApi);
	~xKidRenderer();
	bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len);
	const wchar_t*    name( ){return KID_RENDERER ; }
	bool              drawPass(int idQueue, xRenderPass* pObject);
	bool              setRenderTech(const wchar_t* techName);
	IRenderTech*      getRenderTech();
	bool              begin(unsigned int passedTime);
	bool              end(unsigned int passedTime);


	xRenderPass*      createPass(const wchar_t* passName);
	xMaterial*        createMaterial(const wchar_t* matName);
	xRenderEffect*    createRenderEffect(const wchar_t* effectName);
protected:
	xKidBasicRenderTech* m_pTech;
};



END_NAMESPACE_XEVOL3D

#endif