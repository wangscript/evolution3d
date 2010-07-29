#include "../../xStdPch.h"
#ifdef _UNIX

#ifdef _ENABLE_XLIB_
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


#include <map>
#include "../xOperationSys.h"
#include "../../Application/xWindowMsg.h"

BEGIN_NAMESPACE_XEVOL3D

#ifdef _ENABLE_XLIB_
struct XlibVkeyItem
{
	KeySym         _ksym;
	VIRTUAL_KEY   _vkey;
	unsigned char _chcode;
	bool          _bChar;
};

typedef std::map<KeySym ,  XlibVkeyItem> XlibVkeyItemMaps;

class CXlibVkeyItemMap
{
    XlibVkeyItemMaps m_keymaps;
public:
    XlibVkeyItem findKeyItem(KeySym keysym)
	{
		XlibVkeyItemMaps::iterator pos = m_keymaps.find( keysym );
		if(pos != m_keymaps.end())
			return (pos->second);
		static XlibVkeyItem item = {0 , 0 , 0 , false};
		return item;
	}

	void InsertKeyItem(KeySym ksym  , VIRTUAL_KEY vkey , unsigned char ch , bool bChar = false)
	{
		XlibVkeyItem key;
		key._ksym = ksym;
		key._vkey = vkey;
		key._chcode = ch;
		key._bChar = bChar;
		m_keymaps.insert(  XlibVkeyItemMaps::value_type(ksym , key) );
	}

