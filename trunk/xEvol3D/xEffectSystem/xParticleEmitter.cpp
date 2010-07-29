#include "../xStdPch.h"
#include "xParticle.h"
#include "xParticleEmitter.h"
BEGIN_NAMESPACE_XEVOL3D;
IMPL_OBJECT_FACTORY_MGR(IParticleEmitter ,  xParticleEmitterCreator , xParticleEmitterFactoryMgr )
IParticleEmitter::IParticleEmitter(xParticleSystem* pParticleSystem , int arg)
{
    m_pParticleSystem = pParticleSystem;
    m_Pool            = NULL;
    m_PassedTime      = 0;
    m_EmittedParticle = 0;
    m_PassedTime      = 0;
    m_isEmitter       = false;
    m_fCountNeedGen   = 0;

    memset( &m_Info , 0 , sizeof(m_Info) );
}

bool IParticleEmitter::init(const wchar_t* poolType)
{
    if(poolType) m_EmitterInfo.m_PoolType = poolType;

    m_Pool = xParticlePoolFactoryMgr::singleton()->createInstance(m_EmitterInfo.m_PoolType.c_str() , this , 0 );

    if(m_Pool)
    {
         m_Pool->load(NULL);
    }
    return m_Pool != NULL;
}

void    IParticleEmitter::pushEffector(IParticleEffector* pEffector)
{
    m_vEffectors.push_back(pEffector);
}

void    IParticleEmitter::popEffector()
{
    m_vEffectors.pop_back();
}

int     IParticleEmitter::nEffector()
{
    return (int)m_vEffectors.size();
}

IParticleEffector* IParticleEmitter::getEffector(int idx)
{
    return m_vEffectors[idx];
}

void IParticleEmitter::start()
{
    m_EmittedParticle = 0;
    m_PassedTime = 0;
    m_isEmitter = true;
    m_fCountNeedGen = 0;
}

void IParticleEmitter::stop()
{
       m_isEmitter = false;
}

bool IParticleEmitter::is_active()
{
    if(m_EmitterInfo.m_EmitterType == nsParticleSystem::ET_INSTANT)
    {
        return m_isEmitter || (m_EmittedParticle < m_EmitterInfo.m_EmitterFrequecy.m_Max);
    }
    return m_isEmitter;
}

void IParticleEmitter::emitte(int n)
{
    for(int i= 0;i < n&&_emitte();i++);
}

bool IParticleEmitter::_emitte()
{
    IParticle* p = m_Pool->createParticle();

    //位置
    p->m_Info.m_PosArg.m_Value = emittePos();
    p->m_Info.m_PosArg.m_Velocity   = m_EmitterInfo.m_VelocityRange.rand();
    p->m_Info.m_PosArg.m_Accellator = m_EmitterInfo.m_AcellRange.rand();

    //角度
    p->m_Info.m_AngleArg.m_Value      = xMathLib::xvec3(0.0f , 0.0f , 0.0f);
    p->m_Info.m_AngleArg.m_Velocity   = m_EmitterInfo.m_AngleVelRange.rand();
    p->m_Info.m_AngleArg.m_Accellator = m_EmitterInfo.m_AngleAcellRange.rand();

    //颜色
    p->m_Info.m_ColorArg.m_Value      = m_EmitterInfo.m_ColorRange.rand();
    if(m_EmitterInfo.m_colorChangeType == nsParticleSystem::CHANGE_UNIFORM)
        p->m_Info.m_ColorArg.m_Velocity = m_EmitterInfo.m_ColorVelRange.rand_unifrom();
    else 
        p->m_Info.m_ColorArg.m_Velocity   = m_EmitterInfo.m_ColorVelRange.rand();



    //大小和缩放
    p->m_Info.m_SizeArg.m_Value      = m_EmitterInfo.m_SizeRange.rand();
    p->m_Info.m_SizeArg.m_Velocity   = m_EmitterInfo.m_SizeVelRange.rand();
    p->m_Info.m_SizeArg.m_Accellator = m_EmitterInfo.m_SizeAcellRange.rand();


    //生命
    p->m_Info.m_life  = p->m_Info.m_TotalLife = m_EmitterInfo.m_LifeRange.rand();
    
	if(m_Pool)
	{
		m_Pool->onEmitte(p);
	}
    return true;
}

