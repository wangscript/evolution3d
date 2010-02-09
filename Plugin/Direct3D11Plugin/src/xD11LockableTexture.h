#ifndef _XEVOL_DIRECT3D11_LOCKABLE_TEXTURE_H_
#define _XEVOL_DIRECT3D11_LOCKABLE_TEXTURE_H_
#include "../xDirect3D11DllApi.h"
#include "xD11BaseTexture.h"
#include "xD11BaseRenderTarget.h"
#include "xD11RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

class xD11LockableTexture;


class xD11LockableTexture : public xD11UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11LockableTexture);
protected:
	bool                      m_bReadable;
	xD11TextureBuffer         m_texBuffer;
	ID3D11Texture2D*          m_2DTexture;
	ID3D11Texture3D*          m_3DTexture;
protected:
	//bool                     _load(ID3D11Resource* pTexture);
	bool                     _create2DTexture();
	bool                     _create3DTexture();

	bool                     _lockreadAble(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	bool                     _unlockreadAble(xTextureLockArea& lockInfo);

public:
	xD11LockableTexture(xD3D11RenderApi* pAPI , bool bReadable = false);
	~xD11LockableTexture();
	virtual void*          handle();
	virtual bool           load(const wchar_t* fileName , unsigned long  arg);
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool           isLoaded();
	virtual bool           unload();

	virtual bool           update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel = 0 , int arraySlice = 0);
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	virtual bool           unlock(xTextureLockArea& lockInfo);
	virtual bool           create(int w , int h , ePIXEL_FORMAT fmt , int mipMapLevels = 1 , int arraySize = 1);
	virtual bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt , int mipMapLevels = 1 , int arraySize = 1);
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
};


END_NAMESPACE_XEVOL3D

#endif

