#ifndef _XEVOL_DIRECT3D11_FILE_2DTEXTURE_H_
#define _XEVOL_DIRECT3D11_FILE_2DTEXTURE_H_
#include "xD11BaseTexture.h"
#include "../xDirect3D11DllApi.h"
BEGIN_NAMESPACE_XEVOL3D
class xD11FileTexture : public xD11UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11FileTexture);
	bool                  __loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	xD11FileTexture(xD3D11RenderApi* pAPI );
	~xD11FileTexture();
	virtual bool          load(const wchar_t* fileName , unsigned long  arg);
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	virtual bool          create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) { return false ;}
	virtual bool          create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1){ return false ;}
protected:
	bool _prepareLoadInfo(D3DX11_IMAGE_INFO& imgInfo , D3DX11_IMAGE_LOAD_INFO& loadInfo);
};


END_NAMESPACE_XEVOL3D

#endif

