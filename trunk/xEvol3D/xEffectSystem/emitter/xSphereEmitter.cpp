#include "../../xStdPch.h"
#include "xSphereEmitter.h"
BEGIN_NAMESPACE_XEVOL3D;
xSphereEmitter::xSphereEmitter(xParticleSystem* pParticleSystem , int arg):IParticleEmitter(pParticleSystem , arg)
{
    m_R = 3.0f;
    m_EmitterInfo.m_PoolType = L"xQuadParticlePool";
    return ;
}


bool xSphereEmitter::init(const wchar_t* poolType)
{
    return IParticleEmitter::init(poolType);
}

bool xSphereEmitter::load(xXmlNode* pXml)
{
    bool bRet = IParticleEmitter::load(pXml);
    if(bRet)
    {
        m_R = pXml->float_value(L"Radius");
    }
    return bRet;    
}

bool xSphereEmitter::save(xXmlNode* pXml)
{
    IParticleEmitter::save(pXml);
    pXml->setValue(L"type" , L"xSphereEmitter");
    pXml->setValue(L"Radius" , m_R );    
    return true;
}

void xSphereEmitter::update(float passedTime)
{
    IParticleEmitter::update(passedTime);
}

xMathLib::xvec3 xSphereEmitter::emittePos()
{
     nsParticleSystem::Vec3Range _range;
     _range.setMax( xMathLib::xvec3( m_R ,  m_R ,  m_R ));
     _range.setMin( xMathLib::xvec3(-m_R , -m_R , -m_R ));

     xMathLib::xvec3 _pos = m_Info.m_PosArg.m_Value + _range.rand() ;
     return _pos;
}

IMPL_PARTICLEEMITTER_FACTORY(xSphereEmitter);

END_NAMESPACE_XEVOL3D;