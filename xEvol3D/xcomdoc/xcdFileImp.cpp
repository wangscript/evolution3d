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
#include <assert.h>
#include "xcomdocImp.h"
#include "xcdStringHash.h"
#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

/*
Here is our all operation to the file .

1 : insert a data item : 
get a free hashTable position -> 
(if data entry > hashTable , recreate the hashtable and then recalculate the hashTable)
create a DataEntry.

2 : resize hashTalbe :
recreate the hashtable and then recalculate the hashTable.
if( you give a hashtable size < DataEntry's size . rescale the hashTable size.

3 : delete a data item  :
remove the dataItem;
remove the hashTable item used by this data item;
recaculate the hashTable Item's m_CDIndex value.

4 : Fit the size of the hashTalbe.
if hashTable is > 2 * DataEntry's size let the hashTable size 
recreate the hashtable and then recalculate the hashTable

5: Write the file to disk.
if needed  Fit the size of the hashTalbe.
recacluate the DataEntry's m_StartAddr value.
write all data.

*/
BEGIN_NAMESPACE_XEVOL3D

class xComDocumentCreator : public IComDocBaseCreator
{
public:
	xComDocumentCreator()
	{
		xComDocBaseCreatorMgr::singleton()->registCreator(this);
	}

	~xComDocumentCreator()
	{
		xComDocBaseCreatorMgr::singleton()->unregistCreator(this);
	}

	IComDocBase* createInstace()
	{
		return new xComDocument();
	}
	bool         support(const wchar_t* _ext)
	{
		if(_ext == NULL)
			return false;
		std::wstring strExt = _ext;
		if(strExt == L"xrm" || strExt == L"XRM")
			return true ;
		if(strExt == L"xcd" || strExt == L"XCD")
			return true ;
		if(strExt == L"xpkg" || strExt == L"XPKG")
			return true ;
		return false;
	}
};
static xComDocumentCreator g_ComDocCreator;

bool xComDocument::__check_file_header()
{
	if(strcmp((const char*)m_FileHeader.m_FileID.m_MagicChar,"UFF") == 0)
		return true;
	return false;
}

void xComDocument::__init_file_header_data()
{
	memset(&m_FileHeader,0,sizeof(m_FileHeader));
	m_FileHeader.m_FileID.m_Version = 0x00010000;
	m_FileHeader.m_FileHeaderInfo.m_FileFlag = NO_ENCRYPT_TABLE;//不加密表头
	m_FileHeader.m_FileID.m_MagicChar[0] = 'U';
	m_FileHeader.m_FileID.m_MagicChar[1] = 'F';
	m_FileHeader.m_FileID.m_MagicChar[2] = 'F';
	m_FileHeader.m_FileID.m_MagicChar[3] = '\0';
}
void xComDocument::__clear_hash_table()
{
	/*
	Clear the whole hash table.
	*/
	assert(m_pHashTable);
	int nHashTable = m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
	for(int i = 0 ; i < nHashTable ; ++ i)
	{
		m_pHashTable[i].m_CDIndex    = -1;
		m_pHashTable[i].m_Flag       = HASHFLAG_NOT_USED;//Not used
		m_pHashTable[i].m_HashIndex  = -1;
		m_pHashTable[i].m_HashValue1 = -1;
		m_pHashTable[i].m_HashValue2 = -1;
	}  
}

void xComDocument::__init_hash_table(int hash_table_size)
{
	/*
	Alloc the hash talbe buffer;
	*/
	if(m_pHashTable != NULL)
	{
		delete [] m_pHashTable; 
		m_pHashTable = NULL;
	}
	m_pHashTable = new sCDHashTableItem [hash_table_size];
	m_FileHeader.m_FileHeaderInfo.m_HashTableSize = hash_table_size;
	__clear_hash_table();
}
void xComDocument::__make_file_header()
{
	m_FileHeader.m_FileHeaderInfo.m_CDTableSize = (int)m_DataEntrys.size();
}
int  xComDocument::__find_hash_index(const wchar_t* name)
{
	HashValue hashValue;
	xHashCodeTable::singleton().string_hash(name,hashValue);

	//calculate the item index in the hash table;
	int hashTableIndex = hashValue.indexHash % m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
	for(unsigned int k = 0 ; k < m_FileHeader.m_FileHeaderInfo.m_HashTableSize ; ++k)
	{
		int index = (hashTableIndex + k)%m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
		if(m_pHashTable[index].m_Flag != HASHFLAG_NOT_USED )
		{
			if(m_pHashTable[index].m_HashIndex  == hashValue.indexHash &&
				m_pHashTable[index].m_HashValue1 == hashValue.valueA && 
				m_pHashTable[index].m_HashValue2 == hashValue.valueB )
			{
				/*DataEntry& entry = */m_DataEntrys[ m_pHashTable[index].m_CDIndex ];
				return index;
			}
		}
	} 
	return -1;
}

