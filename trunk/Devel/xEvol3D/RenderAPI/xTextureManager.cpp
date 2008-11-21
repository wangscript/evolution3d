#include "xTextureManager.h"
#include "xRenderView.h"
#include "xRenderAPI.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

xTextureName::xTextureName()
{
	m_Name       = L"";
	m_Width      = 0 ;
	m_Height     = 0 ;
	m_Depth      = 0 ;
	m_fmt        = PIXELFORMAT_None;
	m_ArraySize  = 1;
	m_bR2Texture = false;
	m_Type       = RESOURCE_TEXTURE;
}

xTextureName::xTextureName(const wchar_t* fileName)
{
	m_Name       = fileName;
	m_Width      = 0 ;
	m_Height     = 0 ;
	m_Depth      = 0 ;
	m_fmt        = PIXELFORMAT_None;
	m_ArraySize  = 1;
	m_bR2Texture = false;
	m_Type       = RESOURCE_TEXTURE;
	m_bLockable  = false;
}

xTextureName::xTextureName(const wchar_t* _Name , ePIXEL_FORMAT fmt , eResourceType type ,bool rtt, bool bLockable , int w , int h , int depth , int arraySize)
{
	m_Name       = _Name;
	m_Width      = w ;
	m_Height     = h ;
	m_Depth      = depth ;
	m_fmt        = fmt;
	m_ArraySize  = arraySize;
	m_bR2Texture = rtt;
	m_Type       = type;
	m_bLockable  = bLockable;
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
     xBaseTextureMgr* pMgr = dynamic_cast<xBaseTextureMgr*>(this);
     return pMgr->add(texName, arg ,bLoadImm);
}

HBaseTexture  xBaseTextureLoader::loadTexture(const wchar_t* texName, bool bLoadImm  , bool arg)
{
	xTextureName _name(texName);
	return load(_name, arg ,bLoadImm);
}

HBaseTexture  xBaseTextureLoader::loadTexture(const wchar_t* texName , ePIXEL_FORMAT fmt , int w , int h , int depth , eResourceType type, int arraySize, bool rtt, bool bLockable) 
{
     xTextureName _name(texName);
	 _name.m_ArraySize = arraySize;
	 _name.m_bR2Texture = rtt;
	 _name.m_Depth = depth;
	 _name.m_fmt = fmt;
	 _name.m_Height = h;
	 _name.m_Name = texName;
	 _name.m_Type = type;
	 _name.m_Width = w;
	 _name.m_bLockable = bLockable;
	 return load(_name,true , 0);
}

IBaseTexture*   xBaseTextureLoader::_loadTexture(const  xTextureName& _texName , const wchar_t* fileName ,unsigned char* buf , int bufLen , unsigned int arg)
{
	const wchar_t* _extName = xFileSystem::singleton()->getFileExtName(_texName.m_Name.c_str() );
	ITextureLoader* pTextureLoader = xTextureLdrMgr::singleton()->findTextureLoader(_extName);
	if(pTextureLoader == NULL)
		return m_pRenderApi->createFileTexture( fileName , buf,bufLen,arg );
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
			xResPathManager::ResPackageItem& item = this->m_Packages[i];
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
						pstream->release();
						return true;
					}
					else
					{
						delete[]  buf;
					}
					pstream->release();
				}

			}
		}
	}

	return true;
}

bool  xBaseTextureLoader::_loadResource(const xTextureName& strResName   , IBaseTexture* & pRes , int& ResSize, unsigned int arg)
{
	if(strResName.m_Type == RESOURCE_TEXTURE)
	{
		return _loadTexture(strResName.m_Name.c_str() , pRes , ResSize , arg);        
	}
	else if(strResName.m_Type == RESOURCE_TEXTURE2D)
	{
		if(strResName.m_bR2Texture == false)
		   pRes = m_pRenderApi->createLockableTexture(strResName.m_Width , strResName.m_Height , strResName.m_fmt , strResName.m_bLockable );
		else
		   pRes = m_pRenderApi->createRenderableTexture(strResName.m_Width , strResName.m_Height , strResName.m_Depth , strResName.m_fmt , strResName.m_bLockable); 
		return pRes != NULL;
	}
	else if(strResName.m_Type == RESOURCE_RENDERTARGET)
	{
		IRenderTarget* pRT = m_pRenderApi->createRenderTarget(strResName.m_Width , strResName.m_Height , strResName.m_fmt , false , true);
		pRes = pRT->toTexture();
		return pRes != NULL;
	}
	else if(strResName.m_Type == RESOURCE_DEPTHBUFFER)
	{
		IRenderTarget* pRT = m_pRenderApi->createDepthBuffer(strResName.m_Width , strResName.m_Height , strResName.m_fmt , false , false);
		pRes = pRT->toTexture();
		return pRes != NULL;
	}
	return false;
}

bool  xBaseTextureLoader::_unloadResource(const xTextureName& strResName , IBaseTexture* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= pRes->memUsage();
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

