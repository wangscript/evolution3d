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
#include "stdafx.h"

#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include "io/mstream.h"
#include "xcomdoc/xdocfstream.h"
#include "Image/xImage.h"
#include "xPDEMImage.h"
#include "geotiffio.h"
#include "geo_normalize.h"
#include "libxtiff/xtiffio.h"

BEGIN_NAMESPACE_XEVOL3D

bool xPDEMImage::save(const wchar_t* fileName)
{
     xcomdoc _doc;
     _doc.open(fileName , xcdm_rw  , 0 , true ,  false );     
     save(_doc);
     return _doc.save(fileName);
}
bool xPDEMImage::save(xcomdoc& _doc)
{
    //写入Level0.xml
    {
        wchar_t _xmlname[16] = {0};
        swprintf_x(_xmlname , 16 , L"level-%d.xml" , m_Level);

        xXmlDocument xml;
        xXmlNode* pRootNode = xml.insertNode(L"DEM");
        xXmlNode*  pRegionNode = pRootNode->insertNode(L"region");
        xXmlNode* pLevelNode  = pRootNode->insertNode(L"info");
        m_Region.save(pRegionNode);
        m_LevelInfo.save(pLevelNode);

        xcomdocstream* _desc = _doc.create_stream(_xmlname , xcddt_common, XCOMDOC_COMPRESS_BEST );
        xcdstream  _out(_desc);
        xml.save(_out);
        _out.close();
        _doc.close_stream(_desc);
    }

    //写入数据
    {
        wchar_t _name[16] = {0};
        swprintf_x(_name , 16 , L"level-%d" , m_Level);
        xcomdocstream* dataStream = _doc.create_stream(_name, xcddt_common, XCOMDOC_COMPRESS_BEST );

        uint32 _w = m_imgSize.w;
        uint32 _h = m_imgSize.h;

        dataStream->write(_w);
        dataStream->write(_h);

        dataStream->write(m_Image.m_Data , m_imgSize.w * m_imgSize.h);
    }
    return true;
}
bool xPDEMImage::load(xcomdoc& _doc , int level)
{
    m_Level = level;

    //先打开这个Level对应的xml描述文件
    wchar_t _xmlname[16] = {0};
    swprintf_x(_xmlname , 16 , L"level-%d.xml" , level);

	xcomdocstream* _desc = _doc.open_stream(_xmlname);
    if(_desc == NULL)
        return false;

    xcdstream  _in(_desc);
    xXmlDocument xml;
    xml.load(_in);
    _in.close();
    _doc.close_stream(_desc);

    //打开这个Level对应的数据文件
    wchar_t _name[16] = {0};
    swprintf_x(_name , 16 , L"level-%d" , level);
    xcomdocstream* dataStream = _doc.open_stream(_name);
    if(dataStream == NULL)
        return false;

    
    uint32 _w = 0;
    uint32 _h = 0;
    dataStream->read(_w);
    dataStream->read(_h);

    m_Image.create(_h , _w);
    dataStream->read(m_Image.m_Data , _w * _h);
    _doc.close_stream(dataStream);

    xXmlNode* pRootNode = xml.root();

    //基本信息
    xXmlNode* pRegionNode = pRootNode->findNode(L"region");
    m_Region.load(pRegionNode);

    m_imgSize.fmt = PIXELFORMAT_R32F;
    m_imgSize.pitch = _w * sizeof(float);
    m_imgSize.w = _w;
    m_imgSize.h = _h;

	return m_Image.m_Data != NULL;
}

xPDEMImage::xPDEMImage()
{
    m_imgSize.fmt = PIXELFORMAT_R32F;
    m_imgSize.pitch = 0;
    m_imgSize.w = 0;
    m_imgSize.h = 0;
}

xPDEMImage::~xPDEMImage()
{
    free();
}

int xPDEMImage::byteAlign()
{
     return 4;
}

bool xPDEMImage::load(const wchar_t* fileName, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
    std::wstring strExt = xFileSystem::singleton()->getFileExtName(fileName);
    if(strExt == L"tif" || strExt == L"tiff")
    {
        return loadTiff(fileName);
    }
    xcomdoc doc;
    if(doc.open(fileName , xcdm_read ) == false)
        return false;
    bool bRet = load(doc , _arg);
    doc.close();
    return bRet;
}