void xComDocument::__recalculate_hash_table()
{
	HashValue hashValue;
	size_t nDataItem = m_DataEntrys.size();
	for(size_t iDataItem = 0 ; iDataItem < nDataItem ; ++ iDataItem)
	{
		DataEntry& entry = m_DataEntrys[iDataItem];

		const wchar_t* name = entry.m_DataEntryItem.getDataName();
		xHashCodeTable::singleton().string_hash(name,hashValue);
		//calculate the item index in the hash table;
		int hashTableIndex = hashValue.indexHash % m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
		for(unsigned int k = 0 ; k < m_FileHeader.m_FileHeaderInfo.m_HashTableSize ; ++k)
		{
			int index = (hashTableIndex + k)%m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
			if(m_pHashTable[index].m_Flag == HASHFLAG_NOT_USED )
			{
				m_pHashTable[index].m_HashIndex  = hashValue.indexHash ;
				m_pHashTable[index].m_HashValue1 = hashValue.valueA;
				m_pHashTable[index].m_HashValue2 = hashValue.valueB;
				m_pHashTable[index].m_CDIndex    = (int)iDataItem;
				m_pHashTable[index].m_Flag       = HASHFLAG_COMMON;
				entry.m_hashTableIndex = index;
				break;
			}
		}   
	}
}

void xComDocument::__fit_hash_table()
{
	if((size_t)m_FileHeader.m_FileHeaderInfo.m_HashTableSize > m_DataEntrys.size() * 2)
		return ;
	__resize_hash_table(m_FileHeader.m_FileHeaderInfo.m_HashTableSize*2);
}

void xComDocument::__resize_hash_table(int hash_table_size)
{
	__init_hash_table(hash_table_size);
	__recalculate_hash_table();
}

void xComDocument::__recalc_data_hash_index()
{
	size_t nHashItem = m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
	for(size_t iHashItem = 0 ; iHashItem < nHashItem ; ++ iHashItem)
	{
		sCDHashTableItem& hashItem = m_pHashTable[iHashItem];
		if(hashItem.m_Flag != HASHFLAG_NOT_USED)
		{
			m_DataEntrys[hashItem.m_CDIndex].m_hashTableIndex = (int)iHashItem;
		}        
	}     
}

void xComDocument::__recalc_hash_data_index()
{
	size_t nDataItem = m_DataEntrys.size();
	for(size_t iDataItem = 0 ; iDataItem < nDataItem ; ++ iDataItem)
	{
		DataEntry& entry = m_DataEntrys[iDataItem];
		int n = entry.m_hashTableIndex;
		m_pHashTable[n].m_CDIndex = (int)iDataItem;
	}
}

void xComDocument::__recalculate_data_raw_size()
{
	//int beg = 0;
	size_t nDataItem = m_DataEntrys.size();
	for(size_t iDataItem = 0 ; iDataItem < nDataItem ; ++ iDataItem)
	{
		DataEntry& entry = m_DataEntrys[iDataItem];
		if(xcd_is_dir(entry.m_DataEntryItem.m_DataType))
			entry.m_DataEntryItem.m_DataSize = 0;
		else
			entry.m_DataEntryItem.m_DataSize =  (int)entry.m_pStream->data_len();
	}   
}

