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
#include "xFontRender.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"


BEGIN_NAMESPACE_XEVOL3D
static xWCharTrait wcharTrait;

IMPL_BASE_OBJECT_CLASSID(xFontRender  , IBaseObject);
IMPL_OBJECT_FACTORY_MGR(xFontRender ,xFontRenderCreator , xFontRenderCreatorMgr );
 xFontRender::xFontRender(xFontLoader* pFontLoader , int arg)
 {
	 m_pRendeDevice = pFontLoader->fontRenderDevice();
	 m_pFontLoader = pFontLoader;
 }
bool  xFontRender::drawText(const wchar_t* strText, float x , float y , int maxWidth , const xColor_4f& cl )
{
	float left = x;
	float start_x = float(x) ;
	float start_y = float(y) + getInfo().nHeight;
	float end_x = x + maxWidth;
	int dx , dy;
	float maxY = 0;
	int nChar = 0;
	setFontRenderState();
	//m_pRendeDevice->beginFontRender();
    size_t _len = wcharTrait.strlen(strText);

	for(size_t i = 0 ; i < _len ; i ++)
	{
		//再画一个几出界了
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		if(ch == '\n')
		{
			start_y   += (maxY + getLinePitch() );
			maxY = 0;
			start_x = left;
		}
		else 
		{
			getCharDim(ch,dx , dy);
			if( end_x > 0 && ( (start_x + dx) > end_x)  )
			{
				start_y   +=  (maxY + getLinePitch() );
				maxY = 0;
				start_x = left ;
				drawChar(ch,start_x,start_y,dx,dy,cl);
			}
			else
			{
				drawChar(ch,start_x,start_y,dx,dy,cl);
			}
			if(dy > maxY) maxY = (float)dy;
			start_x += dx;
			nChar ++ ;
		}
	}
	dx = (int)start_x;
	dy = (int)(y - start_y);
	//m_pRendeDevice->endFontRender();
	endFontRenderState();
	return true;
}





// 不换行式的输出一个区间内的文本
bool xFontRender::drawText(const wchar_t* strText, float x , float y , int iBgnIdx, int nCount, const xColor_4f& cl, int nMaxW /*= 0*/)
{
	int nLen = (int)wcharTrait.strlen(strText);
	if ( nCount < 0 )
		nCount = nLen;
	if ( nLen > 0 && iBgnIdx < nLen && nCount > 0 )
	{
		if ( iBgnIdx < 0 )
			iBgnIdx = 0;
		if ( nCount > nLen - iBgnIdx )
			nCount = nLen - iBgnIdx;
		setFontRenderState();
		//m_pRendeDevice->beginFontRender();
		int dx = 0, dy = 0;
		for (size_t i = iBgnIdx; i < size_t(iBgnIdx + nCount); i++)
		{
            xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );

			drawChar(ch, x, y, dx, dy, cl);
			x += dx;
			if ( nMaxW > 0 )
			{
				nMaxW -= dx;
				if ( nMaxW <= 0 )
					break;
			}
		}
		//m_pRendeDevice->endFontRender();
		endFontRenderState();
		return true;
	}
	return false;
}

bool xFontRender::drawText(const wchar_t* strText, float x , float y , const xColor_4f& cl)
{
	int w , h ;
	getTextDim(strText,w , h );
	h -= getLinePitch();

	float sx = x;
	float sy = y;
    float maxY = (float)getInfo().nHeight;
	sy = sy + h;
	float start_x = (float)sx ;
	int dx , dy;
	setFontRenderState();
	//m_pRendeDevice->beginFontRender();
    size_t _len = wcharTrait.strlen(strText);
	for(size_t i = 0 ; i < _len ; i ++)
	{
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		if(ch == '\n')
		{
			sy += maxY + getLinePitch();
			start_x = (float)sx ;
			continue;
		}
		getCharDim(ch,dx , dy);
		drawChar(ch,(float)start_x, sy, dx,dy,cl);
		start_x +=  dx ;
	}
	//m_pRendeDevice->endFontRender();
	endFontRenderState();
	return true;
}

bool  xFontRender::getTextDim(const wchar_t* strText , int& w , int& h)
{
	int start_x = 0 ;
	int maxY = 0;
    size_t _len = wcharTrait.strlen(strText);
	for(size_t i = 0 ; i < _len ; i ++)
	{
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		if(ch == '\n')
		{
			h   += ( maxY + getLinePitch() );
			maxY = 0;
			start_x = 0 ;
			continue;
		}
		getCharDim(  ch , w , h);
		if(h > maxY) maxY = h;
		start_x += w;
	}
	w = start_x;
	return true;
}

