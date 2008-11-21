ENUMCLASS(enum) eShaderParamSemantic
{
   //æÿ’Û–≈œ¢
   eSPS_WorldMatrix         ,  //"WorldMatrix"
   eSPS_ViewMatrix          ,  //"ViewMatrix"
   eSPS_ProjMatrix          ,  //"ProjectionMatrix"
   eSPS_TextureMatrix       ,  //"TextureMatrix"
   eSPS_TextureMatrix0      ,  //"TextureMatrix0"
   eSPS_TextureMatrix1      ,  //"TextureMatrix1"
   eSPS_TextureMatrix2      ,  //"TextureMatrix2"
   eSPS_TextureMatrix3      ,  //"TextureMatrix3"
   eSPS_TextureMatrix4      ,  //"TextureMatrix4"
   eSPS_TextureMatrix5      ,  //"TextureMatrix5"
   eSPS_TextureMatrix6      ,  //"TextureMatrix6"
   eSPS_TextureMatrix7      ,  //"TextureMatrix7"

   eSPS_WVPMatrix           ,  //"WVPMatrix"
   eSPS_WorldViewMatrix     ,  //"WorldViewMatrix"
   eSPS_ViewProjMatrix      ,  //"ViewProjectMatrix"

   eSPS_Inv_WorldMatrix     ,  //"InvWorldMatrix"
   eSPS_Inv_ViewMatrix      ,  //"InvViewMatrix"
   eSPS_Inv_ProjMatrix      ,  //"InvProjectionMatrix"
   eSPS_Inv_WVPMatrix       ,  //"InvWVPMatrix"
   eSPS_Inv_WorldViewMatrix ,  //"InvWorldViewMatrix"
   eSPS_Inv_ViewProjMatrix  ,  //"InvViewProjectionMatrix"

   eSPS_Camera_Pos          ,  //"CameraPos"
   eSPS_Camera_Up           ,  //"CameraUp"
   eSPS_Camera_Dir          ,  //"CameraDir"
   eSPS_Camera_Arg          ,  //"CameraArg"  [Near,Far , Fov , Asepect]

   //Light info
   eSPS_LigthPos             ,   //"LightPostion"      
   eSPS_LightDiffuse         ,   //"LightDiffuse"
   eSPS_LightSpeculer        ,   //"LightSpeculer"
   eSPS_LightAmbient         ,   //"LightAmbient"
   eSPS_LightDir             ,   //"LightDirection"
   eSPS_LightCutoff          ,   //"LightCutoff"

   //Fog argument
   eSPS_FogColor             ,   //"FogColor"
   eSPS_FogArg               ,   //"FogArg"


   //Material
   eSPS_MatDiffuse           ,  //"MaterialDiffuse"
   eSPS_MatSpeculer          ,  //"MaterialSpeculer"
   eSPS_MatAmbient           ,  //"MaterialAmbient"
   eSPS_MatShiness           ,  //"MaterialShiness"

   //HDR
   eSPS_HDR_Exposure         ,  //"HDRExposure"
   eSPS_HDR_MiddleGray       ,  //"HDRMiddleGray"
   eSPS_Blom_Sharpness       ,  //"BlomSharpness"
					   
   //Screen info
   eSPS_ScreenRes            ,  //"ScreenRes"
   eSPS_VPRes                ,  //"ViewportRes"

   //Geometry info
   eSPS_BoundBox            ,   //"BoundBox"
   
   //System info
   eSPS_Random              ,   //"Random"
   eSPS_RandOnce            ,   //"RandOnce"
   eSPS_TimeTickt           ,   //"TimeTickt"
   eSPS_FrameTime           ,   //"FrameTime"

   eSPS_TextureDim0        ,   //"TextureDim0"
   eSPS_TextureDim1        ,   //"TextureDim1"
   eSPS_TextureDim2        ,   //"TextureDim2"
   eSPS_TextureDim3        ,   //"TextureDim3"
   eSPS_TextureDim4        ,   //"TextureDim4"
   eSPS_TextureDim5        ,   //"TextureDim5"
   eSPS_TextureDim6        ,   //"TextureDim6"
   eSPS_TextureDim7        ,   //"TextureDim7"

   eSPS_Count               ,
   eSPS_NotUsed             ,
};
