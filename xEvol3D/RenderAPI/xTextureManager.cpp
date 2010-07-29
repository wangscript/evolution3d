#include "../xStdPch.h"
#include "xTextureManager.h"
#include "xRenderView.h"
#include "xRenderAPI.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

xTextureName::xTextureName()
{
	m_Name       = L"";
    m_InitDesc.m_TextureDesc.m_fmt        = PIXELFORMAT_None;
	m_Type       = RESOURCE_TEXTURE;
}

xTextureName::xTextureName(const wchar_t* fileName)
{
	m_Name       = fileName;
	m_InitDesc.m_TextureDesc.m_fmt        = PIXELFORMAT_None;
	m_Type       = RESOURCE_TEXTURE;
}

xTextureName::xTextureName(const wchar_t* _Name , ePIXEL_FORMAT fmt , eResourceType type ,bool rtt, bool bLockable , int w , int h , int depth , int arraySize)
{
	m_Name       = _Name;
	m_InitDesc.m_TextureDesc.m_width      = w ;
	m_InitDesc.m_TextureDesc.m_height     = h ;
	m_InitDesc.m_TextureDesc.m_depth      = depth ;
	m_InitDesc.m_TextureDesc.m_fmt        = fmt;
    m_InitDesc.m_bReadable = bLockable;
    if(type == RESOURCE_TEXTURECube)
    {
        m_InitDesc.m_TextureDesc.m_nArraySize = eTextureArray_CubeMap;
    }
	m_Type       = type;

}

xBaseTextureMgr*  xBaseTextureLoader::createInstance(IRenderApi* pRenderApi , const wchar_t* _name)
{
    xBaseTextureMgr* pMgr = new xBaseTextureMgr(_name);
    pMgr->setThis(pMgr);
    pMgr->setRenderApi(pRenderApi);
    return pMgr;
}

xBaseTextureLoader::xBaseTextureLoader()
{
    m_pRenderApi = NULL;
}

void xBaseTextureLoader::setRenderApi(IRenderApi* pRenderApi)
{
	m_pRenderApi = pRenderApi;
}

bool  xBaseTextureLoader::_isResLoaded(IBaseTexture* pRes)
{
    return pRes && pRes->isLoaded();
}

HBaseTexture xBaseTextureLoader::load(const xTextureName& texName , bool bLoadImm  , bool arg)
{
     xBaseTextureMgr* pMgr = m_pThis;
     return pMgr->add(texName, arg ,bLoadImm);
}

HBaseTexture  xBaseTextureLoader::loadTexture(const wchar_t* texName, bool bLoadImm  , bool arg)
{
	xTextureName _name(texName);
	return load(_name ,bLoadImm , arg);
}

HBaseTexture  xBaseTextureLoader::loadTexture(const wchar_t* texName , xTextureInitDesc& initDesc , eResourceType type) 
{
     xTextureName _name(texName);
	 _name.m_InitDesc = initDesc;
	 _name.m_Type = type;
	 return load(_name,true , 0);
}

IBaseTexture*   xBaseTextureLoader::_loadTexture(const  xTextureName& _texName , const wchar_t* fileName ,unsigned char* buf , int bufLen , unsigned int arg)
{
	const wchar_t* _extName = xFileSystem::singleton()->getFileExtName(_texName.m_Name.c_str() );
	ITextureLoader* pTextureLoader = xTextureLdrMgr::singleton()->findTextureLoader(_extName);
	if(pTextureLoader == NULL)
		return m_pRenderApi->createFileTexture( fileName , buf,bufLen,arg , &_texName.m_InitDesc);
	return pTextureLoader->loadTexture(m_pRenderApi , fileName , buf , bufLen , _texName);

}

