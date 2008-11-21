#include "../xOperationSys.h"
#include <Windows.h>
BEGIN_NAMESPACE_XEVOL3D

class xWin32Dll : public xDLLModule
{
	HMODULE m_hDLL;
	int     m_RefCount;
public:
	xWin32Dll();
	~xWin32Dll();
	bool    load(const wchar_t* dllName);
	bool    unload();
	void*   getProcAddress(const wchar_t* funcName);
	size_t  release();
	size_t  addRef();
};
END_NAMESPACE_XEVOL3D