	static CXlibVkeyItemMap* singleto()
	{
		static CXlibVkeyItemMap* g_map = NULL;
		if(g_map == NULL) g_map = new CXlibVkeyItemMap;
		return g_map;
	}
protected:
	CXlibVkeyItemMap()
	{
		InsertKeyItem(XK_Home      , VKEY_HOME     , 0,  false);
		InsertKeyItem(XK_Left      , VKEY_LEFT     , 0,  false);
		InsertKeyItem(XK_Up        , VKEY_UP       , 0,  false);
		InsertKeyItem(XK_Right     , VKEY_RIGHT    , 0,  false);
		InsertKeyItem(XK_Down      , VKEY_DOWN     , 0,  false);
		InsertKeyItem(XK_Prior     , VKEY_PRIOR    , 0,  false);
		InsertKeyItem(XK_Page_Up   , VKEY_PRIOR    , 0,  false);
		InsertKeyItem(XK_Next      , VKEY_NEXT     , 0,  false);
		InsertKeyItem(XK_Page_Down , VKEY_NEXT     , 0,  false);
		InsertKeyItem(XK_End       , VKEY_END      , 0,  false);
		InsertKeyItem(XK_Begin     , VKEY_HOME     , 0,  false);
		InsertKeyItem(XK_KP_0      , VKEY_NUMPAD0  , '0', true );
		InsertKeyItem(XK_KP_1      , VKEY_NUMPAD1  , '1', true );
		InsertKeyItem(XK_KP_2      , VKEY_NUMPAD2  , '2', true );
		InsertKeyItem(XK_KP_3      , VKEY_NUMPAD3  , '3', true );
		InsertKeyItem(XK_KP_4      , VKEY_NUMPAD4  , '4', true );
		InsertKeyItem(XK_KP_5      , VKEY_NUMPAD5  , '5', true );
		InsertKeyItem(XK_KP_6      , VKEY_NUMPAD6  , '6', true );
		InsertKeyItem(XK_KP_7      , VKEY_NUMPAD7  , '7', true );
		InsertKeyItem(XK_KP_8      , VKEY_NUMPAD8  , '8', true );
		InsertKeyItem(XK_KP_9      , VKEY_NUMPAD9  , '9', true );
		InsertKeyItem(XK_F1        , VKEY_F1      , 0,  false);
		InsertKeyItem(XK_F2        , VKEY_F2      , 0,  false);
		InsertKeyItem(XK_F3        , VKEY_F3      , 0,  false);
		InsertKeyItem(XK_F4        , VKEY_F4      , 0,  false);
		InsertKeyItem(XK_F5        , VKEY_F5      , 0,  false);
		InsertKeyItem(XK_F6        , VKEY_F6      , 0,  false);
		InsertKeyItem(XK_F7        , VKEY_F7      , 0,  false);
		InsertKeyItem(XK_F8        , VKEY_F8      , 0,  false);
		InsertKeyItem(XK_F9        , VKEY_F9      , 0,  false);
		InsertKeyItem(XK_F10       , VKEY_F10     , 0,  false);
		InsertKeyItem(XK_F11       , VKEY_F11     , 0,  false);
		InsertKeyItem(XK_F12       , VKEY_F12     , 0,  false);
		InsertKeyItem(XK_Alt_L     , VKEY_LMENU   , 0,  false );
		InsertKeyItem(XK_Alt_R     , VKEY_RMENU   , 0,  false );
		InsertKeyItem(XK_Shift_L   , VKEY_LSHIFT  , 0,  false );
		InsertKeyItem(XK_Shift_R   , VKEY_RSHIFT  , 0,  false );
		InsertKeyItem(XK_Control_L , VKEY_LCONTROL, 0,  false );
		InsertKeyItem(XK_Control_R , VKEY_RCONTROL, 0,  false );
		InsertKeyItem(XK_0   , '0'  , '0', true);
		InsertKeyItem(XK_1   , '1'  , '1', true);
		InsertKeyItem(XK_2   , '2'  , '2', true);
		InsertKeyItem(XK_3   , '3'  , '3', true);
		InsertKeyItem(XK_4   , '4'  , '4', true);
		InsertKeyItem(XK_5   , '5'  , '5', true);
		InsertKeyItem(XK_6   , '6'  , '6', true);
		InsertKeyItem(XK_7   , '7'  , '7', true);
		InsertKeyItem(XK_8   , '8'  , '8', true);
		InsertKeyItem(XK_9   , '9'  , '9', true);

		InsertKeyItem(XK_A , 'A', 'A', true);
		InsertKeyItem(XK_B , 'B', 'B', true);
		InsertKeyItem(XK_C , 'C', 'C', true);
		InsertKeyItem(XK_D , 'D', 'D', true);
		InsertKeyItem(XK_E , 'E', 'E', true);
		InsertKeyItem(XK_F , 'F', 'F', true);
		InsertKeyItem(XK_G , 'G', 'G', true);
		InsertKeyItem(XK_H , 'H', 'H', true);
		InsertKeyItem(XK_I , 'I', 'I', true);
		InsertKeyItem(XK_J , 'J', 'J', true);
		InsertKeyItem(XK_K , 'K', 'K', true);
		InsertKeyItem(XK_L , 'L', 'L', true);
		InsertKeyItem(XK_M , 'M', 'M', true);
		InsertKeyItem(XK_N , 'N', 'N', true);
		InsertKeyItem(XK_O , 'O', 'O', true);
		InsertKeyItem(XK_P , 'P', 'P', true);
		InsertKeyItem(XK_Q , 'Q', 'Q', true);
		InsertKeyItem(XK_R , 'R', 'R', true);
		InsertKeyItem(XK_S , 'S', 'S', true);
		InsertKeyItem(XK_T , 'T', 'T', true);
		InsertKeyItem(XK_U , 'U', 'U', true);
		InsertKeyItem(XK_V , 'V', 'V', true);
		InsertKeyItem(XK_W , 'W', 'W', true);
		InsertKeyItem(XK_X , 'X', 'X', true);
		InsertKeyItem(XK_Y , 'Y', 'Y', true);
		InsertKeyItem(XK_Z , 'Z', 'Z', true);

		InsertKeyItem(XK_a , 'A', 'a', true);
		InsertKeyItem(XK_b , 'B', 'b', true);
		InsertKeyItem(XK_c , 'C', 'c', true);
		InsertKeyItem(XK_d , 'D', 'd', true);
		InsertKeyItem(XK_e , 'E', 'e', true);
		InsertKeyItem(XK_f , 'F', 'f', true);
		InsertKeyItem(XK_g , 'G', 'g', true);
		InsertKeyItem(XK_h , 'H', 'h', true);
		InsertKeyItem(XK_i , 'I', 'i', true);
		InsertKeyItem(XK_j , 'J', 'j', true);
		InsertKeyItem(XK_k , 'K', 'k', true);
		InsertKeyItem(XK_l , 'L', 'l', true);
		InsertKeyItem(XK_m , 'M', 'm', true);
		InsertKeyItem(XK_n , 'N', 'n', true);
		InsertKeyItem(XK_o , 'O', 'o', true);
		InsertKeyItem(XK_p , 'P', 'p', true);
		InsertKeyItem(XK_q , 'Q', 'q', true);
		InsertKeyItem(XK_r , 'R', 'r', true);
		InsertKeyItem(XK_s , 'S', 's', true);
		InsertKeyItem(XK_t , 'T', 't', true);
		InsertKeyItem(XK_u , 'U', 'u', true);
		InsertKeyItem(XK_v , 'V', 'v', true);
		InsertKeyItem(XK_w , 'W', 'w', true);
		InsertKeyItem(XK_x , 'X', 'x', true);
		InsertKeyItem(XK_y , 'Y', 'y', true);
		InsertKeyItem(XK_z , 'Z', 'z', true);

		InsertKeyItem(XK_BackSpace   , VKEY_BACK   , 0,  false );
		InsertKeyItem(XK_Tab         , VKEY_TAB    , 0,  false );
		InsertKeyItem(XK_Return      , VKEY_RETURN , 0,  false );
		InsertKeyItem(XK_Pause       , VKEY_PAUSE  , 0,  false );
		InsertKeyItem(XK_Scroll_Lock , VKEY_SCROLL , 0,  false );
		InsertKeyItem(XK_Escape      , VKEY_ESCAPE , 0,  false );
		InsertKeyItem(XK_Delete      , VKEY_DELETE , 0,  false );
		InsertKeyItem(XK_Insert      , VKEY_INSERT , 0,  false );
		InsertKeyItem(XK_Num_Lock    , VKEY_NUMLOCK, 0,  false );

		//InsertKeyItem(XK_Break       , VKEY_BREAK  , 0,  false );
    	//InsertKeyItem( , , , );
		//InsertKeyItem( , , , );
	}
};


