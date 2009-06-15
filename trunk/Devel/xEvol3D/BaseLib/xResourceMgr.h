/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __TEMPLATE_RESOURCE_MGR_H__
#define __TEMPLATE_RESOURCE_MGR_H__
#include <map>
#include <vector>
#include <string>
#include <iostream>
#ifdef _MSC_VER
#include <hash_map>
using namespace stdext;
#endif

#include <algorithm>
#include <assert.h>
#include "stl_dllalloc.h"
using namespace std;

namespace XEvol3D
{

	/**************************************
	资源优先级的比较器定义
	**************************************/
	struct xResPriority
	{
		int           m_index;
		unsigned long m_LastUsedTime;
	};
	//=============================
	inline bool operator < ( const xResPriority& lhv , const xResPriority& rhv )
	{
		return lhv.m_LastUsedTime < rhv.m_LastUsedTime;
	}

	/************************************************
	简单的资源创建策略的定义。
	调用ResType的newInstance和deleteInstance来创建和销毁资源
	************************************************/
	template <typename ResType> class TSimpleResCreator
	{
	public:
		ResType* newInstance(const wchar_t* strResName, unsigned long arg)
		{
			return ResType::newInstance(strResName);//ResType;
		}

		void     deleteInstance(ResType* pRes)
		{
			ResType::deleteInstance(pRes);
		}
	};

	template <typename ResType> class TPureVirtualResCreator
	{
	public:
		virtual ResType* newInstance(const wchar_t* strResName, unsigned long arg) = 0;
		virtual void     deleteInstance(ResType*& pRes) = 0;
	};

	/************************************************
	简单的资源创建策略的定义。
	调用new/delete来创建和销毁资源
	************************************************/
	template <typename ResType> class TOpNewResCreator
	{
	public:
		ResType* newInstance(const wchar_t* strResName , unsigned long arg)
		{
#ifdef _MSC_VER
			return new typename ResType;//ResType;
#else
			return new ResType;//ResType;
#endif
		}

		void     deleteInstance(ResType* pRes)
		{
			delete pRes;
		}
	};

	class xNamedResManager
	{
		wchar_t        m_name[32];
	public:
		const wchar_t* name()
		{
			return     m_name;
		}
		void setName(const wchar_t* name)
		{
			wcsncpy(m_name,name , 32);
		}
	};
	//-----------------------------------------------------------------------------------
	//一个简单的资源加载器，简单的把名字作为文件名去加载。
	//-----------------------------------------------------------------------------------
	template <typename ResType , typename _TCreator = TSimpleResCreator<ResType> > 
	class TSimpleResLoader : public _TCreator
	{

	protected:
        virtual unsigned int _getResSize(ResType* pRes) 
		{
			return pRes->memUsage();
		}

		virtual bool _isResLoaded(ResType* pRes)
		{
			return pRes;
		}


		virtual bool _loadResource(ds_wstring& strResName,ResType*& pRes, int& ResSize , unsigned int arg)
		{
			bool needDelete = false;
			ResSize = 0;
			if(pRes == NULL)
			{
				//pRes = new ResType;
				pRes = this->newInstance(strResName.c_str() , arg );
				if(pRes == NULL)
					return false;
				needDelete = true;
			}

			if(pRes->load(strResName.c_str() , arg ) )
			{
				ResSize = _getResSize(pRes);
				return true;
			}

			if(needDelete)
			{
				this->deleteInstance(pRes);
				pRes = NULL;
			}

			return false;
		}

		virtual bool _unloadResource(ds_wstring& strResName, ResType*& pRes, unsigned int& TotalResSize)
		{
			if(pRes == NULL)
				return false;

			TotalResSize -= _getResSize(pRes);
			pRes->unload();
			this->deleteInstance(pRes);
			pRes =  NULL;
			return true;
		}

		virtual void _deleteResource(ds_wstring& strResName, ResType* pRes)
		{
			if(pRes == NULL)
				return ;
			pRes->unload();
			this->deleteInstance(pRes);
			pRes = NULL;
		}

	};


	/******************************************************
	//这个资源加载器不会delete掉一个指针。只会unload
	******************************************************/
	template <typename ResType, typename _TCreator = TSimpleResCreator<ResType> > 
	class TSimpleResLoader2 : public TSimpleResLoader<ResType,_TCreator>
	{
	protected:
		virtual bool       _unloadResource(ds_wstring& strResName,ResType*& pRes, unsigned int& TotalResSize)
		{
			TotalResSize -= pRes->memUsage();
			pRes->unload();
			return true;
		}
	};


