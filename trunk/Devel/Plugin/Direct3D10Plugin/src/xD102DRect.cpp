#include "xD102DRect.h"
#include "xDirect3D10API.h"
#include "RenderAPI/xShaderDefine.h"
#include "RenderAPI/xBaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D
xD102DRectEnv::xD102DRectEnv(xD3D10RenderApi* pApi)
{
	m_pVertexStream = NULL;
	m_pIdxBuffer = NULL;
	for(int i = 0 ; i <= MAX_UV_CHANEL ; i ++)
	{
		m_pAss[i] = NULL;
	}
	m_pRenderApi = pApi;
	create();
}

xD102DRectEnv::~xD102DRectEnv()
{
	m_pVertexStream->ReleaseObject();
	m_pIdxBuffer->ReleaseObject();
	for(int i = 0 ; i <= MAX_UV_CHANEL ; i ++)
	{
		m_pAss[i]->ReleaseObject();
	}
}

float xD102DRectEnv::zValue()
{
	return m_pRenderApi->get2DZValue() ; 
}


bool xD102DRectEnv::create()
{
	for(int i = 0 ; i <= MAX_UV_CHANEL; i ++)
	{
		xInputLayoutDesc InputDesc;
		InputDesc.addElement(SHADER_SEMANTIC_POSITION     , SHADERVARTYPE_FLOAT4);
		InputDesc.addElement(SHADER_SEMANTIC_COLOR      , SHADERVARTYPE_FLOAT4);
		for(int j = 0 ; j < i ; j ++)
		{
			InputDesc.addElement(SHADER_SEMANTIC_TEXCOORD , SHADERVARTYPE_FLOAT2 , j);
		}
		InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT , RESOURCE_ACCESS_NONE);
		wchar_t _iisName[16] = {0};
		wsprintf(_iisName , L"xD10Rect%d" , i);
		m_pAss[i] = m_pRenderApi->createInputAssembler(_iisName, InputDesc);
	}

	m_pVertexStream = m_pAss[MAX_UV_CHANEL]->createVertexStream();

	unsigned int indices[6] =
	{
		0  ,2 ,1 , 
		2  ,0 ,3 , 
	};
	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage       = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype    = BIND_AS_INDEX_BUFFER;
	m_pIdxBuffer = m_pRenderApi->createInputBuffer(6  , 4 , &idxBufDesc,indices);
	return true;
}

xD102DRect::xD102DRect(xD102DRectEnv*  pRectEnv)
{
	m_bDirty   = false;
	m_pRectEnv = pRectEnv;
	for(int i = 0 ; i < MAX_2DTEXTURE ; i ++)
	{
		m_SrcRect[i].rect.x = 0.0f;
		m_SrcRect[i].rect.y = 0.0f;
		m_SrcRect[i].rect.w = 1.0f;
		m_SrcRect[i].rect.h = 1.0f;
		m_Texture[i] = NULL;
	}
	m_pVertexBuffer = NULL;
    m_pInputAss = NULL;
	m_cl = xColor_4f(1.0f,1.0f,1.0f,1.0f);
	create();
}

void xD102DRect::setTexture(int iStage , IBaseTexture* pTexture)
{
	m_SrcRect[iStage].rect.x = 0.0f;
	m_SrcRect[iStage].rect.y = 0.0f;
	m_SrcRect[iStage].rect.w = 1.0f;
	m_SrcRect[iStage].rect.h = 1.0f;
	m_Texture[iStage] = pTexture;
	m_bDirty   = true;
}

void xD102DRect::setTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4])
{
	xvec4& srcRect = *((xvec4*)(_srcRect));
	xTextureDesc desc;
	pTexture->desc(desc);
	m_SrcRect[iStage].rect.x = srcRect.rect.x / desc.m_width;
	m_SrcRect[iStage].rect.y = srcRect.rect.y / desc.m_height;
	m_SrcRect[iStage].rect.w = srcRect.rect.w / desc.m_width;
	m_SrcRect[iStage].rect.h = srcRect.rect.h / desc.m_height;
	m_Texture[iStage] = pTexture;
	m_bDirty   = true;
}

void xD102DRect::setRect(float _destRect[4])
{
	xvec4& destRect = *((xvec4*)(_destRect));
	m_rectLD.x = destRect.rect.x ;
	m_rectLD.y = destRect.rect.y + destRect.rect.h;
	m_rectLD.z = m_pRectEnv->zValue() ;
	m_rectLD.w = 1.0f;

	m_rectRU.x = destRect.rect.x + destRect.rect.w;
	m_rectRU.y = destRect.rect.y ;
	m_rectRU.z = m_pRectEnv->zValue() ;
	m_rectRU.w = 1.0f;
	m_bDirty   = true;
	return ;
}

