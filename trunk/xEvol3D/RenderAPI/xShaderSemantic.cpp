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
#include "../xStdPch.h"
#include "xShaderSemantic.h"
#include "../BaseLib/xStringHash.h"
#include <map>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D

typedef map<int , eShaderParamSemantic> ShaderParamSemanticMap;
static ShaderParamSemanticMap  __global_static_spsmap;

xShaderSemanticTable::xShaderSemanticTable()
{
	__insert(eSPS_WorldMatrix         ,  L"WorldMatrix");
	__insert(eSPS_ViewMatrix          ,  L"ViewMatrix");
	__insert(eSPS_ProjMatrix          ,  L"ProjectionMatrix");
	__insert(eSPS_WVPMatrix           ,  L"WVPMatrix");
	__insert(eSPS_WorldViewMatrix     ,  L"WorldViewMatrix");
	__insert(eSPS_ViewProjMatrix      ,  L"ViewProjectMatrix");
	__insert(eSPS_TextureMatrix       ,  L"TextureMatrix");
	__insert(eSPS_TextureMatrix0      ,  L"TextureMatrix0");
	__insert(eSPS_TextureMatrix1      ,  L"TextureMatrix1");
	__insert(eSPS_TextureMatrix2      ,  L"TextureMatrix2");
	__insert(eSPS_TextureMatrix3      ,  L"TextureMatrix3");
	__insert(eSPS_TextureMatrix4      ,  L"TextureMatrix4");
	__insert(eSPS_TextureMatrix5      ,  L"TextureMatrix5");
	__insert(eSPS_TextureMatrix6      ,  L"TextureMatrix6");
	__insert(eSPS_TextureMatrix7      ,  L"TextureMatrix7");


	__insert(eSPS_Inv_WorldMatrix     ,  L"InvWorldMatrix");
	__insert(eSPS_Inv_ViewMatrix      ,  L"InvViewMatrix");
	__insert(eSPS_Inv_ProjMatrix      ,  L"InvProjectionMatrix");
	__insert(eSPS_Inv_WVPMatrix       ,  L"InvWVPMatrix");
	__insert(eSPS_Inv_WorldViewMatrix ,  L"InvWorldViewMatrix");
	__insert(eSPS_Inv_ViewProjMatrix  ,  L"InvViewProjectionMatrix");


	__insert(eSPS_Camera_Pos          ,  L"CameraPos");
	__insert(eSPS_Camera_Up           ,  L"CameraUp");
	__insert(eSPS_Camera_Dir          ,  L"CameraDir");
	__insert(eSPS_Camera_Arg          ,  L"CameraArg"); 
										 
	//Light info						 
	__insert(eSPS_LigthPos             , L"LightPostion");      
	__insert(eSPS_LightDiffuse         , L"LightDiffuse");
	__insert(eSPS_LightSpeculer        , L"LightSpeculer");
	__insert(eSPS_LightAmbient         , L"LightAmbient");
	__insert(eSPS_LightDir             , L"LightDirection");
	__insert(eSPS_LightCutoff          , L"LightCutoff");

	//Fog argument
	__insert(eSPS_FogColor             ,  L"FogColor");
	__insert(eSPS_FogArg               ,  L"FogArg");
										  
										  
	//Material							  
	__insert(eSPS_MatDiffuse           ,  L"MaterialDiffuse");
	__insert(eSPS_MatSpeculer          ,  L"MaterialSpeculer");
	__insert(eSPS_MatAmbient           ,  L"MaterialAmbient");
	__insert(eSPS_MatShiness           ,  L"MaterialShiness");
										  
	//HDR								  
	__insert(eSPS_HDR_Exposure         ,  L"HDRExposure");
	__insert(eSPS_HDR_MiddleGray       ,  L"HDRMiddleGray");
	__insert(eSPS_Blom_Sharpness       ,  L"BlomSharpness");
										  
	//Screen info						  
	__insert(eSPS_ScreenRes            ,  L"ScreenRes");
	__insert(eSPS_VPRes                ,  L"ViewportRes");
	//Geometry info
	__insert(eSPS_BoundBox             ,  L"BoundBox");

	//System info
	__insert(eSPS_Random               ,  L"Random");
	__insert(eSPS_RandOnce             ,  L"RandOnce");
	__insert(eSPS_TimeTickt            ,  L"TimeTickt");
	__insert(eSPS_FrameTime            ,  L"FrameTime");
										  
	//texture info						  
	__insert(eSPS_TextureDim0        ,   L"TextureDim0");
	__insert(eSPS_TextureDim1        ,   L"TextureDim1");
	__insert(eSPS_TextureDim2        ,   L"TextureDim2");
	__insert(eSPS_TextureDim3        ,   L"TextureDim3");
	__insert(eSPS_TextureDim4        ,   L"TextureDim4");
	__insert(eSPS_TextureDim5        ,   L"TextureDim5");
	__insert(eSPS_TextureDim6        ,   L"TextureDim6");
	__insert(eSPS_TextureDim7        ,   L"TextureDim7");

}

xShaderSemanticTable::~xShaderSemanticTable()
{
	__global_static_spsmap.clear();
}

eShaderParamSemantic xShaderSemanticTable::parser(const wchar_t* paramName)
{
	int va = xStringHash(paramName);
	ShaderParamSemanticMap::iterator pos = __global_static_spsmap.find(va) ;
	if(pos == __global_static_spsmap.end() )
		return eSPS_NotUsed;
	return pos->second;
}

xShaderSemanticTable& xShaderSemanticTable::singleton()
{
	static xShaderSemanticTable table;
	return table;
}

bool xShaderSemanticTable::__insert(eShaderParamSemantic _samantic , const wchar_t* paramName)
{
	int va =	xStringHash(paramName);
	if(__global_static_spsmap.find(va) != __global_static_spsmap.end() )
		return false;
	__global_static_spsmap.insert(ShaderParamSemanticMap::value_type(va , _samantic) ) ;
	return true;
}

END_NAMESPACE_XEVOL3D
