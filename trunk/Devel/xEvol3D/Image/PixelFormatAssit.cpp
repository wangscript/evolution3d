#include "PixelFormatAssit.h"
#include <map>
#include <string>
#include "../BaseLib/xStringHash.h"
using namespace std;

BEGIN_NAMESPACE_XEVOL3D

class xPixelFormatAssitImp : public xPixelFormatAssit
{
   typedef map<unsigned int , ePIXEL_FORMAT>    STR2FMTMAP;
   typedef map<ePIXEL_FORMAT , wstring>         FMT2STRMAP;
   STR2FMTMAP                m_mapStr2FMT;
   FMT2STRMAP                m_mapFMT2Str;
   bool                      __addPixelFormat(ePIXEL_FORMAT format , const wchar_t* name);
public:
	xPixelFormatAssitImp();
	ePIXEL_FORMAT             GetPixelFormat(const wchar_t* pixelFormat);
	const wchar_t*            GetPixelFormatName(ePIXEL_FORMAT format);
	int                       GetPixelFormatBBP(ePIXEL_FORMAT format);
};

xPixelFormatAssit* xPixelFormatAssit::singleton()
{
    static xPixelFormatAssitImp pfa;
	return & pfa;
}

bool xPixelFormatAssitImp::__addPixelFormat(ePIXEL_FORMAT format , const wchar_t* name)
{
	unsigned int _id = xStringHash( name);
	m_mapFMT2Str.insert( FMT2STRMAP::value_type(format , name) );
	m_mapStr2FMT.insert( STR2FMTMAP::value_type(_id , format) );
	return true;
}

