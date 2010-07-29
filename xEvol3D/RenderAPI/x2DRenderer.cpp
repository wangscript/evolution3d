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

#include "../xStdPch.h"
#include "x2DRenderer.h"
#include "xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D
typedef TRect<float> sDraw_Rect_t;
x2DRenderer::x2DRenderer(IRenderApi* pRenderApi)
{
    m_pRenderApi  = pRenderApi;
    m_p2DRectObject = pRenderApi->create2DRectObject();
    if(m_p2DRectObject)
    {
        m_p2DRectObject->setUVChanel(0);
    }
    m_GpuProgram[0] = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel"                               , NULL);
    m_GpuProgram[1] = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel(0:simple.texture)"             , NULL);
    m_GpuProgram[2] = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel(0:simple.texture,simple.mask)" , NULL);

}

x2DRenderer::~x2DRenderer()
{
    m_p2DRectObject->ReleaseObject();
}


bool x2DRenderer::drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , const xColor_4f& color)
{
    if(m_pRenderApi)
    {
        xvec4& dstRect =  *((xvec4*)vDestRect);
        m_p2DRectObject->setColor(color);
        m_p2DRectObject->setUVChanel(2);//nTextureLayer);
        for(int i = 0 ; i < nTextureLayer ; i ++)
        {
            m_p2DRectObject->setTexture(i,pTexture[i]);
        }        
        m_p2DRectObject->setRect(dstRect);
        m_pRenderApi->pushGpuProgram(m_GpuProgram[nTextureLayer]);
        bool bRet = m_pRenderApi->draw2DRect(m_p2DRectObject);
        m_pRenderApi->popGpuProgram();
        return bRet;
    }
    return false;
}

bool x2DRenderer::drawRectf(int nTextureLayer , IBaseTexture* pTexture[], float vDestRect[4] , float vSrcRect[4]  , const xColor_4f& color)
{
    if(m_pRenderApi)
    {
        xvec4& dstRect =  *((xvec4*)vDestRect);
        xvec4& srcRect =  *((xvec4*)vSrcRect);
        m_p2DRectObject->setColor(color);
        m_p2DRectObject->setUVChanel(2);//nTextureLayer);
        if(nTextureLayer >= 1)
        {
            m_p2DRectObject->setTexture(0,  pTexture[0] , srcRect);
        }
        for(int i = 1 ; i < nTextureLayer ; i ++)
        {
            m_p2DRectObject->setTexture(i,pTexture[i]);
        }        
        m_p2DRectObject->setRect(dstRect);
        m_pRenderApi->pushGpuProgram(m_GpuProgram[nTextureLayer]);
        bool bRet = m_pRenderApi->draw2DRect(m_p2DRectObject);
        m_pRenderApi->popGpuProgram();
        return bRet;
    }
    return false;
}

bool x2DRenderer::drawRectf(I2DRectObject*   p2DRectObject)
{
    if(p2DRectObject == NULL)
        p2DRectObject = m_p2DRectObject;

    m_pRenderApi->pushGpuProgram(m_GpuProgram[ p2DRectObject->nUVChanel() ]);
    bool bRet = m_pRenderApi->draw2DRect(m_p2DRectObject);
    m_pRenderApi->popGpuProgram();
    return bRet;

}


bool x2DRenderer::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color)
{
    if(pTexture == NULL)
        return drawRectf(0 , NULL , vDestRect , color);
    IBaseTexture* TextureArray[1] = { pTexture };
    return drawRectf(1 , TextureArray , vDestRect , color);
}

bool x2DRenderer::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , float vSrcRect[4]  ,const  xColor_4f& color)
{
    if(pTexture == NULL)
        return drawRectf(0 , NULL , vDestRect , vSrcRect ,  color);

    IBaseTexture* TextureArray[1] = { pTexture };
    return drawRectf(1 , TextureArray , vDestRect , vSrcRect ,  color);
}


void x2DRenderer::drawRectFit(IBaseTexture* pTexture, float _dstRect[4], float _srcRect[4] ,xColor_4f& imageColor , xColor_4f& bkColor  )
{
    if(pTexture == NULL)
    {
        drawRectf(0 , NULL ,_dstRect , bkColor);
        return ;
    }
    sDraw_Rect_t& dstRect = *(sDraw_Rect_t*)_dstRect;
    sDraw_Rect_t& srcRect = *(sDraw_Rect_t*)_srcRect;
    if(srcRect.h > dstRect.h || srcRect.w > dstRect.w )
    {
        drawRectFitScale(pTexture , _srcRect, _dstRect , imageColor,bkColor) ;
        return ;
    }
    drawRectCenter(pTexture , _srcRect, _dstRect , imageColor,bkColor) ;
}

