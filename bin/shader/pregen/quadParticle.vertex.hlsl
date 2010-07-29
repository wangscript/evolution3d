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

struct BILLBOARD_INPUT
{
      float4 Pos      : POSITION;
      float4 Color    : COLOR;
      float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
      float4 Pos       : POSITION0;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD;  
      float4 wPosition : TEXCOORD2;
};

PS_INPUT main( BILLBOARD_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
      output.Pos   = mul( output.Pos , TransformBuffer.matView   );
      output.Pos   = mul( output.Pos , TransformBuffer.matProject);
      output.Tex   = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color.xyzw;

      //世界坐标系中的
      output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );

    return output;
}
