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
    xBoneTrans&        boneTrans(int index);
	bool               load(int nBone);
	void               unload();
	void*              data(){return m_FrameMat ; }
protected:
	int                m_nBone;
	xMathLib::xmat4*   m_FrameMat;
    xMathLib::xmat4*   m_WSMats;//世界空间中的
    xBoneTrans*        m_vBoneTrans;
	friend   class     xCoreSkeleton;
};


class  _XEVOL_BASE_API_ xCoreActionAttr
{
public:
	enum 
	{
		eBoneInclude = 0,
		eBoneExclude  ,
	};
public:
	int       boneAttribute(int boneIndex){ return m_BoneAttribtes[boneIndex] ; }
	void      setBoneAttribute(int attr );
	void      setBoneAttribute(int attr , int idx);
	void      setBoneAttribute(int* pAttributes , int nAttr , int iStart = 0);
	void      init(int nBone , int Attri = eBoneInclude);
	bool      boneUsed(int boneIndex);

   int&       operator [](int idx){ return m_BoneAttribtes[idx] ; }
private:
	std::vector<int>  m_BoneAttribtes;
};


struct xBaseActionState
{
    long  m_lTime;
    int   m_Frame1;
    int   m_Frame2;
    float m_fTime;
};


class _XEVOL_BASE_API_ xBaseAction : public IBaseObject
{
IMPL_BASE_OBJECT_INTERFACE(xBaseAction);
public:
    virtual xBoneData*           getBoneData(int boneIndex, int frame ) = 0;
    //这个是专门用来给非常规动作用的
    virtual xBaseActionState*    getActionState(int boneIndex, xBaseActionState* pActionState) { return pActionState; }
    virtual xCoreActionAttr*     attribute() = 0;
    //public interface
    virtual bool                 blend(long time_in_ms,xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton) = 0;
    virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir) = 0;
    virtual bool                 load(const wchar_t * _name , xcomdocstream* stream) = 0;
    virtual bool                 load(const wchar_t * _name , istream& stream) = 0;
    virtual bool                 load(xXmlNode* pCfgNode ) = 0;
    virtual void                 unload() = 0;
    virtual int                  memUsage() = 0;
    virtual bool                 setTime(float _time) = 0;
    virtual float                getDurTime() = 0;
    virtual const xActionInfo*   info() const = 0;
    virtual const xSkeletonID&   skeletonID() const = 0;
    virtual const wchar_t*       name() const = 0;
    virtual const int            nBone() const  = 0;
	virtual const wchar_t*       typeID() const = 0;

public:
    virtual void                 setLoop(bool bLoop) { m_bLoopAction  = bLoop ;}
    virtual bool                 bLoop(){ return m_bLoopAction ; }

protected:
    float          _getBlendFrameLoop(const xActionInfo* pInfo , long _time_in_ms, int& frame1 , int& frame2);
    float          _getBlendFrameClamp(const xActionInfo* pInfo ,long _time_in_ms, int& frame1 , int& frame2);

protected:
    bool       m_bLoopAction;

};

class  _XEVOL_BASE_API_  xCoreAction : public xBaseAction
{
	friend  class  xCoreSkeleton;
    IMPL_BASE_OBJECT_INTERFACE(xCoreAction);
public:
	typedef xarray<xBoneData , 2> xBoneArray;

public:
    xCoreAction();
    ~xCoreAction();
	virtual xBoneData*           getBoneData(int boneIndex, int frame );
    virtual xCoreActionAttr*     attribute(){ return &m_ActionAttr ; }
	//public interface
	virtual bool                 blend(long time_in_ms,xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
	virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	virtual bool                 load(const wchar_t * _name , xcomdocstream* stream);
	virtual bool                 load(const wchar_t * _name , istream& stream);
	virtual bool                 load(xXmlNode* pCfgNode );
	virtual void                 unload();
	virtual int                  memUsage();
    virtual bool                 setTime(float _time);
    virtual float                getDurTime();
	virtual const xActionInfo*   info() const { return &m_Info ; }
	virtual const xSkeletonID&   skeletonID() const { return m_SkeID ; }
	virtual const wchar_t*       name() const { return m_Name.c_str() ; }
	virtual const int            nBone() const{ return m_nBone ;}
    virtual void                 setBoneNumber(int _nBone);
	virtual const xBoneArray&    boneArray() const { return m_BoneFrame ; }
	virtual const wchar_t*       typeID() const;


protected:

protected:
	bool                         m_bLoopAction;
	std::ds_wstring              m_Name;      //Name
	xSkeletonID                  m_SkeID;
	xActionInfo                  m_Info;
	xBoneArray                   m_BoneFrame;//m_BoneData[iBone][nFrame]
    xCoreActionAttr              m_ActionAttr;
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

END_NAMESPACE_XEVOL3D
#endif
