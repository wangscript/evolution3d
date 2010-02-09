#pragma  once

#ifdef _BUILD_SHADER_COMPSER_DLL_
#define SHADERCOMPOSER_API __declspec(dllexport)
#else
#define SHADERCOMPOSER_API __declspec(dllimport)
#endif
#include <windows.h>
class IShaderComposerUI
{
public:
	enum UIEvent
	{
		eUIEVENT_Close,
		eUIEVENT_QueryPreview,
		eUIEVENT_Max,
	};
	class IShaderComposerUICb
	{
	public:
		virtual bool OnShaderComposerUIEvent(UIEvent _event , int param) = 0 ;
		virtual bool OnRenderPreviewEvent(HWND hRenderWindow, DWORD msg , LPARAM lParam , WPARAM wParam) = 0;
	};
public:
	virtual void setEventCallback(IShaderComposerUICb* cb) = 0;
	virtual void removeEventCallback(IShaderComposerUICb* cb) = 0;
	virtual int  ShowShaderComposer() = 0;
	virtual int  HideShaderComposer() = 0;
	virtual bool Init() = 0;
	virtual bool Uninit() = 0;
};

extern "C"
{
   SHADERCOMPOSER_API IShaderComposerUI * GetShaderComposerUI();
};