xPixelFormatAssitImp::xPixelFormatAssitImp()
{
	    __addPixelFormat(PIXELFORMAT_R8G8B8           , L"R8G8B8");
	    __addPixelFormat(PIXELFORMAT_R8G8B8A8         , L"R8G8B8A8");
		__addPixelFormat(PIXELFORMAT_R8G8B8A8S        , L"R8G8B8A8S");
		__addPixelFormat(PIXELFORMAT_R8G8B8A8SINT     , L"R8G8B8A8SINT");
		__addPixelFormat(PIXELFORMAT_R8G8B8A8UINT     , L"R8G8B8A8UINT"); 

	    __addPixelFormat(PIXELFORMAT_R8G8B8X8         , L"R8G8B8X8");
	    __addPixelFormat(PIXELFORMAT_B8G8R8           , L"B8G8R8");
	    __addPixelFormat(PIXELFORMAT_B8G8R8A8         , L"B8G8R8A8");
	    __addPixelFormat(PIXELFORMAT_B8G8R8X8         , L"B8G8R8X8");
													 
		//16bit interger texture format				 	
	    __addPixelFormat(PIXELFORMAT_R16G16B16U        , L"R16G16B16U");
		__addPixelFormat(PIXELFORMAT_R16G16B16A16U     , L"R16G16B16A16U");
		__addPixelFormat(PIXELFORMAT_R16G16B16A16UINT  , L"R16G16B16A16UINT");
													  
													 
		//Signed									 
	    __addPixelFormat(PIXELFORMAT_R16G16B16S       , L"R16G16B16S");
	    __addPixelFormat(PIXELFORMAT_R16G16B16A16S    , L"R16G16B16A16S");
		__addPixelFormat(PIXELFORMAT_R16G16B16A16SINT , L"R16G16B16A16SINT");
													  

			
		//16bit float16 float texture format		 	  	
	    __addPixelFormat(PIXELFORMAT_R16G16B16F       ,  L"R16G16B16F");
	    __addPixelFormat(PIXELFORMAT_R16G16B16A16F    ,  L"R16G16B16A16F");
													  

													  
		//32bit float texture format				 	 
	    __addPixelFormat(PIXELFORMAT_R32G32B32F       ,  L"R32G32B32F");
	    __addPixelFormat(PIXELFORMAT_R32G32B32A32F    ,  L"R32G32B32A32F");

		__addPixelFormat(PIXELFORMAT_R32G32B32S       ,  L"R32G32B32S");
		__addPixelFormat(PIXELFORMAT_R32G32B32A32S    ,  L"R32G32B32A32S");	

		__addPixelFormat(PIXELFORMAT_R32G32B32U       ,  L"R32G32B32U");
		__addPixelFormat(PIXELFORMAT_R32G32B32A32U    ,  L"R32G32B32A32U");


		//None 3/4 channel									   	
	    __addPixelFormat(PIXELFORMAT_ALPHA8           ,  L"ALPHA8");
													  
	    __addPixelFormat(PIXELFORMAT_LUMINANCE8       ,  L"LUMINANCE8");
	    __addPixelFormat(PIXELFORMAT_LUMINANCE8ALPHA8 ,  L"LUMINANCE8ALPHA8");
	    __addPixelFormat(PIXELFORMAT_LUMINANCE16      ,  L"LUMINANCE16");
	    __addPixelFormat(PIXELFORMAT_LUMINANCE16F     ,  L"LUMINANCE16F");
	    __addPixelFormat(PIXELFORMAT_LUMINANCE32F     ,  L"LUMINANCE32F");
													  
													  
	    __addPixelFormat(PIXELFORMAT_R16F             ,  L"R16F");
	    __addPixelFormat(PIXELFORMAT_R16G16F          ,  L"R16G16F");

		__addPixelFormat(PIXELFORMAT_R16U             ,  L"R16U");
		__addPixelFormat(PIXELFORMAT_R16G16U          ,  L"R16G16U");
		__addPixelFormat(PIXELFORMAT_R16G16UINT       ,  L"R16G16UINT");
		__addPixelFormat(PIXELFORMAT_R16S             ,  L"R16S");
		__addPixelFormat(PIXELFORMAT_R16G16S          ,  L"R16G16S");
		__addPixelFormat(PIXELFORMAT_R16G16SINT       ,  L"R16G16SINT");

	    __addPixelFormat(PIXELFORMAT_R32F             ,  L"R32F");
	    __addPixelFormat(PIXELFORMAT_R32G32F          ,  L"R32G32F");

		__addPixelFormat(PIXELFORMAT_R32S             ,  L"R32S");
		__addPixelFormat(PIXELFORMAT_R32G32S          ,  L"R32G32S");
		__addPixelFormat(PIXELFORMAT_R32U             ,  L"R32U");
		__addPixelFormat(PIXELFORMAT_R32G32U          ,  L"R32G32U");
		//Depth											
	    __addPixelFormat(PIXELFORMAT_DEPTH16          ,  L"DEPTH16");
	    __addPixelFormat(PIXELFORMAT_DEPTH24          ,  L"DEPTH24");
	    __addPixelFormat(PIXELFORMAT_DEPTH32          ,  L"DEPTH32");


		//不可以操作的
	    __addPixelFormat(PIXELFORMAT_DXT1             ,  L"DXT1");
	    __addPixelFormat(PIXELFORMAT_DXT2             ,  L"DXT2");
	    __addPixelFormat(PIXELFORMAT_DXT3             ,  L"DXT3");
	    __addPixelFormat(PIXELFORMAT_DXT4             ,  L"DXT4");
	    __addPixelFormat(PIXELFORMAT_DXT5             ,  L"DXT5");

		__addPixelFormat(PIXELFORMAT_YV12             ,  L"YV12");
		__addPixelFormat(PIXELFORMAT_YVY2             ,  L"YUY2");
		__addPixelFormat(PIXELFORMAT_AYUV             ,  L"AYUV");



		//================
	    __addPixelFormat(PIXELFORMAT_None, L"None");
}


ePIXEL_FORMAT xPixelFormatAssitImp::GetPixelFormat(const wchar_t* pixelFormat)
{
	unsigned int _id = xStringHash( pixelFormat );
	STR2FMTMAP::iterator pos = m_mapStr2FMT.find( _id );
	if(pos == m_mapStr2FMT.end() )
		return PIXELFORMAT_None;
	return pos->second;
}

const wchar_t*   xPixelFormatAssitImp::GetPixelFormatName(ePIXEL_FORMAT format)
{
	FMT2STRMAP::iterator pos = m_mapFMT2Str.find( format );
	if(pos == m_mapFMT2Str.end() )
		return L"None";
	return pos->second.c_str();
}

int xPixelFormatAssitImp::GetPixelFormatBBP(ePIXEL_FORMAT format)
{
	return format&0x0f;
}

END_NAMESPACE_XEVOL3D
