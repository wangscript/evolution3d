#ifndef __XCORE_SKELETON_H__
#define __XCORE_SKELETON_H__

#include "xModelFWDecl.h"
#include "xCoreMeshDef.h"
#include "xCoreAction.h"
BEGIN_NAMESPACE_XEVOL3D

//============Action==================
class _XEVOL_BASE_API_ xBoneHiberarchy
{
public:
	xBoneHiberarchy();
	~xBoneHiberarchy();
	void clear();
	bool add_child(int index );
	void push_back(int index );
	int  childIndex(int iChild){ return m_Childrens[iChild] ; }
	int  nChildren(){ return m_nChildren ; }
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
	int                nBone(){return m_nBone;}
	bool               unload();
	xBoneHiberarchy&   boneHiberarchy(int iBone);
	bool               load(xcomdoc& doc,const wchar_t* skeleton_dir);
	bool               loadHiberarchy(std::istream& _in);
	bool               saveHiberarchy(xcomdocstream* boneinfo_stream);
	bool               loadBoneList(std::istream& _in);
	int                findBoneIndex(const wchar_t* boneName);
	const xSkeletonID& id() const { return m_SkeletonID ; }
    const xSkinBone&   getBone(int idx) { return m_Bones[idx] ; }
public:
	bool               blendLerp      (xBaseAction* pAction, xCoreActionAttr* pAttr , xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2);
	bool               blendHiberarchy(xBaseAction* pAction, xCoreActionAttr* pAttr , xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1);
    
    bool               blendSlerp(xBaseAction* pAction, xBaseActionState* pActionState ,xCoreActionFrame& skeletonFrame , int parent = -1);
    bool               blendSlerp(xBaseAction* pAction, xCoreActionAttr* pAttr , xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1);
	
	bool               blendLerp(xBaseAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2)
	{
		return blendLerp(pAction , pAction->attribute() , skeletonFrame , t , frame1 , frame2);
	}

	bool               blendHiberarchy(xBaseAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1)
	{
		return blendHiberarchy(pAction , pAction->attribute() , skeletonFrame , t , frame1 , frame2 , parent);
	}

	bool               blendSlerp(xBaseAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent = -1)
	{
		return blendSlerp(pAction , pAction->attribute() , skeletonFrame , t , frame1 , frame2 , parent);
	}

protected:
	bool               saveHiberarchy(xXmlNode* pRootNode , int rootindex);
	//骨架层次结构
	xBoneHiberarchy     m_RootHibers;
	vBoneHiberarchys    m_BoneHibers;
	//骨架信息
	int                 m_nBone;
	vSkinBones          m_Bones;
	xSkeletonID         m_SkeletonID;
};

END_NAMESPACE_XEVOL3D
#endif
