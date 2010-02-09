/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef  __XEVOL_BASE_API_H__
#define  __XEVOL_BASE_API_H__



#ifndef _xcd_int8
#define _xcd_int8 char
#endif



#include <istream>
#include <ostream>

#include <vector>
#include <iostream>
#include <ios>
#include "../BaseLib/xBaseObject.h"

//#ifndef BEGIN_NAMESPACE_XEVOL3D
//#define BEGIN_NAMESPACE_XEVOL3D namespace XEvol3D {
//#define END_NAMESPACE_XEVOL3D }
//#endif


BEGIN_NAMESPACE_XEVOL3D

enum xcd_rwmode
{
    xcdm_read       = 0x01,
	xcdm_write      = 0x02,
	xcdm_rw         = 0x03
};


enum eENCRYPT_TABLE_STATE
{
	NO_ENCRYPT_TABLE     = 0x0000,
    NORAML_ENCRYPT_TABLE = 0x0001,
};

enum xcd_state
{
    xcds_notopened,
    xcds_ok,
};

typedef std::ios_base::seek_dir xcd_seek_dir;
#define xcdsd_beg  std::ios_base::beg
#define xcdsd_end  std::ios_base::end
#define xcdsd_cur  std::ios_base::cur

enum eCompressLevel
{
	
	XCOMDOC_COMPRESS_ENCRYPT = -100,
	XCOMDOC_COMPRESS_DEFAULT = -1,
	XCOMDOC_NOCOMPRESS       = 0,
	XCOMDOC_COMPRESS_FAST    = 1,
	XCOMDOC_COMPRESS_BLEND   = 5,
    XCOMDOC_COMPRESS_BEST    = 10,
	XCOMDOC_COMPRESS_ULTRA   = 15
};

enum xcd_data_type
{
	xcddt_common            = 0x00000000,//CommonType not set any flag , so it is a common flag , 
    xcddt_dir_flag          = 0x80000000,//dir flag, if this bit is set , the data stream will contains nothing but only a dir. 
	xcddt_forced32bit       = 0xffffffff,
};

typedef void*    HXCDFILE ;
typedef void*    HXCDSTREAM;

//class IStreamIO
//{
//public:
//    virtual int    read(_xcd_int8* buf, int byte_read) = 0 ;
//    virtual void   seekg(int _offset, ios::seekdir dir) = 0;
//    virtual int    tellg() = 0;
//    virtual void   close() = 0 ;
//    virtual bool   eof() = 0;
//};

class IWriteBuffer
{
public:
    virtual void        rewind() = 0;
    virtual bool        duplicate_data() = 0;
    virtual bool        reserver(int len) = 0;
    virtual void        free() = 0;
    virtual int         write(const _xcd_int8* buf,int byte_write) = 0;
    virtual int         read(_xcd_int8* buf, int byte_read) = 0;
    virtual void        seekg(int _offset, std::ios_base::seekdir dir) = 0;

    virtual void        seekw(int _offset, std::ios_base::seekdir dir) = 0;
    virtual int         tellg() = 0;
    virtual int         tellw() = 0;
    virtual _xcd_int8*  get_buf() = 0;
    virtual void        set_buf(const _xcd_int8* buf,int buf_len) = 0;
    virtual int         data_len() = 0;
    virtual bool        set_data_real_size(int real_size) = 0;
    virtual int         capictiy() = 0;
};



class xcomdocstream
{
	int m_writePos;
	int m_readPos;
public:
    xcomdocstream(){}
public:
    template <typename T> size_t write(const T& data)
    {
        return write( (const _xcd_int8*) &data,sizeof(T));
    }
	virtual void Release() = 0;

    template <typename T> size_t write(const T* data, size_t nObject)
    {
        return write( (const _xcd_int8*)data,sizeof(T)*nObject);
    }

    template <typename T> size_t read(T& data)
    {
        return read( (_xcd_int8*) &data,sizeof(T));
    }

    template <typename T> size_t read(T* data, size_t nObject)
    {
        return read( (_xcd_int8*)data,sizeof(T)*nObject);
    }

