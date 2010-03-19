#ifndef __XTRANSCATION_ACTION_H__
#define __XTRANSCATION_ACTION_H__

#include "xCoreAction.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xTransitionAction : public xCoreAction
{
	IMPL_BASE_OBJECT_INTERFACE(xTransitionAction);
public:
	xTransitionAction();
	~xTransitionAction();
    void  setTransitTime(float _time);
    float getDurTime();
	bool  setAction(xCoreAction* pStart , xCoreAction* pEnd , float _time);

    //通过最后一帧，和下一个动作的起始帧来组成一个新的动作。方便做动作过渡.
    bool setAction(xCoreSkeleton* pSkeleton , xCoreActionFrame* pStart , xCoreAction* pEnd , float _time);

public:
	virtual bool            load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir) { return true ; }
	virtual bool            load(const wchar_t * _name , xcomdocstream* stream) { return true ; }
	virtual bool            load(const wchar_t * _name , istream& stream) { return true ; }
	virtual bool            load(xXmlNode* pCfgNode ) { return true ; }
	virtual const wchar_t*  typeID() const { return L"trans"; }
public:
	static bool             IsTransitionName(const std::ds_wstring& _name , std::ds_wstring& _first , std::ds_wstring& _second);
	static std::ds_wstring  MakeName(const wchar_t* _first , const wchar_t* _second);
};

END_NAMESPACE_XEVOL3D
#endif
