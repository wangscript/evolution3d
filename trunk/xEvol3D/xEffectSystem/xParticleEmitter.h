#ifndef __XEVOL_PARTICLE_EMITTER_H__
#define __XEVOL_PARTICLE_EMITTER_H__
#include "xParticleSystem.h"
#include "xParticle.h"
BEGIN_NAMESPACE_XEVOL3D;

struct xEmitterInfo
{
    nsParticleSystem::Vec3Range     m_VelocityRange;//速度的范围  
    nsParticleSystem::Vec3Range     m_AcellRange;   //加速度的范围 
   
    nsParticleSystem::ColorRange    m_ColorRange;
    nsParticleSystem::ColorRange    m_ColorVelRange;

    nsParticleSystem::Vec3Range     m_AngleAcellRange;
    nsParticleSystem::Vec3Range     m_AngleVelRange;//角度变化的速度，角速度

    nsParticleSystem::Vec3Range     m_SizeRange;    //大小的区间
    nsParticleSystem::Vec3Range     m_SizeVelRange; //大小的变化的速度
    nsParticleSystem::Vec3Range     m_SizeAcellRange;//大小的变化的加速度

    nsParticleSystem::IntRange      m_EmitterFrequecy;//每秒喷射的粒子个数．

    nsParticleSystem::ScalarRange   m_LifeRange;//生命的范围
    nsParticleSystem::CHANGE_TYPE   m_colorChangeType;

    nsParticleSystem::EMITTER_TYPE  m_EmitterType;//喷射器的类型．
    nsParticleSystem::ScalarRange   m_InterValTime;//非持续喷射的间隔时间
    std::ds_wstring                 m_PoolType;

public:
    xEmitterInfo()
    {
      m_VelocityRange.setRange( xMathLib::xvec3(-50.0f , -50.0f , 10.0f) , xMathLib::xvec3(50.0f , 50.0f , 50.0f) );//速度的范围  
      m_AcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );;   //加速度的范围 
   
      m_ColorRange   .setRange( xColor_4f(1.0f , 1.0f , 1.0f , 1.0f) , xColor_4f(0.4f , 0.4f , 0.4f , 1.0f) );
      m_ColorVelRange.setRange( xColor_4f(0.0f , 0.0f , 0.0f , 0.0f) , xColor_4f(0.0f , 0.0f , 0.0f , 0.0f) );

      m_AngleAcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(10.0f , 10.0f , 10.0f) );
      m_AngleVelRange  .setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );//角度变化的速度，角速度

      m_SizeRange     .setRange( xMathLib::xvec3(20.0f , 20.0f , 20.0f) , xMathLib::xvec3(40.0f , 40.0f , 40.0f) );    //大小的区间
      m_SizeVelRange  .setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) ); //大小的变化的速度
      m_SizeAcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );//大小的变化的加速度

      m_EmitterFrequecy.setRange( 1 , 10 );//每秒喷射的粒子个数．

      m_LifeRange.setRange( 4.0f , 10.0f );//生命的范围
      m_colorChangeType = nsParticleSystem::CHANGE_UNIFORM;

      m_EmitterType  = nsParticleSystem::ET_CONTINUE ;//喷射器的类型．
      m_InterValTime.setRange( 1.0f , 10.0f );;//非持续喷射的间隔时间
    }
};

class _XEVOL_BASE_API_ IParticleEmitter : public IParticle
{
public:
    typedef ds_vector(IParticleEffector*)  vEffectors;

    IParticleEmitter(xParticleSystem* pParticleSystem , int arg);
public:
    virtual void                 pushEffector(IParticleEffector* pEffector);
    virtual void                 popEffector();
    virtual int                  nEffector();
    virtual IParticleEffector*   getEffector(int idx);

public:
    virtual bool                 init(const wchar_t* poolType) = 0;
    virtual bool                 load(xXmlNode* pXml) = 0;
    virtual bool                 save(xXmlNode* pXml) = 0; 
    virtual xMathLib::xvec3      emittePos() = 0;
public:
    virtual void                 update(float passedTime);
    //控制喷射状态
    virtual void                 start();
    virtual void                 stop();
    virtual bool                 render(float passedTime);
    virtual bool                 is_active();
    virtual void                 emitte(int n );
protected:
    virtual bool                 _emitte();
  
public:
    xParticleSystem*             particleSystem() { return m_pParticleSystem ; }

	xEmitterInfo                 m_EmitterInfo;
protected:
    vEffectors                   m_vEffectors;
    IParticlePool*               m_Pool;
    xParticleSystem*             m_pParticleSystem;
    

    bool                         m_EmitterContinue;
    bool                         m_isEmitter;
    int                          m_EmittedParticle;//已经发射了粒子数目
    float                        m_PassedTime;//已经过去的时间．到上次喷射后开始计时
    float                        m_fCountNeedGen;//保存需要产生的粒子个数,这个数一定要小于1，因为每次计算的时候  可能要产生零点几个粒子，把这个值累加下去
   
};

class _XEVOL_BASE_API_ xParticleEmitterCreator;
class _XEVOL_BASE_API_ xParticleEmitterFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(IParticleEmitter , xParticleEmitterCreator , xParticleEmitterFactoryMgr , xParticleSystem* , int );

#define IMPL_PARTICLEEMITTER_FACTORY_2(EmitterClass , EmitterName) IMPL_OBJECT_FACTORY(EmitterClass , IParticleEmitter ,  EmitterClass##Factory , xParticleEmitterCreator , xParticleEmitterFactoryMgr ,  _TEXT_(EmitterName)  ,  _TEXT_(EmitterName)   , xParticleSystem* , int );
#define IMPL_PARTICLEEMITTER_FACTORY(EmitterClass)                 IMPL_OBJECT_FACTORY(EmitterClass , IParticleEmitter ,  EmitterClass##Factory , xParticleEmitterCreator , xParticleEmitterFactoryMgr ,  _WIDETEXT_(#EmitterClass),  _WIDETEXT_(#EmitterClass) , xParticleSystem* , int );


END_NAMESPACE_XEVOL3D
#endif
