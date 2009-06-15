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

#ifndef __XCOMDOCBASEDEFINE_IMP_H_
#define __XCOMDOCBASEDEFINE_IMP_H_
#include "xcomdoc.h"
BEGIN_NAMESPACE_XEVOL3D
using namespace std;
class IComDocBase
{
public:
	virtual void       addRef() = 0;
	virtual bool       release() = 0;
	IComDocBase(){}
	virtual ~IComDocBase(){};
	virtual bool              open(IReadStream* pReader,xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false) = 0;
	virtual bool              open(istream* stream, xcd_rwmode mode,int _offset = 0 , bool load_to_mem = false) = 0;
	virtual bool              open(xcomdocstream* pDocStream,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false) = 0;
	virtual bool              open(const wchar_t* file_name,xcd_rwmode mode,int _offset = 0 ,bool load_to_mem = false,bool not_create = true) = 0;
	virtual bool              open(const _xcd_int8* buf,  size_t buf_len,   xcd_rwmode mode,bool copyed_data = false) = 0;
	virtual bool              create(const wchar_t* file_name,int hash_table_size=256) = 0;
	virtual void              clear() = 0;
	virtual bool              save(const wchar_t* file_name) = 0;
	virtual bool              save() = 0;
	virtual bool              save(ostream* pOstream) = 0;
	virtual bool              save(IWriteBuffer* pOstream) = 0;
	virtual bool              close() = 0;
	virtual size_t            length() = 0;
	virtual bool              failed() = 0;
	virtual xcd_state         state() = 0;
	virtual void              discard() = 0;
	virtual bool              add_file(const wchar_t* bas_dir,const wchar_t* file_name,int compressed_rate = 0,xcd_data_type type = xcddt_common) = 0;
	
	/*
	Stream open method:
	1: by the name , name is unique.
	2: by type and type index . eg. many stream has the same type.
	*/
	virtual int               count_stream() = 0;
	virtual const wchar_t*    stream_name(int index) = 0;
	virtual xcomdocstream*    open_stream(const wchar_t* name,bool not_create = true) = 0;
	virtual xcomdocstream*    open_stream(xcd_data_type type , int type_index,bool not_create = true) = 0;
	virtual xcomdocstream*    create_stream(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common) = 0;
	virtual xcomdocstream*    create_stream_with_dir(const wchar_t* name, int compressed_rate = 0, xcd_data_type type = xcddt_common) = 0;
	virtual bool              delete_stream(const wchar_t* name) = 0;

	virtual void              close_stream(xcomdocstream* pStream) = 0;
	virtual int               count_stream(xcd_data_type type) = 0;
	virtual void              set_buf(_xcd_int8* buf,size_t buf_len) = 0;
	virtual _xcd_int8*        get_buf(size_t* pbuf_len) = 0;
	virtual size_t            buf_len() = 0;
	virtual void              free_buf() = 0;
};

END_NAMESPACE_XEVOL3D

#endif

