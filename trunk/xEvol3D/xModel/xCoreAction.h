#ifndef __XCORE_ACTION_H__
#define __XCORE_ACTION_H__

#include "xModelFWDecl.h"
#include "xCoreMeshDef.h"
#include "xBaseAction.h"
BEGIN_NAMESPACE_XEVOL3D

class  _XEVOL_BASE_API_  xSkeletonAction : public xBaseAction
{
public:
    xSkeletonAction(xCoreSkeleton* pSkeleton , int param);
    virtual int                  memUsage();
    virtual bool                 setTime(float _time);
    virtual float                getDurTime();
    virtual const xActionInfo*   info() const { return &m_Info ; }
    virtual const xSkeletonID&   skeletonID() const { return m_SkeID ; }
    virtual const wchar_t*       name() const { return m_Name.c_str() ; }
    virtual const int            nBone() const{ return m_nBone ;}
    virtual void                 setBoneNumber(int _nBone);
    virtual xCoreActionAttr*     attribute(){ return &m_ActionAttr ; }
protected:
    xSkeletonID                  m_SkeID;
    xCoreActionAttr              m_ActionAttr;
    int32                        m_nBone;
    xBaseActionState             m_ActionState;
};

class  _XEVOL_BASE_API_  xCoreAction : public xSkeletonAction
{
	friend  class  xCoreSkeleton;
    IMPL_BASE_OBJECT_INTERFACE(xCoreAction);
public:
    xCoreAction(xCoreSkeleton* pSkeleton , int param);
    ~xCoreAction();
	virtual bool                 getBoneTrans(int boneIndex, xBaseActionState* pActionState , xBoneTrans& boneTrans) ;
	//public interface
	virtual bool                 blend(long time_in_ms,xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
	virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	virtual bool                 load(const wchar_t * _name , xcomdocstream* stream);
	virtual bool                 load(const wchar_t * _name , istream& stream);
	virtual bool                 load(xXmlNode* pCfgNode );
	virtual void                 unload();

	virtual const xBoneArray&    boneArray() const { return m_BoneFrame ; }

protected:
	//获取某一帧骨骼的位置;
    xBoneTrans*                  getBoneData(int boneIndex, int frame);
	xBoneArray                   m_BoneFrame;//m_BoneData[iBone][nFrame]

};

END_NAMESPACE_XEVOL3D
#endif
