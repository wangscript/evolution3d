/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __MODEL_DATA_DEFINE_H__
#define __MODEL_DATA_DEFINE_H__
#include <vector>
#include <string>
#include <math.h>
using namespace std;
#pragma  pack(push,1)
namespace XEvol3DModel
{
    //模型类型的定义
    #define MODEL_TYPE_STRING(type) #type
    enum
    {
        XRM_STATIC_MODEL      , //静态模型 (*.xrm)
        XRM_FRAME_ANIMODEL    , //顶点动画模型 (*.xrm)
        XRM_FRAME_CHARACTOR   , //顶点动画的角色(各个顶点) (*.xrc)
        XRM_SKELETON_MODEL    , //骨骼动画模型(不能换装) (*.xrm)
        XRM_SKELETON_CHARACTOR, //骨骼动画的角色(能换装)(*.xrc)
        XRM_SKIN_MESH         , //骨骼动画换装的MESH    (*.xrs)
        XRM_CAMREA_TRACK      , //摄影机的轨迹 (*.xrc)
    };

    enum
    {
        cddt_static_model_dir     = 0x80100000,
        cddt_static_model_desc    = 0x00100000,
        cddt_static_mesh_dir      = 0x80100001,
        cddt_static_mesh_desc     = 0x10100001,

        cddt_frame_ani_model_dir  = 0x80100002,
        cddt_frame_ani_model_desc = 0x00100002,
        cddt_frame_ani_mesh_dir   = 0x80100003,
        cddt_frame_ani_mesh_desc  = 0x00100003,
    };

    //位置
    struct sVector_t
    {
        float x,y,z;
        void normalize()
        {
            float len = sqrt(x*x + y*y + z*z);
            if(len == 0.0f)
                return ;
            x /= len; 
            y /= len;
            z /= len;
        }
    };

    //uv坐标
    struct sUVCoord_t
    {
        float u,v;
    };