	class XNullThreadLocker
	{
	public:
		void thread_lock(){};
		void thread_unlock(){};
	};
	//资源管理器的前向声明
	template <typename ResType , typename _TKey  , typename _TLoader ,typename _TLocker > class TBaseResMgr;

	/******************************************************
	//资源句柄类
	******************************************************/
	template  <typename ResType, 
		typename _TKey     = std::ds_wstring,
		typename _TLoader  = TSimpleResLoader <ResType , TSimpleResCreator<ResType> > ,
		typename _TLocker  = XNullThreadLocker > 
	class TResHandle
	{
		typedef TBaseResMgr<ResType , _TKey , _TLoader,_TLocker> MyResMgr;
        
		friend class TBaseResMgr<ResType, _TKey , _TLoader ,_TLocker>;
		MyResMgr* m_pResMgr;
		int       m_iData;
		//面向资源管理器的类

	public:
		TResHandle(){ m_pResMgr = NULL; }
		
		MyResMgr* getOwner(){return m_pResMgr;}
		int       data(){return m_iData;}

		//面向用户的类
	public:
		~TResHandle();
		TResHandle(const TResHandle<ResType , _TKey , _TLoader,_TLocker>& rhv);
		TResHandle<ResType , _TKey , _TLoader,_TLocker>& operator = (const TResHandle<ResType , _TKey , _TLoader,_TLocker>& rhv)
		{
			m_iData   = rhv.m_iData;
			m_pResMgr = rhv.m_pResMgr;
			addRef();
			return *this;
		}

		ResType* getResource();
		ResType* query();
		ResType* queryResPtr();
		ResType* operator->(){return getResource();}
		void     setNULL(){m_pResMgr = NULL ; m_iData = 0 ; }
		bool     operator == (const TResHandle<ResType , _TKey , _TLoader,_TLocker>& rhv){ return m_pResMgr && m_pResMgr == rhv.m_pResMgr && m_iData == rhv.m_iData ; };
		bool     confirm();
		bool     invalidate();
		ResType* lock();
		void     unlock();
		void     release();
		void     unload();
		void     addRef();
		int      refCount();
		bool     isHandle(){return m_pResMgr != NULL;}
            operator bool(){return isHandle() };
	};

	//------------------------------------------------------
	//Resource Manager的接口，
	//所有的不同类型的Resource Manager都可以用这个接口来进行
	//一些相同功能的操作
	//------------------------------------------------------
	class  IResManager
	{
	protected:
		//这个资源管理器的时间
		int             m_TickCount;
	public:
		IResManager()
		{
			m_TickCount = 0;
		}
		virtual void unload() = 0;
		virtual void loadAll() = 0;
		virtual void clear() = 0;
		int getTime()
		{
			return m_TickCount;
		}
		int advanceTime()
		{
			return ++m_TickCount;
		}

		virtual void update(unsigned int passedTime , unsigned int thisTime)
		{
			m_TickCount  = thisTime;
		};
	};

	/*******************************************************
	//资源管理器类的基类
	//基类不知道如何 创建和销毁一个实际资源，
	//只负责管理逻辑
	*******************************************************/
	template<typename T> class TResMgrAutoLocker
	{
		T* m_pLocker;
	public:
		TResMgrAutoLocker(T* pT){	m_pLocker=pT;if(m_pLocker) m_pLocker->thread_lock();};
		~TResMgrAutoLocker()	{if(m_pLocker)	m_pLocker->thread_unlock();};
	};




	template <typename ResType, 
		typename _TKey     = std::wstring ,
		typename _TLoader  = TSimpleResLoader <ResType , TSimpleResCreator<ResType> > ,
		typename _TLocker  = XNullThreadLocker > 
	class TBaseResMgr : virtual public IResManager , virtual public _TLoader , virtual public _TLocker , public xNamedResManager
	{
	protected:
		


	public:

         //一个资源的插槽
	    template <typename _TResType , typename _TResKey> class TResSlot
	    {
	    public:
		unsigned long  m_LastUsedTime;
		_TResType*     m_pRes;
		_TResKey       m_ResName;
		int            m_RefCount;
		int            m_iLockCount;
		unsigned int   m_Arg;
		bool           m_bIsLoaded;
		TResSlot(int initRefCount){	m_RefCount = initRefCount ; }

	};