XComDocStreamImp* xComDocument::__insert_data_item(const wchar_t* name, int compress_rate ,xcd_data_type type)
{
	size_t nSize = m_DataEntrys.size();
	if(nSize >= m_FileHeader.m_FileHeaderInfo.m_HashTableSize)
	{
		__resize_hash_table((int)m_DataEntrys.size()*2);
	}
	HashValue hashValue;
	xHashCodeTable::singleton().string_hash(name,hashValue);
	//calculate the item index in the hash table;
	int hashTableIndex = hashValue.indexHash % m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
	for(unsigned int i = 0 ; i < m_FileHeader.m_FileHeaderInfo.m_HashTableSize ; ++i)
	{
		int index = (hashTableIndex + i)%m_FileHeader.m_FileHeaderInfo.m_HashTableSize;
		if(m_pHashTable[index].m_Flag == HASHFLAG_NOT_USED )
		{
			int iDataItem = (int) m_DataEntrys.size();
			DataEntry newentry;
			m_DataEntrys.push_back(newentry);

			DataEntry& entry = m_DataEntrys[m_DataEntrys.size() - 1];

			m_pHashTable[index].m_HashIndex  = hashValue.indexHash ;
			m_pHashTable[index].m_HashValue1 = hashValue.valueA;
			m_pHashTable[index].m_HashValue2 = hashValue.valueB;
			m_pHashTable[index].m_CDIndex    = iDataItem;
			m_pHashTable[index].m_Flag       = HASHFLAG_COMMON;
			entry.m_hashTableIndex = index;
			entry.m_pStream = new XComDocStreamImp(this,m_mode, &entry.m_DataEntryItem);
			entry.m_pStream->m_data_type = type;
			entry.m_pStream->reserved(256);
			entry.m_pStream->_set_read_mode(true);
			entry.m_DataEntryItem.m_DataCompressedSize = 0;
			entry.m_DataEntryItem.SetDataName( name );
			entry.m_DataEntryItem.m_DataCompressedSize = 0;
			entry.m_DataEntryItem.m_CompressedRate = compress_rate;

			if(xcd_is_dir(type))
				entry.m_DataEntryItem.m_CompressedRate = XCOMDOC_NOCOMPRESS;
			entry.m_DataEntryItem.m_DataSize = 0;
			entry.m_DataEntryItem.m_DataType      = type;
			entry.m_DataEntryItem.m_Reserved =0;
			entry.m_DataEntryItem.m_StartAddr = 0;

			return entry.m_pStream;

		}
	}

	assert(false);
	return NULL;
}

bool xComDocument::__delete_data_stream(XComDocStreamImp* pStream, int hashIndex)
{
	DataEntrys::iterator i = m_DataEntrys.begin();
	size_t nDataItem = m_DataEntrys.size();
	for(size_t iDataItem = 0 ; iDataItem < nDataItem ; ++ iDataItem,++i)
	{
		DataEntry& entry = m_DataEntrys[iDataItem];
		if(entry.m_pStream == pStream)
		{


			/*remove the hash table  entry point*/
			m_pHashTable[hashIndex].m_CDIndex    = -1;
			m_pHashTable[hashIndex].m_Flag       = HASHFLAG_NOT_USED;//Not used
			m_pHashTable[hashIndex].m_HashIndex  = -1;
			m_pHashTable[hashIndex].m_HashValue1 = -1;
			m_pHashTable[hashIndex].m_HashValue2 = -1;

			m_DataEntrys.erase(i);
			delete pStream;

			__recalc_hash_data_index();
			return true;

		}
	}   
	return false;
}  

size_t xComDocument::__calc_file_len()
{
	size_t len = sizeof(sCDFileHeader);
	len += m_FileHeader.m_FileHeaderInfo.m_HashTableSize * sizeof(sCDHashTableItem);

	size_t  n = m_DataEntrys.size();
	len += n * sizeof(sCDDataEntryItem);
	for(size_t i = 0 ; i < n ; ++i)
	{

		DataEntry & entry = m_DataEntrys[i];
		if(entry._is_compressed())
		{
			int streamLen,compressedSize,dataSize;
			dataSize = (int)entry.m_pStream->data_len();
			entry.m_pStream->get_compress_info(streamLen,compressedSize,m_DataEntrys[i].m_DataEntryItem.m_CompressedRate);
			entry.m_pStream->releaseCompressBuffer();
			if(dataSize < 1.2 * streamLen &&  m_DataEntrys[i].m_DataEntryItem.m_CompressedRate != XCOMDOC_COMPRESS_ENCRYPT)
			{
				entry.m_DataEntryItem.m_CompressedRate = XCOMDOC_NOCOMPRESS;
				entry.m_DataEntryItem.m_DataCompressedSize = 0;
				len += dataSize;
			}
			else
			{
				len += streamLen;
			}

		}
		else
		{
			len += m_DataEntrys[i].m_pStream->data_len();
		}

	}
	return len;
}