void buildTextureUV(xvec2& uvLD , xvec2& uvRU , xvec4& rect)
{
	uvLD.x = rect.rect.x ;
	uvLD.y = rect.rect.y + rect.rect.h ;

	uvRU.x = rect.rect.x + rect.rect.w ;
	uvRU.y = rect.rect.y ;
}

void xD102DRect::commit()
{
	if(m_bDirty == false)
		return ;

	xvec2 uvLD[MAX_UV_CHANEL] , uvRU[MAX_UV_CHANEL];

	buildTextureUV(uvLD[0] , uvRU[0] , m_SrcRect[0] );
	buildTextureUV(uvLD[1] , uvRU[1] , m_SrcRect[1] );
	m_Vertex[0].m_color = m_cl;
	m_Vertex[0].m_pos   = xvec4(m_rectLD.x , m_rectLD.y , m_rectLD.z , 1.0f);
	for(int i = 0 ; i < MAX_UV_CHANEL ; i ++ )
	{
		m_Vertex[0].m_uv[i] = xvec2(uvLD[i].x  , uvLD[i].y);
	}



	m_Vertex[1].m_color = m_cl;
	m_Vertex[1].m_pos   = xvec4(m_rectLD.x , m_rectRU.y , m_rectLD.z , 1.0f);
	for(int i = 0 ; i < MAX_UV_CHANEL ; i ++ )
	{
		m_Vertex[1].m_uv[i] = xvec2(uvLD[i].x  , uvRU[i].y);
	}


	m_Vertex[2].m_color = m_cl;
	m_Vertex[2].m_pos   = xvec4(m_rectRU.x , m_rectRU.y , m_rectRU.z , 1.0f);
	for(int i = 0 ; i < MAX_UV_CHANEL ; i ++ )
	{
		m_Vertex[2].m_uv[i] = xvec2(uvRU[i].x  , uvRU[i].y);
	}


	m_Vertex[3].m_color = m_cl;
	m_Vertex[3].m_pos   = xvec4(m_rectRU.x , m_rectLD.y , m_rectRU.z , 1.0f);
	for(int i = 0 ; i < MAX_UV_CHANEL ; i ++ )
	{
		m_Vertex[3].m_uv[i] = xvec2(uvRU[i].x  , uvLD[i].y);
	}

	m_pVertexBuffer->update(eLock_WriteDiscard , m_Vertex , sizeof(xD102DVertex) * 4);

	m_bDirty = false;
	return ;
}

void xD102DRect::create()
{
	xInputBufferDesc vBufDesc;
	vBufDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
	vBufDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
	vBufDesc.m_bindtype    = BIND_AS_VERTEX_BUFFER;
	m_pRectEnv->m_pVertexStream->createInputBuffer( 0 , 4 , NULL , &vBufDesc );
	m_pVertexBuffer = m_pRectEnv->m_pVertexStream->getInputBuffer(0);
	m_bDirty = true;
	setUVChanel(2);
	return ;
}

void xD102DRect::setUVChanel(int iUVChanel)
{
    if(m_pInputAss && m_nUVChanel == iUVChanel)
        return ;

    XSAFE_RELEASEOBJECT(m_pInputAss);
	m_nUVChanel = iUVChanel;
	wchar_t _iisName[16] = {0};
	wsprintf(_iisName , L"xD10Rect%d" , iUVChanel );
	m_pInputAss = m_pRectEnv->m_pRenderApi->getInputAssembler(_iisName);
}

int xD102DRect::nUVChanel()
{
	return m_nUVChanel;
}

xD102DRect::~xD102DRect()
{
	if(m_pVertexBuffer || m_pRectEnv->m_pVertexStream->getInputBuffer(0) != m_pVertexBuffer )
	{
		m_pVertexBuffer->KillObject();
	}
	m_pVertexBuffer = NULL;
    XSAFE_RELEASEOBJECT(m_pInputAss);
	return ;	
}

bool xD102DRect::draw()
{
	for(int i =  0 ; i < MAX_2DTEXTURE ; i ++)
	{
	    m_pRectEnv->m_pRenderApi->setTexture(i,m_Texture[i]);
	}

	m_pRectEnv->m_pRenderApi->setVertexBuffer(m_pVertexBuffer , 0 , sizeof(xD102DVertex) );
	m_pRectEnv->m_pRenderApi->setInputAssembler( m_pInputAss );
	m_pRectEnv->m_pRenderApi->draw(m_pRectEnv->m_pIdxBuffer , 6 );
	return true;
}

END_NAMESPACE_XEVOL3D
