#ifndef __XEVOL_QUAD_PARTICLE_H__
#define __XEVOL_QUAD_PARTICLE_H__
#include "../xParticle.h"
#include "../../RenderAPI/xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D;
class xQuadParticlePool;
class xQuadParticle : public IParticle
{
public:
    virtual int      RefCount();
    virtual int      AddRef();
    virtual int      ReleaseObject();
    virtual int      KillObject();
public:
    xQuadParticlePool* m_ParticlePool;
    xQuadParticle*     m_NextParticle;
    int                m_RefCount;  
};

struct xQuadVertex
{
    xMathLib::xvec4      m_Pos;
    xColor_4f            m_Color;
    xMathLib::xvec2      m_TexCoord;
};

class xQuadParticlePool : public IParticlePool
{
    IVertexStream*     m_VertexStream;
    IInputAssembler*   m_InputAssembler;
    xQuadVertex*       m_pQuadVertex;
    int                m_nMaxParticle;
    xQuadParticle*     m_Particles;
    xQuadParticle*     m_FreeParticle;
    IParticleEmitter*  m_pEmitter;
    int                m_nActiveQuad;
    HGpuProgram        m_hGpuProgram;
    IBlenderState*     m_BlendState;
    IDepthStencilState*m_pZStencil;
	HBaseTexture       m_hTexture;
	std::wstring       m_strImage;
public:
    IMPL_BASE_OBJECT_INTERFACE(xQuadParticlePool);
    xQuadParticlePool(IParticleEmitter* pEmitter , int arg);
    bool                init(int nMaxParticle);
    IParticle*          createParticle();
    bool                freeParticle(IParticle* pParticle);
    void                update(IParticleEffector* pEffector , float passedTime);
	void                onEmitte(IParticle* pParticle);
 
    void                render(float passedTime);
    bool                load(xXmlNode* pXml);
    bool                save(xXmlNode* pXml);

protected:
    void                _addQuad( xMathLib::xmat4 bbMatrix, xQuadVertex* pVertex, xParticleInfo &ParticleInfo ) ;
    bool                _transBillBoard(xMathLib::xmat4*  pBBMatrix , xQuadVertex* pVertex,float x,float y, float z ,float angle,float size_w ,float size_h);
};

END_NAMESPACE_XEVOL3D;

#endif