		typedef TResSlot<ResType , _TKey>  ResSlot;
		typedef TResSlot<ResType , _TKey>& ResSlotRef;

		typedef vector<ResSlot , std::dllsafe_alloc<ResSlot>  >       ResSlots;
		typedef TResHandle <ResType , _TKey  ,  _TLoader , _TLocker>  MyResHandle;
		typedef TBaseResMgr<ResType , _TKey  ,  _TLoader , _TLocker>  MyResManager;
		typedef ResType*                                              PResType;
        typedef TResMgrAutoLocker< TBaseResMgr<ResType , _TKey  ,  _TLoader , _TLocker> >  ResManagerLocker;  

#ifdef _MSC_VER
		typedef ds_hash_map(_TKey , int ) StringToIndexMap;
		//typedef hash_map< _TKey , int , hash_compare<_TKey> , std::dllsafe_alloc<pair<const _TKey, int> > >       StringToIndexMap;  
#else
		typedef ds_map(_TKey , int )     StringToIndexMap;
		//typedef map< _TKey  , int , std::less<_TKey> , std::dllsafe_alloc<pair<const _TKey, int>  >  >            StringToIndexMap;  
#endif

	protected:
		ResSlots                    m_ResSlots;
		StringToIndexMap            m_StrToIndexMap;
		//资源管理已经加载的资源容量
		unsigned int                m_ResSize;
		//资源管理器的最大资源容量
		unsigned int                m_MaxSize;

		//资源初始创建时候的 RefCount;
		unsigned int                m_InitRefCount;

	protected:     
		//和资源淘汰策略有关的函数。
		virtual bool updateFreePolicy(int passedTime, int thisTime) = 0;
		virtual bool freeResource(unsigned int trimedSize = 0 ) = 0;
		virtual void useResource(int iResIndex) = 0;


	protected:
		//Unload一个slot里的资源
		virtual bool       _unload(ResSlot& slot)
		{
			slot.m_bIsLoaded = false;
			slot.m_LastUsedTime = this->getTime();
			return this->_unloadResource(slot.m_ResName , slot.m_pRes , m_ResSize);			 
		}

		//重新加载一个Slot里的资源
		virtual bool     _load(ResSlot& slot)
		{
			slot.m_LastUsedTime = this->advanceTime();
			int  ResSize = 0;
			slot.m_bIsLoaded = this->_loadResource(slot.m_ResName,slot.m_pRes , ResSize ,slot.m_Arg) ;
			if(slot.m_bIsLoaded)
			{
				this->m_ResSize += ResSize;
			}
			freeResource();
			return slot.m_bIsLoaded;
		}

		//判断一个Slot里的资源是不是已经加载了。
		virtual bool     _isLoaded(ResSlot& slot)
		{
			return (slot.m_bIsLoaded) && (_TLoader::_isResLoaded(slot.m_pRes));
		}

		//把一个已经创建好的资源放到资源管理器里，并重新分配Handle
		MyResHandle        _add(ResType* pRes,const _TKey& strResName,unsigned int arg)
		{
			MyResHandle handle;
			//计算handle
			handle.m_iData = (int)m_ResSlots.size();
			handle.m_pResMgr = this;

			//创建String -> Index的map
			m_StrToIndexMap.insert(typename StringToIndexMap::value_type(strResName,handle.m_iData));

			//创建一个slot,并把这个slot分配给新来的资源
			m_ResSlots.push_back(ResSlot(m_InitRefCount) );
			ResSlot& Slot = m_ResSlots[handle.m_iData];
			Slot.m_pRes = pRes;
			Slot.m_ResName = strResName;
			Slot.m_LastUsedTime = this->advanceTime();
			Slot.m_iLockCount = 0;
			Slot.m_bIsLoaded  = false;
			Slot.m_Arg = arg;
			addRef(handle);
			return handle;
		}

