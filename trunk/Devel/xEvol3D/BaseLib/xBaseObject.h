#ifndef __XEVOL3D_ENGINE_BASE_OBJECT_H__
#define __XEVOL3D_ENGINE_BASE_OBJECT_H__
#include <stdlib.h>
#include "xEvol3DAPI.h"
#include "xLogger.h"
#include "../mem/mem_operator.h"
#include <iostream>
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

template <typename T> int  XSAFE_KILLOBJECT(T& obj)
{
	if(obj)
	{
		int refCount = obj->KillObject();
		obj=NULL;
		return refCount;
	}
	return 0;
}

template <typename T> int  XSAFE_RELEASEOBJECT(T& obj)
{
	if(obj)
	{
		int refCount = obj->ReleaseObject();
		obj=NULL;
		return refCount;
	}
	return 0;
}

template <typename T> bool  XSAFE_UNLOAD(T& obj)
{
	if(obj)
	{
		obj->unload();
		return true;
	}
	return false;
}


template <typename T> int  XSAFE_RELEASE(T& obj)
{
	if(obj)
	{
		int refCount = obj->Release();
		obj=NULL;
		return refCount;
	}
	return 0;
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
public:
    IBaseObject();
    virtual ~IBaseObject();
    static bool DumpObjectList();
};


enum eStreamSeekDir
{
    eSEEK_BEG,
    eSEEK_END,
    eSEEK_CUR,
};

class _XEVOL_BASE_API_ IReadStream : public IRefCountObject
{
public:
    virtual size_t read(char* buf,size_t len) = 0;
    virtual size_t seek(long offs, std::ios::seekdir _dir) = 0;
    virtual size_t tell() = 0;
    virtual int    close() = 0;
    virtual bool   eof() = 0;
};

class _XEVOL_BASE_API_  IRWStream : public IRefCountObject
{
public:
	virtual size_t read(char* buf,size_t len) = 0;
	virtual size_t write(char* buf,size_t len) = 0;
	virtual size_t seek(long offs, std::ios::seekdir _dir) = 0;
	virtual size_t tell() = 0;
	virtual int    close() = 0;
	virtual bool   eof() = 0;
};

class _XEVOL_BASE_API_  IStreamObject : public IRefCountObject
{
public:
    virtual bool load(IRWStream* pStream ) = 0;
    virtual bool save(IRWStream* pStream ) = 0;
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


#define IMPL_REFCOUNT_OBJECT_FUNCTION(ClassName)   \
    int      ClassName##::RefCount()	{ return m_RefCount; }\
    int      ClassName##::AddRef()   { m_RefCount ++;     return m_RefCount; }\
    int      ClassName##::ReleaseObject()	{ m_RefCount --;  int refCount = m_RefCount;   if(m_RefCount == 0) delete this; return refCount; }\
    int      ClassName##::KillObject()     { if(m_RefCount != 1) XEVOL_LOG(eXL_DEBUG_HIGH, "RefCount Object not released class m_RefCount=%d \n" , m_RefCount ) ; m_RefCount = 0 ; delete this; return 0;  }; \


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




#define DECL_BASE_OBJECT_DLLSAFE(ClassName)   \
DECL_BASE_OBJECT_INTERFACE(ClassName)         \


#define IMPL_BASE_OBJECT_DLLSAFE(ClassName, BaseClass) \
      xObjectClassID ClassName::ms_CLASSID = xObjectClassID( _WIDETEXT_(#ClassName) , &BaseClass::ms_CLASSID);    \
      int      ClassName##::RefCount()	{ return m_RefCount; }\
      int      ClassName##::AddRef()   { m_RefCount ++;     return m_RefCount; }\
      int      ClassName##::ReleaseObject()	{ m_RefCount --;  int refCount = m_RefCount;   if(m_RefCount == 0) delete this; return refCount; }\
      int      ClassName##::KillObject()     { if(m_RefCount != 1) XEVOL_LOG(eXL_DEBUG_HIGH, "RefCount Object not released class m_RefCount=%d \n" , m_RefCount ) ; m_RefCount = 0 ; delete this; return 0;  }; \
      const    xObjectClassID&  ClassName##::classID(){ return this->ms_CLASSID; } \
      void*    ClassName##::queryObject(const xObjectClassID& _id){if( this->ms_CLASSID.isType(_id)){this->AddRef(); return this;} return 0;}\
      bool     ClassName##::isType(const    xObjectClassID& _id){return classID().isType(_id) ;}\



template<typename _ChildT , typename _ParentT> bool XEvol_Convert(_ChildT* & _ch , _ParentT* parent)
{
	_ChildT* pChild = dynamic_cast<_ChildT*>(parent);
	if(pChild == NULL) return false;
	_ch = pChild ;
	return true;
}


#define XEVOL_DEFINE_PROPERTY_P(type , name) \
public:                                      \
	type* get##name() {return m_##name ; }           \
void  set##name(type* _v ) {m_##name = _v ;}     \
protected:                                   \
      type* m_##name;                            \



#define XEVOL_DEFINE_PROPERTY(type , name)   \
public:                                      \
	type  get##name() {return m_##name ; }       \
	void  set##name(type* _v ) {m_##name = _v ;} \
protected:                                   \
	type  m_##name;                              \

END_NAMESPACE_XEVOL3D

#endif
