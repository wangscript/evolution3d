#ifndef __XBASE_COREMESH_DEFINE_H__
#define __XBASE_COREMESH_DEFINE_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../RenderAPI/xTextureManager.h"
#include "../xcomdoc/xcomdoc.h"
#include "../BaseLib/xarray.h"
BEGIN_NAMESPACE_XEVOL3D
struct  xSkeletonID;
struct  xMeshTexture;
struct  xMeshMaterial;
typedef std::vector<xMeshTexture , dllsafe_alloc<xMeshTexture>  > vMeshTextures; 
typedef std::vector<xMeshMaterial, dllsafe_alloc<xMeshMaterial> > vMeshMaterials; 
struct xSkeletonID
{
	int32 m_HiWord;
	int32 m_LoWord;
public:
	bool operator == (const xSkeletonID& rhv)
	{
		return (m_HiWord == rhv.m_HiWord) && (m_LoWord == rhv.m_LoWord);
	}
	bool operator != (const xSkeletonID& rhv)
	{
		return (m_HiWord != rhv.m_HiWord) || (m_LoWord != rhv.m_LoWord);
	}
};

struct xMeshTexture
{
	ds_wstring   m_TexName;
	int          m_iChannel;
	HBaseTexture m_hTexture;
};

struct xMeshMaterial
{
public:
	vMeshTextures m_vTextures;
};

//骨骼动画的顶点
struct xSkinMeshVertex
{
	xvec4           m_Position;
	xvec4           m_Normal;
	xvec4           m_Diffuse;
	xvec4           m_Tangent;
	float           m_weight[4];
	unsigned int32  m_windex[4];
	

	//==========================
	xvec2           m_uv[1];
};

//静态Mesh的顶点
struct xStaticMeshVertex
{
	xvec4           m_Position;
	xvec4           m_Normal;
	xvec4           m_Diffuse;
	xvec4           m_Tangent;

	//==========================
	xvec2           m_uv[1];
};

struct xBoneTrans
{
	//Bone的旋转Quaternian
	xquat    m_Rotate;
	//Bone的平移向量
	xvec3    m_Trans;
	xvec3    m_Scale;
};

struct xBoneData
{
	//组合后的向量
	xmat4       m_Matrix;
	//Bone的平移向量
	xBoneTrans  m_BoneTrans;
	//相对父节点的．
	xmat4       m_LocaleTM;
};

struct xSkinBone
{
	//Bone的Name
	wchar_t      m_BoneName[32];
	union{
	int          m_ParentIndex;//导出后，这个是ParentIndex
	int          m_UC4AddStart;
	};

	int          m_ParentBoneID; //导出前，这个是ParentID;
	int          m_BoneID;
	//初始矩阵
	xmat4        m_InitMT;
	xmat4        m_InitMTInv;
};

typedef std::vector<xSkinBone , dllsafe_alloc<xSkinBone> > vSkinBones;

enum eActionType
{
	 eActType_Skel_Keyframe = 0,
     eActType_Skeleton      = 1,

	 eActType_Keyframe      = 2,
	 eActType_Frame         = 3,
	
};

struct xActionInfo
{
	long              m_lTime;         //动作延续时间长短
	int               m_iFirstFrame;   //动作第一帧
	int               m_iLastFrame;    //动作最后一帧
	int               m_nFrame;
	eActionType       m_eActType;

};

END_NAMESPACE_XEVOL3D
#endif
