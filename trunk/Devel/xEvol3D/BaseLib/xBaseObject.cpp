#include "xBaseObject.h"
#include "xStringHash.h"
#include <memory.h>
#include <string.h>
#include <string.h>
#include <wchar.h>

BEGIN_NAMESPACE_XEVOL3D

xObjectClassID IBaseObject::ms_CLASSID = xObjectClassID(L"IBaseObject",0);

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
