#include "../xOperationSys.h"
#include <Windows.h>
BEGIN_NAMESPACE_XEVOL3D

class xWin32Locker : public xThreadLocker
{
	CRITICAL_SECTION m_locker;
public:
	xWin32Locker();
	~xWin32Locker();
	bool    lock() ;
	bool    trylock();
	void    unlock();
	IMPL_REFCOUNT_OBJECT_INTERFACE(xWin32Locker);
};


class xWin32Signal : public  xThreadSignal
{
	HANDLE   m_hEvent;
public :
	~xWin32Signal();
	xWin32Signal();
	bool    wait(unsigned long mSecond);
	bool    fire();
	bool    reset();
	IMPL_REFCOUNT_OBJECT_INTERFACE(xWin32Signal);
};

class xWin32Thread: public  xThreadRunner
{
	HANDLE  m_hThread;
public:
	static  DWORD WINAPI threadProc(void* arg);
	IMPL_REFCOUNT_OBJECT_INTERFACE(xWin32Thread);
public:
	xWin32Thread();
	~xWin32Thread();
	bool    run(xThreadProc* pThread);
	void    wait_exit();//Wait for exit;
	bool    exit_Thread(int exitCode);
	bool    suspend();
	bool    resume() ;
};

END_NAMESPACE_XEVOL3D
