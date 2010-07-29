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
#include "xImage.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <ctype.h>
#include <assert.h>
#include <map>
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D
namespace xImageLib
{
    typedef std::map<std::wstring , xImageLoader*     > ImageCreatorMap;
    typedef std::map<std::wstring , xImageInfoLoader* > ImageInfoLoaderMap;

    ImageCreatorMap* ms_ImageFactory = NULL;
    ImageCreatorMap* ms_SpecImageFactory = NULL;
    ImageInfoLoaderMap* ms_ImageInfoLoaderFactory = NULL;

    bool         registeImageLoader(const wchar_t* _ext , xImageLoader* imageCreateFunc)
    {
        if(ms_ImageFactory == NULL)
        {
            ms_ImageFactory = new ImageCreatorMap;
        }

        ImageCreatorMap& creatorMap = *ms_ImageFactory;

        ImageCreatorMap::iterator pos = creatorMap.find(_ext);
        if(pos != creatorMap.end())
            return false;

        creatorMap[ _ext ] = imageCreateFunc;
        return true;
        
    }

    bool         registeSpecialImageLoader(const wchar_t* _ext , xImageLoader* imageCreateFunc)
    {
        if(ms_SpecImageFactory == NULL)
        {
            ms_SpecImageFactory = new ImageCreatorMap;
        }
        ImageCreatorMap& creatorMap = *ms_SpecImageFactory;
        ImageCreatorMap::iterator pos = creatorMap.find(_ext);

        if(pos != creatorMap.end())
            return false;

        creatorMap[ _ext ] = imageCreateFunc;
        return true;
    }


    bool         registeImageInfoLoader(const wchar_t* _ext , xImageInfoLoader* imageCreateFunc)
    {
        if(ms_ImageInfoLoaderFactory == NULL)
        {
            ms_ImageInfoLoaderFactory = new ImageInfoLoaderMap;
        }
        ImageInfoLoaderMap& creatorMap = *ms_ImageInfoLoaderFactory;
        ImageInfoLoaderMap::iterator pos = creatorMap.find(_ext);
        if(pos != creatorMap.end())
            return false;

        creatorMap[ _ext ] = imageCreateFunc;
        return true;
    }

	static xImage* __createImageByeExtname(const wchar_t* filename )
	{
        if(ms_ImageFactory == NULL)
            return NULL;

		std::wstring nameExt = xImage::GetFileExt(filename);
		const wchar_t* name_ext = nameExt.c_str();

        ImageCreatorMap& creatorMap = *ms_ImageFactory;
        ImageCreatorMap::iterator pos = creatorMap.find(name_ext);
        if(pos == creatorMap.end()) pos = creatorMap.find(L"any");

        if(pos == creatorMap.end())
            return NULL;

        xImageLoader*  funcCreate = pos->second;
        return funcCreate->create(filename);
	}

	xImage*         create(IMAGE_TYPE type)
	{
		assert(0);//return new xILImage;
		return NULL;
	}

	bool            getSize(const wchar_t* wcsFileName , xImageSize& _size)
	{
        if(ms_ImageInfoLoaderFactory == NULL)
            return NULL;

        std::wstring nameExt = xImage::GetFileExt(wcsFileName);
        const wchar_t* name_ext = nameExt.c_str();

        ImageInfoLoaderMap& creatorMap = *ms_ImageInfoLoaderFactory;
        ImageInfoLoaderMap::iterator pos = creatorMap.find(name_ext);
        if(pos == creatorMap.end()) pos = creatorMap.find(L"any");

        if(pos == creatorMap.end())
            return NULL;
        xImageInfoLoader*  funcCreate = pos->second;
        return funcCreate->getSize(wcsFileName , _size);
	}

	xImage*      createSpecialImage(const wchar_t* filename)
	{
        if(ms_SpecImageFactory == NULL)
            return NULL;

        std::wstring nameExt = xImage::GetFileExt(filename);
        const wchar_t* name_ext = nameExt.c_str();

        ImageCreatorMap& creatorMap = *ms_ImageFactory;
        ImageCreatorMap::iterator pos = creatorMap.find(name_ext);
        if(pos == creatorMap.end()) pos = creatorMap.find(L"any");

        if(pos == creatorMap.end())
            return NULL;
        xImageLoader*  funcCreate = pos->second;
        return funcCreate->create(filename);
	}

	xImage*      create(const wchar_t* fileName)
	{
		return __createImageByeExtname( fileName );
	}


    IStreamIO* createImageStream(const wchar_t* fileName , const int8* mem_buf , int buf_len )
    {
        if(mem_buf == NULL)
            return xFileSystem::singleton()->loadFile(fileName);
        return xMemStream::createInstance((void*)mem_buf , buf_len);
    }


	xImage*         load(IMAGE_TYPE type ,xImageSize size)
	{
		xImage* pImage = create(type);
		if(false == pImage->load(size) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage*         load(IMAGE_TYPE type , int w , int h , ePIXEL_FORMAT fmt)
	{
		xImage* pImage = create(type);
		if(false == pImage->load(w,h,fmt) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage* load(IMAGE_TYPE type ,const wchar_t* fileName, IMAGE_ORIGIN img_origin, unsigned int _arg)
	{
		xImage* pImage = __createImageByeExtname(fileName);
		if(false == pImage->load(fileName , img_origin) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage*         load(IMAGE_TYPE type ,const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin, unsigned int _arg)
	{
		xImage* pImage = __createImageByeExtname(fileName);
		if(false == pImage->load(fileName , mem_buf , buf_len , img_origin) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage*         load(IMAGE_TYPE type ,const wchar_t* fileName, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin , unsigned int _arg)
	{
		xImage* pImage = __createImageByeExtname(fileName);
		if(false == pImage->load(fileName , mem_buf , buf_len , format, img_origin) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	void            kill(xImage* pImage)
	{
		delete pImage;
	}
}

END_NAMESPACE_XEVOL3D
