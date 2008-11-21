#include "videoTexLoader.h"
#include "xRE_VideoTexture.h"
#include <fs/xFileSystem.h>
#include <RenderAPI/xVideoTexture.h>
IMPL_BASE_OBJECT_CLASSID(xVideoTextureLoader , ITextureLoader);
IMPL_BASE_OBJECT_CLASSID(xVideoCaptureTextureLoader , ITextureLoader);
//====================VideoTexture Loade =======================================================
IBaseTexture*  xVideoTextureLoader::createTexture(IRenderApi* pApi)
{
	return new xR_VideoTexture(pApi , false , true , true);
}

IBaseTexture*  xVideoTextureLoader::loadTexture(IRenderApi* pApi , const wchar_t* _fileName , void* _buf , int bufLen ,   const xTextureName& _texName )
{
	if(_buf != NULL ) //不支持从Buffer读取
		return NULL;

	if( xFileSystem::singleton()->fileExist(_fileName) == false )
		return NULL;

	xR_VideoTexture * pXRVTexture = new xR_VideoTexture(pApi , true, _texName.m_fmt == PIXELFORMAT_YV12 , _texName.m_fmt == PIXELFORMAT_YVY2);
	IBaseTexture* pVideoTexture  =  dynamic_cast<IBaseTexture*>(pXRVTexture);

	IVideoTextureCtrl* pDecoder =( IVideoTextureCtrl* )pVideoTexture->queryObject( IVideoTextureCtrl::ms_CLASSID );
	if(pDecoder)
	{
		if( pDecoder->load( _fileName )  )
		{
			pDecoder->play();
		}
	}
	else
	{
		pVideoTexture->ReleaseObject();
		pVideoTexture = NULL;
	}
	return pVideoTexture;
}

size_t      xVideoTextureLoader ::nExt()
{
    return 5;
}

const wchar_t* xVideoTextureLoader ::ext(size_t index)
{
	static wchar_t* gs_ext[]=
	{
		L"mpeg"  ,
		L"mpg" ,
		L"asf",
		L"avi",
		L"wmv",
	};
	return gs_ext[index];
}

//===========================================
IBaseTexture*  xVideoCaptureTextureLoader::createTexture(IRenderApi* pApi)
{
	return new xR_VideoTexture(pApi , false, true , true);
}
IBaseTexture*  xVideoCaptureTextureLoader::loadTexture(IRenderApi* pApi , const wchar_t* _fileName , void* _buf , int bufLen,  const xTextureName& _texName )
{
	if(_buf != NULL ) //不支持从Buffer读取
		return NULL;

	if( xFileSystem::singleton()->fileExist(_fileName) == false )
		return NULL;

	xR_VideoTexture * pXRVTexture = new xR_VideoTexture(pApi , false , _texName.m_fmt == PIXELFORMAT_YV12 , _texName.m_fmt == PIXELFORMAT_YVY2);
	IBaseTexture* pVideoTexture  =  dynamic_cast<IBaseTexture*>(pXRVTexture);

	IVideoTextureCtrl* pDecoder =( IVideoTextureCtrl* )pVideoTexture->queryObject( IVideoTextureCtrl::ms_CLASSID );
	if(pDecoder)
	{
		if( pDecoder->load( _fileName )  )
		{
			pDecoder->play();
		}
	}
	else
	{
		pVideoTexture->ReleaseObject();
		pVideoTexture = NULL;
	}
	return pVideoTexture;
}


size_t      xVideoCaptureTextureLoader::nExt()
{
	return 3;
}

const wchar_t* xVideoCaptureTextureLoader::ext(size_t index)
{
	static wchar_t* gs_ext[]=
	{
		L"camera" ,
		L"dev" ,
		L"device"
	};
    return gs_ext[index];
}
