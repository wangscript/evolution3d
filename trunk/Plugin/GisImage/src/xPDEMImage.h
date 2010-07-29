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

#ifndef __XHDR_IMAGE_H__
#define __XHDR_IMAGE_H__
#include "Image/xBaseImage.h"
#include "XMathLib/xMathLib.h"
#include "xGisImgBaseInclude.h"
using namespace xMathLib;
using namespace xGeomLib;

#include <fstream>
BEGIN_NAMESPACE_XEVOL3D

struct _GISIMAGE_API_ xGisImageRegion
{
    float m_MinX;
    float m_MinY;
    float m_MinZ;

    float m_Width;
    float m_Length;
    float m_Height;

    xGisImageRegion()
    {
        Init();
    }

    bool  PtInRect(float _x , float _y )
    {
        if( _x > m_MinX  &&	_x < (m_MinX + m_Width ) && _y > m_MinY && _y < (m_MinY + m_Length) )
        {
            return true;
        }
        return false;
    }

    bool  PtInRect(float2 _pt )
    {
        return PtInRect(_pt.x , _pt.y );
    }


    float2  GetNearestPt(float2 _pt )
    {
        if(  true == PtInRect(_pt.x , _pt.y ) )
            return _pt;
        else
        {
            float _x = _pt.x ; 
            float _y = _pt.y ;
            if( _x < m_MinX              ) _x = m_MinX          ;
            if( _x > (m_MinX + m_Width ) ) _x = m_MinX + m_Width;

            if( _y < m_MinY               ) _y = m_MinY          ;
            if( _y > (m_MinY + m_Length ) ) _y = m_MinY + m_Length;

            return float2(_x , _y);
        }
    }

    void Init()
    {
        m_MinX = 0.0f;
        m_MinY = 0.0f;
        m_MinZ = 0.0f;
        m_Width = 0.0f;
        m_Length = 0.0f;
        m_Height = 0.0f;
    }

    void save(xXmlNode* pXmlNode)
    {
        pXmlNode->setValue(L"MinX"   , (float)m_MinX   );
        pXmlNode->setValue(L"MinY"   , (float)m_MinY   );
        pXmlNode->setValue(L"MinZ"   , (float)m_MinZ   );
        pXmlNode->setValue(L"Width"  , (float)m_Width  );
        pXmlNode->setValue(L"Length" , (float)m_Length );
        pXmlNode->setValue(L"Height" , (float)m_Height );
    }

    void load(xXmlNode* pXmlNode)
    {
        m_MinX    = pXmlNode->float_value(L"MinX"  );
        m_MinY    = pXmlNode->float_value(L"MinY"  );
        m_MinZ    = pXmlNode->float_value(L"MinZ"  );
        m_Width   = pXmlNode->float_value(L"Width" );
        m_Length  = pXmlNode->float_value(L"Length");
        m_Height  = pXmlNode->float_value(L"Height");
    }
};

struct _GISIMAGE_API_ xGisImageLevelInfo
{
    uint32               m_ResolutionX;
    uint32               m_ResolutionY;
    xvec3                m_PixelScale;
    float                m_GridWidth;
    float                m_GridLength;
    float                m_zScale;

    xGisImageLevelInfo()
    {
        m_ResolutionX = 0;
        m_ResolutionY = 0;
        m_PixelScale  = xvec3(1.0f,1.0f,1.0f);
        m_GridWidth   = 0.0f;
        m_GridLength  = 0.0f;
        m_zScale      = 1.0f;
    }

    void save(xXmlNode* pXmlNode)
    {
        pXmlNode->setValue(L"GridWidth"   , m_GridWidth  );
        pXmlNode->setValue(L"GridLength"  , m_GridLength );
        pXmlNode->setValue(L"zScale"      , m_zScale     );
        pXmlNode->setValue(L"ResolutionX" , m_ResolutionX);
        pXmlNode->setValue(L"ResolutionY" , m_ResolutionY);
        pXmlNode->setValue(L"xPixel"      , m_PixelScale.x);
        pXmlNode->setValue(L"yPixel"      , m_PixelScale.y);
        pXmlNode->setValue(L"zPixel"      , m_PixelScale.z);
    }

    void load(xXmlNode* pXmlNode)
    {
        m_GridWidth    = pXmlNode->float_value(L"GridWidth"   );
        m_GridLength   = pXmlNode->float_value(L"GridLength"  );
        m_zScale       = pXmlNode->float_value(L"zScale"      );
        m_ResolutionX  = (uint32)pXmlNode->int_value(L"ResolutionX" );
		m_ResolutionY  = (uint32)pXmlNode->int_value(L"ResolutionY" );
		m_PixelScale.x = pXmlNode->float_value(L"xPixel");
		m_PixelScale.y = pXmlNode->float_value(L"yPixel");
		m_PixelScale.z = pXmlNode->float_value(L"zPixel");
    }
};

#define MAX_HEIGHT 3000000.0f
typedef std::x2DArray<float , dllsafe_alloc<float> > xPDEMImageBuf;

class  _GISIMAGE_API_ xPDEMImage : public xImage
{
public:
	xImageSize           m_imgSize;
	xPDEMImageBuf      m_Image;
    xGisImageRegion      m_Region;
    xGisImageLevelInfo   m_LevelInfo;
    int                  m_Level;
public:
	bool                 load(xcomdoc& _doc , int level);
    bool                 save(xcomdoc& _doc);
public:
	xPDEMImage();
	~xPDEMImage();
	const wchar_t*       supportExt(bool bSave);
	int                  byteAlign();
	bool                 convert(ePIXEL_FORMAT newFmt){ return false ; }; 
    void                 Resample( xPDEMImage* newImage);
    void                 Scale( xPDEMImage* newImage);
    float                Sample(double x , double y , bool& bInRect , int* pNumOfNaN );

	bool                 scale(int w , int h , IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 
	bool                 scalePOT(IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 
	bool                 scaleFit(int w , int h , IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 

	bool                 load(xImageSize size){ return false ; }; 
	bool                 load(int w , int h , ePIXEL_FORMAT fmt){ return false ; }; 
	bool                 load(const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool                 load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool                 load(const wchar_t* filename, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool                 save(const wchar_t* fileName);
	bool                 free();

	xImageSize           getSize();
	void*                data(size_t mipmapLevel = 0 , size_t cubeMapImageIndex = 0/*-1 means not a cubemap*/ , bool bCompressed = false);
	bool                 data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format);
	size_t               nMipmapLevel();
	size_t               nImage();
    size_t               nSlice();

    //For Tiff loading
    bool                 loadTiff(const wchar_t* _fileName);
    void                 FillBlankPos( xPDEMImageBuf& tempImage);
    float                FindNearestPos(uint32 tx , uint32 ty , xPDEMImageBuf& _Image);
    bool                 IsPosInImage(int x , int y, xPDEMImageBuf& _Image);
	
};


END_NAMESPACE_XEVOL3D

#endif
