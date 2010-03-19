#ifndef _XEVOL_BASEMATH_H_
#define _XEVOL_BASEMATH_H_
#include "xEvol3DBaseInc.h"
#include <assert.h>
#include <ctype.h>
BEGIN_NAMESPACE_XEVOL3D

inline int xRoundToByte(int len , int nByte)
{
	if(len == 0) return 0;
	if(nByte == 0) return len;
	if(len % nByte == 0)
		return len;
	int _add = nByte - len % nByte;
	return len + _add;
}

inline unsigned int xFloorToPower2(unsigned int v)
{
	//int i = 1;
	//while(1)
	//{
	//	if( i >= x) 
	//		return i;
	//	i *= 2;
	//}
	float f = (float)(v - 1);  
	return 1 << ((*(unsigned int*)(&f) >> 23) - 126);
}

inline int xGetMipLevelSize(int nFullSize , int mipLv)
{
    return nFullSize >> mipLv;
}

#define xIsPower2(x)   ( (x) & (x-1) ? false : true )

END_NAMESPACE_XEVOL3D

#endif
