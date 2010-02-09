#include "../xStdPch.h"
#include "xWorkingThread.h"

BEGIN_NAMESPACE_XEVOL3D

int xWorkingThread::nJob()
{
	m_pQueueLocker->lock();
	int _n =  (int)m_JobQueue.size() + (m_pCurrentWorkJob == NULL? 0 : 1);
	m_pQueueLocker->unlock();
	return _n;
}

bool xWorkingThread::addJob(IWorkingJob* pJob)
{
	m_pQueueLocker->lock();
	m_JobQueue.push_back(pJob);
	pJob->AddRef();
	m_pSignal->fire();
	m_pQueueLocker->unlock();
	return true;
}

bool xWorkingThread::clearJob()
{
	m_pQueueLocker->lock();
	xJobQueue::iterator pos = m_JobQueue.begin();
	for(; pos != m_JobQueue.end() ; pos ++)
	{
		IWorkingJob* pWorkJob = *pos;
		pWorkJob->ReleaseObject();
	}

	if(m_pCurrentWorkJob)
	{
		m_pCurrentWorkJob->ReleaseObject();
		m_pCurrentWorkJob = NULL;
	}

	m_JobQueue.clear();
	m_pQueueLocker->unlock();
	return true;
}

bool xWorkingThread::pause()
{    
	if(m_pRunner) 
	{
		m_pRunner->suspend();
		return true;
	}
	return false;
}

bool xWorkingThread::resume()
{
	if(m_pRunner) 
	{
		m_pRunner->resume();
		return true;
	}
	return false;
}

bool xWorkingThread::nice(int level)
{
	return true;
}

int xWorkingThread::thread_proc()
{
	m_pSignal->fire();
	while(m_bExitThread == false)
	{
		m_pSignal->wait(10000);//10s 醒来一次
		m_pSignal->reset();

		while(  nJob() > 0 ) //有任务就去做
		{
			m_pQueueLocker->lock();
			m_pCurrentWorkJob = *m_JobQueue.begin();
			m_JobQueue.pop_front();
			m_pQueueLocker->unlock();

			m_pCurrentWorkJob->beginWorking();
			m_pCurrentWorkJob->doWorking();
			m_pCurrentWorkJob->endWorking();
			if(m_pCallBack) m_pCallBack->OnJobFinish(this, m_pCurrentWorkJob);

			m_pCurrentWorkJob = NULL;
		}

		if(m_pCallBack) m_pCallBack->OnThreadIdle(this);
	}
	return 0;

}

int xWorkingThread::start()
{
	if(m_pRunner)
	{
		return 1;
	}
	m_pRunner = xOperationSys::singleton()->createThreadRunner();
	run(m_pRunner);
	return 0;
}

int xWorkingThread::stop()
{
	if(m_pRunner == NULL)
		return 1;
	exitThread(true);
	return 0;
}

void  xWorkingThread::exitThread(bool bWaited)
{
	if(m_pRunner == NULL)
		return ;

	xThreadProc::exitThread(bWaited);
	clearJob();

	m_pRunner->KillObject();
	m_pRunner = NULL;
	return ;
}

xWorkingThread::xWorkingThread(IWorkingThreadCallback* pCallBack)
{
	m_pSignal = xOperationSys::singleton()->createThreadSignal();
	m_pCurrentWorkJob = NULL;
	m_pQueueLocker = xOperationSys::singleton()->createThreadLocker();
	m_pCallBack = pCallBack;
}

xWorkingThread::~xWorkingThread()
{
	stop();
	m_pSignal->KillObject();
	m_pQueueLocker->KillObject();
}


//-----------------------
void xWorkingThreadPool::init(int nThread)
{
	m_pQueueLocker = xOperationSys::singleton()->createThreadLocker();
	m_vThreadPool.resize(nThread);
	for(int i = 0 ; i < nThread ; i ++)
	{
		m_vThreadPool[i] = new xWorkingThread(this);
		m_vThreadPool[i]->start();
	}
}

void xWorkingThreadPool::uninit()
{
	clearJob();
	if(m_pQueueLocker) m_pQueueLocker->KillObject();
}

bool xWorkingThreadPool::addJob(IWorkingJob* pJob)
{
	size_t _nThreadPool = m_vThreadPool.size() ;
	for(size_t i = 0 ; i < _nThreadPool ; i ++)
	{
		xWorkingThread* pWorkThread = m_vThreadPool[i];
		if(pWorkThread->nJob() == 0)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH ,L">> Push a Job to thread[index = %d] \n" , i );
			pWorkThread->addJob(pJob);
			return true;
		}
	}

	XEVOL_LOG(eXL_DEBUG_HIGH ,L">> Queued a job \n" );
	//现在ThreadPool的队列里保存起来。
	m_pQueueLocker->lock();
	m_JobQueue.push_back(pJob);
	pJob->AddRef();
	m_pQueueLocker->unlock();
	return true;
}

bool xWorkingThreadPool::clearJob()
{
	//仅仅清理没有放到Thread的Job;
	m_pQueueLocker->lock();
	xJobQueue::iterator pos = m_JobQueue.begin();
	for(; pos != m_JobQueue.end() ; pos ++)
	{
		IWorkingJob* pWorkJob = *pos;
		pWorkJob->ReleaseObject();
	}
	m_JobQueue.clear();
	m_pQueueLocker->unlock();


	//开始清理Thread里的Job
	size_t _nThreadPool = m_vThreadPool.size() ;
	for(size_t i = 0 ; i < _nThreadPool ; i ++)
	{
		xWorkingThread* pWorkThread = m_vThreadPool[i];
		pWorkThread->clearJob();
	}

	return true;
}

bool xWorkingThreadPool::OnJobFinish(xWorkingThread* pThread , IWorkingJob* pJob)
{
	return true;
}
bool xWorkingThreadPool::OnThreadIdle(xWorkingThread* pThread )
{
	m_pQueueLocker->lock();
	if(m_JobQueue.size() > 0 )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH ,L">>Pull a job to Queue\n");
		IWorkingJob* pJob = *m_JobQueue.begin();
		pThread->addJob(pJob);
		m_JobQueue.pop_front();
	}	
	m_pQueueLocker->unlock();
	return true;
}
xWorkingThreadPool::xWorkingThreadPool()
{
	m_pQueueLocker = NULL;
}
xWorkingThreadPool::~xWorkingThreadPool()
{
	uninit();
}

END_NAMESPACE_XEVOL3D

