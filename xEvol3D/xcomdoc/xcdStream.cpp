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

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

BEGIN_NAMESPACE_XEVOL3D
bool   XComDocStreamImp::is_open()
{  
	return true;
}

const wchar_t* XComDocStreamImp::stream_name()
{
	return m_EntryItem->getDataName();
}

void  XComDocStreamImp::_set_src_file(const wchar_t* file_name , bool bUseSrcFile)
{
	m_bUseSrcFile = bUseSrcFile;
	if(m_bUseSrcFile)
	{
		m_SrcFileName = file_name ; 
	}
	else
	{
		m_SrcFileName = L"";
	}
}

XComDocStreamImp::XComDocStreamImp(xComDocument* pDoc,const _xcd_int8* buf,size_t buf_len,CDDataEntryItemImp* pItem)
{
	m_pComDoc = pDoc;
	m_EntryItem = pItem;
	m_compresseddata_buf = NULL;
	m_pWriteBuffer = &m_WriteBuffer;
	m_DataInMemBuffer = true;
	m_bUseSrcFile =false;
	_set_buf(buf,buf_len);
}


XComDocStreamImp::~XComDocStreamImp()
{
	releaseCompressBuffer();
}

void  XComDocStreamImp::_read_all_data_from_dataprovider_src_file()
{
	if(m_bUseSrcFile)
	{
		FILE* fStream = _xcd_wfopen_rb(m_SrcFileName.c_str());
		if(fStream != NULL)
		{
			fseek(fStream,0,SEEK_END);
			int len = ftell(fStream);
			fseek(fStream,0,SEEK_SET);
			m_pWriteBuffer->reserver(len);
			stream_seekw(xcdsd_beg, 0 );
			fread(m_pWriteBuffer->get_buf(),len,1,fStream);
			m_pWriteBuffer->seekw(len,ios_base::beg);
			m_pWriteBuffer->set_data_real_size(len);
			fclose(fStream);
		}
	}
}

void  XComDocStreamImp::_relase_all_data_from_dataprovider_src_file()
{
	if(m_bUseSrcFile)
	{
		m_pWriteBuffer->seekw(0,ios_base::beg);
		m_pWriteBuffer->seekg(0,ios_base::beg);
		m_pWriteBuffer->free();
	}
}

void  XComDocStreamImp::_set_write_buf()
{
	m_pWriteBuffer = & m_WriteBuffer;
}

void  XComDocStreamImp::_set_write_buf(IWriteBuffer* pWriteBuffer)
{
	m_pWriteBuffer = pWriteBuffer;
}

bool  XComDocStreamImp::_data_in_mem()
{
	return (m_DataInMemBuffer == true);
}

xcd_data_type  XComDocStreamImp::stream_type()
{
	return m_data_type;
}

bool   XComDocStreamImp::_is_compressed()
{
	if(m_EntryItem && m_EntryItem->m_CompressedRate != XCOMDOC_NOCOMPRESS)
		return true;
	return false;
}

bool  XComDocStreamImp::_duplicate_data()
{ 
	return m_pWriteBuffer->duplicate_data();
}

XComDocStreamImp::XComDocStreamImp(xComDocument* pDoc,xcd_rwmode  mode,CDDataEntryItemImp* pItem)
{
	m_compresseddata_buf = NULL;
	m_pComDoc =  pDoc;
	m_EntryItem = pItem;
	m_mode = mode;
	m_data_type = xcddt_common;
	m_pWriteBuffer = &m_WriteBuffer;
	m_bUseSrcFile =false;
	if(mode == xcdm_write || mode == xcdm_rw)
		m_DataInMemBuffer = true;
	else
		m_DataInMemBuffer = false;
}

void XComDocStreamImp::_set_read_mode(bool data_inbuffer)
{
	m_DataInMemBuffer = data_inbuffer;
}