bool xPDEMImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
    if(mem_buf)
    {
        xcomdoc doc;
        if(doc.open(mem_buf , buf_len , xcdm_read) == false)
            return false;
        bool bRet = load(doc , _arg);
        doc.close();
        return bRet;
    }
    return load(fileName , img_origin);
}

bool xPDEMImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
    return load(fileName , mem_buf , buf_len , img_origin);
}


bool xPDEMImage::free()
{
   m_Image.free();
   return true;
}



xImageSize xPDEMImage::getSize()
{
    return m_imgSize;
}


void* xPDEMImage::data(size_t mipmapLevel, size_t cubeMapImageIndex /*-1 means not a cubemap*/ , bool bCompressed) 
{
    if(mipmapLevel == 0 && cubeMapImageIndex == 0)
		return m_Image.m_Data;
	return NULL;
}


bool xPDEMImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
    if(format != PIXELFORMAT_R32F)
        return false;

    if(m_imgSize.w != mWidth || m_imgSize.h != mHeight)
    {
        m_Image.free();
    }

    if(m_Image.m_Data == NULL)
    {
        m_imgSize.pitch = mWidth * sizeof(float);
        m_imgSize.w = mWidth;
        m_imgSize.h = mHeight;
        m_imgSize.fmt = PIXELFORMAT_R32F;
        m_Image.create(mHeight , mWidth);
    }

    memcpy(m_Image.m_Data , image_buffer , mWidth * mHeight * sizeof(float) );
    return true;
}


size_t xPDEMImage::nMipmapLevel()
{
    return 1;
}


size_t xPDEMImage::nImage()
{
   return 1;
}


size_t xPDEMImage::nSlice()
{
   return 1;
}

const wchar_t*   xPDEMImage::supportExt(bool bSave)
{
	if(bSave)
	{
		return NULL;
	}
	else
	{
		static wchar_t* loadExt = L"demp";
		return loadExt;
	}
}

void xPDEMImage::Resample( xPDEMImage* newImage)
{
    double step_x = newImage->m_Region.m_Width  /   (newImage->m_LevelInfo.m_ResolutionX  - 1);
    double step_y = newImage->m_Region.m_Length /   (newImage->m_LevelInfo.m_ResolutionY - 1);

    for(int y = 0 ; y < (int)newImage->m_LevelInfo.m_ResolutionY ; y ++)
    {
        for(int x = 0 ; x < (int)newImage->m_LevelInfo.m_ResolutionX ; x ++)
        {
            double fx = newImage->m_Region.m_MinX + x * step_x;
            double fy = newImage->m_Region.m_MinY + y * step_y;

            bool bInRect = false;
            int  NumOfNaN = 0;
            float v = Sample(fx , fy , bInRect , &NumOfNaN);
            if(bInRect && NumOfNaN == 0)
            {
                //newImage->m_Image[y][x].pos.x = (float)fx - newImage->m_Region.m_MinX ;
                //newImage->m_Image[y][x].pos.x = (float)fx - newImage->m_Region.m_MinY ;
                newImage->m_Image[y][x] = v;
            }
        }
    }
    return ;
}

