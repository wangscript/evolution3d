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

#pragma warning (disable:4273)

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

#include "xcomdoc_common.h"
#include <assert.h>
#include "xcomdocImp.h"
#include "xcomdocbase.h"
#include "xcomdir.h"
#include "xcdStringHash.h"
#include "xcdUtil.h"

using namespace XEvol3D;





BEGIN_NAMESPACE_XEVOL3D

IComDocBase* CDCLASS(HXCDFILE hfile)
{
	return (IComDocBase*) hfile;
}
static HXCDFILE  CDHFILE(IComDocBase* doc)
{
	return (HXCDFILE)  doc;
}

static xStreamContext* CDSTREAMCLASS(HXCDSTREAM hStream)
{
	return (xStreamContext*) hStream;
}

static HXCDSTREAM  CDHSREAM(xStreamContext* stream)
{
	return (HXCDSTREAM)  stream;
}

int    xcd_string_hash(const wchar_t* str , int* valueA , int* valueB)
{
	HashValue value;
	xHashCodeTable::singleton().string_hash(str,value);
	if(valueA) *valueA = value.valueA;
	if(valueB) *valueB = value.valueB;
	return value.indexHash;
}

//=================================================================
xcomdoc::xcomdoc()
{
	m_hFIle = NULL;//CDHFILE(new XComDoc);
}

xcomdoc& xcomdoc::operator = (const xcomdoc& rhv)
{
	m_hFIle = rhv.m_hFIle;
	m_hDir  = rhv.m_hDir;
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc) pDoc->addRef();


	return *this;
}

xcomdoc::xcomdoc(const xcomdoc& doc)
{
	m_hFIle = doc.m_hFIle;
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc) pDoc->addRef();
}

void xcomdoc::release()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc) pDoc->release();
	m_hFIle = NULL;
}

xcomdoc::~xcomdoc()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc && pDoc->release())
	{
		m_hFIle = NULL;
	}
}

bool  xcomdoc::open(IReadStream* pReader,xcd_rwmode mode,int _offset, bool load_to_mem)
{
	if(m_hFIle == NULL) m_hFIle = CDHFILE(new  xComDocument );
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->open(pReader,mode,_offset,load_to_mem);
}

bool  xcomdoc::open(std::istream* stream,xcd_rwmode mode,int _offset, bool load_to_mem)
{
	if(m_hFIle == NULL) m_hFIle = CDHFILE(new xComDocument );
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->open(stream, mode,_offset,load_to_mem);
}

bool  xcomdoc::open_dir(const wchar_t*   file_name , bool not_create ) 
{
	if(m_hFIle == NULL) m_hFIle = CDHFILE(new xdircomdoc );
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->open(file_name, xcdm_rw, 0 , false,not_create);
}

bool       xcomdoc::open(const wchar_t* file_name,xcd_rwmode mode,int _offset,bool load_to_mem ,bool not_create)
{
	if( xcd_is_dir(file_name) )
	{

		if(m_hFIle == NULL) m_hFIle = CDHFILE(new xdircomdoc );
		IComDocBase* pDoc = CDCLASS(m_hFIle);
		assert(pDoc);
		return pDoc->open(file_name, xcdm_rw, 0 , false,not_create);
	}
	else
	{
		if(m_hFIle == NULL) m_hFIle = CDHFILE(new xComDocument );
		IComDocBase* pDoc = CDCLASS(m_hFIle);
		assert(pDoc);
		return pDoc->open(file_name,mode,_offset,load_to_mem,not_create);
	}

}

bool xcomdoc::open(xcomdocstream* pStream ,xcd_rwmode mode,int _offset ,bool load_to_mem)
{
	if(m_hFIle == NULL) m_hFIle = CDHFILE(new xComDocument );
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->open((xStreamContext*)pStream,mode,_offset,load_to_mem);
}

bool xcomdoc::open(const _xcd_int8* buf,  size_t buf_len,   xcd_rwmode mode,bool copyed_data)
{
	if(m_hFIle == NULL) 
	{
		m_hFIle = CDHFILE(new xComDocument );
	}
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->open(buf,buf_len,mode,copyed_data);
}

bool       xcomdoc::create(const wchar_t* file_name,int hash_table_size)
{
	if(m_hFIle == NULL)
	{
		m_hFIle = CDHFILE(new xComDocument );
	}
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->create(file_name,hash_table_size);
}

bool       xcomdoc::close()
{
	if(m_hFIle == NULL) 
		return false;
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->close();
}

bool xcomdoc::save(const wchar_t* file_name)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->save(file_name);
}
bool xcomdoc::save()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->save();
}
bool xcomdoc::save(std::ostream* pOstream)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->save(pOstream);
}
bool xcomdoc::save(IWriteBuffer* pOstream)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->save(pOstream);
}