bool convertXLibMsg(XKeyEvent& key_event, xWindowMsg& basMsg)
{
	KeySym    key_code  = XLookupKeysym(&key_event,0);
	XlibVkeyItem item = CXlibVkeyItemMap::singleto()->findKeyItem(key_code);
	basMsg.Keyboard.bIsCharKey = item._bChar;
	basMsg.Keyboard.cKeyChar   = item._chcode;
	basMsg.Keyboard.nKey       = item._vkey;
	basMsg.Keyboard.nSysVKey   = key_code;
	basMsg.Keyboard.nScanCode  = 0;
	return item._ksym == key_code;
}

bool convertXLibMsg(XEvent& event, xWindowMsg& basMsg)
{
	XKeyEvent key_event;
	
	if(event.type == KeyPress)
	{
		key_event = event.xkey;
		convertXLibMsg(key_event,basMsg);
		basMsg.eMsgID = WIN_KEYDOWN;
		return true;
	}
	else if(event.type == KeyRelease)
	{
		key_event = event.xkey;
		convertXLibMsg(key_event,basMsg);
		basMsg.eMsgID = WIN_KEYUP;
		return true;
	}
	else if(event.type == ConfigureNotify)
	{
		int w = event.xconfigure.width;
		int h = event.xconfigure.height;
		basMsg.eMsgID = WIN_RESIZE;
		basMsg.Common.w   = w; 
		basMsg.Common.h   = h;
        return true;
	}
	return false;
}
#endif

class xLinuxIMEInput : public IIMEInput
{
public:
	xLinuxIMEInput(){}
	bool  setCompositionFont(int hImc, const wchar_t* strFamilly, int nHeight , bool bBold , bool bItalic , bool bUnderLine){return false; }
	int   associateContext(int hWndHandle, int hImc){return 0; }
	bool  releaseContext(int hWndHandle, int hImc){return false; }
	void* getContextImc(int hWndHandle){return 0; }
	void  setCompositionWndPos(int hImc, int x, int y){return ; }
};

unsigned long timeGetTime()
{
	return xGetTickCount();
}
unsigned long xGetTickCount()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv,&tz);
	return (unsigned long) ( tv.tv_sec * 1000 + tv.tv_usec/1000 );
}

void          xSleep(unsigned long mSecond)
{
	usleep(mSecond * 10000);
}
const wchar_t*  xOperationSys::name()
{
	static wchar_t* _osName = L"Linux";
	return _osName;
}

void  xOperationSys::convertSystemFileName(std::ds_wstring& _fileName)
{
	for(size_t i = 0 ; i < _fileName.length() ; i ++)
	{
		if(_fileName[i] == '\\')
		{
			_fileName[i] = '/';
		}
	}
}

IIMEInput* xOperationSys::getImeInput()
{
	if(m_pImeInput == NULL) m_pImeInput = new xLinuxIMEInput;
	return m_pImeInput;
}

xDLLModule*    xOperationSys::createDLL(const wchar_t* dllName)
{
	xLinuxDll* dll = new xLinuxDll;
	if( dll->load(dllName) )
		return dll;
	delete dll;
	return NULL;
}

xThreadLocker* xOperationSys::createThreadLocker()
{
	return NULL;
}

xThreadSignal* xOperationSys::createThreadSignal()
{
	return NULL;
}
xThreadRunner* xOperationSys::createThreadRunner()
{
	return NULL;
}

int  xOperationSys::warnningBox(const wchar_t* message , const wchar_t* title)
{
	return 1;
}

int  xOperationSys::messageBox(const wchar_t* message , const wchar_t* title)
{
	return 1;
}

bool  xOperationSys::clearClipboard()
{
	return false;
}

bool  xOperationSys::copyToClipboard(const wchar_t* clipBox)
{
return false;
}

std::ds_wstring  xOperationSys::getClipboardString()
{
	return L"";
}

void xOperationSys::setCursorStyle(int nStyle)
{
	return ;
}
void xOperationSys::enableConsole()
{

}
END_NAMESPACE_XEVOL3D

#endif
