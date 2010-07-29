#include "../xStdPch.h"
#include "xBaseObject.h"
#include "stl_dllalloc.h"
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

	typedef std::set<void* , std::less<void*> , dllsafe_alloc<void*> > BaseObjectSet;
    BaseObjectSet m_List;
public:
    bool DumpObjectList()
    {
        BaseObjectSet::iterator pos = m_List.begin();
        XEVOL_LOG(eXL_DEBUG_HIGH , "Begin BaseObject dumping......\n");
        for(; pos != m_List.end() ; pos ++)
        {
            IBaseObject* pObject = (IBaseObject*)*pos;
            ///int RefCount = pObject->RefCount();
            const xObjectClassID& clsID = pObject->classID();
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->Object not released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , pObject );
        }
        XEVOL_LOG(eXL_DEBUG_HIGH , "End BaseObject dumping......\n");   
        return true;
    }

    void add(IBaseObject* pBaseObject)
    {
       m_List.insert((void*)pBaseObject);
    }
    void remove(IBaseObject* pBaseObject)
    {
        m_List.erase((void*)pBaseObject);
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

//=================================================
IMPL_REFCOUNT_OBJECT_FUNCTION(xMemStream)

xMemStream* xMemStream::createInstance(void* buf , int bufLen)
{
    return new xMemStream(buf , bufLen);
}
size_t xMemStream::read(char* buf,size_t len)
{
    int byte_need_read = (int)len;
    if( m_BufLen - m_iPos < (int)len )
    {
        byte_need_read = m_BufLen - m_iPos;
    }
    if(byte_need_read == 0)
        return 0;
    memcpy(buf,&m_Buffer[m_iPos],byte_need_read);
    m_iPos += byte_need_read;
    return byte_need_read;
}

size_t  xMemStream::write(const char* buf,size_t len)
{
    int byte_need_write = (int)len;
    if( m_BufLen - m_iPos < (int)len )
    {
        byte_need_write = m_BufLen - m_iPos;
    }

    memcpy(&m_Buffer[m_iPos],buf,byte_need_write);
    m_iPos += byte_need_write;
    return (size_t)byte_need_write;   
}

size_t  xMemStream::seek(long _offset, std::ios::seekdir dir)
{
    if(dir == std::ios_base::cur)
    {
        if( m_iPos + _offset >= m_BufLen )
            m_iPos = m_BufLen-1;
        else
            m_iPos += _offset;
    }
    else if(dir == std::ios_base::beg)
    {
        if( _offset > (int)m_BufLen )
            m_iPos = m_BufLen-1;               
        else
            m_iPos = _offset;                
    }
    else if(dir == std::ios_base::end)
    {
        if( _offset >= (int)m_BufLen)
            m_iPos = 0;  
        else
            m_iPos = m_BufLen -  _offset - 1;
    }

    return m_iPos;
}

size_t  xMemStream::tell()
{
    return m_iPos;
}

int     xMemStream::close()
{
    return 0;
}

bool    xMemStream::eof()
{
    return m_iPos >= m_BufLen;
}

xMemStream::xMemStream(void* buf , int bufLen)
{
    m_iPos = 0;
    m_Buffer = (char*)buf;
    m_BufLen = bufLen;
}


END_NAMESPACE_XEVOL3D
