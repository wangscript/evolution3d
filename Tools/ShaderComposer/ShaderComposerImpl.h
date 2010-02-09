#pragma once
#include "IShaderComposer.h"
#include <vector>

BOOL DoExit();
BOOL DoInit();

class   CShaderComposerUIImpl : public IShaderComposerUI
{
public:
	// CShaderComposerApp initialization
	void setEventCallback(IShaderComposerUI::IShaderComposerUICb* cb)
	{
		removeEventCallback(cb);
		m_Callbacks.push_back( cb );
	}

	void removeEventCallback(IShaderComposerUI::IShaderComposerUICb* cb)
	{
		vShaderComposerUICbs::iterator pos = m_Callbacks.begin();
		for(; pos != m_Callbacks.end() ; pos ++)
		{
			if(*pos == cb)
			{
				m_Callbacks.erase(pos);
				return ;
			}
		}
		return ;
	}

	bool   fireEvent(IShaderComposerUI::UIEvent _event , int param)
	{
		for(size_t i = 0 ;i < m_Callbacks.size() ; i ++)
		{
			if(false == m_Callbacks[i]->OnShaderComposerUIEvent(_event , param) )
				return false;
		}
		return true;
	}

	bool fireRenderPreviewEvent(HWND hRenderWindow, DWORD msg , LPARAM lParam , WPARAM wParam)
	{
		for(size_t i = 0 ;i < m_Callbacks.size() ; i ++)
		{
			if(false == m_Callbacks[i]->OnRenderPreviewEvent(hRenderWindow , msg , lParam , wParam) )
				return false;
		}
		return true;
	}

	bool Init()
	{
		 return DoInit();
	}
	bool Uninit()
	{
		return DoExit();
	}
public:
	typedef std::vector<IShaderComposerUI::IShaderComposerUICb*>  vShaderComposerUICbs;
	vShaderComposerUICbs m_Callbacks;
};

inline CShaderComposerUIImpl * GetShaderComposerUIImpl()
{
	IShaderComposerUI* pApp = (IShaderComposerUI*)GetShaderComposerUI();
	CShaderComposerUIImpl* pUI = dynamic_cast<CShaderComposerUIImpl*>(pApp);
	return pUI;
}