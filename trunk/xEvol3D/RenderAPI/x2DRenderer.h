/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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

#ifndef __2D_RENDERER_H__
#define __2D_RENDERER_H__
#include "xRenderAPI.h"
#include "xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ x2DRenderer 
{

public:
    x2DRenderer(IRenderApi* pRenderApi );
    ~x2DRenderer();
public:
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , const xColor_4f& color);
    bool                     drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , float vSrcRect[4]  ,const  xColor_4f& color);
    bool                     drawRectf(I2DRectObject*   p2DRectObject = NULL);
    
    bool                     drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color);
    bool                     drawRectf(IBaseTexture* pTexture, float vDestRect[4] , float vSrcRect[4]  ,const  xColor_4f& color);
    bool                     drawRectf(IBaseTexture* pTexture, float x , float y , float w , float h , const xColor_4f& color);

public:
    void                     drawRectFit(IBaseTexture* pTexture     , float dst_rect[4] , float src_rect[4] ,xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectScale(IBaseTexture* pTexture   , float dst_rect[4] , float src_rect[4] ,xColor_4f& imageColor) ;
    void                     drawRectFitScale(IBaseTexture* pTexture, float dst_rect[4] , float src_rect[4] ,xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectTitle(IBaseTexture* pTexture   , float dst_rect[4] , float src_rect[4] ,xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectCenter(IBaseTexture* pTexture  , float dst_rect[4] , float src_rect[4] ,xColor_4f& imageColor , xColor_4f& bkColor) ;

    void                     drawRectFit(IBaseTexture* pTexture     , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectScale(IBaseTexture* pTexture   , float dst_rect[4] , xColor_4f& imageColor) ;
    void                     drawRectFitScale(IBaseTexture* pTexture, float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectTitle(IBaseTexture* pTexture   , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor) ;
    void                     drawRectCenter(IBaseTexture* pTexture  , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor) ;

protected:
    I2DRectObject*           m_p2DRectObject;
    HGpuProgram              m_GpuProgram[3];
    IRenderApi*              m_pRenderApi;
    xRefCount                m_RefCount;
};


END_NAMESPACE_XEVOL3D
#endif

