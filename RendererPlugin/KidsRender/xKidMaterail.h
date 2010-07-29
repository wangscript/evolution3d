#ifndef _KID_RENDERER_MATERIAL_TECH_
#define _KID_RENDERER_MATERIAL_TECH_
#include "xKidRdrApi.h"
#include "Renderer/xRenderer.h"
#include "Renderer/xMaterial.h"
#include "Renderer/xDrawElement.h"
#include "RenderApi/xShaderManager.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_RENDERER_API_ xKidRenderer;

class _XEVOL_RENDERER_API_ xKidBasicMaterial : public xMaterial
{
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial);
public:
	xKidBasicMaterial(IBaseRenderer* pRenderer);
	~xKidBasicMaterial();
	virtual bool begin(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool end(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool load(const wchar_t* matName , const int8* buf , int len);
    IGpuProgram* gpuProgram() ;
    bool         setGpuProgram(xGpuProgramName& _name);
	bool         setDepthState(const wchar_t* _name) ;
	bool         setRazState(const wchar_t* _name) ;
	bool         setBlendState(const wchar_t* _name);
public:
	virtual bool load(xXmlNode* pNode);
protected:
	HGpuProgram            m_hGpuPrograme;
	IGpuProgramParamTable* m_pOldGpuCbTable;
	IDepthStencilState*    m_pDepthStencil;
	IBlenderState*         m_pBlender;
	IRasterizerState*      m_pRasterizer;
	IRenderApi*            m_pRenderApi;
	float                  m_AlphaRefValue;
};



class _XEVOL_RENDERER_API_ xKidBasicRenderEffect : public IRenderEffect
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicRenderEffect);
	xKidBasicRenderEffect(xKidRenderer* pRenderer , const wchar_t* _name);
	virtual  ~xKidBasicRenderEffect();
public:
	virtual bool drawImm(IDrawElement* pObject , IRenderPassArg* pArg  , unsigned int passedTime);
	virtual bool draw(IDrawElement* pObject , IRenderPassArg* pArg ) ;
	virtual bool setMaterial(xMaterial* pMaterial) ;
	virtual bool load(xXmlNode* pXml);
	virtual bool save(xXmlNode* pXml);
    virtual bool setQueueName(const wchar_t* queueName);
	virtual const wchar_t* name();
    virtual bool           setArg(const wchar_t* _argName , const wchar_t* _argVal);
    virtual const wchar_t* getArg(const wchar_t* _argName) ;
protected:
	IRenderApi*     m_pRenderApi;
	xKidRenderer*   m_pKidRenderer;
	xRenderPass*    m_pRenderPass;
	std::ds_wstring m_QueueName;
    std::ds_wstring m_shaderName;
	std::ds_wstring m_Name;
};



END_NAMESPACE_XEVOL3D
#endif