void xComDocument::__alloc_buffer(size_t len)
{
	m_WriteBuffer.free();
	if(len == 0)
		len = __calc_file_len();
	m_WriteBuffer.reserver((int)len);
}

bool xComDocument::__write_data()
{
	if(m_mode == xcdm_read)
		return false;


	if(m_file_name[0] != 0)
		return  __write_data_to_file();
	else
		return  __write_data_to_mem();

}

bool xComDocument::__write_data_to_fstream(ostream& file)
{
	unsigned int proccessedSize = 0;
	__make_file_header();
	/**write the file header*/
	file.write((_xcd_int8*)&m_FileHeader,sizeof(sCDFileHeader));

	/**write the hash table*/

	file.write((_xcd_int8*)m_pHashTable,m_FileHeader.m_FileHeaderInfo.m_HashTableSize*sizeof(sCDHashTableItem));
	/**write the data entry items */
	size_t  n = m_DataEntrys.size();
	int startAddr = 0;
	for(size_t i = 0 ; i < n ; ++i)
	{
		DataEntry& entry = m_DataEntrys[i];
		entry.m_DataEntryItem.m_DataCompressedSize = 0;
		entry.m_DataEntryItem.m_StartAddr = startAddr;
		entry.m_pStream->_read_all_data_from_dataprovider_src_file();
		if(m_bInProccesPackage && xcd_is_dir(entry.m_DataEntryItem.m_DataType)==false )
		{
			proccessedSize += entry.m_DataEntryItem.m_DataSize;
			int percent = int(  float(proccessedSize)/m_TotalSrcSize * 100) ;
			//XEVOL_LOG(3,"%d%% 正在处理文件头 file=< %s\n",percent,entry.m_pStream->m_SrcFileName.c_str() );
		}
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
		entry.m_DataEntryItem.ValidateName();
		file.write((_xcd_int8*)& entry.m_DataEntryItem , sizeof(sCDDataEntryItem));

	}

	proccessedSize = 0;
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
				file.write(buf,streamLen);
			}
			else
			{
				file.write(entry.m_pStream->get_buf(),(int)entry.m_pStream->buf_len());
			}

			entry.m_pStream->releaseCompressBuffer();  
			if(m_bInProccesPackage )
			{
				proccessedSize += entry.m_DataEntryItem.m_DataSize;
				int percent = int(  float(proccessedSize)/m_TotalSrcSize * 100) ;
				//XEVOL_LOG(3, "%d%%: 正在写入 file=< %s \n",percent,entry.m_pStream->m_SrcFileName.c_str() );
			}

			entry.m_pStream->_relase_all_data_from_dataprovider_src_file();
		}
	}
	return true;
}
bool xComDocument::__write_data_to_file()
{
	//没有文件名
	if(m_file_name[0] == 0)
		return true;

	ofstream file;
#ifdef _WIN32
	file.open(m_file_name,ios::binary);
#else
	char mbcFileName[512];    
	_xcd_wcsTombs(m_file_name,mbcFileName,512);
	file.open(mbcFileName,ios::binary);
#endif
	if(file.fail())
		return false;

	bool ret  = __write_data_to_fstream(file);

	file.close();
	return ret;
}

