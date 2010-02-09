#include "../../xStdPch.h"
#include "xWin32IME.h"
#include <Windows.h>
#include <WinGDI.h>
#include <Imm.h>

#pragma warning (disable : 4312)
#ifndef _WIN32_WCE
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"gdi32.lib")
#endif

BEGIN_NAMESPACE_XEVOL3D

IIMEInput* xOperationSys::getImeInput()
{
	if ( m_pImeInput == NULL )
		m_pImeInput = new xWnd32IMEInput;
	return m_pImeInput;
}
void xOperationSys::unInit()
{
	if ( m_pImeInput )
		delete m_pImeInput;
	m_pImeInput = NULL;
}

xWnd32IMEInput::xWnd32IMEInput()
{
}

bool xWnd32IMEInput::setCompositionFont(int hImc ,  const wchar_t* strFamilly, int nHeight , bool bBold , bool bItalic , bool bUnderLine)
{
	LOGFONTW logFont;
	logFont.lfHeight = -nHeight;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = bBold ? 900 : 400;
	logFont.lfItalic = bItalic ? 1 : 0;
	logFont.lfUnderline = bUnderLine ? 1 : 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = GB2312_CHARSET;
#ifndef _WIN32_WCE
	logFont.lfOutPrecision = OUT_TT_PRECIS;
#else
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
#endif
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = 5;
	logFont.lfPitchAndFamily = FF_DONTCARE;
	wcscpy(logFont.lfFaceName,strFamilly);

	ImmSetCompositionFontW((HIMC)hImc, &logFont);
	return true;
}

int  xWnd32IMEInput::associateContext(int hWndHandle, int hImc)
{
	ImmAssociateContext((HWND)hWndHandle, (HIMC)hImc);
	return 0;
}

bool xWnd32IMEInput::releaseContext(int hWndHandle, int hImc)
{
	ImmReleaseContext((HWND)hWndHandle, (HIMC)hImc);
	return false;
}

void*  xWnd32IMEInput::getContextImc(int hWndHandle)
{
	return (void*)ImmGetContext((HWND)hWndHandle);
}

void xWnd32IMEInput::setCompositionWndPos(int hImc, int x, int y)
{
	COMPOSITIONFORM cf;
	cf.dwStyle = CFS_POINT;
	cf.ptCurrentPos.x = x;
	cf.ptCurrentPos.y = y;
	ImmSetCompositionWindow((HIMC)hImc, &cf);
}

END_NAMESPACE_XEVOL3D
