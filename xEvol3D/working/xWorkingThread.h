#ifndef _XEVOL_WORKINGTHREAD_H__
#define _XEVOL_WORKINGTHREAD_H__
#include "../BaseLib/xEvol3DBaseInc.h"
#include "../OperationSys/xOperationSys.h"
#include <deque>

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ IWorkingJob : public IRefCountObject
{
public:
	virtual bool  isWorking()    = 0;
	virtual void  beginWorking() = 0;
	virtual void  endWorking()   = 0;
	virtual bool  doWorking()    = 0;
};

class _XEVOL_BASE_API_ xWorkingThread : public xThreadProc
{
public:
	typedef std::deque<IWorkingJob* , dllsafe_alloc<IWorkingJob*> > xJobQueue;
public:
	class     IWorkingThreadCallback
	{
	public:
		virtual bool OnJobFinish(xWorkingThread* pThread , IWorkingJob* pJob) = 0;
		virtual bool OnThreadIdle(xWorkingThread* pThread ) = 0;
	};

	void      exitThread(bool bWaited);
	int       nJob();
	bool      addJob(IWorkingJob* pJob);
	bool      clearJob();
	bool      pause();
	bool      resume();
	bool      nice(int level);
	int       thread_proc();
	int       start();
	int       stop();
	xWorkingThread(IWorkingThreadCallback* pCallBack);
	~xWorkingThread();
private:
	IWorkingThreadCallback* m_pCallBack;
	xJobQueue      m_JobQueue;
	xThreadSignal* m_pSignal;
	IWorkingJob*   m_pCurrentWorkJob;
	xThreadLocker* m_pQueueLocker;
};


class _XEVOL_BASE_API_ xWorkingThreadPool : public xWorkingThread::IWorkingThreadCallback
{
public:
	typedef std::vector<xWorkingThread* , dllsafe_alloc<xWorkingThread*> > vThreadPool;
	typedef std::deque<IWorkingJob* , dllsafe_alloc<IWorkingJob*> >        xJobQueue;
public:
	void init(int nThread);
	void uninit();
    bool addJob(IWorkingJob* pJob);
	bool clearJob();
	bool OnJobFinish(xWorkingThread* pThread , IWorkingJob* pJob);
	bool OnThreadIdle(xWorkingThread* pThread );
	xWorkingThreadPool();
	~xWorkingThreadPool();
private:
	vThreadPool    m_vThreadPool;
	xThreadLocker* m_pQueueLocker;
	xJobQueue      m_JobQueue;
};

END_NAMESPACE_XEVOL3D
#endif