size_t     xcomdoc::length()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc) return pDoc->length();
	return 0;
}
bool       xcomdoc::failed()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc == NULL) return true;
	return pDoc->failed();
}

void  xcomdoc::discard()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	if(pDoc) pDoc->discard();
}

xcd_state   xcomdoc::state()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->state();
}

void  xcomdoc::free_buf()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->free_buf();
}
void  xcomdoc::set_buf(_xcd_int8* buf,size_t buf_len)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	pDoc->set_buf(buf,buf_len);
}
_xcd_int8*          xcomdoc::get_buf(size_t* pbuf_len)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->get_buf(pbuf_len);
}

size_t         xcomdoc::buf_len()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->buf_len();
}

xcomdocstream* xcomdoc::open_stream(const wchar_t* name,bool not_create)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	if(*name == '/') name ++;
	xcomdocstream* pStream = pDoc->open_stream(name,not_create);
	if(pStream == NULL)
		return NULL;
	return pStream;
}

xcomdocstream* xcomdoc::open_stream(xcd_data_type type , int type_index,bool not_create)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	xcomdocstream* pStream = pDoc->open_stream(type,type_index,not_create);
	if(pStream == NULL)
		return NULL;
	return pStream;
}

xcomdocstream* xcomdoc::create_stream(const wchar_t* name , xcd_data_type type, int compressed_rate)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	xcomdocstream* pStream = pDoc->create_stream(name,compressed_rate,type);
	if(pStream == NULL)
		return NULL;
	return pStream;
}

xcomdocstream* xcomdoc::create_stream_with_dir(const wchar_t* name , xcd_data_type type, int compressed_rate )
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	xcomdocstream* pStream = pDoc->create_stream_with_dir(name,compressed_rate,type);
	if(pStream == NULL)
		return NULL;
	return pStream;
}

void           xcomdoc::close_stream(xcomdocstream* pStream)
{
	if(pStream == NULL) return ;
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	pDoc->close_stream(pStream );
	pStream->release();
}

void  xcomdoc::add_dir(const wchar_t* bas_dir,int compressed_rate)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
#ifndef _WIN32_WCE
	AddDirectoryRecurse(pDoc,bas_dir,compressed_rate);
#endif
}

void  xcomdoc::extrac_to_dir(const wchar_t* bas_dir)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
#ifndef _WIN32_WCE
	ExtractToDirectory(pDoc,bas_dir);
#endif
}

bool  xcomdoc::add_file(const wchar_t* bas_dir,const wchar_t* file_name,xcd_data_type type, int compressed_rate )
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->add_file(bas_dir,file_name,compressed_rate,type);
}

const wchar_t* xcomdoc::stream_name(int index)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->stream_name(index);
}

int xcomdoc::count_stream()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->count_stream();
}
int  xcomdoc::count_stream(xcd_data_type type)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->count_stream(type);
}

void     xcomdoc::clear()
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->clear();
}
bool     xcomdoc::delete_stream(const wchar_t* name)
{
	IComDocBase* pDoc = CDCLASS(m_hFIle);
	assert(pDoc);
	return pDoc->delete_stream(name);
}

#ifdef _BUILD_ANSI_SUPPORT_
bool   xcomdoc::open(const char*   file_name , xcd_rwmode mode, int _offset , bool load_to_mem ,bool not_create)
{
	wchar_t wcsFileName[512] = {0};
	_xcd_mbsTowcs(file_name , wcsFileName , 512);
	return open(wcsFileName , mode , _offset , load_to_mem , not_create);
}
bool   xcomdoc::save(const char* file_name)
{	
	wchar_t wcsFileName[512] = {0};
	_xcd_mbsTowcs(file_name , wcsFileName , 512);
	return save(wcsFileName);
}

xcomdocstream*   xcomdoc::open_stream(const char* name,bool not_create )
{	
	wchar_t wcsName[512] = {0};
	_xcd_mbsTowcs(name , wcsName , 512);
	return open_stream(wcsName,not_create);
}

bool   xcomdoc::delete_stream(const char* name)
{	
	wchar_t wcsName[512] = {0};
	_xcd_mbsTowcs(name , wcsName , 512);
	return delete_stream(wcsName);
}


xcomdocstream*   xcomdoc::create_stream(const char* name,xcd_data_type type,int compressed_rate)
{	
	wchar_t wcsName[512] = {0};
	_xcd_mbsTowcs(name , wcsName , 512);
	return create_stream(wcsName,type,compressed_rate);
}

xcomdocstream*   xcomdoc::create_stream_with_dir(const char* name, xcd_data_type type,int compressed_rate)
{	
	wchar_t wcsName[512] = {0};
	_xcd_mbsTowcs(name , wcsName , 512);
	return create_stream_with_dir(wcsName,type,compressed_rate);
}

#endif
END_NAMESPACE_XEVOL3D



