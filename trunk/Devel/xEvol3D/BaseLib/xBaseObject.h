#ifndef __XEVOL3D_ENGINE_BASE_OBJECT_H__
#define __XEVOL3D_ENGINE_BASE_OBJECT_H__
#include <stdlib.h>
#include "xEvol3DAPI.h"
#include "xLogger.h"
#include "../mem/mem_operator.h"

#define _WIDETEXT_(x)  L ## x

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xObjectClassID
{
	unsigned int          m_ID;
	xObjectClassID*       m_pBassClasID;
	wchar_t               m_Name[32];	
public:
	xObjectClassID(const wchar_t* className , xObjectClassID* pBaseClassID);
	xObjectClassID(const xObjectClassID& lhv);
	xObjectClassID& operator = (const xObjectClassID& lhv);
	const wchar_t*           name() const {return m_Name; }
	bool                     isType(const wchar_t* _clasName) const;
	bool                     isType(const xObjectClassID& classID) const;
	bool                     operator == (const xObjectClassID& lhv) const;
};

class _XEVOL_BASE_API_  IRefCountObject
{
public:
	virtual int      RefCount() = 0;
	virtual int      AddRef() = 0;
	virtual int      ReleaseObject() = 0;
	virtual int      KillObject() = 0;
};

template <typename T> bool  XSAFE_KILLOBJECT(T& obj)
{
	if(obj)
	{
		obj-> KillObject();
		obj=NULL;
		return true;
	}
	return false;
}

template <typename T> bool  XSAFE_RELEASEOBJECT(T& obj)
{
	if(obj)
	{
		obj->ReleaseObject();
		obj=NULL;
		return true;
	}
	return false;
}

template <typename T> bool  XSAFE_UNLOAD(T& obj)
{
	if(obj)
	{
		obj->unload();
		obj=NULL;
		return true;
	}
	return false;
}


template <typename T> bool  XSAFE_RELEASE(T& obj)
{
	if(obj)
	{
		obj->Release();
		obj=NULL;
		return true;
	}
	return false;
}

template <typename T> bool  XSAFE_DELETE(T* & pObj)
{
	if(pObj)
	{
		delete pObj;
		pObj=NULL;
		return true;
	}
	return false;
}

template <typename T> bool  XSAFE_DELETE_ARRAY(T* & pObj)
{
	if(pObj)
	{
		delete [] pObj;
		pObj=NULL;
		return true;
	}
	return false;
}

class _XEVOL_BASE_API_  IBaseObject : public IRefCountObject
{
public:
	static xObjectClassID ms_CLASSID;
public:
	virtual const    xObjectClassID&  classID() = 0;
	virtual void*    queryObject(const xObjectClassID& _id) = 0;
	virtual bool     isType(const    xObjectClassID& _id) = 0;
};


class _XEVOL_BASE_API_  IBaseResource : public IBaseObject
{
public:
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool          isLoaded()  = 0;
	virtual bool          unload() = 0;
	virtual unsigned long memUsage() = 0;
};



//Macro for define the Class Interface
#define IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(ClassName)   \
public:                                   \
	virtual int      RefCount()	{ return 1; }\
	virtual int      AddRef()   { return 1; }\
	virtual int      ReleaseObject()  { return 0; }\
	virtual int      KillObject()     { return 0;  }; \


#define IMPL_REFCOUNT_OBJECT_INTERFACE(ClassName)   \
protected:                                \
	int  m_RefCount;                      \
public:                                   \
	virtual int      RefCount()	{ return m_RefCount; }\
	virtual int      AddRef()   { m_RefCount ++;     return m_RefCount; }\
	virtual int      ReleaseObject()	{ m_RefCount --;  int refCount = m_RefCount;   if(m_RefCount == 0) delete this; return refCount; }\
	virtual int      KillObject()     { if(m_RefCount != 1) XEVOL_LOG(eXL_DEBUG_HIGH, "RefCount Object not released class m_RefCount=%d \n" , m_RefCount ) ; m_RefCount = 0 ; delete this; return 0;  }; \



