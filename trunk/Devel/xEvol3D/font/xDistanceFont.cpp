#include "xDistanceFont.h"
#include "../float16/float16.h"
#include "../BaseLib/xI18N.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include "../OperationSys/xOperationSys.h"
#include <windows.h>
#pragma warning (disable : 4244)

BEGIN_NAMESPACE_XEVOL3D

#define UPSAMPLES 8
static FT_Library g_FT2Lib;
float FK[UPSAMPLES][UPSAMPLES] ;
float FK_W = 0.0f;

void InitFilterKernel()
{
	float w = 0.0f;
	for(int y  = 0 ; y < UPSAMPLES ; y ++ )
	{
		for(int x  = 0 ; x < UPSAMPLES ; x ++ )
		{
			float v =  sqrt( (float) (x - UPSAMPLES/2)*(x - UPSAMPLES/2) +  (y - UPSAMPLES/2)*(y - UPSAMPLES/2)  );
			w += v;
			FK[y][x] = v;
		}
	}
	FK_W = w ;
}

template <typename TFLOAT , typename TFLOAT_O> void downSample(TFLOAT* buff_in , TFLOAT_O* buff_out , int w , int h)
{
     for(int y = 0 ; y < h ; y ++)
	 {
		 for(int x = 0 ; x < w ; x ++)
		 {
			    int sx = x * UPSAMPLES;
				int sy = y * UPSAMPLES;
				float _val = 0.0f;
                for(int ky = 0 ; ky < UPSAMPLES ; ky ++)
				{
					for(int kx = 0 ; kx < UPSAMPLES ; kx ++)
					{
						_val += buff_in[  sx + kx +  UPSAMPLES * w * (sy + ky) ] * FK[ky][kx];
					}
				}
				_val/= FK_W;
				buff_out[ x + w * y] = _val;
		 }
	 }
}

void saveBitMap(const wchar_t* pszFile, int w , int h ,unsigned char* _data)
{
	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = 8;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = h;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = w * h ;//图像数据大小
	bih.biWidth = w;//宽度
	bih.biClrUsed = 255;

	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(RGBQUAD) * 255 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	FILE *fp = _wfopen(pszFile, L"w+b");
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
	for(int i = 0 ; i < 255 ; i ++ )
	{
		RGBQUAD rgbQuad;
		rgbQuad.rgbReserved = 255;
		rgbQuad.rgbBlue = rgbQuad.rgbGreen = rgbQuad.rgbRed = i;
		fwrite(&rgbQuad, 1, sizeof(RGBQUAD), fp);//写入位图信息头
	}
    fwrite(_data , 1 , w * h , fp);
	fclose(fp);
}
xDistanceFontGenerator::xDistanceFontGenerator(int fontSize , int ttfSize , wchar_t begin , wchar_t last)
{
	m_Begin = begin;
	m_Last  = last;
	m_FT_Face = NULL;
	m_fontSize = fontSize;
	m_ttfSize  = ttfSize;
	m_ttfBuffer = new unsigned char[ttfSize * ttfSize * 2];
}

xDistanceFontGenerator::xDistanceFontGenerator(int fontSize , int ttfSize)
{
	m_Begin = 0;
	m_Last  = 65536 - 1;
	m_FT_Face = NULL;
	m_fontSize = fontSize;
	m_ttfSize  = ttfSize;
	m_ttfBuffer = new unsigned char[ttfSize * ttfSize * 2];
}

bool xDistanceFontGenerator::generate(const wchar_t* ttfFile , const wchar_t* _distFileName)
{
	InitFilterKernel();
	static bool sb_isInited = false;
	if(sb_isInited == false) 
	{
		if (FT_Init_FreeType( &g_FT2Lib )) 
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"FT_Init_FreeType failed\n");
			assert(0);
		}
	}
	sb_isInited = true;

	char strFontFile[1024] = {0};
	XEvol_UnicodeToLocale(ttfFile,strFontFile,1024);
	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face( g_FT2Lib, strFontFile, 0, (FT_Face*)&m_FT_Face )) 
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"FT_New_Face failed (there is probably a problem with your font file\n   <font = %s> \n" , strFontFile);
		return false;
	}
	FT_Select_Charmap( (FT_Face)m_FT_Face, FT_ENCODING_UNICODE);
	//大小要乘64.这是规定。照做就可以了。
	//FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
	FT_Set_Pixel_Sizes( (FT_Face)m_FT_Face,m_ttfSize, m_ttfSize);

	std::wstring distFileName = std::wstring(_distFileName) + L".dff";
	FILE* file = _wfopen(distFileName.c_str(),L"wb+");
	std::wstring descFileName = std::wstring(_distFileName) + L".dfd";
	FILE* descFile = _wfopen(descFileName.c_str() ,L"wb+");
	if(file == NULL || descFile == NULL)
		return false;

	int Offset = 0;
	int nChar  =  m_Last - m_Begin;
	fwrite(&nChar , sizeof(nChar) , 1 , descFile );
	printf("Bengin........\n");
	for(wchar_t ch = m_Begin ; ch < m_Last ; ch ++)
	{
		xDFFCharDesc desc;
		
		float _noffset = gen(ch , file , Offset , desc);
		desc.m_offset = Offset;
		fwrite(&desc , sizeof(desc) , 1 , descFile );
		Offset += _noffset;
		int _nCH = ch ;
		printf("Total %d char passed\n" , _nCH);
	}
	printf("Finish........\n");
	fclose(descFile);
	fclose(file);
	return true;
}

