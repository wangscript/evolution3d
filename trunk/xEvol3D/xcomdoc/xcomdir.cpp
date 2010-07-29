#include "../xStdPch.h"
#include "xcomdir.h"
#include "xcdUtil.h"
#include "xcomdoc_common.h"
BEGIN_NAMESPACE_XEVOL3D


void xdirfilestream::Release()
{
	delete this;
}

xdirfilestream::xdirfilestream(xcd_rwmode mode)
{
	m_File = NULL;
	m_FileName = L"";
	m_rwMode = mode;
}

bool xdirfilestream::open(const wchar_t* fileName , bool bNotCreated)
{
	m_FileName = fileName;
    if(m_File) fclose(m_File);

	if(m_rwMode == xcdm_read)
	{
		m_File = _xcd_wfopen_rb(fileName);
	}
	else
	{
		if(bNotCreated )
			m_File = _xcd_wfopen_rw_notCreate(fileName);
		else
			m_File = _xcd_wfopen_rwb(fileName);
	}


	if( m_File )
	{
		fseek(m_File , 0 , SEEK_SET);
		m_bDir = false;
		return true;
	}
	if(xcd_is_dir(fileName) )
	{
		m_bDir = true;
		return true;
	}

	m_FileName = L"";
	return false;
}

xcd_data_type    xdirfilestream::stream_type()
{
     if(m_bDir)
		 return xcddt_dir_flag;
	 return xcddt_common;
}

size_t xdirfilestream::read(_xcd_int8* buf,size_t byte_read)
{
     if(m_bDir) 
		 return 0;
	 return  fread(buf , 1 , byte_read , m_File);
}

size_t xdirfilestream::write(const _xcd_int8* buf,size_t byte_write)
{
     if(m_bDir) return 0;
	 return  fwrite(buf , 1 , byte_write , m_File);
}


_xcd_int8* xdirfilestream::get_buf()
{
      return NULL;
}

int xdirfilestream::stream_tellr()
{
     return ftell(m_File);
}

int xdirfilestream::stream_tellw()
{
     return ftell(m_File);
}

xdirfilestream::~xdirfilestream()
{
	if(m_File)
	{
		fclose(m_File);
		m_File = NULL;
	}
}

void xdirfilestream::stream_seekr(xcd_seek_dir  sdir,int _offset)
{
     stream_seekw(sdir , _offset);
}


void xdirfilestream::stream_seekw(xcd_seek_dir sdir, int _offset)
{
	int _seekway = SEEK_SET;
	if(sdir == xcdsd_cur) 
		_seekway = SEEK_CUR;
	else if(sdir == xcdsd_end) 
		_seekway = SEEK_END;
	else if(sdir == xcdsd_beg) 
		_seekway = SEEK_SET;
	fseek(m_File , _offset , _seekway);
}

size_t xdirfilestream::data_len()
{
    return 0;
}

bool xdirfilestream::is_open()
{
	return m_FileName.length() > 0;
}

const wchar_t*   xdirfilestream::stream_name()
{
	return m_FileName.c_str();

}


void xdircomdoc::AddRef()
{
     m_RefCount ++;
}  

bool xdircomdoc::Release()
{
	 m_RefCount -- ;
	 if(m_RefCount == 0)
	 {
		 delete this;
		 return true;
	 }
	 return false;
}

xdircomdoc::xdircomdoc()
{
	m_RefCount = 1;
	m_DirName = L"";
}

xdircomdoc::~xdircomdoc()
{

}

bool  xdircomdoc::open(const wchar_t* file_name,xcd_rwmode mode,int _offset ,bool load_to_mem, bool not_create)
{
	m_rwMode = mode;
	if(xcd_is_dir(file_name) == true)
	{
		m_DirName = file_name;
		return true;
	}

	if(not_create == true)
	{

		return false;
	}
	else
	{
		create_all_dir(L"" , file_name);
		if(xcd_is_dir(file_name) == true)
		{
			m_DirName = file_name;
			return true;
		}
		return false;
	}
	return false;     
}

bool  xdircomdoc::create(const wchar_t* file_name,int hash_table_size )
{
	return open(file_name , xcdm_rw , 0 , false , false);
}

void  xdircomdoc::clear()
{
    return ;

}

bool  xdircomdoc::add_file(const wchar_t* bas_dir,const wchar_t* file_name,int compressed_rate ,xcd_data_type type )
{
	return false;
}

xcomdocstream*  xdircomdoc::open_stream(const wchar_t* name,bool not_create)
{
    xdirfilestream* pStream = new xdirfilestream(m_rwMode);
	std::wstring fullname = m_DirName + L"/" + name;
	if(pStream->open(fullname.c_str() , not_create) == true)
	{
		return pStream;
	}
	delete pStream;
	return NULL;
}

xcomdocstream*  xdircomdoc::create_stream(const wchar_t* name, int compressed_rate , xcd_data_type type)
{
	xdirfilestream* pStream = new xdirfilestream(m_rwMode);
	std::wstring fullname = m_DirName + L"/" + name;
	if(pStream->open(fullname.c_str() , false) == true)
	{
		return pStream;
	}
	delete pStream;
		return NULL;
}

xcomdocstream*  xdircomdoc::create_stream_with_dir(const wchar_t* name, int compressed_rate, xcd_data_type type)
{
	xdirfilestream* pStream = new xdirfilestream(m_rwMode);
	create_all_dir(m_DirName.c_str() , name);
	std::wstring fullname = m_DirName + L"/" + name;
	if(pStream->open(fullname.c_str() , false) == true)
	{
		return pStream;
	}
	delete pStream;
		return NULL;
}

bool  xdircomdoc::delete_stream(const wchar_t* name)
{
    std::wstring fullname = m_DirName + L"/" + name;
#ifdef _WIN32
	_wrmdir(fullname.c_str() );
	DeleteFileW(fullname.c_str() );
	return true;
#endif
	return false;
}

void  xdircomdoc::close_stream(xcomdocstream* pStream)
{
	//pStream->Release();
}

END_NAMESPACE_XEVOL3D