void XComDocStreamImp::_set_file(IStreamIO* file,int beginPos, int len)
{
	m_DataInMemBuffer = false;
	if(_is_compressed())
		m_compressedReadBuffer.set_stream(file,beginPos,len,m_EntryItem->m_DataCompressedSize);
	else
		m_ReadBuffer.set_stream(file,beginPos,len);

}

void  XComDocStreamImp::_set_buf(const _xcd_int8* buf,size_t buf_len)
{
	m_DataInMemBuffer = true;
	m_pWriteBuffer->set_buf(buf,(int)buf_len);
}

size_t XComDocStreamImp::_read_all_data_from_file()
{
	if(_is_compressed())
	{
		m_DataInMemBuffer = true;
		int pos = m_compressedReadBuffer.get_file_read_pos();
		m_compressedReadBuffer.seekg(0,ios::beg);
		int len = m_compressedReadBuffer.data_len();
		m_pWriteBuffer->free();
		m_pWriteBuffer->reserver(len);
		m_compressedReadBuffer.read(  m_pWriteBuffer->get_buf(), len );
		m_pWriteBuffer->set_data_real_size(len);
		m_compressedReadBuffer.set_file_read_pos(pos);
		return 0;
	}

	m_DataInMemBuffer = true;
	int pos = m_ReadBuffer.get_file_read_pos();
	m_ReadBuffer.seekg(0,ios::beg);
	int len = m_ReadBuffer.data_len();
	m_pWriteBuffer->free();
	m_pWriteBuffer->reserver(len);
	m_ReadBuffer.read(  m_pWriteBuffer->get_buf(), len );
	m_pWriteBuffer->set_data_real_size(len);
	m_ReadBuffer.set_file_read_pos(pos);
	return 0;
}

size_t XComDocStreamImp::_read_file(_xcd_int8* buf,size_t byte_read)
{
	if(_is_compressed())
	{
		return (size_t)m_compressedReadBuffer.read(buf,(int)byte_read);
	}
	return (size_t )m_ReadBuffer.read(buf,(int)byte_read);
}

size_t XComDocStreamImp::_read_mem(_xcd_int8* buf,size_t byte_read)
{
	return (size_t)m_pWriteBuffer->read(buf,(int)byte_read);
}

size_t XComDocStreamImp::read(_xcd_int8* buf,size_t byte_read)
{
	if(m_mode == xcdm_write)
		return 0;

	if(m_DataInMemBuffer == false)
	{
		return _read_file(buf,byte_read);
	}
	else
	{
		return _read_mem(buf,byte_read);
	}
}

size_t XComDocStreamImp::write(const _xcd_int8* buf,size_t byte_write)
{
	return (size_t)m_pWriteBuffer->write(buf,(int)byte_write);   
}

size_t XComDocStreamImp::reserved(size_t len)
{
	if(false == m_pWriteBuffer->reserver((int)len) )
		return 0;
	else
		return len;
}

size_t XComDocStreamImp::capictiy()
{
	if(m_mode == xcdm_write || m_mode == xcdm_rw)
		return m_pWriteBuffer->capictiy();
	return 0;
}

size_t XComDocStreamImp::buf_len()
{
	if(m_DataInMemBuffer == true)
		return m_pWriteBuffer->data_len();

	if(_is_compressed())
		return m_compressedReadBuffer.data_len();
	return m_ReadBuffer.data_len();
}

size_t XComDocStreamImp::data_len()
{
	if(m_bUseSrcFile)
	{
#ifdef _LINUX
		char fileNameAnsi[256]={0};
		XEvol_UnicodeToFsEnc(m_SrcFileName.c_str() , fileNameAnsi , 256);
		ifstream ifile(fileNameAnsi,ios::binary);
#else
		ifstream ifile(m_SrcFileName.c_str(),ios::binary);
#endif
		ios::pos_type beg_pos = ifile.tellg();
		ifile.seekg(0,ios::end);
		int len   = ifile.tellg() - beg_pos ;
		ifile.close();
		return len;
	}
	if(m_DataInMemBuffer == true)
		return m_pWriteBuffer->data_len();
	if(_is_compressed())
		return m_compressedReadBuffer.data_len();
	return m_ReadBuffer.data_len();
}

