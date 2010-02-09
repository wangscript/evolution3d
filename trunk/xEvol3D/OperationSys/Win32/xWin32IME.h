#include "../xOperationSys.h"
#include <Windows.h>



BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xWnd32IMEInput : public IIMEInput
{
public:
	xWnd32IMEInput();
	bool   setCompositionFont(int hImc, const wchar_t* strFamilly, int nHeight , bool bBold , bool bItalic , bool bUnderLine);
	int    associateContext(int hWndHandle, int hImc);
	bool   releaseContext(int hWndHandle, int hImc);
	void*  getContextImc(int hWndHandle);
	void   setCompositionWndPos(int hImc, int x, int y);
};

END_NAMESPACE_XEVOL3D