float xPDEMImage::Sample(double x , double y , bool& bInRect , int* pNumOfNaN )
{
    if( m_Region.PtInRect( (float)x , (float)y ) == false)
    {
        bInRect = false;
        return -10000000.0f;
    }

    x -= m_Region.m_MinX;
    y -= m_Region.m_MinY;
    //计算出格子的位置
    int _GridW = m_LevelInfo.m_ResolutionX  - 1;
    int _GridL = m_LevelInfo.m_ResolutionY - 1;
    int   cell_x   = int(x/m_Region.m_Width  * _GridW);
    int   cell_y   = int(y/m_Region.m_Length * _GridL);

    double f_cell_x = (double)cell_x/_GridW * m_Region.m_Width ;
    double f_cell_y = (double)cell_y/_GridL * m_Region.m_Length;

    uint32 sample_x[2];
    uint32 sample_y[2];

    if(cell_x > f_cell_x) { sample_x[0] = cell_x - 1 ; sample_x[1] = cell_x    ; } 
    else                  { sample_x[0] = cell_x     ; sample_x[1] = cell_x + 1; }

    if(cell_y > f_cell_y) { sample_y[0] = cell_y - 1 ; sample_y[1] = cell_y    ; } 
    else                  { sample_y[0] = cell_y     ; sample_y[1] = cell_y + 1; }


    //检测NaN的个数
    if(pNumOfNaN)
    {
        int& nNaN = *pNumOfNaN;
        nNaN = 0;
        if( xIsNaN( m_Image[ sample_y[0]] [ sample_x[0] ] ) == true)  nNaN ++;
        if( xIsNaN( m_Image[ sample_y[0]] [ sample_x[1] ] ) == true)  nNaN ++;
        if( xIsNaN( m_Image[ sample_y[1]] [ sample_x[0] ] ) == true)  nNaN ++;
        if( xIsNaN( m_Image[ sample_y[1]] [ sample_x[1] ] ) == true)  nNaN ++;
    }


    // 取此像素的高度
    float factor_x = (float)sample_x[1] - (float)f_cell_x ;

    float v1 = m_Image[ sample_y[0]] [ sample_x[0] ]  *  factor_x +
        m_Image[ sample_y[0]] [ sample_x[1] ]  *  (1.0f - factor_x ) ;

    float v2 = m_Image[ sample_y[1]] [ sample_x[0] ]  *  factor_x +
        m_Image[ sample_y[1]] [ sample_x[1] ]  *  (1.0f - factor_x ) ;

    float factor_y = (float)sample_y[1] - (float)f_cell_y ;


    float sampleValue = v1 * factor_y + v2 * (1.0f - factor_y)  ;

    return sampleValue;
}

void xPDEMImage::Scale( xPDEMImage* pNewImage)
{
    // 原始（如：1000 X 800）算到1025 X 1025
    int OriginW = m_LevelInfo.m_ResolutionX ;
    int OriginH = m_LevelInfo.m_ResolutionY;
    int newWidth  = pNewImage->m_LevelInfo.m_ResolutionX;
    int newLength = pNewImage->m_LevelInfo.m_ResolutionY;

    float xfactor = ((float) (OriginW  - 1)) / newWidth;
    float yfactor = ((float) (OriginH - 1))  / newLength;

    // 生成高度信息
    uint32 px , py;
    uint32 sample_x[2];
    uint32 sample_y[2];

    float fDx = (float)m_Region.m_Width  / newWidth;
    float fDy = (float)m_Region.m_Length / newLength;

    for ( int y = 0 ; y < newLength ; ++y )
    {
        for ( int x = 0 ; x < newWidth ; ++x )
        {
            // 找到这个顶点在像素图上的像素位置
            float fx = x * xfactor;
            float fy = y * yfactor;
            px = (uint32)fx;     
            py = (uint32)fy;

            if(px > fx) { sample_x[0] = px - 1 ; sample_x[1] = px    ; } 
            else        { sample_x[0] = px     ; sample_x[1] = px + 1; }

            if(py > fy) { sample_y[0] = py - 1 ; sample_y[1] = py    ; } 
            else        { sample_y[0] = py     ; sample_y[1] = py + 1; }


            // 取此像素的高度
            float factor_x = sample_x[1] - fx ;

            float v1 = m_Image[ sample_y[0]] [ sample_x[0] ]  *  factor_x +
                m_Image[ sample_y[0]] [ sample_x[1] ]  *  (1.0f - factor_x ) ;

            float v2 = m_Image[ sample_y[1]] [ sample_x[0] ]  *  factor_x +
                m_Image[ sample_y[1]] [ sample_x[1] ]  *  (1.0f - factor_x ) ;

            float factor_y = sample_y[1] - fy ;


            pNewImage->m_Image[y][x]  = v1 * factor_y + v2 * (1.0f - factor_y)  ;//OriginDepth[py*newWidth + px]; 
            //m_Image[y][x].pos.x = fDx * x;
            //m_Image[y][x].pos.y = fDy * y;

        }
    }
}

