#ifndef _XSTRING_TO_MATH_H_
#define _XSTRING_TO_MATH_H_

#include "../BaseLib/xEvol3DAPI.h"
#include "../XMathLib/xMathLib.h"
BEGIN_NAMESPACE_XEVOL3D
inline void xStringToVec2(xMathLib::xvec2& _v , const wchar_t* _str)
{
	if(_str == NULL) return ;
	swscanf(_str , L"[%f,%f]" , &_v.x , &_v.y);
}
inline void xStringToVec3(xMathLib::xvec3& _v , const wchar_t* _str)
{
	if(_str == NULL) return ;
    swscanf(_str , L"[%f,%f,%f]" , &_v.x , &_v.y , &_v.z );
}

inline void xStringToVec4(xMathLib::xvec4& _v , const wchar_t* _str)
{
	if(_str == NULL) return ;
	swscanf(_str , L"[%f,%f,%f,%f]" , &_v.x , &_v.y , &_v.z  , &_v.w);
}

inline void xStringToColor(xColor_4f& _v , const wchar_t* _str)
{
	if(_str == NULL) return ;
	swscanf(_str , L"[%f,%f,%f,%f]" , &_v.r , &_v.g , &_v.b  , &_v.a);
}

END_NAMESPACE_XEVOL3D
#endif