bool  xBaseTextureLoader::_loadTexture(const xTextureName& _texName, IBaseTexture*& pTexture , int& ResSize , unsigned int arg)
{
	const wchar_t* texName = _texName.m_Name.c_str(); 
	std::ds_wstring full_name;
	ResSize = 0;

	if(XEvol_IsAbsPath( texName  ) )
	{
		pTexture = _loadTexture(_texName , texName,NULL,0,arg);
		if( pTexture != NULL)
		{
			ResSize = pTexture->memUsage();
			XEVOL_LOG(eXL_DEBUG_NORMAL,L"Load from abs path name = %s \n", texName );
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		int nResItem = (int)this->m_Packages.size();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPkgPathItem& item = this->m_Packages[i];
			if(item.m_Package.failed())
			{
				full_name = item.m_ResDir + ds_wstring(texName)  ;
				pTexture = _loadTexture(_texName , full_name.c_str() , NULL , 0  , arg);
				if( pTexture != NULL )
				{
					ResSize = pTexture->memUsage();
					XEVOL_LOG(eXL_DEBUG_NORMAL,L"Load from path, search index = %d , name = %s \n",i,texName );
					return true;
				}
				else
				{
					;
				}
			}
			else
			{
				full_name = item.m_ResDir +  texName ;
				xcomdocstream* pstream = item.m_Package.open_stream( (const wchar_t*)full_name.c_str());
				if(pstream)
				{
					int len = (int)pstream->data_len();
					unsigned int8* buf = new unsigned int8[len + 1];

					//读到内存里来
					int offs = pstream->stream_tellr();
					pstream->stream_seekr(xcdsd_beg,0);
					pstream->read(buf,len);
					pstream->stream_seekr(xcdsd_beg,offs);
					buf[len] = 0;

					pTexture = _loadTexture(_texName , texName, buf , len  , arg);
					if( pTexture != NULL)
					{
						ResSize = pTexture->memUsage();
						delete[] buf;
						XEVOL_LOG(eXL_DEBUG_NORMAL,  L"Load from package, search index = %d , name = %s\n",i,texName);
						pstream->Release();
						return true;
					}
					else
					{
						delete[]  buf;
					}
					pstream->Release();
				}

			}
		}
	}

	return false;
}

bool  xBaseTextureLoader::_loadResource(const xTextureName& strResName   , IBaseTexture* & pRes , int& ResSize, unsigned int arg)
{
    bool bReadable = (strResName.m_InitDesc.m_access & RESOURCE_ACCESS_READ) != 0 ;
    const xTextureDesc& textDesc = strResName.m_InitDesc.m_TextureDesc;

	if(strResName.m_Type == RESOURCE_TEXTURE)
	{
		return _loadTexture(strResName.m_Name.c_str() , pRes , ResSize , arg);        
	}
	else if(strResName.m_Type == RESOURCE_TEXTURE2D)
	{
        if(strResName.m_InitDesc.m_bindType & BIND_AS_RENDER_TARGET)
            pRes = m_pRenderApi->createRenderableTexture(textDesc.m_width , textDesc.m_height , textDesc.m_depth , textDesc.m_fmt , bReadable); 
		else
            pRes = m_pRenderApi->createTexture(strResName.m_InitDesc , NULL , 0);
		   
		return pRes != NULL;
	}
	else if(strResName.m_Type == RESOURCE_RENDERTARGET)
	{
		IRenderTarget* pRT = m_pRenderApi->createRenderTarget(textDesc.m_width , textDesc.m_height ,  textDesc.m_fmt , false , true);
		pRes = pRT->toTexture();
		return pRes != NULL;
	}
	else if(strResName.m_Type == RESOURCE_DEPTHBUFFER)
	{
		IRenderTarget* pRT = m_pRenderApi->createDepthBuffer(textDesc.m_width , textDesc.m_height ,  textDesc.m_fmt , false , false);
		pRes = pRT->toTexture();
		return pRes != NULL;
	}
	return false;
}

bool  xBaseTextureLoader::_unloadResource(const xTextureName& strResName , IBaseTexture* & pRes , unsigned int& TotalResSize)
{
	int ResSize = TotalResSize - pRes->memUsage();
	if(ResSize < 0) ResSize = 0;
	TotalResSize = ResSize;
	pRes->unload();
	return false;
}

void  xBaseTextureLoader::_deleteResource(const xTextureName& strResName , IBaseTexture* pRes)
{
	pRes->KillObject();
	pRes=NULL;
	return ;
}

unsigned int  xBaseTextureLoader::_getResSize(IBaseTexture* pRes)
{
	if(pRes == NULL)
		return 0;
	return pRes->memUsage();
}

END_NAMESPACE_XEVOL3D