int  xDistanceFontGenerator::gen(wchar_t _char , FILE* file , int Offset , xDFFCharDesc& desc)
{
	FT_Face  pFT_Face = (FT_Face)m_FT_Face;
	FT_Int32 load_flags = FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT ;

	bool bAntilias = true;
	if(bAntilias) 
		load_flags |= ( FT_LOAD_TARGET_NORMAL| FT_LOAD_TARGET_LIGHT);
	else
		load_flags |= ( FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO );



	//得到字模
	FT_UInt  glyph_index;
	/* retrieve glyph index from character code */
	glyph_index = FT_Get_Char_Index( pFT_Face, _char );
	/* load glyph image into the slot (erase previous one) */
	int error = FT_Load_Glyph( pFT_Face, glyph_index, FT_LOAD_DEFAULT );

	error = FT_Render_Glyph( pFT_Face->glyph, FT_RENDER_MODE_NORMAL );

	memset(&desc , 0 , sizeof(desc) ) ;

	FT_Glyph glyph;
	if(FT_Get_Glyph( pFT_Face->glyph, &glyph ))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"FT_Load_Glyph failed\n");
		return false;
	}
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int width  =  bitmap.width;
	int height =  bitmap.rows;
	if(width == 0 || height == 0 )
	{
		FT_Done_Glyph(glyph);
		return 0;
	}

	pFT_Face->size->metrics.y_ppem;
	pFT_Face->glyph->metrics.horiAdvance;
	desc.m_adv_x = pFT_Face->glyph->advance.x / 64.0f;
	desc.m_adv_y = pFT_Face->size->metrics.y_ppem; //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	desc.m_left  = (float)bitmap_glyph->left;
	desc.m_top   = (float)bitmap_glyph->top;
	desc.m_w     = width;
	desc.m_h     = height;

	if(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned char _vl =  0;
				if(x < bitmap.width && y < bitmap.rows) 
				{
					_vl =  (x>=bitmap.width || y>=bitmap.rows) ? 0 : bitmap.buffer[x + bitmap.width*y];
				}
				if(_vl >= 0x40) 
					_vl = 255; 
				else 
					_vl = 0;
				m_ttfBuffer[(1*x + _y * width)+0] = _vl;			
			}
		}
	}
	else if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO) //单色图
	{
		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned char _vl =  0;
				if(x < bitmap.width && y < bitmap.rows) 
				{
					_vl =  ((bitmap.buffer[(y * bitmap.pitch) + x / 8] << (x % 8)) & 0x80) ? 0xFFFFFFFF : 0x00000000;
				}
				if(_vl > 0x7f) _vl = 255; else _vl = 0;
				m_ttfBuffer[(1*x + _y * width)+0] = _vl;
			}
		}
	}

    FT_Done_Glyph(glyph);

	wchar_t name[]=L" .bmp";
	name[0] = _char;
	ds_wstring fullName = _ABSPATH(name);
	//saveBitMap(fullName.c_str() , width , height , m_ttfBuffer);
	//从Freetype里取出来了。
	//现在开始降采样和计算.
	float advx = desc.m_adv_x * (m_fontSize/(float)m_ttfSize); 
	float advy = desc.m_adv_y * (m_fontSize/(float)m_ttfSize); 

	desc.m_adv_x *= (m_fontSize/(float)m_ttfSize);  
	desc.m_adv_y *= (m_fontSize/(float)m_ttfSize);  
	desc.m_left  *= (m_fontSize/(float)m_ttfSize);    
	desc.m_top   *= (m_fontSize/(float)m_ttfSize);   
	desc.m_w     *= (m_fontSize/(float)m_ttfSize);     
	desc.m_h     *= (m_fontSize/(float)m_ttfSize);


	if(desc.m_adv_x > 0 && advx == 0) desc.m_adv_x = 1; else  desc.m_adv_x  = advx;
	if(desc.m_adv_y > 0 && advy == 0) desc.m_adv_y = 1; else  desc.m_adv_y	= advy;
	if(desc.m_w == 0) desc.m_w = 1;
	if(desc.m_h == 0) desc.m_h = 1;

	int ow = desc.m_w * UPSAMPLES;
	int oh = desc.m_h * UPSAMPLES;


	float* tBuffer = new float[ow * oh];
	float dw = ((float)width) /ow;
	float dh = ((float)height)/oh;
	for(int y = 0 ; y < oh ; y ++)
	{
		for(int x = 0 ; x < ow  ; x ++)
		{
			int bx = (float)(x * dw + dw/2.0f);
			int by = (float)(y * dh + dh/2.0f);
			unsigned char* cl = getPixel(bx , by , width , height , m_ttfBuffer);
			float d = 0;
			if( *cl > 200)
			{
				d = GetNearest(bx , by , 0x00 , width , height , m_ttfBuffer);
			}
			else
			{
				d = -1.0f * GetNearest(bx , by , 0x00 , width , height , m_ttfBuffer);
			}
			tBuffer[y * ow + x] = d ;
		}
	}

	float _min = 0.0f;
	float _max = 0.0f;
	for(int y = 0 ; y < oh ; y ++)
	{
		for(int x = 0 ; x < ow  ; x ++)
		{
			if(tBuffer[y * ow + x] > _max)
				_max =  tBuffer[y * ow + x];
			if(tBuffer[y * ow + x] < _min)
				_min =  tBuffer[y * ow + x];
		}
	}

	//归一化
	for(int y = 0 ; y < oh ; y ++)
	{
		for(int x = 0 ; x < ow  ; x ++)
		{
			float v = 0.0f;
			if( tBuffer[y * ow + x] > 0 )
			{
				v = tBuffer[y * ow + x] / _max;
				tBuffer[y * ow + x] = v ;
			}
			else
			{
				v = -tBuffer[y * ow + x] / _min;
				tBuffer[y * ow + x] = v ;
			}	
		}
	}

	//开始输出
	int nByte = sizeof(float16) * desc.m_w * desc.m_h;
	float16* outBuffer = new float16[desc.m_w * desc.m_h];
    downSample(tBuffer , outBuffer , desc.m_w , desc.m_h);
	fwrite(outBuffer , 1 , nByte , file);

	
	delete [] tBuffer;
	delete [] outBuffer;
	return nByte;
}


