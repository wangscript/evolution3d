#ifndef __XCORE_SKELETON_H__
#define __XCORE_SKELETON_H__

#include "xCoreMeshDef.h"

BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_ xCoreActionFrame;
class  _XEVOL_BASE_API_ xBoneHiberarchy;
class  _XEVOL_BASE_API_ xCoreSkeleton;
class  _XEVOL_BASE_API_ xCoreAction;


class  _XEVOL_BASE_API_ xCoreActionFrame
{
public:
	xCoreActionFrame();
	~xCoreActionFrame();
	int      nBone();
	xmat4&   operator[](int index);
	bool     load(int nBone);
	void     unload();
	void*    data(){return m_FrameMat ; }
protected:
	int      m_nBone;

	xmat4*   m_FrameMat;
	friend   class   xCoreSkeleton;
};

class  _XEVOL_BASE_API_  xCoreAction : public IBaseObject
{
	friend  class  xCoreSkeleton;
    IMPL_BASE_OBJECT_INTERFACE(xCoreAction);
public:
    xCoreAction();
    ~xCoreAction();
	xBoneData& getBoneData(int boneIndex, int frame);
	//public interface
	bool blend(long time_in_ms,xCoreActionFrame& skeletonFrame, xCoreSkeleton* pSkeleton);
	bool load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	void unload();
	int                     m_nBone;
protected:
	std::ds_wstring         m_Name;      //Name
	xSkeletonID             m_SkeID;
	xActionInfo             m_Info;

	xarray<xBoneData , 2>   m_BoneFrame;//m_BoneData[iBone][nFrame]
};

class _XEVOL_BASE_API_ xBoneHiberarchy
{
public:
	xBoneHiberarchy();
	~xBoneHiberarchy();
	void clear();
	bool add_child(int index );
	void push_back(int index );
	friend xCoreSkeleton;
protected:
	typedef std::vector<int , dllsafe_alloc<int> > sBoneIndexs_t;
	short                 m_nChildren;
	sBoneIndexs_t         m_Childrens;
};

typedef vector<xBoneHiberarchy>  vBoneHiberarchys;

class _XEVOL_BASE_API_ xCoreSkeleton : public IBaseObject
{
    IMPL_BASE_OBJECT_INTERFACE(xCoreSkeleton);
public:
	xCoreSkeleton();
	~xCoreSkeleton();
	int  nBone(){return m_nBone;}
	bool unload();
	bool load(xcomdoc& doc,const wchar_t* skeleton_dir);
	bool loadHiberarchy(std::istream& _in);
	bool saveHiberarchy(xcomdocstream* boneinfo_stream);
	bool loadBoneList(std::istream& _in);
public:
	bool blendLerp(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2);
	bool blendHiberarchy(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1);
	bool blendSlerp(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1);
protected:
	bool saveHiberarchy(xXmlNode* pRootNode , int rootindex);
	//骨架层次结构
	xBoneHiberarchy   m_RootHibers;
	vBoneHiberarchys  m_BoneHibers;

	//骨架信息
	int                 m_nBone;
	vSkinBones          m_Bones;
	xSkeletonID         m_SkeletonID;
};

END_NAMESPACE_XEVOL3D
#endif
