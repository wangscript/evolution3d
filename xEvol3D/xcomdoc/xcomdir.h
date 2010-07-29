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

#ifndef  __XCOMDOC_DIR_H__
#define  __XCOMDOC_DIR_H__
#include "xcomdoc.h"
#include "xcomdocbase.h"
#include <fstream>
#include <string>
BEGIN_NAMESPACE_XEVOL3D

class xdirfilestream : public xcomdocstream
{
	std::wstring m_FileName;
	FILE*        m_File;
	bool         m_bDir;
	xcd_rwmode   m_rwMode;
public:
	xdirfilestream(xcd_rwmode mode);
	~xdirfilestream();
public:
	void             Release();
	bool             open(const wchar_t* fileName , bool bNotCreated);
	xcd_data_type    stream_type(); 
	size_t           read(_xcd_int8* buf,size_t byte_read);
	size_t           write(const _xcd_int8* buf,size_t byte_write);
	_xcd_int8*       get_buf();
	int              stream_tellr();
	int              stream_tellw();
	void             stream_seekr(xcd_seek_dir,int _offset);
	void             stream_seekw(xcd_seek_dir,int _offset);
	size_t           data_len();
	bool             is_open()  ;
	const wchar_t*   stream_name();
};

class xdircomdoc : public IComDocBase
{
	std::wstring         m_DirName;
	int                  m_RefCount;
	xcd_rwmode           m_rwMode;
public:
	virtual void       AddRef();
	virtual bool       Release();
	xdircomdoc();
	~xdircomdoc();

	bool              open(const wchar_t* file_name,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false,bool not_create = true);
	bool              create(const wchar_t* file_name,int hash_table_size );
	void              clear();
	bool              add_file(const wchar_t* bas_dir,const wchar_t* file_name,int compressed_rate ,xcd_data_type type ) ;
	xcomdocstream*    open_stream(const wchar_t* name,bool not_create = true);
	xcomdocstream*    create_stream(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common);
	xcomdocstream*    create_stream_with_dir(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common);
	bool              delete_stream(const wchar_t* name);
	void              close_stream(xcomdocstream* pStream);
	const wchar_t*    name(){ return m_DirName.c_str() ;}


	bool              open(IStreamIO* pReader,xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false){return false ; }
	bool              open(istream* stream, xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false){return false ; }
	bool              open(xcomdocstream* pDocStream,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false){return false ; }
	bool              open(const _xcd_int8* buf,  size_t buf_len,   xcd_rwmode mode,bool copyed_data = false){return false ; }
	bool              save(const wchar_t* file_name){return false ; }
	bool              save(){return false ; }
	bool              save(ostream* pOstream){return false ; }
	bool              save(IWriteBuffer* pOstream){return false ; }
	bool              close(){ m_DirName = L"" ; return true ; }
	size_t            length(){ return 0 ; }
	bool              failed(){ return m_DirName.length() > 0 ;}
	xcd_state         state() {if(failed() ) return xcds_notopened ; return xcds_ok ; }
	void              discard() { return ; }
	

	int               count_stream(){return 0 ; }
	const wchar_t*    stream_name(int index) {return NULL;}
	int               count_stream(xcd_data_type type){return 0;}
	xcomdocstream*    open_stream(xcd_data_type type , int type_index,bool not_create = true) {return NULL ;}
	void              set_buf(_xcd_int8* buf,size_t buf_len) {return ;}
	_xcd_int8*        get_buf(size_t* pbuf_len) { return NULL ; }
	size_t            buf_len()  {return 0;}
	void              free_buf() {return;}
};

END_NAMESPACE_XEVOL3D
#endif

