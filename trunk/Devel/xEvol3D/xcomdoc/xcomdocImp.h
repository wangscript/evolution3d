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

#ifndef __CDAPI_IMP_H_
#define __CDAPI_IMP_H_
#include "xcomdocbase.h"
#ifdef _USED_IN_EDITOR
#define __protected public
#define __private   public
#else
#define __protected protected
#define __private   private
#endif

#pragma warning(disable : 4996)
#include <stdio.h>
#include <vector>
#include <fstream>
using namespace std;

#include "xcomdoc_common.h"

#include "xcdReadBuffer.h"
#include "xcdWriteBuffer.h"
#include "zlibStream.h"

#pragma  pack(push,1)



BEGIN_NAMESPACE_XEVOL3D
class xcomdoc;
struct sCDFileID
{
	union 
	{
		int                   m_Magic;
		unsigned _xcd_int8    m_MagicChar[4];
	};
	int       m_Version;
	_xcd_int8 m_Key[64];
};


struct sCDFileHeaderInfo
{
	unsigned int m_FileLen;//long  4 - byte :  File Len (Low-Order 4 byte)
	unsigned int m_FileLenH;//long  4 - byte :  File Len (High-Order 4 byte)
	unsigned int m_FileFlag;//long  4 - byte :  FileFlag
	unsigned int m_HashTableSize;//long  4 - byte :  HashTable Size
	unsigned int m_CDTableSize;//long  4 - byte :  File Entry Table Size
	unsigned int m_Reserved[4];//long  16 byte :  Reserved Byte
};

struct sCDFileHeader
{
	sCDFileID         m_FileID;
	sCDFileHeaderInfo m_FileHeaderInfo;
};

/*
The struct above is the FileHeader Information 
*/


/*
Hash Table Item.
*/

enum
{
	HASHFLAG_COMMON = 1,
	HASHFLAG_NOT_USED = -1,
};

struct sCDHashTableItem
{
	/*用来计算HashIndex的索引，这是没有%nHashTableSize前的值 */
	int m_HashIndex;    /*long 4-byte : INDEX HASH*/
	int m_HashValue1;	 /*long 4-byte : ID  Hash Value 1 */ 
	int m_HashValue2;	 /*long 4-byte : ID  Hash Value 2 */
	int m_Flag;    	 /*long 4-byte : Flag */
	int m_CDIndex;	 /*long 4-byte : BlockIndex */
	sCDHashTableItem()
	{
		m_HashIndex = 0;    /*long 4-byte : INDEX HASH*/
		m_HashValue1 = 0;	 /*long 4-byte : ID  Hash Value 1 */ 
		m_HashValue2 = 0;	 /*long 4-byte : ID  Hash Value 2 */
		m_Flag = 0;    	 /*long 4-byte : Flag */
		m_CDIndex = 0;	 /*long 4-byte : BlockIndex */
	}

};

/*
Data Entry . for index the data entry.
*/
struct sCDDataEntryItem
{
	int            m_StartAddr;           /*long 4 - byte : FileStartAt */
	xcd_data_type  m_DataType;
	int            m_DataCompressedSize;	/*long 4 - byte : PackedSize */
	int            m_DataSize;	        /*long 4 - byte : FileSize-Without packed */

	int            m_CompressedRate;       /**/
	int            m_Reserved ;	        /*long 4 - byte : Reserved */
	unsigned short m_us2Name[128];
    unsigned int   m_ucs4DataName[128];

	wchar_t*  getDataName()
	{
		if(sizeof(wchar_t) == 4)
			return (wchar_t*)m_ucs4DataName;
		return  (wchar_t*)m_us2Name;
	}
    void ValidateName()
	{

	}
	void SetDataName(const wchar_t* name)
	{
		if(sizeof(wchar_t) == 4)
		{
			wcscpy( (wchar_t*)m_ucs4DataName,name);
		}
		else
		{
			wcscpy( (wchar_t*)m_us2Name,name);
		}
	}

	sCDDataEntryItem()
	{
		m_StartAddr = 0;           /*long 4 - byte : FileStartAt */
		m_DataType = xcddt_common;
		m_DataCompressedSize = 0;	/*long 4 - byte : PackedSize */
		m_DataSize = 0;	        /*long 4 - byte : FileSize-Without packed */

		m_CompressedRate = XCOMDOC_NOCOMPRESS;       /**/
		m_Reserved  = 0 ;	        /*long 4 - byte : Reserved */
		m_us2Name[0] = 0;	    /*char       128 - byte : File Name(Readable) */
		m_ucs4DataName[0] = 0;
	}
};