//Tiff加载......................
bool xPDEMImage::loadTiff(const wchar_t* _fileName)
{
    //_demImg
    TIFF* objTiff = XTIFFOpenW(_fileName , "r");
    if(objTiff == NULL)
        return false;

    GTIF* objGisTiff = GTIFNew(objTiff);
    if(objGisTiff == NULL)
        return false;


    // 获取Normalize信息
    GTIFDefn GTIFDefN;
    GTIFGetDefn( objGisTiff, &GTIFDefN );

    uint32               tileWidth; 
    uint32               tileLength;
    xvec3                tiePointSize;

    // 分析所有Geo Tiff Tag
    TIFFGetField(objTiff, TIFFTAG_IMAGEWIDTH  , &m_LevelInfo.m_ResolutionX);
    TIFFGetField(objTiff, TIFFTAG_IMAGELENGTH , &m_LevelInfo.m_ResolutionY);
    TIFFGetField(objTiff, TIFFTAG_TILEWIDTH   , &tileWidth);
    TIFFGetField(objTiff, TIFFTAG_TILELENGTH  , &tileLength);
    //double tiepoints[6]={0,0,0,0,0,0};
    //double pixscale[3]={0,0,0};
    // Geospecific Tags
    short tiepointsize, pixscalesize;
    double* tiepoints;//[6];
    double* pixscale;//[3];
    TIFFGetField(objTiff, TIFFTAG_GEOTIEPOINTS,  &tiepointsize,&tiepoints);
    TIFFGetField(objTiff, TIFFTAG_GEOPIXELSCALE, &pixscalesize,&pixscale);

    m_LevelInfo.m_GridWidth  = (float)pixscale[0];	
    m_LevelInfo.m_GridLength = (float)pixscale[1];	
    m_LevelInfo.m_zScale     = (float)pixscale[2];
    if ( pixscale[2] < 0.0001f )
        m_LevelInfo.m_zScale = (float)pixscale[0];

    // Corner
    double MinX = 0.0;
    double MinY = m_LevelInfo.m_ResolutionY;
    double MaxX = m_LevelInfo.m_ResolutionX;
    double MaxY = 0.0;
    if( !GTIFImageToPCS( objGisTiff, &MinX, &MinY ) )
        return false;
    if( !GTIFImageToPCS( objGisTiff, &MaxX, &MaxY ) )
        return false;


    ///计算区域
    m_Region.m_MinX   = (float)MinX;
    m_Region.m_MinY   = (float)MinY;
    m_Region.m_Width  = (float)MaxX - (float)MinX;
    m_Region.m_Length = (float)MaxY - (float)MinY;
    float fDx = (float)m_Region.m_Width  / m_LevelInfo.m_ResolutionX;
    float fDy = (float)m_Region.m_Length / m_LevelInfo.m_ResolutionY;
    float minZ =  10000000.0f;
    float maxZ = -10000000.0f;
    m_Image.create(m_LevelInfo.m_ResolutionY , m_LevelInfo.m_ResolutionX);
    xPDEMImageBuf tempImage;
    tempImage.create(m_LevelInfo.m_ResolutionY , m_LevelInfo.m_ResolutionX);
    if ( TIFFIsTiled(objTiff) )
    {
        ///读出原始数据（可能是1000X800）
        tdata_t buf;
        buf = _TIFFmalloc(TIFFTileSize(objTiff));
        for (uint32 y = 0; y < m_LevelInfo.m_ResolutionY; y += tileLength)
        {
            for (uint32 x = 0; x < m_LevelInfo.m_ResolutionX; x += tileWidth)
            {
                TIFFReadTile(objTiff, buf, x, y, 0,0 );
                float * pBuf = (float*)buf;
                for ( uint32 ty = y; (ty < m_LevelInfo.m_ResolutionY)&&(ty<(y + tileLength)) ; ++ty )		
                {
                    for ( uint32 tx = x; (tx < m_LevelInfo.m_ResolutionX)&&(tx<(x + tileWidth)) ; ++tx )
                    {
                        //m_Image[ty][tx].pos.x = fDx * tx;
                        //m_Image[ty][tx].pos.y = fDy * ty;
                        float h = pBuf[(ty-y)* tileWidth + (tx-x)]; 
                        if(xIsNaN(h) == true || h > MAX_HEIGHT || h < -MAX_HEIGHT)
                        {
                            h = -MAX_HEIGHT;
                            tempImage[ty][tx]  =  h ;
                        }
                        else
                        {
                            tempImage[ty][tx]  =  h ;
                            if(h > maxZ) maxZ = h;
                            if(h < minZ) minZ = h;
                        }
                    }
                }
            }
        }

        FillBlankPos(tempImage);
        ////计算地表的起伏
        m_Region.m_MinZ   = minZ;
        m_Region.m_Height = maxZ - minZ;
        _TIFFfree(buf);
        TIFFClose(objTiff);
        GTIFFree(objGisTiff);
    }
    else
    {
        XEVOL_LOG(eXL_ERROR_FALT , "Not tiled tiff , not supported\n");
        assert(0);
    }
    tempImage.free();
    return true;
}