bool xComDocument::__write_data_to_mem()
{
	size_t len = __calc_file_len();
	if(m_WriteBuffer.capictiy() < (int)len)
	{
		m_WriteBuffer.free();
		m_WriteBuffer.reserver((int)len);
	}

	XComDocStreamImp stream(this,xcdm_write,NULL);
	stream._set_buf(m_WriteBuffer.get_buf(),m_WriteBuffer.capictiy());
	stream.write( (const _xcd_int8*)&m_FileHeader,sizeof(sCDFileHeader));

	/**write the hash table*/
	stream.write((const _xcd_int8*)m_pHashTable,m_FileHeader.m_FileHeaderInfo.m_HashTableSize*sizeof(sCDHashTableItem));

	/**write the data entry items */
	size_t  n = m_DataEntrys.size();
	int startAddr = 0;
	for(size_t i = 0 ; i < n ; ++i)
	{

		m_DataEntrys[i].m_DataEntryItem.m_DataCompressedSize = 0;
		m_DataEntrys[i].m_pStream->_read_all_data_from_dataprovider_src_file();
		if(m_DataEntrys[i]._is_compressed())
		{
			int streamLen,compressedSize;
			m_DataEntrys[i].m_pStream->get_compress_info(streamLen,compressedSize,m_DataEntrys[i].m_DataEntryItem.m_CompressedRate);
			m_DataEntrys[i].m_DataEntryItem.m_DataCompressedSize = streamLen;

			int dataSize = (int)m_DataEntrys[i].m_pStream->data_len();
			if(dataSize < 1.2 * streamLen && m_DataEntrys[i].m_DataEntryItem.m_CompressedRate != XCOMDOC_COMPRESS_ENCRYPT)
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

		m_DataEntrys[i].m_pStream->_relase_all_data_from_dataprovider_src_file();
		
		m_DataEntrys[i].m_DataEntryItem.ValidateName();
		stream.write( (const _xcd_int8*)&m_DataEntrys[i].m_DataEntryItem ,sizeof(sCDDataEntryItem));
	}

	/**write the data*/
	for(size_t i = 0 ; i < n ; ++i)
	{
		if(xcd_is_dir(m_DataEntrys[i].m_DataEntryItem.m_DataType) == false)/*If it is a dir flag .it has no datas*/
		{
			m_DataEntrys[i].m_pStream->_read_all_data_from_dataprovider_src_file();
			if(m_DataEntrys[i]._is_compressed())
			{
				int streamLen,compressedSize;
				_xcd_int8* buf = m_DataEntrys[i].m_pStream->get_compress_info(streamLen,compressedSize,m_DataEntrys[i].m_DataEntryItem.m_CompressedRate);
				stream.write( (const _xcd_int8*)buf,streamLen);
			}
			else
			{
				stream.write( (const _xcd_int8*)m_DataEntrys[i].m_pStream->get_buf(),m_DataEntrys[i].m_pStream->buf_len());
			}
			m_DataEntrys[i].m_pStream->releaseCompressBuffer();
			m_DataEntrys[i].m_pStream->_relase_all_data_from_dataprovider_src_file();
		}   
	}

	return true;
}

void xComDocument::__release_all_stream()
{
	size_t  n = m_DataEntrys.size();
	for(size_t i = 0 ; i < n ; ++i)
	{
		m_DataEntrys[i].m_pStream->releaseCompressBuffer();
		delete m_DataEntrys[i].m_pStream;
	}
	m_DataEntrys.clear();
}

void xComDocument::__release_hash_table()
{
	if(m_pHashTable)
	{
		delete [] m_pHashTable; 
		m_pHashTable = NULL; 
	}
}
void xComDocument::__init_doc_data_member()
{
	m_state = xcds_notopened;
	m_mode  = xcdm_read;
	m_pHashTable = NULL;
	m_pReadStream = NULL;
	m_file_name[0]=0;
	m_DataLoc = eDL_FILE;
	//m_FileStream = NULL;
	m_WriteBuffer.free();
	__init_file_header_data();
}
xComDocument::xComDocument()
{
	m_bInProccesPackage = false;
	m_RefCount = 1;
	__init_doc_data_member();
	__init_file_header_data();
}

void xComDocument::__release_doc_data_member()
{
	memset( &m_FileHeader,0,sizeof(sCDFileHeader));
	__release_hash_table();
	__release_all_stream();
	m_WriteBuffer.free();
	m_DataEntrys.clear();

	if(m_pReadStream)
	{
		m_pReadStream->close();
		delete m_pReadStream;
	}
	//if(m_FileStream.is_open())
	//    m_FileStream.close();
}

void xComDocument::__reset_all_state()
{
	__release_doc_data_member();
	__init_doc_data_member();    
}

int   XComDoc_Encrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len);
int   XComDoc_DeEncrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len);