class xComDocument;
class XComDocStreamImp;

class xStreamContext : public xcomdocstream
{
	friend class XComDocStreamImp;
	friend class xComDocument;
	int            m_RefCount; 
public:
	XComDocStreamImp* m_pStream;
	int            m_readPos;
	int            m_writePos;
public:
	xStreamContext(XComDocStreamImp* pStreamImp);
	xcd_data_type   stream_type(void);
	void            addRef();
	void            release();
	size_t          read(_xcd_int8* buf,size_t byte_read);
	size_t          write(const _xcd_int8* buf,size_t byte_write);
	_xcd_int8*      get_buf();
	int             stream_tellr();
	int             stream_tellw();
	void            stream_seekr(xcd_seek_dir,int _offset);
	void            stream_seekw(xcd_seek_dir,int _offset);
	bool            is_open();
	const wchar_t*  stream_name();
	size_t          data_len();
	size_t          buf_len();
};

class XComDocStreamImp
{
__private:
	/*
	Stream's reading method:
	1: Read from a memory buffer . all data has alread in the buffer. the buffer in the reading proccess is read-only
	2: Read from a file.  the data have readed from the file to the buffer.
	3: Read from a file.  but the data not in the mem-buffer(in the FILE's)
	4: When open a File.  for write . the Data must be in the Memory.
	*/
	friend class xComDocument;
	/*
	Data in the file.
	*/
	bool              m_DataInMemBuffer;
	CReadBuffer       m_ReadBuffer;
	CWriteBuffer      m_WriteBuffer;
	CZLibReadBuffer   m_compressedReadBuffer;
	IWriteBuffer*     m_pWriteBuffer;
	xcd_rwmode           m_mode;
	xcd_data_type      m_data_type;
	sCDDataEntryItem* m_EntryItem;

	xComDocument*          m_pComDoc;


protected:
	//这里专门为读写文件而分配
	wstring           m_SrcFileName;
	bool              m_bUseSrcFile;
	/**Here comes the data for compress*/
protected:
	_xcd_int8*        m_compresseddata_buf;
	int               m_compressedSize;
	int               m_compressedStreamLen;

	_xcd_int8*        _compress(int& streamLen , int& compressedSize , int compressRate , int blockSize );
public:

	_xcd_int8*       get_compress_info(int& streamLen , int& compressedSize , int compressRate, int blockSize = 1024*64);
	void             releaseCompressBuffer();
public:
	XComDocStreamImp(xComDocument* pDoc,xcd_rwmode  mode,sCDDataEntryItem* pItem = NULL);
	XComDocStreamImp(xComDocument* pDoc,const _xcd_int8* buf,size_t buf_len,sCDDataEntryItem* pItem = NULL);
	~XComDocStreamImp();
public:
	void _set_src_file(const wchar_t* file_name , bool bUseSrcFile = true);
	void _read_all_data_from_dataprovider_src_file();
	void _relase_all_data_from_dataprovider_src_file();
	void _set_write_buf();
	void _set_write_buf(IWriteBuffer* pWriteBuffer);
	bool _data_in_mem();

	xcd_data_type stream_type();


	size_t           read(_xcd_int8* buf,size_t byte_read);
	size_t           write(const _xcd_int8* buf,size_t byte_write);
	_xcd_int8*       get_buf();
	int              stream_tellr();
	int              stream_tellw();
	void             stream_seekr(xcd_seek_dir,int _offset);
	void             stream_seekw(xcd_seek_dir,int _offset);
	bool             is_open();
	const wchar_t*   stream_name();
	size_t           data_len();
	size_t           buf_len();

__protected:
	void             _set_read_mode(bool data_inbuffer = false);
	size_t           _read_file(_xcd_int8* buf,size_t byte_read);
	size_t           _read_mem(_xcd_int8* buf,size_t byte_read);
	bool             _is_compressed();
	bool             _duplicate_data();
	void             _set_buf(const _xcd_int8* buf,size_t buf_len);
	void             _set_file(IReadStream*,int beginPos, int len);
	size_t           _read_all_data_from_file();

__protected:
	size_t           reserved(size_t len);
	size_t           capictiy();

};


enum eDataLoc
{
	eDL_FILE,
	eDL_MEM,
};



