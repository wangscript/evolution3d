#ifndef __xVIDEO_TEXTURE_LOADER_H__
#define __xVIDEO_TEXTURE_LOADER_H__
#include <RenderAPI/xBaseTexture.h>
#include <RenderAPI/xRenderAPI.h>

USING_NS_XEVOL3D;

class xVideoTextureLoader : public ITextureLoader
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xVideoTextureLoader);
	IBaseTexture*  createTexture(IRenderApi* pApi);
	IBaseTexture*  loadTexture(IRenderApi* pApi , const wchar_t* textFile, void* _buf , int bufLen ,  const xTextureName& _texName );
	size_t         nExt();
	const wchar_t* ext(size_t index);
    xVideoTextureLoader();
    ~xVideoTextureLoader();
};

class xVideoCaptureTextureLoader : public ITextureLoader
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xVideoCaptureTextureLoader);
	IBaseTexture*  createTexture(IRenderApi* pApi);
	IBaseTexture*  loadTexture(IRenderApi* pApi , const wchar_t* textFile , void* _buf , int bufLen,  const xTextureName& _texName );
	size_t         nExt();
	const wchar_t* ext(size_t index);
    xVideoCaptureTextureLoader();
    ~xVideoCaptureTextureLoader();
};

#endif