void  IParticleEmitter::update(float passedTime)
{
    //////////////////////////////////////////////////////////////////////////

    float fn_gen = 0;
    m_PassedTime += passedTime;
    switch (m_EmitterInfo.m_EmitterType)
    {
    case nsParticleSystem::ET_CONTINUE:
        {
			if(m_isEmitter)
			{
				int n = m_EmitterInfo.m_EmitterFrequecy.m_Max;
				fn_gen = (n * passedTime);//计算出来．这个Frame里要产生多少个粒子．
				fn_gen += m_fCountNeedGen;
				n = int(fn_gen);
				m_fCountNeedGen = fn_gen - n;
				for(int i= 0;i < n&&_emitte();i++);
				if(n==0)n=1;
				m_PassedTime = 0;
			}
        }
        break;
    case nsParticleSystem::ET_INTERVAL:
        {
            if(m_isEmitter)//处于发射状态
            {
                if(m_PassedTime < m_EmitterInfo.m_InterValTime.m_Min)
                {
                    fn_gen  = (m_EmitterInfo.m_EmitterFrequecy.m_Max * passedTime  /  m_EmitterInfo.m_InterValTime.m_Min);
                    fn_gen += m_fCountNeedGen;
                    int n = int(fn_gen);
                    m_fCountNeedGen = fn_gen - n;

                    for(int i= 0;i < n&&_emitte();i++);
                }
                else
                {
                    m_PassedTime -= m_EmitterInfo.m_InterValTime.m_Min;
                    m_isEmitter = false;
                }
            }
            else //不是处于发射状态
            {
                if(m_PassedTime > m_EmitterInfo.m_InterValTime.m_Max)
                { 
                    m_PassedTime -= m_EmitterInfo.m_InterValTime.m_Max;
                    m_isEmitter = true;
                }
            }
        }//end case ET_INTERVAL
        break;

    case nsParticleSystem::ET_INSTANT:
        {
            if(m_PassedTime < m_EmitterInfo.m_InterValTime.m_Min)
            {
                fn_gen = m_EmitterInfo.m_EmitterFrequecy.m_Max * passedTime /  m_EmitterInfo.m_InterValTime.m_Min;//计算出来．这个Frame里要产生多少个粒子．
                fn_gen += m_fCountNeedGen;
                int n = int(fn_gen);
                m_fCountNeedGen = fn_gen - n;
                for(int i= 0;i < n&&_emitte();i++);
            }
            else
            {
                m_isEmitter = false;
            }
        }
        break;

    }//End switch



    //////////////////////////////////////////////////////////////////////////
    if(m_vEffectors.size() == 0 )
    {
        m_Pool->update(NULL, passedTime);
    }
    else
    {
        size_t n  = m_vEffectors.size();
        for(size_t i = 0 ; i < n ; i ++)
        {
           m_Pool->update( m_vEffectors[i] , passedTime);
        }
    }
}

bool IParticleEmitter::load(xXmlNode* pXml)
{
    xXmlNode* pPosNode = pXml->findNode(L"pos");
    if(pPosNode)
    {
        nsParticleSystem::getVec3     ( pPosNode , L"Position"      , m_Info.m_PosArg.m_Value);
        nsParticleSystem::getVec3     ( pPosNode , L"Velocity"      , m_Info.m_PosArg.m_Velocity);
        nsParticleSystem::getVec3     ( pPosNode , L"Acellator"     , m_Info.m_PosArg.m_Accellator);
        nsParticleSystem::getVec3Range( pPosNode , L"VelocityRange" , m_EmitterInfo.m_VelocityRange);
        nsParticleSystem::getVec3Range( pPosNode , L"AcellateRange" , m_EmitterInfo.m_AcellRange);
    }


    xXmlNode* pColorNode = pXml->findNode(L"color");
    if(pColorNode)
    {
        nsParticleSystem::getColorRange( pColorNode , L"ColorRange"    , m_EmitterInfo.m_ColorRange);
        nsParticleSystem::getColorRange( pColorNode , L"AcellateRange" , m_EmitterInfo.m_ColorVelRange);
        const wchar_t * strColorChangeType = pColorNode->value(L"ChangeType");
        if(strColorChangeType == NULL || strColorChangeType == std::wstring(L"Uniform"))
            m_EmitterInfo.m_colorChangeType = nsParticleSystem::CHANGE_UNIFORM;
        else
            m_EmitterInfo.m_colorChangeType = nsParticleSystem::CHANGE_INDEPEDENT;
    }


    xXmlNode* pAngleNode = pXml->findNode(L"angle");
    if(pAngleNode)
    {
        nsParticleSystem::getVec3Range( pAngleNode , L"VelocityRange" , m_EmitterInfo.m_AngleVelRange);
        nsParticleSystem::getVec3Range( pAngleNode , L"AcellateRange" , m_EmitterInfo.m_AngleAcellRange);
    }


    xXmlNode* pSizeNode = pXml->findNode(L"size");
    if(pSizeNode)
    {
        nsParticleSystem::getVec3Range( pSizeNode , L"Range" , m_EmitterInfo.m_SizeRange);
        nsParticleSystem::getVec3Range( pSizeNode , L"VelocityRange" , m_EmitterInfo.m_SizeVelRange);
        nsParticleSystem::getVec3Range( pSizeNode , L"AcellateRange" , m_EmitterInfo.m_SizeAcellRange);
    }


    nsParticleSystem::getIntRange   ( pXml , L"Frequecy"     , m_EmitterInfo.m_EmitterFrequecy);
    nsParticleSystem::getScalarRange( pXml , L"LifeRange"    , m_EmitterInfo.m_LifeRange);
    nsParticleSystem::getScalarRange( pXml , L"InterValTime" , m_EmitterInfo.m_InterValTime);

    //读取EmitteType;
    const wchar_t* _EmitteType = pXml->value(L"EmitteType");
    if(_EmitteType == NULL) _EmitteType = L"Continue";
    if(     _EmitteType == std::wstring(L"Continue") ) m_EmitterInfo.m_EmitterType = nsParticleSystem::ET_CONTINUE;
    else if(_EmitteType == std::wstring(L"Interval") ) m_EmitterInfo.m_EmitterType = nsParticleSystem::ET_INTERVAL;
    else if(_EmitteType == std::wstring(L"Instant") )  m_EmitterInfo.m_EmitterType = nsParticleSystem::ET_INSTANT;

    if(pXml->value(L"PoolType") != NULL)
        m_EmitterInfo.m_PoolType = pXml->value(L"PoolType");

    //创建Pool;
    XSAFE_RELEASEOBJECT(m_Pool);
    m_Pool = xParticlePoolFactoryMgr::singleton()->createInstance(m_EmitterInfo.m_PoolType.c_str() , this , 0 );  

    //初始化Pool;
    if(m_Pool)
    {
        m_Pool->load(pXml->findNode(m_EmitterInfo.m_PoolType.c_str()));
    }
    return true;
}

