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


//Insert Node Declaration Node= d:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\hlsl\simpleMesh.vertex.hlsl

cbuffer TransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
}

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
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

#define VS_INPUT STATICMESHVS_INPUT

//Insert Node Declaration Node= d:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\hlsl\SkinAni.hlsl

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


#ifdef  _CBUFFER_ANI_
cbuffer BoneFrame
{       
      float4x4 matBone[256];
}
#else
Buffer<float4> BoneFrame;//[256]
#endif

VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs);
#undef VS_INPUT
#define VS_INPUT SKINMESHVS_INPUT
//==================================================

//============================================

//Insert Node Code Node= d:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\hlsl\SkinAni.hlsl



VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
      VM_SkinRet ret;

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
#ifdef _CBUFFER_ANI_
            float4x4 matPos = matBone[ vBoneIdxs[i] ];
#else
            int iBone = vBoneIdxs[i];
            iBone *= 4;
            float4 row1 = BoneFrame.Load( iBone );
            float4 row2 = BoneFrame.Load( iBone + 1 );
            float4 row3 = BoneFrame.Load( iBone + 2 );
            float4 row4 = BoneFrame.Load( iBone + 3 );

            float4x4 matPos = transpose(float4x4( row1, row2, row3, row4 ));
#endif

            float4x4 matNor = normalMatrix(matPos);
            vPos += vWeight[i] * mul( matPos   ,     Pos );
            vNor += vWeight[i] * mul( matNor   ,     Nor );
            vTan += vWeight[i] * mul( matNor   ,     Tan );
      }

      vPos.w = 1.0f;
      //先保存世界空间的返回值
      ret.wPos = vPos;
      ret.wNor = vNor;
      ret.wTan = vTan;

      //计算变换后的位置
      vPos    = mul( matWorld   , vPos);
      vPos    = mul( matView    , vPos);
      ret.Pos = mul( matProject , vPos);


      //计算世界空间的位置和法/切向量
      float4x4 matWordNormal = normalMatrix(matWorld);
      ret.wPos = mul( matWorld      , ret.wPos);
      ret.wNor = mul( matWordNormal , ret.wNor ); ;
      ret.wTan = mul( matWordNormal , ret.wTan ); ;

      //计算观察空间的位置和法/切向量
      float4x4 matNor = normalMatrix(matView);
      ret.Nor = mul( matNor   , ret.wNor ); ;
      ret.Tan = mul( matNor   , ret.wTan ); ;

      return ret;
}
//============================================


//============================================
//Begin Main Node'code 

PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( matWorld   , input.Pos );
      output.Pos   = mul( matView    , output.Pos);
      output.Pos   = mul( matProject , output.Pos);
      output.Tex   = input.Tex;
      output.Color = input.Color.xyzw;


	  float4x4 matModelView = matView * matWorld;
	  float4x4 matNormal = normalMatrix(matModelView);

	  //观察坐标系中的
      output.Tan   = mul(matNormal , input.Tan);
      output.Nor   = mul(matNormal , input.Nor);

   	  //世界坐标系中的
	  matNormal = normalMatrix(matWorld);
      output.wPosition = mul(matWorld  , input.Pos );
      output.wNormal   = mul(matNormal , input.Nor);
      output.wTangent  = mul(matNormal , input.Tan);

   //应用修改器 name=SkinAni 
   VM_SkinRet Node_0_Ret = VM_DoSkin(input.Pos , input.Nor , input.Color , input.Weights , input.BoneIdxs);

   output.Pos = Node_0_Ret.Pos; 
   output.Nor = Node_0_Ret.Nor; 
   output.Tan = Node_0_Ret.Tan; 
   output.wPosition = Node_0_Ret.wPos; 
   output.wNormal = Node_0_Ret.wNor; 
   output.wTangent = Node_0_Ret.wTan; 
   output.Color = input.Color; 
   output.Tex = input.Tex; 

    return output;
}
