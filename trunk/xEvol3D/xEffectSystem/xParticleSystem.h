#ifndef __XEVOL_PARTICLE_SYSTEM_H__
#define __XEVOL_PARTICLE_SYSTEM_H__
#include "xParticleData.h"

BEGIN_NAMESPACE_XEVOL3D;
class _XEVOL_BASE_API_ xParticleInfo;
class _XEVOL_BASE_API_ IParticle ;
class _XEVOL_BASE_API_ IParticlePool;
class _XEVOL_BASE_API_ IParticleEmitter;
class _XEVOL_BASE_API_ IParticleEffector;

class _XEVOL_BASE_API_ xParticleSystem : public IBaseObject
{
public:
    DECL_BASE_OBJECT_INTERFACE(xParticleSystem);
public:
    
    IParticleEmitter*  createEmitter(xXmlNode* pXml);
    IParticleEmitter*  createEmitter(const wchar_t* _type);
    bool               releaseEmitter(IParticleEmitter* pEmitter);
    bool               load(xXmlNode* pXml);
	void               update(float passedTime);
	void               render(float passedTime);
	int                nEmitter();
	IParticleEmitter*  operator[](int idx);
	void               startAll();
	void               stopAll();
public:
    IRenderApi*        renderApi();
    IBaseRenderer*     renderer();
    xBaseTextureMgr*   texManager(){ return m_pTextureMgr ; }

    ~xParticleSystem();
    static xParticleSystem*  createInstance(IBaseRenderer* pBaseRenderer , xBaseTextureMgr* pTexMgr);
protected:
     xParticleSystem(IBaseRenderer* pBaseRenderer , xBaseTextureMgr* pTexMgr);     
public:
    typedef ds_vector(IParticleEmitter*)  vParticleEmitters;
protected:
    IRenderApi*        m_RenderApi;
    IBaseRenderer*     m_Renderer;
    vParticleEmitters  m_ParticleEmitters;
    xBaseTextureMgr*   m_pTextureMgr;
};

END_NAMESPACE_XEVOL3D
#endif
