#ifndef _XEVOL_OPENGL20_FILE_2DTEXTURE_H_
#define _XEVOL_OPENGL20_FILE_2DTEXTURE_H_
#include "xGL2BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D
class xGL2FileTexture : public xGL2UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2FileTexture);
	bool                  __loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	xGL2FileTexture(xGL2RenderApi* pAPI );
	~xGL2FileTexture();
	virtual bool          load(const wchar_t* fileName , unsigned long  arg);
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	virtual bool          create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) { return false ;}
	virtual bool          create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1){ return false ;}
};


END_NAMESPACE_XEVOL3D

#endif