bool xComDocument::__open_for_read(IStreamIO* pStream , int _offset , bool load_to_mem)
{
	int old_pos =  (int)pStream->tell();
	m_FileHeader.m_FileID.m_Magic = 0;
	pStream->seek(_offset,ios::beg);
	pStream->read((_xcd_int8*)&m_FileHeader,sizeof(sCDFileHeader));
	if(pStream->eof()) return false;
	//fread(&m_FileHeader,1,sizeof(sCDFileHeader),m_FileStream);
	if(__check_file_header() == false)
	{
		pStream->seek(old_pos,ios_base::beg);
		return false;
	}

	bool isEncrypot = m_FileHeader.m_FileHeaderInfo.m_FileFlag & NORAML_ENCRYPT_TABLE; 

	/**read the hash table*/
	m_pHashTable = new sCDHashTableItem[m_FileHeader.m_FileHeaderInfo.m_HashTableSize];

	pStream->read((_xcd_int8*)m_pHashTable,m_FileHeader.m_FileHeaderInfo.m_HashTableSize*sizeof(sCDHashTableItem));

	//如果是加密的话，就解密HashTable
	//if(isEncrypot)
	//	XComDoc_DeEncrypot((_xcd_int8*)m_pHashTable,(_xcd_int8*)m_pHashTable,m_FileHeader.m_FileHeaderInfo.m_HashTableSize*sizeof(sCDHashTableItem));

	if(pStream->eof()) return false;

	/**read data entry table*/
	size_t  n = m_FileHeader.m_FileHeaderInfo.m_CDTableSize;
	size_t begPos = sizeof(sCDFileHeader);
	begPos += m_FileHeader.m_FileHeaderInfo.m_HashTableSize * sizeof(sCDHashTableItem);
	begPos += n * sizeof(sCDDataEntryItem);

	m_DataEntrys.reserve(n);
	m_DataEntrys.resize(n);
	for(size_t i = 0 ; i < n ; ++i)
	{
		DataEntry& entry = m_DataEntrys[i];
		pStream->read((_xcd_int8*)&entry.m_DataEntryItem ,sizeof(sCDDataEntryItem));
		//如果是加密的话，就解密EntryItem;
		//if(isEncrypot)
		//	XComDoc_DeEncrypot((_xcd_int8*)&entry.m_DataEntryItem,(_xcd_int8*)&entry.m_DataEntryItem,sizeof(sCDDataEntryItem));
       
		entry.m_DataEntryItem.ValidateName();
		entry.m_pStream = new XComDocStreamImp(this,m_mode,&entry.m_DataEntryItem);
		entry.m_pStream->m_data_type = entry.m_DataEntryItem.m_DataType;
		entry.m_pStream->_set_file(pStream, entry.m_DataEntryItem.m_StartAddr+(int)begPos + _offset ,entry.m_DataEntryItem.m_DataSize);
		entry.m_pStream->_set_read_mode(false);
		if(load_to_mem)
		{
			entry.m_pStream->_read_all_data_from_file();
		}
	}

	if(m_mode != xcdm_read)//如果不是只读的话，需要重新计算Hash Table
		__recalc_data_hash_index();
	m_state = xcds_ok;
	m_WriteBuffer.free();
	pStream->seek(old_pos,ios::beg);
	if(load_to_mem)
	{
		m_DataLoc = eDL_MEM;
	}
	return true;
}

bool xComDocument::__open_for_read(const wchar_t* file_name,int _offset,bool load_to_mem)
{
	CCPPIFStream* pStream = new CCPPIFStream;
	m_pReadStream = pStream;
	if(pStream->open_stream(file_name) == false)
	{
		m_pReadStream = NULL;
		delete pStream;
		return false;
	}
	return __open_for_read(m_pReadStream,_offset,load_to_mem);
}

XComDocStreamImp* xComDocument::__insert_data_item_with_dir(const wchar_t* name,int compress_rate , xcd_data_type type)
{
	wchar_t dir_name[128];
	for(int i = 0 ; i < (int) wcslen(name) ; ++i)
	{
		if(name[i] == '/' || name[i] == '\\')
		{
			wcsncpy(dir_name,name,128);
			dir_name[i] = 0;
			int hashIndex = __find_hash_index(dir_name);
			if(hashIndex !=-1)
			{                 
				int dataIndex = m_pHashTable[hashIndex].m_CDIndex;
				if(xcd_is_dir(m_DataEntrys[dataIndex].m_DataEntryItem.m_DataType) == false)
					return NULL;
			}
			else
			{
				if(NULL ==  __insert_data_item(dir_name,compress_rate,xcddt_dir_flag))
					return NULL;
			}

		}
	}
	return  __insert_data_item(name,compress_rate,type);
}

END_NAMESPACE_XEVOL3D


