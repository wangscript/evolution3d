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

#ifndef __XFONT_RENDERER_H__
#define __XFONT_RENDERER_H__
#include "../BaseLib/xResourceMgr.h"
#include "../BaseLib/xBaseObject.h"
#include "../Image/Color.h"
#include "../XMathLib/XMathLib.h"
#include "../RenderAPI/xRenderApiFwDecl.h"
BEGIN_NAMESPACE_XEVOL3D



struct xFontInfo
{
      wchar_t strFamilly[32];
      wchar_t strName[16];
      int     nWidth;
      int     nHeight;
      int     nMaxWidth;
      int     nMaxHeight;
      bool    bAntialias;
      bool    bBold;
      bool    bItalic;
      bool    bUnderLine;
};

enum eFontFilter
{
      eFontFilter_Linear,
      eFontFilter_Point,
      eFontFilter_SubPixel,
};

class _XEVOL_BASE_API_ IFontRenderDevice
{
public:
      virtual I2DRectObject*  get2DObject()= 0;
      virtual bool            setUVLayer(int nUVLayer)= 0;
      virtual bool            setShaderProgram(IGpuProgram* pProgram)= 0;
      virtual bool            init() = 0;
      virtual bool            setFontFilter(eFontFilter filter) = 0;
      virtual bool            beginFontRender() = 0;
      virtual bool            endFontRender() = 0;
      virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color) = 0;
      virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color) = 0;
      virtual IBaseTexture*   createLockableTexture(int w , int h , ePIXEL_FORMAT fmt ,bool bReadable , int nMipMap = 1, int nArraySize = 1 ) = 0;
      virtual bool            isTextureSupport(ePIXEL_FORMAT fmt , bool lockable = true) = 0;
      virtual IRenderApi*     renderApi() = 0;
};

class _XEVOL_BASE_API_ xFontRender : public IBaseObject
{
public:
      enum eAlignMode
      {
            eAlign_Center,
            eAlign_Left,
            eAlign_Right,
            eAlign_Top,
            eAlign_Bottom,
      };
      IFontRenderDevice* m_pRendeDevice;
      IMPL_BASE_OBJECT_CLASSID_INTERFACE(xFontRender);
public:
      xFontRender(): m_pRendeDevice(NULL){  }
      xFontRender(IFontRenderDevice* pRenderer){  m_pRendeDevice = pRenderer ;}
      virtual ~xFontRender(){}
      virtual const  xFontInfo& getInfo() = 0;
      virtual bool  drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, xColor_4f cl) = 0;
      virtual void  enableAntialias(bool bAnti) = 0;
      virtual bool  isAntialias() = 0;

      virtual void  setCacheSize(int maxSize) = 0;
      virtual int   getCacheSize() = 0;
      virtual void  releaseChache() = 0;

      virtual bool  getCharDim(wchar_t _chr , int& w , int& h) = 0;
      virtual bool  getTextDim(const wchar_t* text , int& w , int& h);
      virtual bool  getTextDim(const wchar_t* strText, int& dx , int& dy , int maxWidth);
      virtual int	getTextWidth(const wchar_t* strText, int iBegin, int& nLen);
      virtual int	getCharCount(const wchar_t* strText, int iBegin, int& nMaxWidth, bool bRoundVal = false);
      virtual int	getFontHeight();
      virtual int   getLinePitch() = 0;

      //¹«¹²º¯Êý
      virtual bool  drawText(const wchar_t* strText, float x , float y , int maxWidth , xColor_4f cl );
      virtual bool  drawText(const wchar_t* strText, float x , float y ,xColor_4f cl );
      virtual bool  drawText(const wchar_t* strText, float x , float y , int iBgnIdx, int nCount, xColor_4f cl, int nMaxW = 0);
      bool          drawTextOneLine(const wchar_t* strText, xMathLib::xvec4& rect, xColor_4f cl, eAlignMode hAlignMode , eAlignMode vAlignMode);

public:
      template<typename T1 , typename T2 , typename T3> bool  drawText(const wchar_t* strText, T1 x , T2 y , T3 maxWidth , xColor_4f cl )
      {
            return drawText(strText, (float)x , (float) y, (int)maxWidth , cl);

      }

      template<typename T1 , typename T2> bool  drawText(const wchar_t* strText, T1 x , T2 y ,xColor_4f cl )
      {
            return drawText(strText , (float)x  ,  (float)y , cl);
      }

      template<typename T1 , typename T2 > bool  drawText(const wchar_t* strText, T1 x , T2 y ,  int iBgnIdx, int nCount, xColor_4f cl, int nMaxW = 0)
      {
            return drawText(strText , (float)x , (float)y , iBgnIdx , nCount , cl, nMaxW);
      }

protected:
      virtual void  setFontRenderState();
      virtual void  endFontRenderState();
};

END_NAMESPACE_XEVOL3D

#endif
