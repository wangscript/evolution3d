#include "xBaseLexer.h"
#include <assert.h>
BEGIN_NAMESPACE_XEVOL3D

_XEVOL_BASE_API_ bool  XEvol_IsReadalbeChar(int ch)
{
	if(XEvol_IsAlphaNumChar(ch))
		return true;
	return false;
}

END_NAMESPACE_XEVOL3D

