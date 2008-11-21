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

#ifndef __WRITE_BUFFER_H__
#define __WRITE_BUFFER_H__

#include <ios>
#include "xcomdoc_common.h"
BEGIN_NAMESPACE_XEVOL3D

class CWriteBuffer : public IWriteBuffer
{
	_xcd_int8* m_data;

	int   m_read_pos;
	int   m_write_pos;
	int   m_len;
	int   m_real_len;
	bool  m_need_free;

public:
	CWriteBuffer();
	~CWriteBuffer();

public:
	void   rewind();
	bool   duplicate_data();
	bool   reserver(int len);
	void   free();
	int    write(const _xcd_int8* buf,int byte_write);
	int    read(_xcd_int8* buf, int byte_read);
	void   seekg(int _offset, ios::seekdir dir);
	void   seekw(int _offset, ios::seekdir dir);
	int    tellg();
	int    tellw();
	_xcd_int8*  get_buf();
	void   set_buf(const _xcd_int8* buf,int buf_len);
	int    data_len();
	bool   set_data_real_size(int real_size);
	int    capictiy();
};

END_NAMESPACE_XEVOL3D

#endif

