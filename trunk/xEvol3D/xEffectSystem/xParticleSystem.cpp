#include "xParticleSystem.h"
#include "xParticleEmitter.h"
BEGIN_NAMESPACE_XEVOL3D;
IMPL_BASE_OBJECT_DLLSAFE(xParticleSystem , IBaseObject);

xParticleSystem*  xParticleSystem::createInstance(IBaseRenderer* pBaseRenderer , xBaseTextureMgr* pTexMgr)
{
    return new xParticleSystem(pBaseRenderer , pTexMgr);
}

xParticleSystem::xParticleSystem(IBaseRenderer* pBaseRenderer , xBaseTextureMgr* pTexMgr )
{
    m_RenderApi = NULL;
    if(pBaseRenderer)
        m_RenderApi = pBaseRenderer->renderApi();
    m_Renderer  = pBaseRenderer;
    m_pTextureMgr = pTexMgr;
}

xParticleSystem::~xParticleSystem()
{
    m_Renderer  = NULL;
    m_RenderApi = NULL;
}
void  xParticleSystem::update(float passedTime)
{
	size_t _nEmitter = m_ParticleEmitters.size();
	for(size_t i = 0 ; i < _nEmitter ; i ++)
	{
		m_ParticleEmitters[i]->update(passedTime);
	}
}
void  xParticleSystem::startAll()
{
	size_t _nEmitter = m_ParticleEmitters.size();
	for(size_t i = 0 ; i < _nEmitter ; i ++)
	{
		m_ParticleEmitters[i]->start();
	}
}
void  xParticleSystem::stopAll()
{
	size_t _nEmitter = m_ParticleEmitters.size();
	for(size_t i = 0 ; i < _nEmitter ; i ++)
	{
		m_ParticleEmitters[i]->stop();
	}
}
void  xParticleSystem::render(float passedTime)
{
	size_t _nEmitter = m_ParticleEmitters.size();
	for(size_t i = 0 ; i < _nEmitter ; i ++)
	{
		m_ParticleEmitters[i]->render(passedTime);
	}
}

int   xParticleSystem::nEmitter()
{
	return (int)m_ParticleEmitters.size();
}

IParticleEmitter*  xParticleSystem::operator[](int idx)
{
	return m_ParticleEmitters[idx];
}

bool  xParticleSystem::load(xXmlNode* pXml)
{
    if(pXml == NULL)
        return false;
    if(pXml->name() == std::wstring(L"emitter"))
    {
        IParticleEmitter* pEmitter = createEmitter(pXml);
        if(pEmitter != NULL)
        {
            m_ParticleEmitters.push_back( pEmitter);
            return true;
        }
        return false;
    }
    else
    {
        xXmlNode::XmlNodes emitNodes;
        pXml->findNode(L"emitter" , emitNodes);
        int nEmitter = 0 ; 
        for(size_t i = 0 ; i < emitNodes.size() ; i ++)
        {
            xXmlNode* pEmitterXml = emitNodes[i];
            IParticleEmitter* pEmitter = createEmitter(pEmitterXml);
            if(pEmitter != NULL)
            {
                m_ParticleEmitters.push_back( pEmitter);
                nEmitter ++ ;
            }
        }
        return nEmitter != 0;
    }
    return false;
}

IParticleEmitter*  xParticleSystem::createEmitter(xXmlNode* pXml)
{
    const wchar_t* _emitterType = pXml->value(L"type");
    IParticleEmitter* pEmitter = xParticleEmitterFactoryMgr::singleton()->createInstance(_emitterType , this , 0);
    if(pEmitter && pEmitter->load(pXml) )
    {
        return pEmitter;
    }

    XSAFE_RELEASEOBJECT( pEmitter);
    return pEmitter;    
}

IParticleEmitter*  xParticleSystem::createEmitter(const wchar_t* _type)
{
    IParticleEmitter* pEmitter = xParticleEmitterFactoryMgr::singleton()->createInstance(_type , this , 0);
    return pEmitter;
}

//ÊÍ·ÅÒ»¸öEmitter;
bool   xParticleSystem::releaseEmitter(IParticleEmitter* pEmitter)
{
    vParticleEmitters::iterator pos = m_ParticleEmitters.begin();
    for(; pos != m_ParticleEmitters.end() ; pos ++)
    {
        if(*pos == pEmitter)
        {
            m_ParticleEmitters.erase(pos);
            pEmitter->ReleaseObject();
            return true;
        }
    }
    return false;
}

IRenderApi*   xParticleSystem::renderApi()
{
    return m_RenderApi;
}

IBaseRenderer* xParticleSystem::renderer()
{
    return m_Renderer;
}


END_NAMESPACE_XEVOL3D

