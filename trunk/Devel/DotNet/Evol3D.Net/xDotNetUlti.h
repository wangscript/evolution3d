#ifndef __PREDEFINE_H__
#define __PREDEFINE_H__
#pragma once

#define P_ReadOnly(_type,_name,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix;\
}

#define P_WriteOnly(_type,_name,_prefix,_suffix) property _type _name{\
	_prefix void set(_type) _suffix;\
}

#define P_ReadWrite(_type,_name,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix;\
	_prefix void set(_type) _suffix;\
}

#define PS_ReadOnly(_type,_name,_member,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix{\
	return (_type)_member;\
}\
}

#define PS_WriteOnly(_type,_name,_member,_prefix,_suffix) property _type _name{\
	_prefix void set(_type value) _suffix{\
	_member = value;\
}\
}

#define PS_ReadWrite(_type,_name,_member,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix{\
	return (_type)_member;\
}\
	_prefix void set(_type value) _suffix{\
	_member = value;\
}\
}
//--------

#define PF_WriteOnly(_type,_name, ptr , _setf ,_prefix,_suffix) property _type _name{\
	_prefix void  set(_type value) _suffix{	ptr->_setf(value) ; }\
}

#define PF_ReadOnly(_type,_name, ptr , _getf,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix{ return (_type)ptr->_getf();  }\
}

#define PF_ReadWrite(_type,_name, ptr , _getf, _setf ,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix{ return (_type)ptr->_getf();  }\
	_prefix void  set(_type value) _suffix{	ptr->_setf(value) ; }\
}


#define PFIDX_WriteOnly(_type,_name, ptr , _setf ,_prefix,_suffix) property _type _name{\
	_prefix void  set(int idx , _type value) _suffix{	ptr->_setf(idx ,value) ; }\
}

#define PFIDX_ReadOnly(_type,_name, ptr , _getf,_prefix,_suffix) property _type _name{\
	_prefix _type get(int idx) _suffix{ return (_type)ptr->_getf(idx);  }\
}


#define PFIDX_ReadWrite(_type,_name, ptr , _getf, _setf ,_prefix,_suffix) property _type _name{\
	_prefix _type get(int idx) _suffix{ return (_type)ptr->_getf( idx );  }\
	_prefix void  set(int idx , _type value) _suffix{	ptr->_setf(idx , value) ; }\
}

#define PS_ReadWrite2(_type,_name,_get,_set,_prefix,_suffix) property _type _name{\
	_prefix _type get() _suffix{\
	return (_type)_get;\
}\
	_prefix void set(_type value) _suffix{\
	_set\
}\
}

template<typename T> T& handle_refcast(int Handle)
{
	return *(T*)Handle;
}

template<typename T> T& handle_ptrcast(int Handle)
{
	return (T*)Handle;
}

#define P_ImpGet(C,N,T) T C::N::get()
#define P_ImpSet(C,N,T) void C::N::set(T value)

using namespace	System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
#include <vcclr.h>
inline const wchar_t* ToPtr(String^ text) { pin_ptr<const wchar_t> _text   = PtrToStringChars(text); return _text ; }
#endif