_xcd_int8*  XComDocStreamImp::get_buf()
{
	if(m_DataInMemBuffer)
	{
		return m_pWriteBuffer->get_buf();
	}
	else
	{
		//if(_is_compressed())
		//	return m_compressedReadBuffer.get_buf();
		return NULL;//m_ReadBuffer.get_buf();
	}
	return NULL;
}

int XComDocStreamImp::stream_tellr()
{
	if(m_DataInMemBuffer == false)
	{
		if(_is_compressed())
			return m_compressedReadBuffer.tellg();
		return m_ReadBuffer.tellg();
	}
	else
	{
		return m_pWriteBuffer->tellg();
	}
}

int XComDocStreamImp::stream_tellw()
{
	if(m_DataInMemBuffer == false)
	{
		return -1;
	}
	else
	{
		return m_pWriteBuffer->tellw();
	}
}

void XComDocStreamImp::stream_seekr(xcd_seek_dir pos,int _offset)
{
	ios::seekdir dir = ios::cur;
	if(pos == xcdsd_cur) dir = ios::cur;
	else if(pos == xcdsd_end) dir = ios::end;
	else if(pos == xcdsd_beg) dir = ios::beg;
	else _offset = 0;
	if(m_DataInMemBuffer == false)
	{
		if(_is_compressed())
			return m_compressedReadBuffer.seekg(_offset,dir);
		return m_ReadBuffer.seekg(_offset,dir);
	}
	else
	{
		return m_pWriteBuffer->seekg(_offset,dir);
	}
}

void XComDocStreamImp::stream_seekw(xcd_seek_dir pos ,int _offset)
{
	ios::seekdir dir = ios::cur;
	if(pos == xcdsd_cur) dir = ios::cur;
	else if(pos == xcdsd_end) dir = ios::end;
	else if(pos == xcdsd_beg) dir = ios::beg;
	else _offset = 0;
	if(m_DataInMemBuffer == false)
	{
		return ;
	}
	else
	{
		return m_pWriteBuffer->seekw(_offset,dir);
	}
}

_xcd_int8*  XComDocStreamImp::get_compress_info(int& streamLen , int& compressedSize,int compressRate , int blockSize)
{
	if(m_compresseddata_buf)
	{
		//streamLen = m_compressedStreamLen;
		//compressedSize = m_compressedSize;
		//return m_compresseddata_buf;
		releaseCompressBuffer();
	}
	return _compress(streamLen , compressedSize ,compressRate, blockSize);
}

struct compItem
{
	sCDDataCompressPack m_ItemInfo;
	_xcd_int8*               m_buff;
};

