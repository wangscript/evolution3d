#include "xWin32Thread.h"


BEGIN_NAMESPACE_XEVOL3D
//Locker

xWin32Locker::xWin32Locker()
{
	InitializeCriticalSection(&m_locker);
}
xWin32Locker::~xWin32Locker()
{
	DeleteCriticalSection(&m_locker);
}
bool    xWin32Locker::lock()
{
	EnterCriticalSection(&m_locker);
	return true;
}
bool    xWin32Locker::trylock()
{
	return TryEnterCriticalSection(&m_locker) == TRUE;
}

void    xWin32Locker::unlock()
{
	LeaveCriticalSection(&m_locker);
}
size_t xWin32Locker::release()
{
	m_RefCount -- ;
	if(m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

size_t xWin32Locker::addRef()
{
	m_RefCount ++ ;
	return m_RefCount;
}

//Singal

xWin32Signal::xWin32Signal()
{
	static int signalID = 0;
	wchar_t signalName[64]={0};
	swprintf_x(signalName,64,L"XRSignal:%d",signalID);
	signalID ++;
	m_hEvent = CreateEventW(NULL,FALSE,FALSE,signalName) ;
	m_RefCount = 1;
}
xWin32Signal::~xWin32Signal()
{
	if(m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}
bool    xWin32Signal::wait(unsigned long mSecond)
{
	DWORD  wTime = mSecond;

	if(mSecond != -1)  
	{
		wTime = INFINITE;
	}
	DWORD ret = WaitForSingleObject(m_hEvent , mSecond);
	if(ret == WAIT_OBJECT_0) 
		return true;
	return false;
}
bool    xWin32Signal::reset()
{
	if(m_hEvent != NULL)
	{
		ResetEvent(m_hEvent);
		return true;
	}
	return false;
}
bool    xWin32Signal::fire()
{
	if(m_hEvent != NULL)
	{
		SetEvent(m_hEvent);
		return true;
	}
	return false;       
}

size_t xWin32Signal::release()
{
	m_RefCount -- ;
	if(m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

size_t xWin32Signal::addRef()
{
	m_RefCount ++ ;
	return m_RefCount;
}



//Thread
DWORD WINAPI xWin32Thread::threadProc(void* arg)
{
	xThreadProc* pThread = (xThreadProc*)arg;
	return pThread->thread_proc();
}

xWin32Thread::xWin32Thread()
{
	m_RefCount = 1;
	m_hThread = NULL;
}

xWin32Thread::~xWin32Thread()
{
	if(NULL != m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void xWin32Thread::wait_exit()
{
	if(m_hThread != NULL)
	{
		::WaitForSingleObject(m_hThread , INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

bool xWin32Thread::run(xThreadProc* pThread)
{
	if(m_hThread != NULL)
		return false;
	DWORD threadID = 0;
	m_hThread = CreateThread(NULL,0, this->threadProc,(void*)this,0, &threadID);
	return m_hThread != NULL;
}

bool xWin32Thread::exit_Thread(int exitCode)
{
	ExitThread(exitCode);
	return true;
}

size_t xWin32Thread::release()
{
	m_RefCount -- ;
	if(m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

size_t xWin32Thread::addRef()
{
	m_RefCount ++ ;
	return m_RefCount;
}

END_NAMESPACE_XEVOL3D