bool IParticleEmitter::save(xXmlNode* pXml)
{
    //速度
    xXmlNode* pPosNode = pXml->insertNode(L"pos");
    nsParticleSystem::setVec3     ( pPosNode , L"Position"      , m_Info.m_PosArg.m_Value);
    nsParticleSystem::setVec3     ( pPosNode , L"Velocity"      , m_Info.m_PosArg.m_Velocity);
    nsParticleSystem::setVec3     ( pPosNode , L"Acellator"     , m_Info.m_PosArg.m_Accellator);
    nsParticleSystem::setVec3Range( pPosNode , L"VelocityRange" , m_EmitterInfo.m_VelocityRange);
    nsParticleSystem::setVec3Range( pPosNode , L"AcellateRange" , m_EmitterInfo.m_AcellRange);

    xXmlNode* pColorNode = pXml->insertNode(L"color");
    nsParticleSystem::setColorRange( pColorNode , L"ColorRange"    , m_EmitterInfo.m_ColorRange);
    nsParticleSystem::setColorRange( pColorNode , L"AcellateRange" , m_EmitterInfo.m_ColorVelRange);
    if(m_EmitterInfo.m_colorChangeType == nsParticleSystem::CHANGE_UNIFORM)
        pColorNode->setValue(L"ChangeType" , L"Uniform");
    else
        pColorNode->setValue(L"ChangeType" , L"NonUniform");

    xXmlNode* pAngleNode = pXml->insertNode(L"angle");
    nsParticleSystem::setVec3Range( pAngleNode , L"VelocityRange" , m_EmitterInfo.m_AngleVelRange);
    nsParticleSystem::setVec3Range( pAngleNode , L"AcellateRange" , m_EmitterInfo.m_AngleAcellRange);

    xXmlNode* pSizeNode = pXml->insertNode(L"size");
    nsParticleSystem::setVec3Range( pSizeNode , L"Range" , m_EmitterInfo.m_SizeRange);
    nsParticleSystem::setVec3Range( pSizeNode , L"VelocityRange" , m_EmitterInfo.m_SizeVelRange);
    nsParticleSystem::setVec3Range( pSizeNode , L"AcellateRange" , m_EmitterInfo.m_SizeAcellRange);

    nsParticleSystem::setIntRange   ( pXml , L"Frequecy"     , m_EmitterInfo.m_EmitterFrequecy);
    nsParticleSystem::setScalarRange( pXml , L"LifeRange"    , m_EmitterInfo.m_LifeRange);
    nsParticleSystem::setScalarRange( pXml , L"InterValTime" , m_EmitterInfo.m_InterValTime);
    switch (m_EmitterInfo.m_EmitterType)
    {
    case nsParticleSystem::ET_CONTINUE: pXml->setValue(L"EmitteType" , L"Continue"); break; 
    case nsParticleSystem::ET_INTERVAL: pXml->setValue(L"EmitteType" , L"Interval"); break; 
    case nsParticleSystem::ET_INSTANT : pXml->setValue(L"EmitteType" , L"Instant");  break; 
    }//End switch

    pXml->setValue(L"PoolType" , m_EmitterInfo.m_PoolType.c_str() );


    if(m_Pool)
    {
        xXmlNode* pPoolNode = pXml->insertNode( m_EmitterInfo.m_PoolType.c_str() );
        m_Pool->save(pPoolNode);
    }

    return true;
}

bool  IParticleEmitter::render(float passedTime)
{
    if(m_Pool == NULL) return false;
    m_Pool->render(passedTime);
    return true;
}

END_NAMESPACE_XEVOL3D;

