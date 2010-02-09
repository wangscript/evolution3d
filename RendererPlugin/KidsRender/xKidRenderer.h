#ifndef __XEVOL3D_KIDS_RENDERER_H__
#define __XEVOL3D_KIDS_RENDERER_H__
#include "xKidRdrApi.h"
#include "Renderer/xRenderer.h"
#include "BaseLib/xXmlManager.h"

/*********************************************

*********************************************/
BEGIN_NAMESPACE_XEVOL3D
#define KID_RENDERER L"KidRenderer"
class _XEVOL_RENDERER_API_ xKidBasicRenderTech;
class _XEVOL_RENDERER_API_ xKidRenderer : public IBaseRenderer
{
	DECL_BASE_OBJECT_DLLSAFE(xKidRenderer);
public:
	xKidRenderer(IRenderApi* pRenderApi);
	~xKidRenderer();
	bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len);
	const wchar_t*    name( ){return KID_RENDERER ; }
	bool              drawPass(int idQueue, xRenderPass* pObject);
	bool              drawPass(xRenderPass* pObject , unsigned int passedTime , bool bDrawImm = true);
	bool              setRenderTech(const wchar_t* techName);
	IRenderTech*      getRenderTech();
	bool              begin(unsigned int passedTime);
	bool              end(unsigned int passedTime);


	xRenderPass*      createPass(const wchar_t* passName);
	xMaterial*        createMaterial(const wchar_t* matName);
	IRenderEffect*    createRenderEffect(const wchar_t* effectName);
	size_t            nRenderEffect() ;
	const wchar_t*    renderEffectName(int idx) ;
	size_t            nMaterial();
	const wchar_t*    materialName(int idx);
protected:
	xKidBasicRenderTech* m_pTech;
	xXMLDocMgr           m_XMLMgr;
};



END_NAMESPACE_XEVOL3D

#endif