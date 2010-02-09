#ifndef _XEVOL_IDMGR_H_
#define _XEVOL_IDMGR_H_
#include "xEvol3DBaseInc.h"
#include <assert.h>
#include <ctype.h>
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xIDManager
{
public:
	int  getID();
	void freeID(int id);
	int  getID(int nID);
	void setBase(int _base){ m_lastID = _base; }
	xIDManager();
	~xIDManager();
protected:
	int              m_lastID;
	std::vector<int> m_freeIds;
};

END_NAMESPACE_XEVOL3D

#endif
