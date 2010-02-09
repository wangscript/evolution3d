#include "Color.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xPixelFormatAssit
{
public:
	static xPixelFormatAssit* singleton() ;
	virtual int               GetPixelFormatBBP(ePIXEL_FORMAT format) = 0;
	virtual ePIXEL_FORMAT     GetPixelFormat(const wchar_t* pixelFormat) = 0;
	virtual const wchar_t*    GetPixelFormatName(ePIXEL_FORMAT format) = 0;
};

END_NAMESPACE_XEVOL3D