	public:
		TBaseResMgr(const wchar_t* _name , unsigned int initRefCount )
		{
			m_InitRefCount = initRefCount;
			this->setName(_name);
			m_ResSize = 0;
			m_MaxSize = 64 * 1024 * 1024; //64M
		}
		//Debug的时候调试信息
		void dump()
		{
			ResManagerLocker alocker(this);
			XEVOL_LOG(eXL_DEBUG_NORMAL,L"Dump Resource Manager: name=%s\n" , this->name() );
			XEVOL_LOG(eXL_DEBUG_NORMAL, " MaxSize: %f M\n" , float(m_MaxSize)/(1024*1024) );
			XEVOL_LOG(eXL_DEBUG_NORMAL, " CurSize: %f M\n" , float(m_ResSize)/(1024*1024) );
			XEVOL_LOG(eXL_DEBUG_NORMAL, " ResCount: %d \n" ,(int)m_ResSlots.size() );
		}

		~TBaseResMgr()
		{
			clear();
		}

		unsigned long getSize()
		{
			ResManagerLocker alocker(this);
			return m_ResSize;
		}

		unsigned long getMaxSize()
		{
			ResManagerLocker alocker(this);
			return m_MaxSize;
		}

		void setMaxSize(unsigned long maxSize)
		{
			ResManagerLocker alocker(this);
			m_MaxSize = maxSize;
		}




		/*
		锁定一个资源，防止在做LRU淘汰算法的时候被释放掉
		*/
		void     lock(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(isHandle(handle) == false)
				return ;
			m_ResSlots[handle.m_iData].m_iLockCount ++;
		}

		/*
		解除锁定一个资源，让其可以在做LRU淘汰算法的时候被释放掉
		*/
		void     unlock(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(isHandle(handle) == false)
				return ;
			if(m_ResSlots[handle.m_iData].m_iLockCount >0)
				m_ResSlots[handle.m_iData].m_iLockCount --; 

		}

		ResType*   queryResPtr(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(isHandle(handle) == false)
				return NULL;
			ResSlot& slot = m_ResSlots[handle.m_iData];
			slot.m_LastUsedTime = this->getTime();
			useResource(handle.m_iData);
			return slot.m_pRes;
		}	

		ResType*   query(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			useResource(handle.m_iData);
			if(isHandle(handle) == false)
				return NULL;

			ResSlot& slot = m_ResSlots[handle.m_iData];
			if(slot.m_bIsLoaded && slot.m_pRes && _isResLoaded(slot.m_pRes) )
			{
				slot.m_LastUsedTime = this->advanceTime();
				return slot.m_pRes;
			}
			return NULL;
		}
		/*
		得到一个句柄的实际资源对象
		*/
		ResType*   getResource(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			useResource(handle.m_iData);
			if(isHandle(handle) == false)
				return NULL;

			ResSlot& slot = m_ResSlots[handle.m_iData];
			//锁定这个资源，防止使用的时候被释放掉
			slot.m_iLockCount ++;
			confirm(handle);
			slot.m_LastUsedTime = this->getTime();
			slot.m_iLockCount --;
			return slot.m_pRes;
		}

		/*
		释放一个句柄中的资源的引用计数
		*/
		void        release(const wchar_t* strResName)
		{
			ResManagerLocker alocker(this);
			MyResHandle handle = find(strResName);
			if(handle.isHandle())
			{
				ResSlot& slot = m_ResSlots[handle.m_iData];
				//减少引用计数
				if(slot.m_RefCount <= 0)
				{
					assert(false);
				}

				slot.m_RefCount --;
				if(slot.m_RefCount == 0)
				{
					unload(handle);
				}
			}
		}

		/*
		释放一个句柄中的资源的引用计数
		*/
		void        release(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(handle.isHandle())
			{
				ResSlot& slot = m_ResSlots[handle.m_iData];
				//减少引用计数
				if(slot.m_RefCount <= 0)
				{
					assert(false);
				}
				slot.m_RefCount --;
				if(slot.m_RefCount == 0)
				{
					unload(handle);
				}
			}
		}
		/*
		增加一个句柄中的资源的引用计数
		*/
		void        addRef(const _TKey& strResName)
		{
			ResManagerLocker alocker(this);
			MyResHandle handle = find(strResName);
			if(handle.isHandle())
			{
				ResSlot& slot = m_ResSlots[handle.m_iData];
				//增加引用计数
				slot.m_RefCount ++;
			}
		}

		/*
		增加一个句柄中的资源的引用计数
		*/
		void        addRef(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(handle.isHandle())
			{
				ResSlot& slot = m_ResSlots[handle.m_iData];
				//增加引用计数
				slot.m_RefCount ++;
			}
		}

