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

    output.oPos = output.Pos;
    return output;
}
