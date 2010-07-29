#include "../../xStdPch.h"
#include "xBoxEmitter.h"
BEGIN_NAMESPACE_XEVOL3D;
xBoxEmitter::xBoxEmitter(xParticleSystem* pParticleSystem , int arg):IParticleEmitter(pParticleSystem , arg)
{
    m_BoxSize = xMathLib::xvec3(400.0f,400.0f,1.0f);
    m_EmitterInfo.m_PoolType = L"xQuadParticlePool";
    return ;
}


bool xBoxEmitter::init(const wchar_t* poolType)
{
    return IParticleEmitter::init(poolType);
}

bool xBoxEmitter::load(xXmlNode* pXml)
{
    bool bRet = IParticleEmitter::load(pXml);
    if(bRet)
    {
		nsParticleSystem::getVec3(pXml , L"BoxSize" , m_BoxSize);
    }
    return bRet;    
}

bool xBoxEmitter::save(xXmlNode* pXml)
{
    IParticleEmitter::save(pXml);
    pXml->setValue(L"type" , L"xBoxEmitter");
    nsParticleSystem::setVec3(pXml,L"BoxSize",m_BoxSize);    
    return true;
}

void xBoxEmitter::update(float passedTime)
{
    IParticleEmitter::update(passedTime);
}

xMathLib::xvec3 xBoxEmitter::emittePos()
{
     nsParticleSystem::Vec3Range _range ;
	 _range.setRange( xMathLib::xvec3(-0.5f , -0.5 , -0.5f) , xMathLib::xvec3(0.5f , 0.5 , 0.5f) );
     xMathLib::xvec3 _pos = m_Info.m_PosArg.m_Value + _range.rand() * m_BoxSize ;
     return _pos;
}

IMPL_PARTICLEEMITTER_FACTORY(xBoxEmitter);

END_NAMESPACE_XEVOL3D;