// Evol3D.Net.h

#pragma once

#include "BaseLib/xXmlDocument.h"
#include "xDotNetUlti.h"
using namespace XEvol3D;
using namespace System;
namespace xEvol3DNet 
{
	public ref   class xXmlNode
	{
        XEvol3D::xXmlNode* m_pObject;
		bool               m_bNeedDeleted;
	internal:
		xXmlNode(XEvol3D::xXmlNode*  pObject): m_pObject(pObject),m_bNeedDeleted(false) {};
	public:
		xXmlNode() : m_bNeedDeleted(false) {m_pObject = new XEvol3D::xXmlNode; }
		~xXmlNode(){if(m_bNeedDeleted && m_pObject ) delete m_pObject ; }
	public:
		xXmlNode^                 find(String^ nodeName , int idx){ return gcnew xXmlNode( m_pObject->findNode(ToPtr(nodeName)  , idx ) ) ; }
		xXmlNode^                 find(int idx){ return gcnew xXmlNode( m_pObject->findNode( idx ) ) ; }
		int                       countNode(String^ nodeName ){ return  m_pObject->countNode( ToPtr(nodeName) ) ; }
		void                      clear() {  return m_pObject->clear() ; } 
		array<xXmlNode^>^ find(String^ nodeName)
		{
			XEvol3D::xXmlNode::XmlNodes nodes;
			m_pObject->findNode( ToPtr(nodeName) , nodes );
			array<xXmlNode^>^ _array = gcnew array<xXmlNode^>(nodes.size());
			for(size_t i = 0 ; i < nodes.size() ; i ++)
			{
				xXmlNode^ pXmlNode = gcnew xXmlNode( nodes[i] ) ;
				_array[i] = pXmlNode;
			}
			return _array;
		}
		//{
		//	xXmlNode::XmlNodes nodes;
		//	xXmlNode[] _array = gcnew xXmlNode()[ nodes.size() ];
		//	m_pObject->findNode( ToPtr(nodeName) , nodes );
		//	for(size_t i = 0 ; i < nodes.size() ; i ++)
		//	{
		//		xXmlNode^ pXmlNode = gcnew xXmlNode( nodes[i] ) ;
		//		_array[i] = pXmlNode;
		//	}
		//	return _array;
		//}
	public:
		void         setValue(String^ nodeName , String^ valueName , int          _value) {  m_pObject->setValue(ToPtr(nodeName)  , ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ nodeName , String^ valueName , bool         _value) {  m_pObject->setValue(ToPtr(nodeName)  , ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ nodeName , String^ valueName , float        _value) {  m_pObject->setValue(ToPtr(nodeName)  , ToPtr(valueName) , _value ) ;  }
		void         setHex(String^ nodeName   , String^ valueName , unsigned int _value) {  m_pObject->setHex(ToPtr(nodeName)    , ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ nodeName , String^ valueName , String^      _value) {  m_pObject->setValue(ToPtr(nodeName)  , ToPtr(valueName) , ToPtr(_value) ) ;  }
		int          AsInt(String^ nodeName    , String^ valueName){ return m_pObject->int_value  (ToPtr(nodeName) , ToPtr(valueName)); }
		float        AsFloat(String^ nodeName  , String^ valueName){ return m_pObject->float_value(ToPtr(nodeName) , ToPtr(valueName)); }
		String^      AsString(String^ nodeName , String^ valueName){ return gcnew String(m_pObject->value      (ToPtr(nodeName) , ToPtr(valueName)) ); }
		bool         AsBolean(String^ nodeName , String^ valueName){ return m_pObject->bool_value (ToPtr(nodeName) , ToPtr(valueName)); }
		unsigned int AsHex(String^ nodeName    , String^ valueName){ return m_pObject->hex_value  (ToPtr(nodeName) , ToPtr(valueName)); }

	public:
		void         setValue(String^ valueName , int          _value) {  m_pObject->setValue(ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ valueName , bool         _value) {  m_pObject->setValue(ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ valueName , float        _value) {  m_pObject->setValue(ToPtr(valueName) , _value ) ;  }
		void         setHex(  String^ valueName , unsigned int _value) {  m_pObject->setHex(  ToPtr(valueName) , _value ) ;  }
		void         setValue(String^ valueName , String^      _value) {  m_pObject->setValue(ToPtr(valueName) , ToPtr(_value) ) ;  }
		int          AsInt(   String^ valueName){ return m_pObject->int_value  (ToPtr(valueName)); }
		float        AsFloat( String^ valueName){ return m_pObject->float_value(ToPtr(valueName)); }
		String^      AsString(String^ valueName){ return gcnew String(m_pObject->value      (ToPtr(valueName)) ); }
		bool         AsBolean(String^ valueName){ return m_pObject->bool_value (ToPtr(valueName)); }
		unsigned int AsHex(   String^ valueName){ return m_pObject->hex_value  (ToPtr(valueName)); } 
	};


	public ref   class xXmlDoc
	{
		xXmlDocument* m_pObject;
		bool          m_bNeedDeleted;
    internal:
		xXmlDoc(xXmlDocument*  pObject): m_pObject(pObject),m_bNeedDeleted(false) {};
	public:
		xXmlDoc() : m_bNeedDeleted(false) {m_pObject = new xXmlDocument; }
		~xXmlDoc(){if(m_bNeedDeleted && m_pObject ) delete m_pObject ; }
	public:
		bool          load(String^ fileName , bool bLoadFileHeader) {return  m_pObject->load(ToPtr(fileName) , bLoadFileHeader) ; }
		bool          save(String^ fileName){ return m_pObject->save(ToPtr(fileName)) ; }
		xXmlNode^     root(String^ nodeName){ return gcnew xXmlNode( m_pObject->root( ToPtr(nodeName) ) ) ;}
		xXmlNode^     root(){ return gcnew xXmlNode( m_pObject->root() ) ;}
	};

}
