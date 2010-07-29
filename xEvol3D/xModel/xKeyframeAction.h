#ifndef __XKEYFRAME_ACTION_H__
#define __XKEYFRAME_ACTION_H__

#include "xCoreAction.h"

BEGIN_NAMESPACE_XEVOL3D

struct xTransKeyFrame
{
    xBoneTrans m_Trans;
    int32      m_iFrame;
    int32      m_iTime;
};

class xTransKeyFrameTrack
{
public:
    typedef ds_vector(xTransKeyFrame) vTransKeyFrams;

public:
    void              init(int nKeyFrame, const xActionInfo& info);
    float             getBoneTrans(int lTime , int& iKey1 , int& iKey2 , bool bLoop);
    xTransKeyFrame&   keyFrame(int iKey);
    xTransKeyFrameTrack();
public:
    vTransKeyFrams    m_vKeyframs;
    xActionInfo       m_Info;
};

class _XEVOL_BASE_API_ xKeyFrameAction : public xSkeletonAction
{
	IMPL_BASE_OBJECT_INTERFACE(xKeyFrameAction);
    typedef ds_vector(xTransKeyFrameTrack) vBoneTracks;
public:
	xKeyFrameAction(xCoreSkeleton* pSkeleton , int param);
	~xKeyFrameAction();
public:
	virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	virtual bool                 load(const wchar_t * _name , xcomdocstream* stream);
	virtual bool                 load(const wchar_t * _name , istream& stream);
	virtual bool                 load(xXmlNode* pCfgNode );
    virtual bool                 blend(long time_in_ms,xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
    virtual void                 unload();
    virtual bool                 getBoneTrans(int boneIndex, xBaseActionState* pActionState , xBoneTrans& boneTrans);
protected:
    vBoneTracks                  m_vBoneTracks;
};

END_NAMESPACE_XEVOL3D
#endif