		int        refCount(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(handle.isHandle())
			{
				ResSlot& slot = m_ResSlots[handle.m_iData];
				//增加引用计数
				return slot.m_RefCount ;
			}
			return  -1;
		}

		/*
		往一个资源管理器里添加一个资源
		*/
		MyResHandle add(const _TKey& strResName , ResType* pRes ,  unsigned int arg)
		{
			ResManagerLocker alocker(this);
			//首先查找Manager里有没有这个资源
			MyResHandle handle = find(strResName);
			if(handle.isHandle())
			{
				//存在这个资源
				return handle;
			}
			else
			{
				//不存在这个资源 把加载的资源分配一个slot后，把句柄返回给用户
				handle = _add(pRes,strResName,arg);
				ResSlot& slot = m_ResSlots[handle.m_iData];
				slot.m_bIsLoaded  = true;
				return handle;
			}
		}

		/*
		往一个资源管理器里添加一个资源
		*/
		MyResHandle add(const _TKey& strResName, unsigned int arg , bool bLoadImmediate)
		{
			ResManagerLocker alocker(this);
			//首先查找Manager里有没有这个资源
			MyResHandle handle = find(strResName);
			if(handle.isHandle())
			{//存在这个资源
				//确认这个资源是不是加载
				if(bLoadImmediate)
					confirm(handle);

				ResSlot& slot = m_ResSlots[handle.m_iData];
				slot.m_Arg = arg;
				//增加引用计数
				slot.m_RefCount ++;
				return handle;
			}
			else
			{//不存在这个资源，则创建一个资源
				ResType* pRes =  NULL;
				//把加载的资源分配一个slot后，把句柄返回给用户
				handle = _add(pRes,strResName,arg);
				if(bLoadImmediate)
				{
					confirm(handle);
				}
				return handle;
			}
		}

		/*
		判断一个句柄是不是有效
		*/
		bool isHandle(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if(handle.m_iData < 0 ) return false;
			if(handle.m_iData >= (int)m_ResSlots.size()) return false;
			if(handle.m_pResMgr != this) return false;
			return true;
		}

		/*
		确认一个资源已经被加载
		*/
		bool confirm(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			if( !isHandle(handle))
				return false;
			ResSlot& Slot = m_ResSlots[handle.m_iData];
			//已经加载
			if( _isLoaded(Slot) )
				return true;
			else
				return _load(Slot);
		}


		/***************************************
		查找一个资源
		****************************************/
		MyResHandle find(const _TKey& strResName)
		{
			ResManagerLocker alocker(this);
			MyResHandle handle;

			typename StringToIndexMap::iterator iPos = m_StrToIndexMap.find(strResName);
			if(iPos != m_StrToIndexMap.end() )
			{
				handle.m_iData = iPos->second;
				handle.m_pResMgr = this;
				return handle;
			}
			handle.m_pResMgr = NULL;
			return handle;
		}

		/*
		把一个句柄里的资源释放掉。
		*/
		bool     unload(MyResHandle& handle)
		{
			ResManagerLocker alocker(this);
			assert(handle.m_pResMgr == this);
			ResSlot& slot = m_ResSlots[handle.m_iData];
			if(_isLoaded(slot))
			{
				return _unload(slot);
			}
			return true;
		}

		virtual bool onDeviceLost() 
		{
			return false;
		}

		//虚函数，行为可以继承
	public:
		/*
		加载这个管理器里所有没有加载的资源
		*/
		virtual void loadAll()
		{
			ResManagerLocker alocker(this);
			size_t i = 0;
			size_t n = m_ResSlots.size();
			for(i = 0 ; i < n ; ++i)
			{
				ResSlot& Slot = m_ResSlots[i];
				if(_isLoaded(Slot) == false)
				{
					_load(Slot);
				}
			}
		}

		/*
		每经过一次循环，资源管理器需要更新自己的状态
		*/
		virtual void update(unsigned int passedTime,unsigned int thisTime)
		{
			ResManagerLocker alocker(this);
			IResManager::update(passedTime,thisTime);
			updateFreePolicy(passedTime , thisTime );
		}

