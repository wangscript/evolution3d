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

#include <assert.h>
#include <string>
using namespace std;
#include "xcomdocImp.h"
#include "xcdStringHash.h"

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

BEGIN_NAMESPACE_XEVOL3D
static bool file_exist(const wchar_t* file_name)
{
	FILE* file = _xcd_wfopen_rb(file_name);
	if(file == NULL)
	{
		return false;
	}
	fclose(file);
	return true;
}

bool  xComDocument::open(IReadStream* pReader,xcd_rwmode /*mode*/,int /*_offset*/, bool load_to_mem)
{
	if(pReader == NULL)
		return false;
	if(m_state != xcds_notopened )
		return false;  

	return __open_for_read(pReader,0,load_to_mem);
}

bool  xComDocument::open(xcomdocstream* pStream,xcd_rwmode mode,int _offset , bool load_to_mem)
{
	xStreamContext* pDocStream = dynamic_cast<xStreamContext*>(pStream);

	if(pDocStream == NULL)
		return false;
	if(m_state != xcds_notopened )
		return false;


	if( (mode == xcdm_write || mode == xcdm_rw) 
		&& ( pDocStream->m_pStream->m_mode != xcdm_rw && pDocStream->m_pStream->m_mode != xcdm_write) )
		return false;
	//数据不在内存中，不能写 
	if(pDocStream->m_pStream->_data_in_mem() == false && (mode&xcdm_write) )
		return false;

	//数据在内存中
	if(pDocStream->m_pStream->_data_in_mem())
	{
		m_DataLoc = eDL_MEM;
		return open(pDocStream->get_buf(),pDocStream->data_len(), mode,false);
	}
	//数据不在内存中
	m_DataLoc = eDL_FILE;
	CXComDocReadStream* pReadStream = new CXComDocReadStream;
	m_pReadStream = pReadStream;
	pReadStream->set_stream(pDocStream);
	return __open_for_read(m_pReadStream,_offset,load_to_mem);
}

bool       xComDocument::open(istream* pistream,xcd_rwmode mode,int _offset, bool load_to_mem)
{
	if(pistream == NULL)
		return false;
	if(m_state != xcds_notopened )
		return false;
	m_DataLoc = eDL_FILE;
	CCPPIStream* pReadStream = new CCPPIStream;
	m_pReadStream = pReadStream;
	pReadStream->set_stream(pistream);
	m_mode = mode;
	return __open_for_read(m_pReadStream,_offset,load_to_mem);
}

bool       xComDocument::open(const wchar_t* file_name,xcd_rwmode mode,int _offset ,bool load_to_mem,bool not_create)
{

	if(file_name == NULL)
		return false;

	if(m_state != xcds_notopened )
		return false;
	m_DataLoc = eDL_FILE;


	m_mode = mode;
	if(file_exist(file_name) == false && mode != xcdm_read && not_create == false)
	{
		return create(file_name,256);
	}

	if(m_mode == xcdm_write || m_mode == xcdm_rw)
		load_to_mem = true;
	bool ret =  __open_for_read(file_name,_offset,load_to_mem);
	if(ret) wcscpy(m_file_name,file_name);
	return ret;
}

bool       xComDocument::open(const _xcd_int8* buf,  size_t buf_len,   xcd_rwmode mode,bool copyed_data)
{
	if(buf == NULL)
		return false;

	if(buf_len < 20)
		return false;

	if(m_state != xcds_notopened )
		return false;

	m_DataLoc = eDL_FILE;
	CMemIFStream* pReadStream = new CMemIFStream;
	pReadStream->set_buf(buf,(int)buf_len);
	m_pReadStream = pReadStream;
	m_mode = mode;
	m_file_name[0] = 0;
	return __open_for_read(m_pReadStream,0,copyed_data);
}

bool       xComDocument::create(const wchar_t* file_name,int hash_table_size)
{
	if(state() != xcds_notopened)
	{
		return false;
	}
	m_mode = xcdm_write;
	__init_file_header_data();
	__init_hash_table(hash_table_size);
	wcsncpy(m_file_name,file_name,128);
	m_state = xcds_ok;
	return true;
}

void xComDocument::discard()
{
	__reset_all_state();
}
bool xComDocument::save(ostream* pOstream)
{
	if(pOstream == NULL)
		return false;
	if(pOstream->fail())
		return false;

	__fit_hash_table();
	__recalculate_data_raw_size();

	return __write_data_to_fstream(*pOstream);
}

bool xComDocument::save(const wchar_t* file_name)
{
	if(file_name == 0)
		return true;

	wcsncpy(m_file_name , file_name , 128);

	ofstream file;
	
	_xcd_wfstream_bin(m_file_name,file);
	
	if(file.fail())
		return false;

	__fit_hash_table();
	__recalculate_data_raw_size();
	bool ret =  __write_data_to_fstream(file);
	file.close();
	return ret;
}

