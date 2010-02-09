/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __XEVOL_SHADER_SEMANTIC_H__
#define __XEVOL_SHADER_SEMANTIC_H__

#include "xRenderAPIDef.h"
BEGIN_NAMESPACE_XEVOL3D

#include "xShaderSemanticEnum.h"

class _XEVOL_BASE_API_  xShaderSemanticTable
{
	xShaderSemanticTable();
	~xShaderSemanticTable();
public:
	eShaderParamSemantic parser(const wchar_t* semanticName);
    static xShaderSemanticTable& singleton();
    
protected:
	bool    __insert(eShaderParamSemantic _samantic , const wchar_t* paramName);

};

END_NAMESPACE_XEVOL3D

#endif
