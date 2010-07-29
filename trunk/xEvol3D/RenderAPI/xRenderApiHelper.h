#pragma once
#include <stack>
#include "RenderAPI/xBaseShader.h"
#include "RenderAPI/xMemBuffer.h"
#include "OperationSys/xOperationSys.h"
BEGIN_NAMESPACE_XEVOL3D
typedef std::map<int , IShaderConstBinder*> mapConstBinder;

class _XEVOL_BASE_API_ xRenderApiLocker
{
	xThreadLocker* m_locker;
public:
	xRenderApiLocker()   
	{
		m_locker = xOperationSys::singleton()->createThreadLocker();
	}
	~xRenderApiLocker() 
	{ 
		m_locker->KillObject();
	}
	bool    lock()  
	{
		m_locker->lock();	
		return true;
	}
	bool    trylock()
	{ 
		return m_locker->trylock();	
	}
	void    unlock() 
	{ 
		m_locker->unlock();
	}
};

template<typename TObject> class xObjectStack
{
	TObject*              m_pObject;
	std::stack<TObject*>  m_Stack;
public:
	xObjectStack(){ m_pObject = NULL; }
	TObject* operator ->() { return m_pObject ;}
	operator TObject*(){ return m_pObject ; }
	void operator = (TObject* rhv){m_pObject = rhv ; }
	bool equal(const TObject* rhv){return m_pObject == rhv ; } 
	bool operator !=(const TObject* rhv){return m_pObject != rhv ; } 
	template <typename TTypeCv> TTypeCv dynamic_convert() { return type_cast<TTypeCv> (m_pObject) ; }
	template <typename TTypeCv> TTypeCv static_convert()  { return static_cast<TTypeCv> (m_pObject) ; }
public:
	bool push()
	{
		m_Stack.push(m_pObject);
		return true;
	}

	TObject* pop()
	{
		if(m_Stack.size() == 0 )
			return false;
		TObject* pObject = m_Stack.top();
		m_Stack.pop();
		return pObject;
	}

};


template <typename T> class TRenderApiObjectTable
{
	typedef ds_map(size_t , T * ) vObjectTable;
	vObjectTable m_vObjTable; 
public:
	bool add(const wchar_t* name , T* object)
	{
		size_t hname = xStringHash(name);
		return add(hname , object);
	}
	bool add(size_t  hash_name , T* object)
	{
		if( find(hash_name) ) return false;
		m_vObjTable.insert( vObjectTable::value_type(hash_name , object));
		return true;
	}

	T*   find(size_t  hash_name)
	{
		vObjectTable::iterator pos = m_vObjTable.find( hash_name );
		if(pos != m_vObjTable.end() )
			return pos->second;
		return NULL;
	}

	T*   find(const wchar_t* name)
	{
		size_t hname = xStringHash(name);
		return find(hname);
	}

	bool remove(T* object)
	{
		vObjectTable::iterator pos = m_vObjTable.begin( );
		for(; pos != m_vObjTable.end()  ; pos ++)
		{
			if(object == pos->second )
			{
				m_vObjTable.erase(pos);
				return true;
			}
		}	
		return false;
	}

	bool remove(const wchar_t* name)
	{
		size_t hname = xStringHash(name);
		vObjectTable::iterator pos = m_vObjTable.find( hname );
		if(pos != m_vObjTable.end()  )
		{
			m_vObjTable.erase(pos);
			return true;
		}	
		return false;
	}

	bool remove(size_t  hash_name , T* object)
	{
		vObjectTable::iterator pos = m_vObjTable.find( hash_name );
		if(pos != m_vObjTable.end()  )
		{
			if(object == NULL || object == pos->second )
			{
				m_vObjTable.erase(pos);
				return true;
			}
		}	
		return false;
	}

};

class _XEVOL_BASE_API_ xRenderApiObjectManager
{
	TRenderApiObjectTable<IInputAssembler>     m_InputAssemblerMgr;
	TRenderApiObjectTable<IInputBuffer>        m_InputBufferMgr;
	TRenderApiObjectTable<ILightingEnv>        m_LightStateMgr;
public:
	TRenderApiObjectTable<IInputAssembler>&    inputAssemblerMgr(){return m_InputAssemblerMgr ; }
	TRenderApiObjectTable<IInputBuffer>&       InputBufferMgr(){return m_InputBufferMgr ;}
	TRenderApiObjectTable<ILightingEnv>&       LightEnvMgr(){return m_LightStateMgr ; }
};


END_NAMESPACE_XEVOL3D