#include "xBaseLexer.h"
#include <assert.h>
BEGIN_NAMESPACE_XEVOL3D

bool  XEvol_IsReadalbeChar(int ch)
{
	if(XEvol_IsAlphaNumChar(ch))
		return true;
	return false;
}

END_NAMESPACE_XEVOL3D