bool  xFontRender::getTextDim(const wchar_t*  strText , int& dx , int& dy , int maxWidth)
{
	int start_x = 0 ;
	dx = 0 ; dy = 0;
	int maxY = 0;
    size_t _len = wcharTrait.strlen(strText);
    for(size_t i = 0 ; i < _len ; i ++)
	{
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		if(ch == '\n')
		{
			dy   += ( maxY + getLinePitch() );
			maxY = 0;
			start_x = 0 ;
			continue;
		}
		getCharDim(ch,dx , dy);
		if( maxWidth >0 && ( (start_x + dx) > maxWidth)  )
		{
			dy   += ( maxY + getLinePitch() );
			maxY = 0;
			start_x = 0 ;
		}
		if(dy > maxY) maxY = dy;
		start_x += dx;
	}
	dx = start_x;
	return true;
}

int	xFontRender::getFontHeight()
{
	return getInfo().nHeight;
}

int	xFontRender::getTextWidth(const wchar_t* strText, int iBegin, int& nLen)
{
	size_t nRealLen =  wcharTrait.strlen(strText);
	int w = 0, dx = 0, dy = 0;
	if ( iBegin < (int)nRealLen )
	{
		if ( nLen > (int)nRealLen - iBegin)
			nLen = (int)nRealLen - iBegin;
		if ( iBegin < 0 )
			iBegin = 0;
		for (size_t i = iBegin; i < (size_t)(iBegin + nLen); i++)
		{
            xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
			getCharDim(ch, dx, dy);
			w += dx;
		}
	}
	return w;
}

int	xFontRender::getCharCount(const wchar_t* strText, int iBegin, int& nMaxWidth, bool bRoundVal /*= false*/)
{
	size_t nRealCnt = (size_t)wcharTrait.strlen(strText);
	if ( iBegin >= (int)nRealCnt || nMaxWidth <= 0 )
		return 0;
	size_t i = iBegin;
    int dx = 0, dy = 0, w = 0;
	while (i < nRealCnt)
	{
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		getCharDim(ch, dx, dy);
		w += dx;
		if ( w >= nMaxWidth )
		{
			int step = w - nMaxWidth;
			if ( step > dx/2 || !bRoundVal )
				nMaxWidth = w - dx;
			else 
            {
				++i; nMaxWidth = w;
			}
			return int(i - iBegin);
		}
		++i;
	}
	nMaxWidth = w;
	return int(i - iBegin);
}

bool  xFontRender::drawTextOneLine(const wchar_t* strText, xMathLib::xvec4& rect, const xColor_4f& cl, eAlignMode hAlignMode , eAlignMode vAlignMode)
{
	int w , h ;
	getTextDim(strText,w , h );

	float rect_x = (float)rect.rect.x;
	float rect_y = (float)rect.rect.y;
	float rect_w = (float)rect.rect.w;
	float rect_h = (float)rect.rect.h;


	if(h > rect_h)
		return false;

    h -= getLinePitch();
	float sx = rect_x;
	float sy = rect_y;

	switch(vAlignMode)
	{
	case eAlign_Top:
		sy = rect_y;
		break;
	case eAlign_Bottom:
		sy = (rect_y + rect_h -  h);
		break;
	case eAlign_Center:
		sy = ((rect_h -  h)/2 + rect_y);
		break;
	}

	switch(hAlignMode)
	{
	case eAlign_Left:
		sx = rect_x;
		break;
	case eAlign_Right:
		sx = (rect_x + rect_w -  h);
		break;
	case eAlign_Center:
		sx = ((rect_w -  w)/2 + rect_x);
		break;
	}

	sy = sy + h;
	float start_x = (float)sx;
	int dx , dy;
	setFontRenderState();
	//m_pRendeDevice->beginFontRender();
    size_t _len = wcharTrait.strlen(strText);
	for(size_t i = 0 ; i < _len ; i ++)
	{
        xWCharTrait::CharType ch = wcharTrait.GetCodePoint(strText , i );
		getCharDim(ch, dx, dy);
		if( (start_x +  dx) > (rect_x + rect_w) )
		{
			break;
		}
		drawChar(ch, start_x, sy, dx,dy,cl);
		start_x +=  dx ;
	}
	//m_pRendeDevice->endFontRender();
	endFontRenderState();
	return true;
}

void  xFontRender::setFontRenderState()
{
	m_pRendeDevice->beginFontRender();
}

void  xFontRender::endFontRenderState()
{
	m_pRendeDevice->endFontRender();
}
END_NAMESPACE_XEVOL3D

