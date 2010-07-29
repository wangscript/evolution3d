#include "../../xStdPch.h"
#include "xQuadParticle.h"
#include "../xParticleEffect.h"
#include "../xParticleEmitter.h"
#include "../../RenderAPI/xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D;

int xQuadParticle::RefCount()
{
    return m_RefCount;
}

int xQuadParticle::AddRef()
{
    m_RefCount ++;
    return m_RefCount;
}

int xQuadParticle::ReleaseObject()
{
    m_RefCount --;
    if(m_RefCount == 0)
    {
        m_ParticlePool->freeParticle(this);
    }
    return m_RefCount;
}

int xQuadParticle::KillObject()
{
    m_RefCount = 0;
    m_ParticlePool->freeParticle(this);
    return m_RefCount;
}


//////////////////////////////////////////////////////////////////////////
IMPL_BASE_OBJECT_CLASSID(xQuadParticlePool , IParticlePool)
xQuadParticlePool::xQuadParticlePool(IParticleEmitter* pEmitter , int arg) : IParticlePool(pEmitter , arg)
{
    m_FreeParticle   = NULL;
    m_InputAssembler = NULL;
    m_VertexStream   = NULL;
    m_Particles      = NULL;
    m_pQuadVertex    = NULL;
    m_nMaxParticle   = 512;
    m_pEmitter       = pEmitter;
    m_nActiveQuad    = 0;
	m_strImage       = L"star.tga";
    if(pEmitter)
    {
        xParticleSystem* pParticleSystem = pEmitter->particleSystem();

        IRenderApi* pRenderapi = pParticleSystem->renderApi();
        const wchar_t* _assName = L"QuadParticle";
        IInputAssembler* pAss = pRenderapi->getInputAssembler(_assName);
        if(pAss == NULL) 
        {
            xInputLayoutDesc InputDesc;
            int BuffIdx = 0;
            InputDesc.addElement(SHADER_SEMANTIC_POSITION     , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
            InputDesc.addElement(SHADER_SEMANTIC_COLOR        , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
            InputDesc.addElement(SHADER_SEMANTIC_TEXCOORD     , SHADERVARTYPE_FLOAT2 , 0 , BuffIdx);
            InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT    , RESOURCE_ACCESS_NONE);
            pAss = pRenderapi->createInputAssembler(_assName, InputDesc);
        }

        m_InputAssembler = pAss;
        assert(m_InputAssembler);

        m_VertexStream = m_InputAssembler->createVertexStream();
        //加载Shader
        m_hGpuProgram  = pRenderapi->gpuProgramManager()->load(L"quadParticle.vertex" , L"quadParticle.pixel(0:simple.texture)" , NULL);

        m_BlendState   = pRenderapi->createBlendState(L"QuadParticle");
        m_pZStencil    = pRenderapi->createDepthStencilState(L"Overlay");
    }
    else
    {
        XEVOL_WARNNING_NOT_IMPLEMENT_INFO(L"致命错误: pEmitter is NULL , when create the Particle Pool\n");
        assert(0);
    }
    return ;

    
}

bool xQuadParticlePool::load(xXmlNode* pXml)
{
    if(pXml)
    {
        m_nMaxParticle = pXml->int_value(L"MaxParticle");
		if( pXml->value(L"image") )
		{
			m_strImage = pXml->value(L"image");
		}

    }
    else
    {
        m_nMaxParticle = 512;
    }
    return init(m_nMaxParticle);
}

bool xQuadParticlePool::save(xXmlNode* pXml)
{
    if(pXml == NULL)
        return false;
    pXml->setValue(L"MaxParticle" , m_nMaxParticle);
	pXml->setValue(L"image" , m_strImage.c_str() );
    return true;
}

bool xQuadParticlePool::init(int nMaxParticle)
{
    XSAFE_DELETE_ARRAY(m_Particles);
    XSAFE_DELETE_ARRAY(m_pQuadVertex);

    m_FreeParticle = NULL;
    m_nMaxParticle  = nMaxParticle;
    m_Particles     = new xQuadParticle[nMaxParticle];
    


    //一个粒子6个顶点
    int nVertex     = nMaxParticle * 6;
    m_pQuadVertex   = new xQuadVertex  [nVertex];
    xInputBufferDesc InputBuffDesc;
    InputBuffDesc.m_usage = RESOURCE_USAGE_DYNAMIC;
    IInputBuffer* pVb = m_VertexStream->createInputBuffer(0 , nVertex , NULL , &InputBuffDesc);

	m_hTexture = m_pEmitter->particleSystem()->texManager()->add(m_strImage.c_str() , 0 , true);
    for(int i =  0 ; i  < nMaxParticle ; i ++)
    {
        m_Particles[i].m_NextParticle = m_FreeParticle;
        m_Particles[i].m_ParticlePool = this;
        m_Particles[i].m_RefCount = 1;
        m_FreeParticle = &m_Particles[i];
    }
    return true;
}

IParticle* xQuadParticlePool::createParticle()
{
    if(m_FreeParticle == NULL)
        return NULL;

    xQuadParticle* pParticle = m_FreeParticle;
    m_FreeParticle = pParticle->m_NextParticle;
    pParticle->m_RefCount = 1;
    return pParticle;
}

bool  xQuadParticlePool::freeParticle(IParticle* pParticle)
{
    xQuadParticle* pQuadParticle = (xQuadParticle*)pParticle;
    pQuadParticle->m_RefCount = 0;
    pQuadParticle->m_NextParticle = m_FreeParticle;
    m_FreeParticle = pQuadParticle;
    return true;
}

void   xQuadParticlePool::update(IParticleEffector* pEffector , float passedTime)
{
    m_nActiveQuad = 0;
    xQuadVertex* pVertex = m_pQuadVertex;
    IRenderApi* pApi = m_pEmitter->particleSystem()->renderApi();
    IRenderCamera* pCamera = pApi->getCamera();

    xMathLib::xmat4 worldMat;
    pApi->getMatrix( worldMat.data , MATRIXMODE_World );

    xMathLib::xmat4  bbMatrix;
    pCamera->calcBillboardMatrix(worldMat , bbMatrix);

    if(pEffector == NULL)
    {
        for(int i = 0 ; i < m_nMaxParticle ; i ++)
        {
            if(m_Particles[i].m_RefCount != 0)
            {
                xParticleInfo& ParticleInfo = m_Particles[i].m_Info;
                m_Particles[i].update(passedTime);
                //============增加一个Quad=========================
                _addQuad(bbMatrix, pVertex, ParticleInfo);

                pVertex += 6;
                m_nActiveQuad ++;
            }
        }
    }
    else
    {


        for(int i = 0 ; i < m_nMaxParticle ; i ++)
        {
            if(m_Particles[i].m_RefCount != 0)
            {
                xParticleInfo& ParticleInfo = m_Particles[i].m_Info;
                pEffector->effect( ParticleInfo , passedTime);

                //============增加一个Quad=========================
                _addQuad(bbMatrix, pVertex, ParticleInfo);

                pVertex += 6;
                m_nActiveQuad ++;
            }
        }


    }

    //更新VertexBuffer;
    if(m_nActiveQuad > 0 )
       m_VertexStream->getInputBuffer(0)->update(eLock_WriteDiscard , m_pQuadVertex , m_nActiveQuad * 6 * sizeof(xQuadVertex) );
}

void xQuadParticlePool::render(float passedTime)
{
    if(m_nActiveQuad == 0 )
        return ;

    IRenderApi* pApi = m_pEmitter->particleSystem()->renderApi();
    pApi->setBlendState(m_BlendState);
    pApi->setDepthStencilState(m_pZStencil);
    pApi->setTexture(0 , m_hTexture.getResource() );
    pApi->pushGpuProgram( m_hGpuProgram.getResource() );
    pApi->setInputAssembler(m_InputAssembler);
    pApi->setVertexStream(m_VertexStream);
    pApi->draw(m_nActiveQuad * 6 , 0 , ePrimtiveType_Triangles);
    pApi->popGpuProgram();
}

void xQuadParticlePool::_addQuad( xMathLib::xmat4 bbMatrix, xQuadVertex* pVertex, xParticleInfo &ParticleInfo ) 
{
    _transBillBoard(&bbMatrix , pVertex , ParticleInfo.m_PosArg.m_Value.x, ParticleInfo.m_PosArg.m_Value.y, ParticleInfo.m_PosArg.m_Value.z,
        ParticleInfo.m_AngleArg.m_Value.z,
        ParticleInfo.m_SizeArg.m_Value.x , ParticleInfo.m_SizeArg.m_Value.y  );


    float u1 = 0.0f;
    float u2 = 1.0f;

    float v1 = 0.0f;
    float v2 = 1.0f;
    xColor_4f& cl = ParticleInfo.m_ColorArg.m_Value;
    pVertex[0].m_Color =  cl; pVertex[0].m_TexCoord.s = u1;     pVertex[0].m_TexCoord.t = v1;
    pVertex[1].m_Color =  cl; pVertex[1].m_TexCoord.s = u2;     pVertex[1].m_TexCoord.t = v1;
    pVertex[2].m_Color =  cl; pVertex[2].m_TexCoord.s = u1;     pVertex[2].m_TexCoord.t = v2;

    pVertex[3].m_Color =  cl; pVertex[3].m_TexCoord.s = u2;     pVertex[3].m_TexCoord.t = v1;
    pVertex[4].m_Color =  cl; pVertex[4].m_TexCoord.s = u2;     pVertex[4].m_TexCoord.t = v2;
    pVertex[5].m_Color =  cl; pVertex[5].m_TexCoord.s = u1;     pVertex[5].m_TexCoord.t = v2;
}

bool xQuadParticlePool::_transBillBoard(xMathLib::xmat4*  pBBMatrix , xQuadVertex* pVertex,float x,float y, float z ,float angle,float size_w ,float size_h)
{
    size_w = size_w/2;
    size_h = size_h/2;
    if(size_h == 0)
        size_h = size_w;
    xMathLib::xvec4 v1;//(-size_w,-size_h,0,1);
    xMathLib::xvec4 v2;//( size_w,-size_h,0,1);
    xMathLib::xvec4 v3;//( size_w, size_h,0,1);
    xMathLib::xvec4 v4;//(-size_w, size_h,0,1);

    xMathLib::xmat4   rotMatrix;
    XM_Rotate(rotMatrix,xMathLib::xvec4(0,0,1,1),angle);

    xMathLib::xmat4   transMatrix;
    XM_Transform(transMatrix,x,y,z);

    xMathLib::xmat4   tempmat;

    xMathLib::xmat4 idMat;
    idMat.identity();
    // rotation then face the camera , finally transform to postion;
    //M = rotMatrix * BBMatrix  * transMatrix 
    XM_Mul(rotMatrix,*pBBMatrix,tempmat);
    //XM_Mul(rotMatrix,idMat,tempmat);
    XM_Mul(tempmat,transMatrix,rotMatrix);

    //rotmatrix  is the final matrix;

    XM_Mul(xMathLib::xvec4(-size_w,-size_h,0,1),rotMatrix,v1);
    XM_Mul(xMathLib::xvec4( size_w,-size_h,0,1),rotMatrix,v2);
    XM_Mul(xMathLib::xvec4( size_w, size_h,0,1),rotMatrix,v3);
    XM_Mul(xMathLib::xvec4(-size_w, size_h,0,1),rotMatrix,v4);

    pVertex[0].m_Pos.x = v1.x; pVertex[0].m_Pos.y = v1.y;  pVertex[0].m_Pos.z = v1.z;   pVertex[0].m_Pos.w = 1.0f;
    pVertex[1].m_Pos.x = v2.x; pVertex[1].m_Pos.y = v2.y;  pVertex[1].m_Pos.z = v2.z;   pVertex[1].m_Pos.w = 1.0f;
    pVertex[2].m_Pos.x = v4.x; pVertex[2].m_Pos.y = v4.y;  pVertex[2].m_Pos.z = v4.z;   pVertex[2].m_Pos.w = 1.0f;

    pVertex[3].m_Pos.x = v2.x; pVertex[3].m_Pos.y = v2.y;  pVertex[3].m_Pos.z = v2.z;   pVertex[3].m_Pos.w = 1.0f;
    pVertex[4].m_Pos.x = v3.x; pVertex[4].m_Pos.y = v3.y;  pVertex[4].m_Pos.z = v3.z;   pVertex[4].m_Pos.w = 1.0f;
    pVertex[5].m_Pos.x = v4.x; pVertex[5].m_Pos.y = v4.y;  pVertex[5].m_Pos.z = v4.z;   pVertex[5].m_Pos.w = 1.0f;
    /*
    *((XVector3D*) (&pVertex[0].x)) = *((XVector3D*)&v1);
    *((XVector3D*) (&pVertex[1].x)) = *((XVector3D*)&v2);
    *((XVector3D*) (&pVertex[2].x)) = *((XVector3D*)&v4);

    *((XVector3D*) (&pVertex[3].x)) = *((XVector3D*)&v2);
    *((XVector3D*) (&pVertex[4].x)) = *((XVector3D*)&v3);
    *((XVector3D*) (&pVertex[5].x)) = *((XVector3D*)&v4);
    */

    return true;
}
void  xQuadParticlePool::onEmitte(IParticle* p)
{
	p->m_Info.m_SizeArg.m_Value.y      =  p->m_Info.m_SizeArg.m_Value.z      = p->m_Info.m_SizeArg.m_Value.x;
	p->m_Info.m_SizeArg.m_Velocity.y   =  p->m_Info.m_SizeArg.m_Velocity.z   = p->m_Info.m_SizeArg.m_Velocity.x;
	p->m_Info.m_SizeArg.m_Accellator.y =  p->m_Info.m_SizeArg.m_Accellator.z = p->m_Info.m_SizeArg.m_Accellator.x;
}

IMPL_PARTICLEPOOL_FACTORY(xQuadParticlePool);
END_NAMESPACE_XEVOL3D;