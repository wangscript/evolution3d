#ifndef __XEVOL3D_ENGINE_OBJECT_FACTORY_H__
#define __XEVOL3D_ENGINE_OBJECT_FACTORY_H__
#include <stdlib.h>
#include "xEvol3DAPI.h"
#include "xLogger.h"
#include "../mem/mem_operator.h"
#include "stl_dllalloc.h"
#include <iostream>
#include <map>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D

template<typename _TFactory , typename _TObject , typename ArgType1 , typename ArgType2> 
class TFactoryManager
{
public:
	typedef ds_vector(_TFactory*)   Factories;
	typedef ds_map(std::ds_wstring , _TFactory*) FactoryMap;

public:
	size_t       nFactories()
	{
		return m_vFactories.size();
	}

	_TObject*   createInstance(const wchar_t* name , ArgType1 param1 , ArgType2 param2)
	{
		if(name == NULL)
			return NULL;
		_TFactory* object = find(name);
		if(object==NULL)
			return NULL;
		return object->createInstance(param1 , param2);
	}

	_TFactory*  find(size_t idx)
	{
		if(m_vFactories.size() <= idx)
			return NULL;
		return m_vFactories[idx];
	}
	_TFactory*  find(const wchar_t* name)
	{
		if(name == NULL)
			return NULL;

		FactoryMap::iterator it = m_mapFactories.find(name);
		if(it == m_mapFactories.end() )
			return NULL;
		return it->second;

	}

	bool        registe(_TFactory* object)
	{
		if(object==NULL)
			return false;

		if(find(object->name() ) != NULL )
			return false;
		m_mapFactories.insert(FactoryMap::value_type(object->name() , object) );
		m_vFactories.push_back(object);
		return true;
	}


	bool       unregiste(_TFactory* object)
	{

		bool bRet = false;
		{
			FactoryMap::iterator it = m_mapFactories.begin();
			for(;it != m_mapFactories.end() ; it++)
			{
				if(it->second == object)
				{
					m_mapFactories.erase(it);
					bRet = true;
					break;
				}
			}
		}

		if(bRet == true)
		{
			Factories::iterator it = m_vFactories.begin();
			for(;it != m_vFactories.end() ; it++)
			{
				if(*it == object)
				{
					m_vFactories.erase(it);
					break;
				}
			}
		}  
		return bRet;
	}

protected:
	FactoryMap    m_mapFactories;
	Factories     m_vFactories;

};


#define DECL_OBJECT_FACTORY(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2)  \
class  I_OBJECT_Factory                                                                                         \
{                                                                                                               \
public:																					                        \
	virtual const wchar_t*   name() = 0;													                    \
	virtual IBaseObjName*     createInstance(ArgType1 param1 , ArgType2 param2) = 0;					            \
	virtual int              type()= 0;                                                                         \
	virtual const wchar_t *  desc() = 0;                                                                        \
};																						                        \


//实现一个派生对象的Creator ,在CPP里写一次就可以了。
#define IMPL_OBJECT_FACTORY(xObjectName , IBaseObjName , xOBJECT_Factory , I_OBJECT_Factory_Base , I_OBJECT_Factory_Manager , obj_name , _descString , ArgType1 , ArgType2)  \
class  xOBJECT_Factory :public I_OBJECT_Factory_Base                                                                                 \
{                                                                                                                                    \
public:																					                                             \
	virtual const wchar_t*   name(){return  obj_name ; }     													                     \
	virtual IBaseObjName*     createInstance(ArgType1 param1 , ArgType2 param2) { return new xObjectName(param1 , param2) ; }         \
	virtual int              type(){return 0; }                                                                                      \
	virtual const wchar_t *  desc(){return _descString;}                                                                             \
	xOBJECT_Factory(){I_OBJECT_Factory_Manager::singleton()->registe( this );	}                                                    \
    ~xOBJECT_Factory(){I_OBJECT_Factory_Manager::singleton()->unregiste( this );	}												 \
    static xOBJECT_Factory ms_##xOBJECT_Factory##Instance;                                                                           \
};																						                                             \
xOBJECT_Factory xOBJECT_Factory::ms_##xOBJECT_Factory##Instance;




