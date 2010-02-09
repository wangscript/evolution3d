#ifndef __XCORE_ACTION_H__
#define __XCORE_ACTION_H__

#include "xModelFWDecl.h"
#include "xCoreMeshDef.h"

BEGIN_NAMESPACE_XEVOL3D

class  _XEVOL_BASE_API_ xCoreActionFrame
{
public:
	xCoreActionFrame();
	~xCoreActionFrame();
	int                nBone();
	xMathLib::xmat4&   operator[](int index);
    xMathLib::xmat4&   wsMat(int index);//拿到世界空间变换矩阵
	bool               load(int nBone);
	void               unload();
	void*              data(){return m_FrameMat ; }
protected:
	int                m_nBone;
	xMathLib::xmat4*   m_FrameMat;
    xMathLib::xmat4*   m_WSMats;//世界空间中的
	friend   class     xCoreSkeleton;
};

class  _XEVOL_BASE_API_  xCoreAction : public IBaseObject
{
	friend  class  xCoreSkeleton;
    IMPL_BASE_OBJECT_INTERFACE(xCoreAction);
public:
	typedef xarray<xBoneData , 2> xBoneArray;
public:
    xCoreAction();
    ~xCoreAction();
	xBoneData& getBoneData(int boneIndex, int frame);
	//public interface
	virtual bool                 blend(long time_in_ms,xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
	virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	virtual bool                 load(const wchar_t * _name , xcomdocstream* stream);
	virtual bool                 load(const wchar_t * _name , istream& stream);
	virtual bool                 load(xXmlNode* pCfgNode );
	virtual void                 unload();
	virtual int                  memUsage();
	virtual bool                 setTime(float _time);
	virtual void                 setLoop(bool bLoop) { m_bLoopAction  = bLoop ;}
	virtual bool                 bLoop(){ return m_bLoopAction ; }
	virtual const xActionInfo&   info() const { return m_Info ; }
	virtual const xSkeletonID&   skeletonID() const { return m_SkeID ; }
	virtual const wchar_t*       name() const { return m_Name.c_str() ; }
	virtual const int            nBone() const{ return m_nBone ;}
    virtual void                 setBoneNumber(int _nBone);
	virtual const xBoneArray&    boneArray() const { return m_BoneFrame ; }
protected:
	float          _getBlendFrameLoop(long _time_in_ms, int& frame1 , int& frame2);
	float          _getBlendFrameClamp(long _time_in_ms, int& frame1 , int& frame2);
protected:
	bool                         m_bLoopAction;
	std::ds_wstring              m_Name;      //Name
	xSkeletonID                  m_SkeID;
	xActionInfo                  m_Info;
	xBoneArray                   m_BoneFrame;//m_BoneData[iBone][nFrame]
    int                          m_nBone;
};

//动作的管理器，不会释放xEmbAction。但是模型被释放后，动作还是会被释放，会成悬挂指针。
//所以。如果使用动作管理器来管理动作，就不要使用模型内部的内置动作。
class _XEVOL_BASE_API_ xEmbAction : public xCoreAction
{
	IMPL_BASE_OBJECT_INTERFACE(xEmbAction);
public:
	xEmbAction();
	~xEmbAction();
	void unload();
};

class _XEVOL_BASE_API_ xTransitionAction : public xCoreAction
{
	IMPL_BASE_OBJECT_INTERFACE(xTransitionAction);
public:
	xTransitionAction();
	~xTransitionAction();
    void setTransitTime(float _time);
	bool setAction(xCoreAction* pStart , xCoreAction* pEnd , float _time);

public:
	virtual bool         load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir) { return true ; }
	virtual bool         load(const wchar_t * _name , xcomdocstream* stream) { return true ; }
	virtual bool         load(const wchar_t * _name , istream& stream) { return true ; }
	virtual bool         load(xXmlNode* pCfgNode ) { return true ; }

public:
	static bool            IsTransitionName(const std::ds_wstring& _name , std::ds_wstring& _first , std::ds_wstring& _second);
	static std::ds_wstring MakeName(const wchar_t* _first , const wchar_t* _second);
};

END_NAMESPACE_XEVOL3D
#endif
