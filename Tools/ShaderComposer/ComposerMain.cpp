#include <windows.h>
#ifdef _DEBUG
#pragma comment(lib,"ShaderComposerD.lib")
#else
#pragma comment(lib,"ShaderComposer.lib")
#endif

#include "IShaderComposer.h"
#include "EditorRenderPreview.h"

BEGIN_NAMESPACE_XEVOL3D
bool ConvertToXEvolMsg(xWindowMsg& WinMsg , HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
END_NAMESPACE_XEVOL3D

class SimpleUICallback : public xEditorPreviewApp ,  public IShaderComposerUI::IShaderComposerUICb
{
public:
	HWND m_hRenderWindow;
public:
	SimpleUICallback()
	{
		m_hRenderWindow = NULL;
	}

	bool OnShaderComposerUIEvent(IShaderComposerUI::UIEvent _event , int param)
	{
		if(_event == IShaderComposerUI::eUIEVENT_Close)
		{
			PostQuitMessage(0);
			return true;
		}

		if(_event == IShaderComposerUI::eUIEVENT_QueryPreview)
		{
			return true;
		}
	}

	bool OnRenderPreviewEvent(HWND hRenderWindow, DWORD msg , LPARAM lParam , WPARAM wParam)
	{
		if(msg == WM_CREATE)
		{
			m_hRenderWindow = hRenderWindow;
			return true;
		}

		xWindowMsg WinMsg;
		WinMsg.m_pWndReciver = NULL;
		ConvertToXEvolMsg(WinMsg , hRenderWindow , msg , wParam , lParam);
		return onMessage(WinMsg);
	}
};


int WINAPI WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance,  LPSTR lpCmdLine,   int nCmdShow   )
{
	SimpleUICallback app;
	if(GetShaderComposerUI())
	{
		GetShaderComposerUI()->setEventCallback(&app);
		GetShaderComposerUI()->Init();
		GetShaderComposerUI()->ShowShaderComposer();
	}
    if(app.m_hRenderWindow == NULL)
		return 0;

	

	wchar_t appname[512] = {0};
	GetModuleFileName(NULL , appname , 512);
	xOperationSys::singleton()->init( appname );

	app.init(app.m_hRenderWindow);
	app.run();
	app.exit();
	IBaseObject::DumpObjectList();
	return 0;
}