    //颜色
    struct sColor_t
    {
        union{
            long cl;
            struct 
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            };
        };
    };

    struct sQuat_t
    {
        float m_x;
        float m_y;
        float m_z;
        float m_w;
    };

    struct sMatrix4x4_t;
    struct sMatrix4x3_t
    {
        float m_M[4][3];
        sMatrix4x3_t & operator = (sMatrix4x4_t& m4);
    };

    struct sMatrix4x4_t
    {
    public:
        float m_M[4][4];
        sMatrix4x4_t& operator = (sMatrix4x3_t& m3);
    };

    inline sMatrix4x4_t& sMatrix4x4_t::operator = (sMatrix4x3_t& m3)
    {
        m_M[0][0] = m3.m_M[0][0];   
        m_M[0][1] = m3.m_M[0][1];  
        m_M[0][2] = m3.m_M[0][2];  
        m_M[0][3] = 0;

        m_M[1][0] = m3.m_M[1][0];   
        m_M[1][1] = m3.m_M[1][1];  
        m_M[1][2] = m3.m_M[1][2];  
        m_M[1][3] = 0;

        m_M[2][0] = m3.m_M[2][0];   
        m_M[2][1] = m3.m_M[2][1];  
        m_M[2][2] = m3.m_M[2][2];  
        m_M[2][3] = 0;

        m_M[3][0] = m3.m_M[3][0];   
        m_M[3][1] = m3.m_M[3][1];  
        m_M[3][2] = m3.m_M[3][2];  
        m_M[3][3] = 1;
        return *this;
    }

    inline sMatrix4x3_t & sMatrix4x3_t::operator = (sMatrix4x4_t& m4)
    {
        m_M[0][0] = m4.m_M[0][0];
        m_M[0][1] = m4.m_M[0][1];
        m_M[0][2] = m4.m_M[0][2];

        m_M[1][0] = m4.m_M[1][0];
        m_M[1][1] = m4.m_M[1][1];
        m_M[1][2] = m4.m_M[1][2];

        m_M[2][0] = m4.m_M[2][0];
        m_M[2][1] = m4.m_M[2][1];
        m_M[2][2] = m4.m_M[2][2];

        m_M[3][0] = m4.m_M[3][0];
        m_M[3][1] = m4.m_M[3][1];
        m_M[3][2] = m4.m_M[3][2];

    }

    //一个面中三个点的索引
    struct sFace_t
    {
        unsigned int vert[3];
    };


    //静态模型描述---------------------------------------
    struct  StaticModelDesc_t
    {
#ifndef  _UNICODE32_
		wchar_t          m_Name[32];       //这个模型的名字
#else
		unsigned short   m_Name[32];
#endif
        int     m_nMesh;     //这个模型有几个Mesh
    };

    struct sSubMeshDesc_t
    {
        int    m_nFace;       //几个Face
        int    m_nMatIndex;   //第几个Material
    };

    struct MeshFurInfo_t
    {
        float       m_furShellHeight;
        int         m_furLevel;
        bool        m_isFur;
        float       m_SkinUTile;
        float       m_SkinVTile;
    };

    //一个静态模型中的一个Mesh的描述信息
    struct sMeshDesc_t
    {
#ifndef  _UNICODE32_
		wchar_t          m_Name[32];       //这个Mesh的名字
#else
		unsigned short   m_Name[32];
#endif
        int            m_nSubMesh;     //SubMesh的个数(和Material的个数是相同的)
        int            m_nUVMaps;      //这个Mesh里使用了几个UVMapChannel
        int            m_nFace;        //这个Mesh一共有几个面
        int            m_nVertex;
        MeshFurInfo_t  m_FurInfo;      //这个Mesh的毛发效果信息
    };
    //=========================================================================================



    //帧动画模型的数据结构
    struct FrameAniModelDesc_t
    {
#ifndef  _UNICODE32_
		wchar_t          m_Name[32];       
#else
		unsigned short   m_Name[32];
#endif
        int     m_nSubMesh;
    };

    /*
    ID_AM - Ambient (value 0)
    ID_DI - Diffuse (value 1)
    ID_SP - Specular (value 2)
    ID_SH - Shininess (value 3). In R3 and later this is called Glossiness.
    ID_SS - Shininess strength (value 4). In R3 and later this is called Specular Level.
    ID_SI - Self-illumination (value 5)
    ID_OP - Opacity (value 6)
    ID_FI - Filter color (value 7)
    ID_BU - Bump (value 8)
    ID_RL - Reflection (value 9)
    ID_RR - Refraction (value 10)
    ID_DP - Displacement (value 11)
    */

    enum eTexLayerUsage
    {
        tex_usage_none          = -1,
        tex_usage_ambient       = 0,  // ambient
        tex_usage_diffuse       = 1,  // diffuse
        tex_usage_specular      = 2,   // specular
        tex_usage_shininesNs    = 3,   // shininesNs
        tex_usage_shininess     = 4,   // shininess strength
        tex_usage_self_illu     = 5,   // self-illumination
        tex_usage_opacity       = 6,   // opacity
        tex_usage_filter_color  = 7,   // filter color
        tex_usage_bump          = 8,   // bump 
        tex_usage_reflection    = 9,   // reflection
        tex_usage_refraction    = 10,  // refraction 
        tex_usage_displacement  = 11,  // displacement 
		tex_usage_max
    };

    //代表一个纹理层
    //一个材质可以有多个纹理层．纹理层和纹理对象不一样
    //纹理层属于一个Material．而纹理对象可以共享
    struct sModelTexLayer
    {
#ifndef  _UNICODE32_
		wchar_t          m_TexName[64];       
#else
		unsigned short   m_TexName[64];
#endif
        eTexLayerUsage    m_Usage;         //纹理的用途
        unsigned short    m_UVChannel;     //使用的哪一个UVChannel
    };


    //代表包围盒
    struct sBoundBox_t
    {
        float m_minx;
        float m_miny;
        float m_minz;

        float m_maxx;
        float m_maxy;
        float m_maxz;

        void clear(float len)
        {
            m_minx = fabs(len);
            m_miny = fabs(len);
            m_minz = fabs(len);

            m_maxx = -1*fabs(len);
            m_maxy = -1*fabs(len);
            m_maxz = -1*fabs(len);
        }

        sVector_t center()
        {
            sVector_t v;
            v.x  = 0.5f *(m_minx + m_maxx);
            v.y  = 0.5f *(m_miny + m_maxy);
            v.z  = 0.5f *(m_minz + m_maxz); 
            return v;
        }
        sVector_t edge_len()
        {
            sVector_t v;
            v.x  = (m_maxx - m_minx);
            v.y  = (m_maxy - m_miny);
            v.z  = (m_maxz - m_minz); 
            return v;
        }

        void add_vertex(sVector_t& v)
        {
            if(v.x > m_maxx) m_maxx = v.x;
            if(v.y > m_maxy) m_maxy = v.y;
            if(v.z > m_maxz) m_maxz = v.z;

            if(v.x < m_minx) m_minx = v.x;
            if(v.y < m_miny) m_miny = v.y;
            if(v.z < m_minz) m_minz = v.z;
        }

        void combine(sBoundBox_t& box)
        {
            if(box.m_maxx > m_maxx) m_maxx = box.m_maxx;
            if(box.m_maxy > m_maxy) m_maxy = box.m_maxy;
            if(box.m_maxz > m_maxz) m_maxz = box.m_maxz;

            if(box.m_minx < m_minx) m_minx = box.m_minx ;
            if(box.m_miny < m_miny) m_miny = box.m_miny ;
            if(box.m_minz < m_minz) m_minz = box.m_minz ;
        }

    };
    typedef std::vector<sModelTexLayer> ModelTexLayers_t;

    struct sModelMaterialHeader
    {
        //int              m_nFace;     //有几个面使用了这个材质
        
    };

    struct sMateril_t
    {
#ifndef  _UNICODE32_
		wchar_t          m_MatName[64];       
#else
		unsigned short   m_MatName[64];
#endif
        sColor_t         m_Ambient ;
        sColor_t         m_Diffuse ;
        sColor_t         m_Speculer ;
        sColor_t         m_Emissive;
        float            m_fShiness;
        float            m_fOpactiy;
        bool             m_bTwoSide;

        unsigned short   m_RenderState; //渲染状态
        int              m_Reserved0;
        int              m_Reserved1;
        int              m_Reserved2;
        int              m_Reserved3;

    };


	//表情动画用途
	struct sMorphTarget_t
	{
#ifndef  _UNICODE32_
		wchar_t        m_Name[16];      
#else
		unsigned short m_Name[16];
#endif
        int      m_MaterialIndex;//Material的序号
	};

	struct sMorphActionInfo_t
	{
        int m_nMorphTarget;//这个表情里一共有几个MorphTarget
		int m_nMorphFrame;//这个动画共有几个关键点
		int m_iDurTime   ;//这个动画延续的时间
	};

	struct sMorphActionFrame_t
	{
		vector<wstring> m_MorphTargets;
		vector<int>     m_TargetWeights;
	};

	struct sMorphAction_t
	{
		sMorphActionInfo_t          m_Info;
		vector<sMorphActionFrame_t> m_Frames;
	};

    //一个模型的材质信息
    struct sModelMaterial
    {
        int                  m_nTexLayer; //这个材质有几个纹理层
        sMateril_t           m_Material;
        ModelTexLayers_t     m_TexLayers;  //纹理层
    };

    typedef std::vector<sVector_t>  sVectors_t;
    typedef std::vector<sUVCoord_t> sUVCoords_t;
    typedef std::vector<sColor_t>   sColors_t;

    enum
    {
        ACTION_NONE    = 0    ,  //空动作。通常表示这是一个静态的死模型
        ACTION_SKELENTON      ,  //骨骼动画模型的动作
        ACTION_FA_MODEL       ,  //帧动画模型的动作
        ACTION_SKEL_CHARACTOR ,  //骨骼角色的动作
        ACTION_FA_CHARACTOR       ,  //帧动画角色的动作
    };

    struct sActionInfo_t
    {
        wchar_t m_Name[32]; 
        long    m_lTime;         //动作延续时间长短
        int     m_iFirstFrame;    //动作第一帧
        int     m_iLastFrame;     //动作最后一帧
		int     m_nFrame;         //
        int     m_ActionType;    //动作类型。通常和模型类型对应
    };

	inline int GetActNumFrame(sActionInfo_t& act)
	{
		return act.m_iLastFrame - act.m_iFirstFrame + 1;
	}

    /*
    以下数据为Skeleton-Bone动画专用
    */
    typedef std::vector<sActionInfo_t> sActionInfos_t;
    struct sVertWeight_t
    {
        int    m_BoneIndex;
        int    m_BoneID;
        float  m_fWeight;
    };

    enum 
    {
        MAX_INFLUENCE_BONE = 4,
    };

    struct sWeightedVertex_t
    {
        sVector_t       m_InitPos;
        int             m_nEffBone; //影响到的骨头数目
        sVertWeight_t   m_BoneWeights[MAX_INFLUENCE_BONE];
    };


	inline bool operator < (const sVertWeight_t& lhv ,const  sVertWeight_t& rhv)
	{
		if(lhv.m_fWeight < rhv.m_fWeight)
			return true;
		return false;
	}

	inline bool operator > (const sVertWeight_t& lhv ,const  sVertWeight_t& rhv)
	{
		if(lhv.m_fWeight > rhv.m_fWeight)
			return true;
		return false;
	}


	struct sSkeletonID_t
	{
		__int32 m_HiWord;
		__int32 m_LoWord;
		bool operator == (const sSkeletonID_t& rhv)
		{
			return (m_HiWord == rhv.m_HiWord) && (m_LoWord == rhv.m_LoWord);
		}

		bool operator != (const sSkeletonID_t& rhv)
		{
			return (m_HiWord != rhv.m_HiWord) || (m_LoWord != rhv.m_LoWord);
		}

	};

    typedef std::vector<int> sBoneIndexs_t;

	struct sBoneTrans_t
	{
		//Bone的旋转Quaternian
		sQuat_t      m_Rotate;
		//Bone的平移向量
		sVector_t    m_Trans;
		sVector_t    m_Scale;
	};
    struct sBoneData_t
    {
        //组合后的向量
        sMatrix4x4_t m_Matrix;
		//相对父节点的变换
		sBoneTrans_t m_lTranform;
        //相对父节点的．
        sMatrix4x4_t m_LocaleTM;
    };
    struct sBone_t
    {
#ifndef _UNICODE32_
		wchar_t          m_BoneName[32];
#else
		unsigned short   m_BoneName[32];
#endif
        //Bone的Name
        int          m_ParentIndex;//导出后，这个是ParentIndex
        int          m_ParentBoneID; //导出前，这个是ParentID;
        int          m_BoneID;
        //初始矩阵
        sMatrix4x4_t m_InitMT;
        sMatrix4x4_t m_InitMTInv;
    };

    
    struct sBoneHiberarchy_t
    {
        sBoneHiberarchy_t()
        {
            m_nChildren = 0;
        }
        ~sBoneHiberarchy_t()
        {
            m_nChildren = 0 ;
            m_Childrens.clear();
        }
        void clear()
        {
            m_nChildren = 0 ;
            m_Childrens.clear();
        }
        bool add_child(int index )
        {
            for(size_t i = 0 ; i < m_Childrens.size() ; ++i)
            {
                if(m_Childrens[i] == index)
                    return false;
            }
            m_Childrens.push_back(index);
            m_nChildren ++;
            return true;
        }
        void push_back(int index )
        {
            m_Childrens.push_back(index);
            m_nChildren ++;   
        }
        short         m_nChildren;
        sBoneIndexs_t m_Childrens;
    };

    typedef std::vector<sBoneHiberarchy_t>  sBoneHiberarchys_t;
    //帧动画模型的数据结构
    struct SkeletonModelDesc_t
    {
#ifndef _UNICODE32_
		wchar_t          m_Name[32];
#else
		unsigned short   m_Name[32];
#endif
        int              m_nSkinGroup;
    };

    struct SkinGroupDesc_t
    {
        int n_SkinMesh;
    };


	//目前的Socket内建类型有以下几个：
	//1 : 纯socket， 没有任何类型 SOCKET_NON_TYPE,
	//2 : 粒子发射器, SOCKET_PARTICLE   //上面绑了Particle
	//3 : Billboard,  SOCKET_BILLBOARD  //上面绑了Billboard
	struct sSocketData_t
	{
		int           m_SocketState; //Socket的状态
		//组合后的向量
		sBoneData_t   m_BoneData;
	};

	struct sSocket_t
	{
		int           m_SocketType;
		int           m_BoneIndex;   //这个Socket连接到的骨骼
		int           m_BoneID;      //这个Socket连接到的骨骼ID
		sSocketData_t m_SocketData[1];
	};


}





#pragma  pack(pop)

#endif
