#pragma once
#include <BaseLib/xStringHash.h>
#include "xD10_InputAssembler.h"

BEGIN_NAMESPACE_XEVOL3D

template <typename T> class TD10ObjectTable
{
	typedef std::map<size_t , T * > vObjectTable;
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

class xD3D10ObjectManager
{
	TD10ObjectTable<xD10InputAssembler> m_InputAssemblerMgr;
	TD10ObjectTable<IInputBuffer>       m_InputBufferMgr;
	TD10ObjectTable<ILightingEnv>       m_LightStateMgr;
public:
	TD10ObjectTable<xD10InputAssembler>& inputAssemblerMgr(){return m_InputAssemblerMgr ; }
	TD10ObjectTable<IInputBuffer>&       InputBufferMgr(){return m_InputBufferMgr ;}
	TD10ObjectTable<ILightingEnv>&       LightEnvMgr(){return m_LightStateMgr ; }
};

END_NAMESPACE_XEVOL3D
