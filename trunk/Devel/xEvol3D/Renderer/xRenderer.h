#ifndef __XEVOL3D_RENDERER_H__
#define __XEVOL3D_RENDERER_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../BaseLib/xObjectFactory.h"
/*********************************************

*********************************************/
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xRendererQueue;
class _XEVOL_BASE_API_ IRenderTech;
class _XEVOL_BASE_API_ xRenderPass;
class _XEVOL_BASE_API_ xMaterial;
class _XEVOL_BASE_API_ xRenderEffect;

class _XEVOL_BASE_API_ IBaseRenderer : public IBaseObject
{
      DECL_BASE_OBJECT_DLLSAFE(IBaseRenderer);
public:
	  virtual IRenderApi*       renderApi() const { return m_pRenderApi ; }
	  virtual ~IBaseRenderer(){};
	  IBaseRenderer(IRenderApi* pApi) : m_pRenderApi(pApi){};
	  
      virtual bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len);
      virtual size_t            nRenderQueue() const ;
      virtual xRendererQueue*   RenderQueueByName(int QueueName) const ;
      virtual xRendererQueue*   RenderQueueByName(const wchar_t* QueueName) const ;
      virtual xRendererQueue*   RenderQueueByIndex(size_t iQueue) const ;


	  virtual const wchar_t*    name( )  = 0;
      virtual bool              drawPass(int idQueue, xRenderPass* pObject) = 0;
      virtual bool              setRenderTech(const wchar_t* techName) = 0;
      virtual IRenderTech*      getRenderTech() = 0;
      virtual bool              begin(unsigned int passedTime) = 0;
      virtual bool              end(unsigned int passedTime) = 0;


	  virtual xRendererQueue*   createQueue(xXmlNode* pCfg) ;
      virtual xRenderPass*      createPass(const wchar_t* passName) = 0;
      virtual xMaterial*        createMaterial(const wchar_t* matName) = 0 ;
      virtual xRenderEffect*    createRenderEffect(const wchar_t* effectName) = 0;
protected:
	xRendererQueue*             __findQueue(int _idName)  const;
	xRendererQueue*             __findQueue(const wchar_t* rqName)  const;
protected:
	typedef ds_map(int , xRendererQueue*)    xRendererQueueMaps;
	typedef ds_vector(xRendererQueue*)       xRendererQueueVec;
	xRendererQueueMaps                       m_mapQueues;
	xRendererQueueVec                        m_vQueues;
	IRenderApi*                              m_pRenderApi;
};


class _XEVOL_BASE_API_ IRenderTech : public IRenderObject
{
public:
	virtual void           setRenderer(IBaseRenderer* pRenderer) = 0;
	virtual IBaseRenderer* getRenderer() = 0;
	virtual bool           commitRenderer(unsigned int passedTime) = 0;
public:
	DECL_BASE_OBJECT_DLLSAFE(IRenderTech);
protected:
	IRenderTech(IRenderApi* pRenderApi):IRenderObject(pRenderApi) {};
	virtual ~IRenderTech(){};
};


class _XEVOL_BASE_API_ IRendererCreator
{
public:
	virtual const wchar_t*   name() = 0;													                    
	virtual IBaseRenderer*   createInstance(IRenderApi* param1 , int param2) = 0;					            
	virtual const wchar_t *  desc() = 0;     
};

class _XEVOL_BASE_API_ xRendererManager
{
      xRendererManager();
public:
      static xRendererManager* singleton();
      bool                     registeRenderer(IRendererCreator* pRendererCreator);
      bool                     unregisteRenderer(IRendererCreator* pRendererCreator);
      IBaseRenderer*           createRenderer(const wchar_t* name , IRenderApi* pRenderApi);
};

END_NAMESPACE_XEVOL3D

#endif