float xDistanceFontGenerator::GetNearest(int x , int y , unsigned char v , int w , int h , unsigned char* _data)
{
	float n = 4096;
	for(int r = 1 ; r < 1024 ; r ++)
	{

		//左边
		int rx = x - r;
		for(int ry = y - r ; ry < y + r ; ry ++)
		{
			unsigned char* cl = getPixel(rx , ry , w , h , _data);
			if(cl && *cl == v)
			{
				float dist = sqrt( float( (rx - x) * (rx - x)  + (ry - y) * (ry - y) ) );
				if(dist < n) n = dist;
			}
		}
		//右边
		rx = x + r;
		for(int ry = y - r ; ry < y + r ; ry ++)
		{
			unsigned char* cl = getPixel(rx , ry , w , h , _data);
			if(cl && *cl == v)
			{
				float dist = sqrt( float( (rx - x) * (rx - x)  + (ry - y) * (ry - y) ) );
				if(dist < n) n = dist;
			}
		}

		//上边
		int ry = y - r;
		for(int rx = x - r ; rx < x + r ; rx ++)
		{
			unsigned char* cl = getPixel(rx , ry , w , h , _data);
			if(cl && *cl == v)
			{
				float dist = sqrt( float( (rx - x) * (rx - x)  + (ry - y) * (ry - y) ) );
				if(dist < n) n = dist;
			}
		}

		//下边
		ry = y + r;
		for(int rx = x - r ; rx < x + r ; rx ++)
		{
			unsigned char* cl = getPixel(rx , ry , w , h , _data);
			if(cl && *cl  == v)
			{
				float dist = sqrt( float( (rx - x) * (rx - x)  + (ry - y) * (ry - y) ) );
				if(dist < n) n = dist;
			}
		}
		if(n < 4096)
		{
			return n;
		}
	}
	return n;
}

unsigned char* xDistanceFontGenerator::getPixel(int x , int y  , int w , int h , unsigned char* _data)
{
	if(x >=w ) return NULL;
	if(x < 0)    return NULL;
	if(y >=h ) return NULL;
	if(y < 0   ) return NULL;
	return  _data + x + y * w;
}
END_NAMESPACE_XEVOL3D
