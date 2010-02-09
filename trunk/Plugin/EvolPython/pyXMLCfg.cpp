#include <boost/python.hpp>
#include "BaseLib/xCfgParser.h"
USING_NS_XEVOL3D


using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(xCfgDocument_load, xCfgDocument::load, 1, 2)

BOOST_PYTHON_MODULE(pyEvol)
{
	class_<xCfgDocument , boost::noncopyable>("xCfgDocument")
		.def("unload", &xCfgDocument::unload)
		.def( "save" , &xCfgDocument::load , xCfgDocument_load ,boost::python::args("fileName" , "isHeader") , "save")
		//.def("load"  , &xCfgDocument::load , boost::python::args("fileName") ,  boost::python::args("isHeader") )
		;
}

int afunc()
{
	void* pFunct = (void*)&xCfgDocument::save;
	return 0;
}
