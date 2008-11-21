#include "../xOperationSys.h"
#include <Windows.h>
BEGIN_NAMESPACE_XEVOL3D

class xWin32Locker : public xThreadLocker
{
	int              m_RefCount;
	CRITICAL_SECTION m_locker;
public:
	xWin32Locker();
	~xWin32Locker();
	bool    lock() ;
	bool    trylock();
	void    unlock();
	size_t  release();
	size_t  addRef();
};


class xWin32Signal : public  xThreadSignal
{
	HANDLE   m_hEvent;
	int      m_RefCount;
public :
	~xWin32Signal();
	xWin32Signal();
	bool    wait(unsigned long mSecond);
	bool    fire();
	bool    reset();
	size_t  release();
	size_t  addRef();
};

class xWin32Thread: public  xThreadRunner
{
	HANDLE  m_hThread;
	int     m_RefCount;
public:
	static  DWORD WINAPI threadProc(void* arg);
	xWin32Thread();
	~xWin32Thread();
	size_t  release();
	size_t  addRef();
	bool    run(xThreadProc* pThread);
	void    wait_exit();//Wait for exit;
	bool    exit_Thread(int exitCode);
};

END_NAMESPACE_XEVOL3D