_xcd_int8* XComDocStreamImp::_compress(int& streamLen , int& compressedSize,int compressRate,int blockSize)
{
	int sizeRemain = m_WriteBuffer.data_len();
	_xcd_int8*  buf = m_WriteBuffer.get_buf();
	_xcd_int8*  oldbuf = buf;
	vector<compItem> compItems;
	compressedSize = 0;
	while(sizeRemain > 0)
	{
		compItem item;
		int curSize = 0;
		if(sizeRemain <= 2 * blockSize)
		{
			curSize = sizeRemain;
			sizeRemain = 0;
		}
		else
		{
			curSize = blockSize;
			sizeRemain -= curSize;
		}
		int buf_size = curSize;
		if(curSize < 512) buf_size = 512;
		item.m_buff = new _xcd_int8 [buf_size * 10];
		int destLen = XComDoc_Compress(item.m_buff,buf,curSize,compressRate);
		item.m_ItemInfo.m_CompreessedSize = destLen;
		item.m_ItemInfo.m_DataSize = curSize;
		item.m_ItemInfo.m_CompreessedRate = compressRate;
		buf += curSize;
		compItems.push_back(item);
		compressedSize += destLen;
	}

	XComDocStreamImp mBuffer(NULL,xcdm_write,NULL);;
	int nItem = (int)compItems.size();
	int headerSize = 4 + nItem*sizeof(sCDDataCompressPack);
	_xcd_int8* compressedBuffer  = t_malloc<_xcd_int8>(compressedSize + headerSize );

	mBuffer.write((_xcd_int8*)&nItem,4);
	_xcd_int8* compressedBufferWritePos = compressedBuffer + headerSize;
	for(int i = 0 ; i < nItem ; i++)
	{
		compItem& item = compItems[i];
		int dcpSize = sizeof(sCDDataCompressPack);
		int byteWrite = (int)mBuffer.write((_xcd_int8*)&item.m_ItemInfo , dcpSize);
		if(byteWrite !=   dcpSize ) 
			assert(0);

		memcpy(compressedBufferWritePos,item.m_buff,item.m_ItemInfo.m_CompreessedSize);
		compressedBufferWritePos += item.m_ItemInfo.m_CompreessedSize;
		assert(oldbuf == m_WriteBuffer.get_buf());
	}

	memcpy(compressedBuffer,mBuffer.get_buf (),headerSize);

	assert(oldbuf == m_WriteBuffer.get_buf());
	streamLen = compressedSize + headerSize;


	for(int i = 0 ; i < nItem ; i++)
	{
		compItem& item = compItems[i];
		delete [] item.m_buff;

		item.m_buff = NULL;
	}

	m_compressedSize = compressedSize;
	m_compressedStreamLen = streamLen;
	m_compresseddata_buf = compressedBuffer;
	return compressedBuffer;
}

void XComDocStreamImp::releaseCompressBuffer()
{
	if(m_compresseddata_buf)
		::free(m_compresseddata_buf);
	m_compresseddata_buf = NULL;
}



//==========xStreamContext======================
void xStreamContext::AddRef()
{
   m_RefCount ++;
}
void xStreamContext::Release()
{
	m_RefCount -- ;
	if(m_RefCount <= 0)
       delete this;
}
xStreamContext::xStreamContext(XComDocStreamImp* pStreamImp)
{
	m_pStream  = pStreamImp;
	m_readPos  = 0;
	m_writePos = 0;
	m_RefCount = 1;
}

size_t      xStreamContext::read(_xcd_int8* buf,size_t byte_read)
{
	m_pStream->stream_seekr(xcdsd_beg,m_readPos);
	size_t nRead = m_pStream->read(buf,byte_read);
	m_readPos = m_pStream->stream_tellr();
	return nRead;
}
size_t      xStreamContext::write(const _xcd_int8* buf,size_t byte_write)
{
	m_pStream->stream_seekw(xcdsd_beg,m_writePos);
	size_t nWrite =  m_pStream->write(buf,byte_write);
	m_writePos = m_pStream->stream_tellw();
	return nWrite;
}

_xcd_int8*       xStreamContext::get_buf()
{
	return m_pStream->get_buf();
}

int         xStreamContext::stream_tellr()
{
	return m_readPos;
}

int         xStreamContext::stream_tellw()
{
	return m_writePos;
}

void        xStreamContext::stream_seekr(xcd_seek_dir seekpos,int _offset)
{
	m_pStream->stream_seekr(seekpos,_offset);
	m_readPos = m_pStream->stream_tellr();
}

void        xStreamContext::stream_seekw(xcd_seek_dir seekpos,int _offset)
{
	m_pStream->stream_seekw(seekpos,_offset);
	m_writePos = m_pStream->stream_tellw();
}

bool        xStreamContext::is_open()
{

	return m_pStream && m_pStream->is_open();
}

const wchar_t* xStreamContext::stream_name()
{
	return m_pStream->stream_name();
}

xcd_data_type  xStreamContext::stream_type(void)
{
	return m_pStream->stream_type();

}

size_t      xStreamContext::data_len()
{
	return m_pStream->data_len();
}

size_t      xStreamContext::buf_len()
{
	return m_pStream->buf_len();
}
END_NAMESPACE_XEVOL3D