bool xComDocument::save()
{
	if(m_mode != xcdm_read)
	{
		__fit_hash_table();
		__recalculate_data_raw_size();
		__write_data();
		return true;
	}
	return false;
}

bool  xComDocument::save(IWriteBuffer* pOstream)
{
	__fit_hash_table();
	__recalculate_data_raw_size();

	__make_file_header();
	/**write the file header*/
	pOstream->write((_xcd_int8*)&m_FileHeader,sizeof(sCDFileHeader));

	/**write the hash table*/

	pOstream->write((_xcd_int8*)m_pHashTable,m_FileHeader.m_FileHeaderInfo.m_HashTableSize*sizeof(sCDHashTableItem));
	/**write the data entry items */
	size_t  n = m_DataEntrys.size();
	int startAddr = 0;
	for(size_t i = 0 ; i < n ; ++i)
	{
		DataEntry& entry = m_DataEntrys[i];
		entry.m_DataEntryItem.m_DataCompressedSize = 0;
		entry.m_DataEntryItem.m_StartAddr = startAddr;
		entry.m_pStream->_read_all_data_from_dataprovider_src_file();
		if(entry._is_compressed())
		{
			int streamLen,compressedSize;

			entry.m_pStream->get_compress_info(streamLen,compressedSize,entry.m_DataEntryItem.m_CompressedRate);
			entry.m_DataEntryItem.m_DataCompressedSize = streamLen;

			int dataSize = (int)m_DataEntrys[i].m_pStream->data_len();
			if(dataSize < 1.2 * streamLen && entry.m_DataEntryItem.m_CompressedRate != XCOMDOC_COMPRESS_ENCRYPT)
			{
				m_DataEntrys[i].m_DataEntryItem.m_CompressedRate = XCOMDOC_NOCOMPRESS;
				m_DataEntrys[i].m_DataEntryItem.m_DataCompressedSize = 0;
				m_DataEntrys[i].m_pStream->releaseCompressBuffer();
				startAddr += m_DataEntrys[i].m_DataEntryItem.m_DataSize;
			}
			else
			{
				startAddr += m_DataEntrys[i].m_DataEntryItem.m_DataCompressedSize;
			}
		}
		else
		{
			startAddr += m_DataEntrys[i].m_DataEntryItem.m_DataSize;
		}
		entry.m_pStream->_relase_all_data_from_dataprovider_src_file();
		pOstream->write((_xcd_int8*)& entry.m_DataEntryItem , sizeof(sCDDataEntryItem));

	}

	/**write the data*/
	for(size_t i = 0 ; i < n ; ++i)
	{
		DataEntry& entry = m_DataEntrys[i];
		if(xcd_is_dir(entry.m_DataEntryItem.m_DataType)==false)/*If it is a dir flag .it has no datas*/
		{
			entry.m_pStream->_read_all_data_from_dataprovider_src_file();
			if(entry._is_compressed())
			{
				int streamLen,compressedSize;
				_xcd_int8* buf = entry.m_pStream->get_compress_info(streamLen,compressedSize,entry.m_DataEntryItem.m_CompressedRate);
				pOstream->write(buf,streamLen);
			}
			else
			{
				pOstream->write(entry.m_pStream->get_buf(),(int)entry.m_pStream->buf_len());
			}

			entry.m_pStream->releaseCompressBuffer();    
			entry.m_pStream->_relase_all_data_from_dataprovider_src_file();
		}
	}
	return true;
}
void xComDocument::addRef()
{
	m_RefCount ++;
}

bool xComDocument::release()
{
	m_RefCount--;
	if(m_RefCount <= 0)
	{
		delete this;
		return true;
	}
	return false;
}

xComDocument::~xComDocument()
{
	if(m_RefCount <= 0)
		discard();
}

bool  xComDocument::close()
{
    save();
	__reset_all_state();
	if(m_RefCount <= 0)
	   return false;
	return true;
}
void xComDocument::free_buf()
{
	m_WriteBuffer.free();
}
bool xComDocument::failed()
{
	return m_state != xcds_ok;
}

xcd_state  xComDocument::state()
{
	return m_state;
}

size_t     xComDocument::length()
{
	assert(failed() == false);
	return __calc_file_len();
}
void  xComDocument::clear()
{
	__release_all_stream();
	__clear_hash_table();
}

bool xComDocument::delete_stream(const wchar_t* name)
{
	int i = __find_hash_index(name);
	if(i == -1)
	{
		return false;
	}
	DataEntry& entry = m_DataEntrys[  m_pHashTable[i].m_CDIndex ];
	__delete_data_stream(entry.m_pStream , i);
	return true;
}

