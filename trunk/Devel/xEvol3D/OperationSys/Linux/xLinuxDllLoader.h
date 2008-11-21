#include "../xOperationSys.h"
#include <dlfcn.h>
BEGIN_NAMESPACE_XEVOL3D

class xLinuxDll : public xDLLModule
{
	void *  m_hDLL;
	int     m_RefCount;
public:
	xLinuxDll();
	~xLinuxDll();
	bool    load(const wchar_t* dllName);
	bool    unload();
	void*   getProcAddress(const wchar_t* funcName);
	size_t  release();
	size_t  addRef();
};
END_NAMESPACE_XEVOL3D