		/*
		把所有的资源都卸载掉
		但是不释放所有句柄，下次重新使用句柄，会导致所有资源重新加载
		*/
		virtual void    unload()
		{
			ResManagerLocker alocker(this);
			size_t nRes = m_ResSlots.size();
			for(size_t i = 0 ; i < nRes ; i++)
			{
				ResSlot& slot = m_ResSlots[i];
				if(_isLoaded(slot))
				{
					_unload(slot);
				}
			}
			this->thread_unlock();
		}

		/*
		清理这个ResManager里的所有的资源
		*/
		virtual void clear()
		{
			unload();
			ResManagerLocker alocker(this);
			for(size_t i = 0 ; i < m_ResSlots.size() ; ++i)
			{
				ResSlot& slot = m_ResSlots[i];
				if(slot.m_pRes)
				{
					this->_deleteResource(slot.m_ResName , slot.m_pRes);
				}
			}

			m_ResSlots.clear();
			m_StrToIndexMap.clear();
		}
	protected:



	};

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//基于 LRU 淘汰算法的资源管理器
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template <typename ResType, 
		typename _TKey     = std::wstring ,
		typename _TLoader  = TSimpleResLoader <ResType , TSimpleResCreator<ResType> > ,
		typename _TLocker  = XNullThreadLocker > 
	class TLRUPolicyResMgr : public TBaseResMgr<ResType , _TKey , _TLoader , _TLocker>
	{
	protected:
		typedef TBaseResMgr<ResType , _TKey , _TLoader , _TLocker> TBaseClass;
		//排序后的资源优先级改变次数
		typedef vector<xResPriority ,  std::dllsafe_alloc<xResPriority> > ResPrioritys;

		ResPrioritys             m_ResPrioritys;
		unsigned int             m_ResPriorChangedTime;


		bool updateFreePolicy(int passedTime , int thisTime)
		{
			freeResource();
			return true;
		}

		//-------------------------------------------------------
		//资源的LRU淘汰算法，释放那些最长时间没有用过的资源，
		//把资源管理器的容量释放到一定的大小。这个大小由trimedToSize
		//指定。
		//-------------------------------------------------------


		virtual bool   freeResource(unsigned int trimedToSize = 0)
		{
			if(this->m_MaxSize == 0)
				return true;

			if(trimedToSize == 0)
			{
				trimedToSize = this->m_MaxSize;
			}
			//尺寸本来就够大，返回。
			if(this->m_ResSize <= trimedToSize)
			{
				return true;
			}

			size_t nRes = m_ResPrioritys.size();
			size_t nResSlot =  this->m_ResSlots.size();
			if(nRes != nResSlot || this->m_ResPriorChangedTime > 100) 
			{
				calcPriority();
			}

			for(size_t i = 0 ; i < nRes ; i++)
			{
				xResPriority& prior = m_ResPrioritys[i];
				typename TBaseClass::ResSlot& slot = this->m_ResSlots[prior.m_index];
				//资源存在并且没有被锁定
				if(_isLoaded(slot) && (slot.m_iLockCount == 0)  )
				{
					_unload(slot);
					slot.m_bIsLoaded = false;
					if(this->m_ResSize <= trimedToSize)
					{
						return true;
					}
				}
			}
			return false;

		}
		//--------------------------------------------------
		//计算资源的LRU优先级别
		//--------------------------------------------------
		bool calcPriority()
		{
			typename TBaseClass::ResManagerLocker alocker(this);
			//为这个容器reseve容量
			m_ResPrioritys.clear();
			m_ResPrioritys.reserve( this->m_ResSlots.size());

			//把所有的资源放到这个队列里。以便对LastUsedTime进行排序
			for(size_t i = 0 ; i <this-> m_ResSlots.size() ; i++)
			{
				xResPriority p;
				p.m_index = (int)i;
				typename TBaseClass::ResSlot& slot =  this->m_ResSlots[i];
				p.m_LastUsedTime = slot.m_LastUsedTime;
				m_ResPrioritys.push_back(p);
			}

			//进行排序
			sort(m_ResPrioritys.begin(),m_ResPrioritys.end());

			//设置排序后的资源优先级改变次数为0
			m_ResPriorChangedTime = 0;
			return true;
		}

		void useResource(int iResIndex)
		{
			m_ResPriorChangedTime ++ ;
			typename TBaseClass::ResSlot& slot =  this->m_ResSlots[iResIndex];
			slot.m_LastUsedTime = this->getTime();
		}