void xPDEMImage::FillBlankPos( xPDEMImageBuf& tempImage)
{
    for (uint32 ty = 0; ty < m_LevelInfo.m_ResolutionY; ty += 1)
    {
        for (uint32 tx = 0; tx < m_LevelInfo.m_ResolutionX; tx += 1)
        {
            float h =  tempImage[ty][tx];
            if(h == -MAX_HEIGHT)
            {
                h =  FindNearestPos(tx , ty , tempImage);
                m_Image[ty][tx] = h;
            }
            else
            {
                m_Image[ty][tx] = h;
            }
        }
    }
}

#define GET_POS(x , y , _Image) if( IsPosInImage(x , y , _Image) ) {   return _Image[y][x] ;}

float xPDEMImage::FindNearestPos(uint32 tx , uint32 ty , xPDEMImageBuf& _Image)
{
    int r = 1 ;
    for(r = 1 ; r < 400 ; r ++)
    {
        int x1 = tx - r;
        int x2 = tx + r;
        int y1 = ty - r;
        int y2 = ty + r;
        for(int _d = 0 ;  _d < r ; _d ++)
        {
            int _ty1 = ty + _d;
            int _ty2 = ty - _d;
            int _tx1 = tx + _d;
            int _tx2 = tx - _d;

            //XEVOL_LOG(eXR_LOG_LEVEL::eXL_DEBUG_HIGH , "r = %d , _d  = %d \n" , r , _d);
            //左右
            GET_POS(x1   , _ty1 , _Image) ;  //if( IsPosInImage(x1   , _ty1) ) {   return m_Image[_ty1] [x1   ];}
            GET_POS(x1   , _ty2 , _Image) ;  //if( IsPosInImage(x1   , _ty2) ) {   return m_Image[_ty2] [x1   ];}
            GET_POS(x2   , _ty1 , _Image) ;  //if( IsPosInImage(x2   , _ty1) ) {   return m_Image[_ty1] [x2   ];}
            GET_POS(x2   , _ty2 , _Image) ;  //if( IsPosInImage(x2   , _ty2) ) {   return m_Image[_ty2] [x2   ];}

            //上下
            GET_POS(_tx1 , y1   , _Image) ;  //if( IsPosInImage(_tx1 , y1  ) ) {   return m_Image[y1  ] [_tx1 ];}
            GET_POS(_tx2 , y1   , _Image) ;  //if( IsPosInImage(_tx2 , y1  ) ) {   return m_Image[y1  ] [_tx2 ];}
            GET_POS(_tx1 , y2   , _Image) ;  //if( IsPosInImage(_tx1 , y2  ) ) {   return m_Image[y2  ] [_tx1 ];}
            GET_POS(_tx2 , y2   , _Image) ;  //if( IsPosInImage(_tx2 , y2  ) ) {   return m_Image[y2  ] [_tx2 ];}  
        }
    }
    return -MAX_HEIGHT;
}

bool xPDEMImage::IsPosInImage(int x , int y, xPDEMImageBuf& _Image)
{
    if(x < 0 ) return false;
    if(x >= (int)m_LevelInfo.m_ResolutionX ) return false;
    if(y < 0 ) return false;
    if(y >= (int)m_LevelInfo.m_ResolutionY  ) return false;

    if(_Image[y][x] == -MAX_HEIGHT) return false;
    return true ;
}
//增加加载器
class xPDEMmageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xPDEMImage;
    }
};




static xPDEMmageLoader loader;
const bool bPDEMLoader     = xImageLib::registeImageLoader(L"demp" , &loader);

END_NAMESPACE_XEVOL3D