    virtual xcd_data_type stream_type() = 0; 
    virtual size_t           read(_xcd_int8* buf,size_t byte_read) = 0;
    virtual size_t           write(const _xcd_int8* buf,size_t byte_write) = 0;
    virtual _xcd_int8*       get_buf() = 0;
    virtual int              stream_tellr() = 0;
    virtual int              stream_tellw() = 0;
    virtual void             stream_seekr(xcd_seek_dir,int _offset) = 0;
    virtual void             stream_seekw(xcd_seek_dir,int _offset) = 0;
    virtual size_t           data_len() = 0;
    virtual bool             is_open() = 0 ;
    virtual const wchar_t*   stream_name() = 0;

public:
};

class _XEVOL_BASE_API_ xcomdoc
{
    HXCDFILE    m_hFIle;
public:
    xcomdoc();
	xcomdoc(const xcomdoc& doc);
	xcomdoc& operator = (const xcomdoc& rhv);
    ~xcomdoc();
public:
    bool                open(IStreamIO*     pReader   , xcd_rwmode mode = xcdm_read , int _offset = 0 , bool load_to_mem = false , const wchar_t* _type = NULL);
    bool                open(std::istream*    stream    , xcd_rwmode mode = xcdm_read , int _offset = 0 , bool load_to_mem = false , const wchar_t* _type = NULL);
    bool                open(xcomdocstream*   pStream   , xcd_rwmode mode             , int _offset = 0 , bool load_to_mem = false , const wchar_t* _type = NULL);

	bool                open_dir(const wchar_t*   file_name , bool not_create = false) ;
    bool                open(const wchar_t*   file_name , xcd_rwmode mode             , int _offset = 0 , bool load_to_mem = false,bool not_create = true , const wchar_t* _type = NULL);
    bool                open(const _xcd_int8* buf       , size_t buf_len,   xcd_rwmode mode,bool copyed_data = false, const wchar_t* _type = NULL);
    bool                create(const wchar_t* file_name , int hash_table_size=256, const wchar_t* _type = NULL);
    bool                close();
	void                Release();

	const wchar_t*      name();
    bool                save(const wchar_t* file_name);
    bool                save();
    bool                save(std::ostream* pOstream);
    bool                save(IWriteBuffer* pOstream);



    void                discard();
    size_t              length();
    bool                failed();
    xcd_state           state();
    void                clear();
   
    void                set_buf(_xcd_int8* buf,size_t buf_len);
    _xcd_int8*          get_buf(size_t* pbuf_len);
    size_t              buf_len();
    void                free_buf();

    const wchar_t*      stream_name(int index);
    int                 count_stream();
    xcomdocstream*      open_stream(const wchar_t* name,bool not_create = true);
    xcomdocstream*      open_stream(xcd_data_type type , int type_index,bool not_create = true);
    bool                delete_stream(const wchar_t* name);
    xcomdocstream*      create_stream(const wchar_t* name,xcd_data_type type = xcddt_common,int compressed_rate = 0);
    xcomdocstream*      create_stream_with_dir(const wchar_t* name, xcd_data_type type = xcddt_common,int compressed_rate = 0);
    void                close_stream(xcomdocstream* pStream);
    int                 count_stream(xcd_data_type type);

#ifdef _BUILD_ANSI_SUPPORT_
	bool                open(const char*   file_name , xcd_rwmode mode             , int _offset = 0 , bool load_to_mem = false,bool not_create = true);
	bool                save(const char* file_name);
	xcomdocstream*      open_stream(const char* name,bool not_create = true);
	bool                delete_stream(const char* name);
	xcomdocstream*      create_stream(const char* name,xcd_data_type type = xcddt_common,int compressed_rate = 0);
	xcomdocstream*      create_stream_with_dir(const char* name, xcd_data_type type = xcddt_common,int compressed_rate = 0);
#endif

	static xcomdoc*     newInstance(const wchar_t* strResName, unsigned long arg);
	static void         deleteInstance(xcomdoc* pRes);

/**Util helper function*/
    void                add_dir(const wchar_t* bas_dir,int compressed_rate = 0);
    void                extrac_to_dir(const wchar_t* bas_dir);
    bool                add_file(const wchar_t* bas_dir,const wchar_t* file_name,xcd_data_type type = xcddt_common,int compressed_rate = 0);
};

END_NAMESPACE_XEVOL3D
#endif