	public:
		TLRUPolicyResMgr(const wchar_t* name, unsigned int initRefCount)
			:TBaseClass(name , initRefCount)
		{
			m_ResPriorChangedTime = 10000;
		}

		~TLRUPolicyResMgr()
		{

		}

		virtual bool   onDeviceLost() 
		{
			m_ResPriorChangedTime = 1000;
			this->m_ResSize = 0;
			size_t nRes = this->m_ResSlots.size();
			for(size_t i = 0 ; i < nRes ; i++)
			{
				typename TBaseClass::ResSlot& slot = this->m_ResSlots[i];
				//资源存在并且没有被锁定
				if(false == _isLoaded(slot) )
				{
					slot.m_bIsLoaded = false;
				}
				else
				{
					this->m_ResSize += this->_getResSize(slot.m_pRes);
				}
			}
			return true;
		}

		void clear()
		{
			TBaseClass::clear();
			m_ResPrioritys.clear();
		}
	};

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//资源不需要淘汰的资源管理器
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template <typename ResType, 
		typename _TKey     = std::wstring ,
		typename _TLoader  = TSimpleResLoader <ResType , TSimpleResCreator<ResType> > ,
		typename _TLocker  = XNullThreadLocker > 
	class TSimpleResMgr : public TBaseResMgr<ResType , _TKey , _TLoader , _TLocker>
	{
	protected:
		typedef TBaseResMgr<ResType , _TKey , _TLoader , _TLocker> TBaseClass;

		bool updateFreePolicy(int passedTime , int thisTime)
		{
			return true;
		}

		bool   freeResource(unsigned int trimedToSize = 0)
		{
			return true;
		}
		void useResource(int iResIndex)
		{
			typename TBaseClass::ResSlot& slot =  this->m_ResSlots[iResIndex];
			slot.m_LastUsedTime = this->getTime();
		}

	public:
		TSimpleResMgr(const wchar_t* name, unsigned int initRefCount)
			:TBaseClass(name , initRefCount)
		{
		}

		~TSimpleResMgr()	{}
	};

	/*******************************************************
	// Resource Hanlde 的实现函数
	*******************************************************/
	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	ResType* TResHandle<ResType ,  _TKey , _TLoader , _TLocker >::getResource()
	{
		if(m_pResMgr == NULL) return NULL;
		return m_pResMgr->getResource(*this);
	}


	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	ResType* TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::query()
	{
		if(m_pResMgr == NULL) return NULL;
		return m_pResMgr->query(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	ResType* TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::queryResPtr()
	{
		if(m_pResMgr == NULL) return NULL;
		return m_pResMgr->queryResPtr(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	bool TResHandle<ResType ,  _TKey ,  _TLoader , _TLocker >::confirm()
	{
		if(m_pResMgr == NULL) return false;
		return m_pResMgr->confirm(*this);
	}


	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	bool TResHandle<ResType ,  _TKey ,  _TLoader , _TLocker >::invalidate()
	{  
		if(m_pResMgr == NULL) return false;
		return m_pResMgr->unload(*this);
	}


	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	void TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::release()
	{
		if(m_pResMgr == NULL) return;
		m_pResMgr->release(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker > 
	TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::~TResHandle()
	{
		release();
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::TResHandle(const TResHandle<ResType , _TKey , _TLoader,_TLocker>& rhv)
	{
		m_pResMgr = rhv.m_pResMgr;
		m_iData   = rhv.m_iData;
        addRef();
	}


	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	int TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::refCount()
	{
		if(m_pResMgr == NULL) return -1;
		return m_pResMgr->refCount(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	void TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::addRef()
	{
		if(m_pResMgr == NULL) return ;
		m_pResMgr->addRef(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	ResType* TResHandle<ResType ,  _TKey ,  _TLoader , _TLocker >::lock()
	{
		if(m_pResMgr == NULL) return NULL;
		m_pResMgr->lock(*this);
		return getResource();
	}


	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	void TResHandle<ResType , _TKey ,  _TLoader , _TLocker >::unlock()
	{
		if(m_pResMgr == NULL) return;
		m_pResMgr->unlock(*this);
	}

	template <typename ResType , typename _TKey ,  typename _TLoader , typename _TLocker >
	void TResHandle<ResType ,  _TKey ,  _TLoader , _TLocker >::unload()
	{
		if(m_pResMgr == NULL) return;
		m_pResMgr->unload(*this);
	}
}

#endif

