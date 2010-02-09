#ifndef _KID_RENDERER_MATERIAL_TECH_
#define _KID_RENDERER_MATERIAL_TECH_
#include "../xRenderer.h"
#include "../xMaterial.h"
#include "../xDrawElement.h"
#include "../../RenderApi/xShaderManager.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xKidRenderer;

class _XEVOL_BASE_API_ xKidBasicMaterial : public xMaterial
{
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial);
public:
	xKidBasicMaterial(IRenderApi* pRenderApi);
	~xKidBasicMaterial();
	virtual bool begin(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool end(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool load(const wchar_t* matName , const int8* buf , int len);

public:
	virtual bool load(xXmlNode* pNode);
protected:
	HGpuProgram            m_hGpuPrograme;
	IGpuProgramParamTable* m_pOldGpuCbTable;
	IDepthStencilState*    m_pDepthStencil;
	IBlenderState*         m_pBlender;
	IRasterizerState*      m_pRasterizer;
};



class _XEVOL_BASE_API_ xKidBasicRenderEffect : public IRenderEffect
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicRenderEffect);
	xKidBasicRenderEffect(IRenderApi* pRenderApi , xKidRenderer* pRenderer);
	virtual  ~xKidBasicRenderEffect();
public:
	virtual bool draw(IDrawElement* pObject , unsigned int passedTime);
	virtual bool draw(IDrawElement* pObject) ;
	virtual bool setMaterial(xMaterial* pMaterial) ;
	virtual bool load(xXmlNode* pXml);
	virtual bool save(xXmlNode* pXml);
    virtual bool setQueueName(const wchar_t* queueName);
protected:
	IRenderApi*     m_pRenderApi;
	xKidRenderer*   m_pRenderer;
	xRenderPass*    m_pRenderPass;
	std::ds_wstring m_QueueName;
};



END_NAMESPACE_XEVOL3D
#endif
