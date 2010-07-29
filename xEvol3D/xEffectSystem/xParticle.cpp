#include "../xStdPch.h"
#include "xParticle.h"
BEGIN_NAMESPACE_XEVOL3D;
IMPL_OBJECT_FACTORY_MGR(IParticlePool ,  xParticlePoolCreator , xParticlePoolFactoryMgr )
void  IParticle::update(float passedTime)
{
    m_Info.m_life -= passedTime;
    if(m_Info.m_life < 0.0f )
    {
        ReleaseObject();
        return ;
    }

    m_Info.m_AngleArg.update(passedTime);
    m_Info.m_PosArg.update(passedTime);
    m_Info.m_SizeArg.update(passedTime);
    m_Info.m_ColorArg.update(passedTime);
}

IMPL_BASE_OBJECT_CLASSID(IParticlePool , IBaseObject)
IParticlePool::IParticlePool(IParticleEmitter* pEmitter , int)
{
    m_Emitter = pEmitter;
}

END_NAMESPACE_XEVOL3D