void x2DRenderer::drawRectScale(IBaseTexture* pTexture, float dstRect[4], float srcRect[4] ,xColor_4f& imageColor )
{
    drawRectf(pTexture,dstRect, srcRect,imageColor);
}



void x2DRenderer::drawRectFitScale(IBaseTexture* pTexture, float _dstRect[4], float _srcRect[4] ,xColor_4f& imageColor , xColor_4f& bkColor) 
{
    if(pTexture == NULL)
    {
        drawRectf(0 , NULL ,_dstRect , bkColor);
        return ;
    }

    sDraw_Rect_t& dstRect = *(sDraw_Rect_t*)_dstRect;
    sDraw_Rect_t& srcRect = *(sDraw_Rect_t*)_srcRect;

    static vector<sDraw_Rect_t> _Blanklist;
    float sh = (float)srcRect.h;
    float sw = (float)srcRect.w;

    float dh = (float)dstRect.h;
    float dw = (float)dstRect.w;

    float x = 0 , y = 0 , w = dstRect.w, h = dstRect.h;
    if( (float)sw / sh > (float)dw / dh)
    {
        h =  (float) ( (float)dw / sw * sh   );
        y =  (float) ( ( dh - h ) / 2.0  ) ;
    }
    else
    {
        w = (float) (  (float)dh / sh * sw   );
        x = (float) (  (dw - w ) /2.0 );
    }

    sDraw_Rect_t _finalRect = dstRect;
    _finalRect.x  +=  x;
    _finalRect.y  +=  y;
    _finalRect.w  =   w ;
    _finalRect.h  =   h ;

    drawRectScale(pTexture , _finalRect.v , srcRect.v , imageColor) ;

    if(x >0 )
    {
        drawRectf(NULL , dstRect.x                     , dstRect.y,  x + 4 , dstRect.h, bkColor);
        drawRectf(NULL , dstRect.x + dstRect.w  - x -4 , dstRect.y,  x + 4 , dstRect.h, bkColor);
    }

    if(y > 0)
    {
        drawRectf(NULL , dstRect.x, dstRect.y            , dstRect.w , y + 4, bkColor);
        drawRectf(NULL , dstRect.x, dstRect.y + h + y - 4, dstRect.w , y + 4, bkColor);
    }
}

bool x2DRenderer::drawRectf(IBaseTexture* pTexture, float x , float y , float w , float h , const xColor_4f& color)
{
    float v[4] = {x  , y , w , h };
    return drawRectf(pTexture , v , color);
}

void x2DRenderer::drawRectTitle(IBaseTexture* pTexture, float _dstRect[4], float _srcRect[4] , xColor_4f& imageColor , xColor_4f& bkColor  ) 
{
    if(pTexture == NULL)
    {
        drawRectf(0 , NULL ,_dstRect , bkColor);
        return ;
    }

    sDraw_Rect_t& dstRect = *(sDraw_Rect_t*)_dstRect;
    sDraw_Rect_t& srcRect = *(sDraw_Rect_t*)_srcRect;
    //
    float wRemain = dstRect.w;

    float dx = dstRect.x;
    float dw = srcRect.w;

    float sx = srcRect.x ;
    float sw = srcRect.w;

    while(wRemain > 0)
    {
        if(wRemain < sw)
        {
            sw = wRemain; dw = sw;
        }

        float hRemain = dstRect.h;
        float sy = srcRect.y  ;
        float sh = srcRect.h;
        float dh = srcRect.h;
        float dy = dstRect.y ;
        while(hRemain > 0)
        {
            if(hRemain < sh)
            {
                sh = hRemain;
                dh = sh;
            }
            sDraw_Rect_t _tdst = {dx,dy,dw,dh};
            sDraw_Rect_t _tsrc = {sx,sy,sw,sh};
            drawRectf(pTexture,_tdst.v,_tsrc.v,imageColor);
            hRemain -= sh;
            dy += sh;
        }

        wRemain -= sw;
        dx += sw;
    }
}

