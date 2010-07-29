#ifndef __XBASE_ACTION_H__
#define __XBASE_ACTION_H__

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

class _XEVOL_BASE_API_ xBoneArray
{
public:
    class xBoneArray1D
    {
    public:
        xBoneArray1D(xBoneTrans* pData , int Len)
        {
            m_pData = pData;
            m_Len = Len;
        }
        xBoneTrans&  operator[](int index)
        {
            if(index >= m_Len || index < 0)
            {
                assert(0);
            }
            return m_pData[index];
        }
    protected:
        xBoneTrans* m_pData;
        int         m_Len;
    };

public:
    xBoneArray();
    ~xBoneArray();
    void         create(int* DimSize);
    void         create(int Row , int Col);
#ifndef _DEBUG
    xBoneTrans*  operator[](int index);
#else
    xBoneArray1D operator[](int index);
#endif
    void         free();
public:
    xBoneTrans*      m_Data;
    int              m_Col;
    int              m_Row;
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
    //public interface
	xBaseAction(xCoreSkeleton* pSkeleton , int param);
	virtual xCoreActionAttr*     attribute() = 0;
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

    static xBaseAction*          createInstance(eActionType _type , xCoreSkeleton* pSkeleton = NULL , int param = 0 );
    static xBaseAction*          createInstance(const wchar_t*  _class , xCoreSkeleton* pSkeleton = NULL , int param = 0 );
public:
	//这个是专门用来给非常规动作用的
	virtual xBaseActionState*    getActionState(int boneIndex, xBaseActionState* pActionState) { return pActionState; }
    virtual void                 setLoop(bool bLoop) { m_bLoopAction  = bLoop ;}
    virtual bool                 bLoop(){ return m_bLoopAction ; }

	//默认的实现，可以重载
	virtual bool                 getBoneTrans(int boneIndex, xBaseActionState* pActionState , xBoneTrans& boneTrans) = 0;
    virtual void                 updateState(long _time_in_ms , xBaseActionState& _as , bool bLoop);
    virtual void                 updateState(long _time_in_ms , xBaseActionState& _as);
protected:
    void          _getBlendFrameLoop(const xActionInfo* pInfo  , long _time_in_ms , xBaseActionState& _as );
    void          _getBlendFrameClamp(const xActionInfo* pInfo  , long _time_in_ms , xBaseActionState& _as);
	

protected:
    bool              m_bLoopAction;
    std::ds_wstring   m_Name;      //Name
    xActionInfo       m_Info;
};

DECL_OBJECT_FACTORY_AND_MGR(xBaseAction ,xBaseActionCreator , xBaseActionFactoryMgr , xCoreSkeleton* , int );

#define IMPL_ACTION_FACTORY_2(ActionClass , ActionName) IMPL_OBJECT_FACTORY(ActionClass ,xBaseAction ,  ActionClass##Factory , xBaseActionCreator , xBaseActionFactoryMgr ,  _TEXT_(ActionName)      ,  _TEXT_(ActionName)      , xCoreSkeleton* , int );
#define IMPL_ACTION_FACTORY(ActionClass)                IMPL_OBJECT_FACTORY(ActionClass ,xBaseAction ,  ActionClass##Factory , xBaseActionCreator , xBaseActionFactoryMgr ,  _WIDETEXT_(#ActionClass), _WIDETEXT_(#ActionClass) , xCoreSkeleton* , int );


END_NAMESPACE_XEVOL3D
#endif