#define DECL_REFCOUNT_OBJECT_INTERFACE(ClassName)   \
protected:                                      \
	int  m_RefCount;                            \
public:                                         \
	virtual int                       RefCount()	                         ;\
	virtual int                       AddRef()                               ;\
	virtual int                       ReleaseObject()	                     ;\
	virtual int                       KillObject()                           ;\

//Base Object Class
//=============================================================================================
#define IMPL_BASE_OBJECT_INTERFACE(ClassName)   \
protected:                                \
	int  m_RefCount;                      \
public :                                  \
	static xObjectClassID   ms_CLASSID;   \
public:                                   \
	virtual int      RefCount()	{ return m_RefCount; }\
	virtual int      AddRef()   { m_RefCount ++;     return m_RefCount; }\
	virtual int      ReleaseObject()	{ m_RefCount --;  int refCount = m_RefCount;   if(m_RefCount == 0) delete this; return refCount; }\
	virtual int      KillObject()     { if(m_RefCount != 1) XEVOL_LOG(eXL_DEBUG_HIGH, L"RefCount Object not released class m_RefCount=%d type=%s\n" , m_RefCount ,ms_CLASSID.name() ) ; m_RefCount = 0 ; delete this; return 0;  }; \
public :                                                              \
	virtual const    xObjectClassID&  classID(){ return this->ms_CLASSID; } \
	virtual void*    queryObject(const xObjectClassID& _id){if( this->ms_CLASSID.isType(_id)){this->AddRef(); return this;} return 0;}\
	virtual bool     isType(const    xObjectClassID& _id){return classID().isType(_id) ;}\



#define DECL_BASE_OBJECT_CLASSID(ClassName)   \
public:                                      \
	static xObjectClassID   ms_CLASSID;         \


#define IMPL_BASE_OBJECT_CLASSID(ClassName, BaseClass) \
	xObjectClassID ClassName::ms_CLASSID = xObjectClassID( _WIDETEXT_(#ClassName) , &BaseClass::ms_CLASSID);    \


#define DECL_BASE_OBJECT_INTERFACE(ClassName)   \
protected:                                      \
	int  m_RefCount;                            \
public:                                         \
    static xObjectClassID   ms_CLASSID;         \
public:                                         \
	virtual int                       RefCount()	                         ;\
	virtual int                       AddRef()                               ;\
	virtual int                       ReleaseObject()	                     ;\
	virtual int                       KillObject()                           ;\
	virtual const    xObjectClassID&  classID()                              ;\
	virtual void*                     queryObject(const xObjectClassID& _id) ;\
	virtual bool                      isType(const    xObjectClassID& _id)   ;\



//Base Object Class
//=============================================================================================
#define IMPL_BASE_OBJECT_CLASSID_INTERFACE(ClassName)   \
public :                                  \
	static xObjectClassID   ms_CLASSID;   \
public :                                                              \
	virtual const    xObjectClassID&  classID(){ return this->ms_CLASSID; } \
	virtual void*    queryObject(const xObjectClassID& _id){if( this->ms_CLASSID.isType(_id)){this->AddRef(); return this;} return 0;}\
	virtual bool     isType(const    xObjectClassID& _id){return classID().isType(_id) ;}\


#define DECL_BASE_OBJECT_CLASSID_INTERFACE(ClassName)   \
public:                                      \
	static xObjectClassID   ms_CLASSID;         \
public:                                         \
	virtual const    xObjectClassID&  classID()                              ;\
	virtual void*                     queryObject(const xObjectClassID& _id) ;\
	virtual bool                      isType(const    xObjectClassID& _id)   ;\



template<typename _ChildT , typename _ParentT> bool XEvol_Convert(_ChildT* & _ch , _ParentT* parent)
{
	_ChildT* pChild = dynamic_cast<_ChildT*>(parent);
	if(pChild == NULL) return false;
	_ch = pChild ;
	return true;
}


END_NAMESPACE_XEVOL3D

#endif