void x2DRenderer::drawRectCenter(IBaseTexture* pTexture, float _dstRect[4], float _srcRect[4] ,xColor_4f& imageColor , xColor_4f& bkColor)
{
    sDraw_Rect_t& dstRect = *(sDraw_Rect_t*)_dstRect;
    sDraw_Rect_t& srcRect = *(sDraw_Rect_t*)_srcRect;

    float sx , sy , sw , sh ;
    float dx , dy , dw , dh ;

    //caculate the source rectangle
    if(srcRect.w < dstRect.w)
    {
        sx = srcRect.x;
        dx = dstRect.x + ( dstRect.w - srcRect.w ) / 2;

        sw = srcRect.w ;
        dw = srcRect.w ;
    }
    else
    {
        dx = dstRect.x;
        sx = srcRect.x + ( srcRect.w - dstRect.w ) / 2;

        sw = dstRect.w ;
        dw = dstRect.w ;
    }

    if(srcRect.h < dstRect.h)
    {
        sy = srcRect.y;
        dy = dstRect.y + ( dstRect.h - srcRect.h ) / 2;

        sh = srcRect.h ;
        dh = srcRect.h ;
    }
    else
    {
        dy = dstRect.y;
        sy = srcRect.y + ( srcRect.h - dstRect.h ) / 2;

        sh = dstRect.h ;
        dh = dstRect.h ;
    }

    //We only need to draw the background not filled by the image
    //Draw background
    if(srcRect.h < dstRect.h || srcRect.w < dstRect.w)
    {
        float bh = dy - dstRect.y ;
        if(bh >0)
        {
            drawRectf(NULL  , dstRect.x, dstRect.y                     , dstRect.w , bh + 1 , bkColor);
            drawRectf(NULL  , dstRect.x, dstRect.y + dstRect.h - bh -1 , dstRect.w , bh + 1 , bkColor);
        }

        float bw = dx - dstRect.x ;
        if(bw > 0)
        {
            drawRectf(NULL , dstRect.x                     , dstRect.y + bh - 1 , bw + 1 , dstRect.h - 2 * bh + 2 , bkColor);
            drawRectf(NULL , dstRect.x + dstRect.w - bw - 1, dstRect.y + bh - 1 , bw + 1 , dstRect.h - 2 * bh + 2 , bkColor);
        }
    }
    //draw the image
    sDraw_Rect_t _tdst = {dx,dy,dw,dh};
    sDraw_Rect_t _tsrc = {sx,sy,sw,sh};
    drawRectf(pTexture,_tdst.v,_tsrc.v,imageColor);
}

void x2DRenderer::drawRectFit(IBaseTexture* pTexture     , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor)
{
    xTextureDesc _desc;
    pTexture->desc(_desc);
    float src_rect[4] = { 0 , 0 , (float)_desc.m_width , (float)_desc.m_height } ;
    return drawRectFit(pTexture , dst_rect , src_rect , imageColor , bkColor);

}
void x2DRenderer::drawRectScale(IBaseTexture* pTexture   , float dst_rect[4] , xColor_4f& imageColor)
{
    xTextureDesc _desc;
    pTexture->desc(_desc);
    float src_rect[4] = { 0 , 0 , (float)_desc.m_width , (float)_desc.m_height } ;
    return drawRectScale(pTexture , dst_rect , src_rect , imageColor);

}
void x2DRenderer::drawRectFitScale(IBaseTexture* pTexture, float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor)
{
    xTextureDesc _desc;
    pTexture->desc(_desc);
    float src_rect[4] = { 0 , 0 , (float)_desc.m_width , (float)_desc.m_height } ;
    return drawRectFitScale(pTexture , dst_rect , src_rect , imageColor , bkColor);

}
void x2DRenderer::drawRectTitle(IBaseTexture* pTexture   , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor)
{
    xTextureDesc _desc;
    pTexture->desc(_desc);
    float src_rect[4] = { 0 , 0 , (float)_desc.m_width , (float)_desc.m_height } ;
    return drawRectTitle(pTexture , dst_rect , src_rect , imageColor , bkColor);

}
void x2DRenderer::drawRectCenter(IBaseTexture* pTexture  , float dst_rect[4] , xColor_4f& imageColor , xColor_4f& bkColor)
{
    xTextureDesc _desc;
    pTexture->desc(_desc);
    float src_rect[4] = { 0 , 0 , (float)_desc.m_width , (float)_desc.m_height } ;
    return drawRectCenter(pTexture , dst_rect , src_rect , imageColor , bkColor);

}

END_NAMESPACE_XEVOL3D