xcomdocstream* xComDocument::open_stream(const wchar_t* name,bool not_create)
{
	assert(failed() == false);
	int i = __find_hash_index(name);
	if(i == -1)
	{
		if(not_create)
			return NULL;
		if(m_mode == xcdm_read)
			return NULL;

		XComDocStreamImp* pStream =  __insert_data_item(name,0,xcddt_common);
		xStreamContext* pContex = new xStreamContext ( pStream );
		return pContex;
	}
	DataEntry& entry = m_DataEntrys[  m_pHashTable[i].m_CDIndex ];
	xStreamContext* pContex = new xStreamContext ( entry.m_pStream );
	return pContex;
}

xcomdocstream* xComDocument::open_stream(xcd_data_type type , int type_index,bool /*not_create*/)
{
	assert(failed() == false);
	int count = 0 ;
	for(size_t i = 0 ; i < m_DataEntrys.size(); ++i)
	{
		DataEntry& entry = m_DataEntrys[  i ];
		if(entry.m_DataEntryItem.m_DataType == type)
		{
			if(count == type_index)
			{
				xStreamContext* pContex = new xStreamContext ( entry.m_pStream );
				return pContex;
			}
			++count;
		}
	}
	return NULL;
}

xcomdocstream* xComDocument::create_stream(const wchar_t* name,int compress_rate , xcd_data_type type)
{
	if(m_mode == xcdm_read)
		return NULL;

	assert(failed() == false);
	int i = __find_hash_index(name);
	if(i != -1)
		return NULL;
	XComDocStreamImp * pStreamImp = __insert_data_item(name,compress_rate,type);
	xStreamContext* pContex = new xStreamContext ( pStreamImp );
	return pContex;
}

xcomdocstream* xComDocument::create_stream_with_dir(const wchar_t* name,int compress_rate, xcd_data_type type)
{
	if(m_mode == xcdm_read)
		return NULL;

	assert(failed() == false);
	XComDocStreamImp * pStreamImp = __insert_data_item_with_dir(name,compress_rate,type);
	xStreamContext* pContex = new xStreamContext ( pStreamImp );
	return pContex;
}

void       xComDocument::close_stream(xcomdocstream* /*pStream*/)
{
	assert(failed() == false);
}

int        xComDocument::count_stream(xcd_data_type type)
{
	assert(failed() == false);
	int count = 0 ;
	for(size_t i = 0 ; i < m_DataEntrys.size(); ++i)
	{
		DataEntry& entry = m_DataEntrys[  i ];
		if(entry.m_DataEntryItem.m_DataType == type)
		{
			++count;
		}
	}
	return count;
}

void  xComDocument::set_buf(_xcd_int8* buf,size_t buf_len)
{
	assert(failed() == false);
	m_WriteBuffer.free();
	m_WriteBuffer.set_buf(buf,(int)buf_len);
}

_xcd_int8* xComDocument::get_buf(size_t* pbuf_len)
{
	assert(failed() == false);
	if(pbuf_len)
		*pbuf_len = m_WriteBuffer.data_len(); 
	return m_WriteBuffer.get_buf();
}

size_t xComDocument::buf_len()
{
	assert(failed() == false);
	return m_WriteBuffer.data_len();
}

bool xComDocument::add_file(const wchar_t* bas_dir,const wchar_t* file_name,int compressed_rate,xcd_data_type type)
{
	int i = 0;
	for(i = (int)wcslen(file_name); i>=0 ; i--)
	{
		if(file_name[i] == '/' || file_name[i] == '\\' )
		{
			break;
		}
	}
	wstring file_name_noext = &(file_name[i]);
	wstring item_name;
	size_t bas_dir_len = wcslen(bas_dir);
	if(bas_dir[bas_dir_len-1]=='/' || bas_dir[bas_dir_len-1]=='\\')
	{
		item_name = wstring(bas_dir) + file_name_noext;
	}
	else
	{
		item_name = wstring(bas_dir) + L"/"+ file_name_noext;
	}

	ifstream file ;
	_xcd_wfstream_bin(file_name,file);
	if(file.fail())
		return false;

	XComDocStreamImp* Stream =  __insert_data_item_with_dir(item_name.c_str(),compressed_rate,type);
	if(Stream == NULL)
		return false;

	while(!file.eof())
	{
		_xcd_int8 buf;
		file.read(&buf,1);
		Stream->write(&buf,1);
	}
	file.close();
	return true;
}

int            xComDocument::count_stream()
{
	return (int)m_DataEntrys.size();
}

const wchar_t*    xComDocument::stream_name(int index)
{
	if(index < 0 || index >= count_stream())
		return NULL;
	return (const wchar_t*) m_DataEntrys[index].m_DataEntryItem.getDataName();
}





END_NAMESPACE_XEVOL3D


