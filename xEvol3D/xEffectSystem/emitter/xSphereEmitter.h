#ifndef __XEVOL_SPHRERE_EMITTER_H__
#define __XEVOL_SPHRERE_EMITTER_H__
#include "../xParticle.h"
#include "../xParticleEmitter.h"
#include "../../RenderAPI/xRenderAPI.h"

BEGIN_NAMESPACE_XEVOL3D;

class xSphereEmitter : public IParticleEmitter
{
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(xSphereEmitter);
    xSphereEmitter(xParticleSystem* pParticleSystem , int arg);
    bool                 load(xXmlNode* pXml);
    bool                 save(xXmlNode* pXml);
    bool                 init(const wchar_t* poolType);
    void                 update(float passedTime);
    xMathLib::xvec3      emittePos();
protected:
    float                m_R;

};

END_NAMESPACE_XEVOL3D;

#endif