class xComDocument : public IComDocBase
{
__private:
	long   m_TotalSrcSize;
	bool   m_bInProccesPackage;
	struct DataEntry
	{
		~DataEntry()
		{
			m_pStream = NULL;
		}
		int                 m_hashTableIndex; //这个东西基本没什么用处
		XComDocStreamImp*   m_pStream;
		sCDDataEntryItem    m_DataEntryItem;

		bool _is_compressed()
		{
			return m_DataEntryItem.m_CompressedRate != XCOMDOC_NOCOMPRESS;
		}

	};
	/*
	File Image.
	*/
	sCDFileHeader              m_FileHeader;
	sCDHashTableItem*          m_pHashTable;
	typedef vector<DataEntry>  DataEntrys;
	DataEntrys                 m_DataEntrys;
	IReadStream*               m_pReadStream;
	CWriteBuffer               m_WriteBuffer;
__private:

public:
	int m_RefCount;

	void addRef();
	bool release();
	xComDocument();
	~xComDocument();
	bool       open(IReadStream* pReader,xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false);
	bool       open(istream* stream, xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false);
	bool       open(xcomdocstream* pDocStream,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false);
	bool       open(const wchar_t* file_name,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false,bool not_create = true);
	bool       open(const _xcd_int8* buf,  size_t buf_len,   xcd_rwmode mode,bool copyed_data = false);
	bool       create(const wchar_t* file_name,int hash_table_size=256);
	void       clear();
	bool       save(const wchar_t* file_name);
	bool       save();
	bool       save(ostream* pOstream);
	bool       save(IWriteBuffer* pOstream);
	bool       close();
	size_t     length();
	bool       failed();
	xcd_state   state();
	void       discard();
	bool       add_file(const wchar_t* bas_dir,const wchar_t* file_name,int compressed_rate = 0,xcd_data_type type = xcddt_common);
	/*
	Stream open method:
	1: by the name , name is unique.
	2: by type and type index . eg. many stream has the same type.
	*/
	int            count_stream();
	const wchar_t*    stream_name(int index);
	xcomdocstream* open_stream(const wchar_t* name,bool not_create = true);
	xcomdocstream* open_stream(xcd_data_type type , int type_index,bool not_create = true);
	xcomdocstream* create_stream(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common);
	xcomdocstream* create_stream_with_dir(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common);
	bool           delete_stream(const wchar_t* name);

	void           close_stream(xcomdocstream* pStream);
	int            count_stream(xcd_data_type type);
	void           set_buf(_xcd_int8* buf,size_t buf_len);
	_xcd_int8*     get_buf(size_t* pbuf_len);
	size_t         buf_len();
	void           free_buf();
__protected:
	//base operation
	void           __resize_hash_table(int hash_table_size);
	bool           __write_data();
	bool           __delete_data_stream(XComDocStreamImp* ,int hashIndex);
	XComDocStreamImp* __insert_data_item(const wchar_t* name, int compress_rate ,xcd_data_type type = xcddt_common);
	XComDocStreamImp* __insert_data_item_with_dir(const wchar_t* name,int compress_rate , xcd_data_type type = xcddt_common);
	void           __delete_date_item(XComDocStreamImp* pDataStream);
	void           __fit_hash_table();

	bool           __open_for_read(IReadStream* pStream , int _offset , bool load_to_mem);
	bool           __open_for_read(const wchar_t* file_name,int _offset,bool load_to_mem);
	//base helper operation
	void           __init_hash_table(int hash_table_size);

	void           __recalc_data_hash_index();
	void           __recalc_hash_data_index();
	void           __recalculate_data_raw_size();
	void           __clear_hash_table();
	void           __recalculate_hash_table();

	bool           __write_data_to_file();
	bool           __write_data_to_mem();
	bool           __write_data_to_fstream(ostream& stream);

	void           __alloc_buffer(size_t len = 0);


	size_t         __calc_file_len();
	void           __reset_all_state();
	void           __init_doc_data_member();
	void           __release_hash_table();
	void           __release_doc_data_member();
	void           __make_file_header();
	int            __find_hash_index(const wchar_t* name);

	void           __release_all_stream();
	bool           __check_file_header();
	void           __init_file_header_data();


__protected:
	/*?Control state*/
	xcd_rwmode        m_mode;
	eDataLoc       m_DataLoc;
	xcd_state       m_state;
	wchar_t         m_file_name[128];
};

END_NAMESPACE_XEVOL3D

#pragma  pack(pop)
#endif

