//===============================================
//Begin import lib
//Import lib name=libMatrix

float4x4 normalMatrix(float4x4 mat)
{
      float4x4 matNormal = mat;//transpose(mat);
      matNormal[3] = float4(0.0f,0.0f,0.0f,1.0f);
      matNormal[0][3] = 0.0f; 
      matNormal[1][3] = 0.0f;  
      matNormal[2][3] = 0.0f;  
      return matNormal;
}//Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\hlsl\simpleMesh.vertex.hlsl

struct cTransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
};
cTransformBuffer TransformBuffer;


struct STATICMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
      float4 oPos      : POSITION0;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD0;  
      float4 Pos       : TEXCOORD1;
      float4 Nor       : TEXCOORD2;     
      float4 Tan       : TEXCOORD3;      

      float4 wPosition : TEXCOORD4;
      float4 wNormal   : TEXCOORD5;  
      float4 wTangent  : TEXCOORD6;   
};

#define VS_INPUT STATICMESHVS_INPUT

//Insert Node Declaration Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\hlsl\SkinAni.hlsl

#define _CBUFFER_ANI_
struct SKINMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Weights  : BLENDWEIGHT;
      int4   BoneIdxs : BLENDINDICES;
      float2 Tex      : TEXCOORD;  
};

struct VM_SkinRet
{
      float4 Pos;
      float4 Tan;
      float4 Nor;
      float4 wPos;
      float4 wNor;
      float4 wTan;	
};


struct cBoneFrame
{       
      float4 matBone[72 * 3];
};

cBoneFrame BoneFrame;
VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs);
#undef VS_INPUT
#define VS_INPUT SKINMESHVS_INPUT
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\hlsl\SkinAni.hlsl



VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
      VM_SkinRet ret;
      //目前这段代码还没打开
      float vWeight[4];
      vWeight[0] = Weights.x;
      vWeight[1] = Weights.y;
      vWeight[2] = Weights.z;
      vWeight[3] = Weights.w;

      int vBoneIdxs[4];
      vBoneIdxs[0] = BoneIdxs.x;
      vBoneIdxs[1] = BoneIdxs.y;
      vBoneIdxs[2] = BoneIdxs.z;
      vBoneIdxs[3] = BoneIdxs.w;


      float4 vPos = float4(0.0f,0.0f,0.0f,0.0f);
      float4 vNor = float4(0.0f,0.0f,0.0f,0.0f);
      float4 vTan = float4(0.0f,0.0f,0.0f,0.0f);

      for(int i = 0 ; i < 4 ; i ++ )
      {
            int boneIdx = vBoneIdxs[i];

            float4x4  matPosT;
            matPosT[0] = BoneFrame.matBone[ boneIdx * 3 + 0  ];
            matPosT[1] = BoneFrame.matBone[ boneIdx * 3 + 1  ];
            matPosT[2] = BoneFrame.matBone[ boneIdx * 3 + 2  ];

            matPosT[3] = float4(0.0 , 0.0 , 0.0 , 1.0);

            float4x4  matPos   = transpose(matPosT);

            float4x4 matNor = normalMatrix(matPos);
            vPos += vWeight[i] * mul(  Pos  ,  matPos  );
            vNor += vWeight[i] * mul(  Nor  ,  matNor  );
            vTan += vWeight[i] * mul(  Tan  ,  matNor  );
      }
      
      //这段代码暂时屏蔽了骨骼动画的功能
      //vPos = Pos;
      //vNor = Nor;
      //vTan = Tan;

      vPos.w = 1.0f;
      //先保存世界空间的返回值
      ret.wPos = vPos;
      ret.wNor = vNor;
      ret.wTan = vTan;

      //计算变换后的位置
      vPos    = mul(vPos  , TransformBuffer.matWorld  );
      vPos    = mul(vPos  , TransformBuffer.matView   );
      ret.Pos = mul(vPos  , TransformBuffer.matProject);


      //计算世界空间的位置和法/切向量
      float4x4 matWordNormal = normalMatrix(TransformBuffer.matWorld);
      ret.wPos = mul( ret.wPos , TransformBuffer.matWorld );
      ret.wNor = mul( ret.wNor , matWordNormal            );
      ret.wTan = mul( ret.wTan , matWordNormal            );

      //计算观察空间的位置和法/切向量
      float4x4 matNor = normalMatrix(TransformBuffer.matView);
      ret.Nor = mul( ret.wNor , matNor ); 
      ret.Tan = mul( ret.wTan , matNor );

      return ret;
}
//============================================


//============================================
//Begin Main Node'code 

PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
      output.Pos   = mul( output.Pos , TransformBuffer.matView   );
      output.Pos   = mul( output.Pos , TransformBuffer.matProject);
      output.Tex   = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color.xyzw;


	  float4x4 matModelView = TransformBuffer.matWorld * TransformBuffer.matView;
	  float4x4 matNormal    = normalMatrix(matModelView);

	  //观察坐标系中的
      output.Tan   = mul(matNormal , input.Tan);
      output.Nor   = mul(matNormal , input.Nor);

   	  //世界坐标系中的
	  matNormal = normalMatrix(TransformBuffer.matWorld);
      output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
      output.wNormal   = mul( input.Nor  , matNormal                );
      output.wTangent  = mul( input.Tan  , matNormal                );

   //应用修改器 name=SkinAni 
   VM_SkinRet Ret_VertexModifier_Node0 = VM_DoSkin(input.Pos , input.Nor , input.Color , input.Weights , input.BoneIdxs);

   output.Pos = Ret_VertexModifier_Node0.Pos; 
   output.Nor = Ret_VertexModifier_Node0.Nor; 
   output.Tan = Ret_VertexModifier_Node0.Tan; 
   output.wPosition = Ret_VertexModifier_Node0.wPos; 
   output.wNormal = Ret_VertexModifier_Node0.wNor; 
   output.wTangent = Ret_VertexModifier_Node0.wTan; 
   output.Color = input.Color; 

    output.oPos = output.Pos;
    return output;
}
