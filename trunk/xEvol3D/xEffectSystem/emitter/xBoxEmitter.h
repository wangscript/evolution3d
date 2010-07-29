#ifndef __XEVOL_BOX_EMITTER_H__
#define __XEVOL_BOX_EMITTER_H__
#include "../xParticle.h"
#include "../xParticleEmitter.h"
#include "../../RenderAPI/xRenderAPI.h"

BEGIN_NAMESPACE_XEVOL3D;

class xBoxEmitter : public IParticleEmitter
{
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(xBoxEmitter);
    xBoxEmitter(xParticleSystem* pParticleSystem , int arg);
    bool                 load(xXmlNode* pXml);
    bool                 save(xXmlNode* pXml);
    bool                 init(const wchar_t* poolType);
    void                 update(float passedTime);
    xMathLib::xvec3      emittePos();
protected:
    xMathLib::xvec3      m_BoxSize;

};

END_NAMESPACE_XEVOL3D;

#endif
