#ifndef __XEVOL_PARTICLE_EFFECTOR_H__
#define __XEVOL_PARTICLE_EFFECTOR_H__

#include "xParticleSystem.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ IParticleEffector : public IBaseObject
{
public:
    virtual void effect(xParticleInfo& particle , float passedTime) = 0;
};

END_NAMESPACE_XEVOL3D
#endif
