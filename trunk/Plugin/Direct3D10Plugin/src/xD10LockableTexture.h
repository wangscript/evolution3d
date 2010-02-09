#ifndef _XEVOL_DIRECT3D10_LOCKABLE_TEXTURE_H_
#define _XEVOL_DIRECT3D10_LOCKABLE_TEXTURE_H_

#include "xD10BaseTexture.h"
#include "xD10BaseRenderTarget.h"
#include "xD10RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

class xD10LockableTexture;


class xD10LockableTexture : public xD10UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10LockableTexture);
protected:
	bool                      m_bReadable;
	xD10TextureBuffer         m_texBuffer;
	ID3D10Texture2D*          m_2DTexture;
	ID3D10Texture3D*          m_3DTexture;
protected:
	//bool                     _load(ID3D10Resource* pTexture);
	bool                     _create2DTexture();
	bool                     _create3DTexture();

	bool                     _lockreadAble(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	bool                     _unlockreadAble(xTextureLockArea& lockInfo);

public:
	xD10LockableTexture(xD3D10RenderApi* pAPI , bool bReadable = false);
	~xD10LockableTexture();
	virtual void*          handle();
	virtual bool           load(const wchar_t* fileName , unsigned long  arg);
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool           isLoaded();
	virtual bool           unload();

	virtual bool           update(void* data  , int dateLen , int rowPitch , int depthPicth =0 , int mipmapLevel = 0 , int arraySlice = 0);
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	virtual bool           unlock(xTextureLockArea& lockInfo);
	virtual bool           create(int w , int h , ePIXEL_FORMAT fmt , int mipMapLevels = 1 , int arraySize = 1);
	virtual bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt , int mipMapLevels = 1 , int arraySize = 1);
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
};


END_NAMESPACE_XEVOL3D

#endif

