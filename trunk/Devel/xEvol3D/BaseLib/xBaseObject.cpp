#include "xBaseObject.h"
#include "xStringHash.h"
#include <memory.h>
#include <string.h>
#include <string.h>
#include <wchar.h>
#include <set>
BEGIN_NAMESPACE_XEVOL3D

xObjectClassID IBaseObject::ms_CLASSID = xObjectClassID(L"IBaseObject",0);

class xBaseObjectList
{
     static xBaseObjectList* g_BaseObjectList;
public:
    xBaseObjectList()
    {

    }
    ~xBaseObjectList()
    {

    }

    typedef std::set<IBaseObject*> BaseObjectSet;
    BaseObjectSet m_List;
public:
    bool DumpObjectList()
    {
        BaseObjectSet::iterator pos = m_List.begin();
        XEVOL_LOG(eXL_DEBUG_HIGH , "Begin BaseObject dumping......\n");
        for(; pos != m_List.end() ; pos ++)
        {
            IBaseObject* pObject = *pos;
            int refCount = pObject->RefCount();
            const xObjectClassID& clsID = pObject->classID();
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->Object not released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , (int)pObject );
        }
        XEVOL_LOG(eXL_DEBUG_HIGH , "End BaseObject dumping......\n");   
        return true;
    }

    void add(IBaseObject* pBaseObject)
    {
       m_List.insert(pBaseObject);
    }
    void remove(IBaseObject* pBaseObject)
    {
        m_List.erase(pBaseObject);
    }
   
    static xBaseObjectList* singleton()
    { 
        if(g_BaseObjectList == NULL) 
            g_BaseObjectList = new xBaseObjectList ; 
        return g_BaseObjectList ;
    }
};

xBaseObjectList* xBaseObjectList::g_BaseObjectList = NULL;
bool IBaseObject::DumpObjectList()
{
    return xBaseObjectList::singleton()->DumpObjectList();
}
IBaseObject::IBaseObject()
{
    xBaseObjectList::singleton()->add(this);
}

IBaseObject::~IBaseObject()
{
    xBaseObjectList::singleton()->remove(this);
}

bool  xObjectClassID::isType(const wchar_t* _clasName) const
{
	xObjectClassID _id(_clasName,0);
	return isType(_id);
}

bool  xObjectClassID::operator == (const xObjectClassID& lhv) const
{
	return (lhv.m_ID == m_ID);
}

bool  xObjectClassID::isType(const xObjectClassID& classID) const
{
	const xObjectClassID* pID  = this;
	while(pID)
	{
		if(pID->m_ID == classID.m_ID) return true;
		pID=pID->m_pBassClasID;
	}
	return false;
}

xObjectClassID::xObjectClassID(const wchar_t* className , xObjectClassID* pBaseClassID)
{
	wcsncpy(m_Name,className,32);
	m_ID = xStringHash(className);
	m_pBassClasID = pBaseClassID;
}

xObjectClassID::xObjectClassID(const xObjectClassID& lhv)
{
	memcpy(m_Name,lhv.m_Name,32 * sizeof(wchar_t));
	m_ID = lhv.m_ID;
	m_pBassClasID = lhv.m_pBassClasID;
}

xObjectClassID& xObjectClassID::operator = (const xObjectClassID& lhv)
{
	memcpy(m_Name,lhv.m_Name,32 * sizeof(wchar_t));
	m_ID = lhv.m_ID;
	return *this;
}

END_NAMESPACE_XEVOL3D
