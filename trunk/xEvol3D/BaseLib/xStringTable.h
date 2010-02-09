#ifndef __XEVOL_STRING_TABLE_H__
#define __XEVOL_STRING_TABLE_H__
#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"

#include "stl_dllalloc.h"
#include "xXmlDocument.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xStringTable
{
public:
	void                  addPath(const wchar_t* _path);
	void                  clearPath();
	bool                  setLanguage(const wchar_t* lang);
	bool                  load(xXmlNode* pTable);
    bool                  addString(const wchar_t* key , const wchar_t* _val);
	const wchar_t*        trans(const wchar_t* key);
	void                  clear();
	static xStringTable*  singleton();

protected:
	xStringTable();
	~xStringTable();
protected:
	typedef std::vector<std::ds_wstring , dllsafe_alloc<std::ds_wstring> > vStrings;
	typedef ds_map(std::ds_wstring , std::ds_wstring) vStringMaps;
	vStringMaps      m_strMap;
	std::ds_wstring  m_strLang;
	vStrings         m_paths;
};

inline const wchar_t* _TRANSLATE(const wchar_t* _key)
{
	return xStringTable::singleton()->trans(_key);
}

END_NAMESPACE_XEVOL3D

#endif