//声明一种对象的Creator 管理器，在根类的头文件里声明， （在CPP里还要用IMPL_OBJECT_FACTORY_MGR)
//一般场合不需要使用这个宏
#define DECL_OBJECT_FACTORY_MGR(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2)  \
class  _XEVOL_BASE_API_ I_OBJECT_Factory_Manager : public TFactoryManager<I_OBJECT_Factory , IBaseObjName , ArgType1 , ArgType2>	    \
{																						  \
	static I_OBJECT_Factory_Manager* g_singleton;                                         \
public:																					  \
	static I_OBJECT_Factory_Manager* singleton();										  \
	static void                      deleteSinglton();                                    \
};																						  \

//声明一种对象的Creator 管理器，在根类的头文件里声明，这个是在DLL里用的，可以指定 export前缀 （在CPP里还要用IMPL_OBJECT_FACTORY_MGR)
//一般场合不需要使用这个宏
#define DECL_OBJECT_FACTORY_MGR_DLL(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2 , _DLLAPI_)  \
class  _DLLAPI_ I_OBJECT_Factory_Manager : public TFactoryManager<I_OBJECT_Factory , IBaseObjName , ArgType1 , ArgType2>	    \
{																						  \
	static I_OBJECT_Factory_Manager* g_singleton;                                         \
public:																					  \
	static I_OBJECT_Factory_Manager* singleton();										  \
	static void                      deleteSinglton();                                    \
};																						  \


//定义一种对象的Creator虚类，和Creator管理器，在根类的头文件里声明， （在CPP里还要用IMPL_OBJECT_FACTORY_MGR) ，
//跟DECL_OBJECT_FACTORY_MGR不同，这个类还声明了IObjectFactory, 推荐使用这个。
#define DECL_OBJECT_FACTORY_AND_MGR(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2) \
DECL_OBJECT_FACTORY(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2);                \
DECL_OBJECT_FACTORY_MGR(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2);            \

//同上，只是在DLL里用而已。
#define DECL_OBJECT_FACTORY_AND_MGR_DLL(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2 , _DLLAPI_) \
	DECL_OBJECT_FACTORY(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2);                \
	DECL_OBJECT_FACTORY_MGR_DLL(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager , ArgType1 , ArgType2 , _DLLAPI_);            \


//实现一个类的Creator单件对象，只需要在一种类的CPP文件写一次就可以了。
#define IMPL_OBJECT_FACTORY_MGR(IBaseObjName , I_OBJECT_Factory , I_OBJECT_Factory_Manager)  \
	I_OBJECT_Factory_Manager* I_OBJECT_Factory_Manager::g_singleton = NULL;                   \
	I_OBJECT_Factory_Manager* I_OBJECT_Factory_Manager::singleton() { if(g_singleton == NULL) g_singleton = new I_OBJECT_Factory_Manager; return g_singleton ; }\
	void                      I_OBJECT_Factory_Manager::deleteSinglton(){ if(g_singleton) delete g_singleton ; g_singleton = NULL ; }                           \



																				 
																				 
#define IMPL_OBJECT_FACTORY_NO_ARG(xObjectName , IBaseObjName , xOBJECT_Factory , I_OBJECT_Factory_Base , I_OBJECT_Factory_Manager , obj_name , _descString )  \
class  xOBJECT_Factory :public I_OBJECT_Factory_Base                                                                                 \
{                                                                                                                                    \
public:																					                                             \
	virtual const wchar_t*   name(){return obj_name ; }     													                     \
	virtual IBaseObjName*     createInstance(int param1 , int param2) { return new xObjectName() ; }         \
	virtual int              type(){return 0; }                                                                                      \
	virtual const wchar_t *  desc(){return _descString;}                                                                             \
	xOBJECT_Factory(){I_OBJECT_Factory_Manager::singleton()->registe( this );	}                                                    \
	~xOBJECT_Factory(){I_OBJECT_Factory_Manager::singleton()->unregiste( this );	}												 \
    static xOBJECT_Factory ms_ObjectFactor##xObjectName;                                                                              \
};																						                                             \
xOBJECT_Factory xOBJECT_Factory::ms_ObjectFactor##xObjectName;                                                                        \

  


//用法，如果要对IBaseComponent使用 ， 这个类有一个派生类叫 CSomeComponent.
//1. 在IBaseComponent头文件里 DECL_OBJECT_FACTORY_AND_MGR (DECL_OBJECT_FACTORY_AND_MGR_DLL)
//2. 在IBaseComponent代码文件里IMPL_OBJECT_FACTORY_MGR();
//3. 在CSomeComponent代码文件里IMPL_OBJECT_FACTORY();

END_NAMESPACE_XEVOL3D

#endif
