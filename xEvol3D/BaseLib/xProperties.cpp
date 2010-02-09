#include "../xStdPch.h"
#include "xProperties.h"
#include "xStringHash.h"
#include "xXmlDocument.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_REFCOUNT_OBJECT_FUNCTION(xPropertyValue)
IMPL_REFCOUNT_OBJECT_FUNCTION(xPropertyPtrValue)
//常见的属性值，能容纳内存变量
const void*  xPropertyValue::getValuePtr()
{
	return m_pData;
}

size_t xPropertyValue::getValueDataLen()
{
	return m_DataLen;
}
bool   xPropertyValue::setValulePtr(const void* pData , size_t dataLen, int _offset )
{
	reAlloc( dataLen ) ;
	if(m_pData == NULL || m_DataLen < dataLen)
		return false;
	memcpy( (int8*)m_pData + _offset , pData , dataLen);
	return true;
}

bool   xPropertyValue::reAlloc(size_t iData)
{
	if(iData == 0) //删除所有内存
	{
		m_DataLen = 0;
		if(m_pData) { delete [] m_pData ;}
		m_pData = NULL;
	}
	else
	{
		if(m_DataLen < iData)//只有内存小的时候才需要重新分配
		{
			if(m_pData) { delete [] m_pData ;}
			m_pData = new int8[iData];
			m_DataLen = iData;
		}
	}
	return true;
}

xPropertyValue::xPropertyValue(size_t iDataLen , const void* pInitData , int RefCount)
{
	m_RefCount = RefCount ;
	m_DataLen = 0;
	m_pData   = NULL;
	reAlloc(iDataLen);
	if(iDataLen >=0 && pInitData)
	{
		memcpy(m_pData , pInitData , iDataLen);
	}

}

xPropertyValue::~xPropertyValue()
{
	reAlloc(0);
}

//外部Void型的PtrValue
const void*  xPropertyPtrValue::getValuePtr()
{
	return m_pData;
}

size_t xPropertyPtrValue::getValueDataLen()
{
	return m_DataLen;
}
bool   xPropertyPtrValue::setValulePtr(const void* pData , size_t dataLen, int _offset)
{
	if(m_pData == NULL || m_DataLen < dataLen || pData == NULL)
		return false;
	memcpy( (int8*)m_pData + _offset , pData , dataLen);
	return true;
}

bool   xPropertyPtrValue::reAlloc(size_t iData)
{
	return false;
}

xPropertyPtrValue::xPropertyPtrValue(size_t iDataLen , void* pData , int RefCount)
{
	m_RefCount = RefCount ;
	m_DataLen = iDataLen;
	m_pData   = (void*)pData;
}

xPropertyPtrValue::~xPropertyPtrValue()
{
	
}
IMPL_REFCOUNT_OBJECT_FUNCTION(xProperty);
//常见的
xProperty::xProperty(const wchar_t* _name ,size_t dataLen  , void* pData)
{
	m_Name      = _name;
    m_pValue    = new xPropertyPtrValue(dataLen , pData);
}

xProperty::xProperty(const wchar_t* _name)
{
	m_Name      = _name;
	m_pValue    = new xPropertyValue(0,NULL);
}

xProperty::xProperty(const wchar_t* _name , IPropertyValue* pValue)
{
	m_Name      = _name;
	m_pValue    = pValue;
	pValue->AddRef();
}


xProperty::~xProperty()
{
	m_pValue->ReleaseObject();
}

const void* xProperty::value()  const 
{
	return m_pValue->getValuePtr();
}

const wchar_t* xProperty::string_value()   const 
{
	return (const wchar_t*)m_pValue->getValuePtr();
}


void xProperty::setValue(const wchar_t* _v, int _offset)    
{
	if(m_pValue == NULL)
		return ;

	size_t len =  (wcslen(_v) + 1) * sizeof(wchar_t);
	m_pValue->setValulePtr(_v , len , _offset);
	this->onValueChange();
}

void xProperty::setValue(const void* _v , size_t dataLen, int _offset)    
{

	if(m_pValue == NULL)
		return ;
	m_pValue->setValulePtr(_v , dataLen , _offset);
	this->onValueChange();
}

//=================
void xPropertyItem::setEventHanlder(xEventHanlder*   pHandler , void* pUserData)
{
	m_pEventHandler = pHandler;
	m_pUserData     = pUserData;
}

bool xPropertyItem::onValueChange()
{
	if(m_pEventHandler) return m_pEventHandler->onChange(m_pUserData);
	return true;
}

bool xPropertyItem::updateValue()
{
	if(m_pEventHandler) return m_pEventHandler->onUpdateValue(m_pUserData);
	return true;
}

xProperty* xPropertyItem::descriptor(const wchar_t* _name)
{
	//int hashName = xStringHash(_name);
	DescriptorMap::iterator pos = m_Descriptors.find(_name);
	if(pos == m_Descriptors.end() )
		return NULL;

	xProperty* pProperty = pos->second;
	return pProperty;
}

void  xPropertyItem::__removeDescriptor(xProperty* pDescritor)
{
	DescriptorMap::iterator pos = m_Descriptors.find(pDescritor->name());
	if(pos != m_Descriptors.end() )
	{
		m_Descriptors.erase(pos);
	}
	return ;
}

void  xPropertyItem::__removeDescriptor(const wchar_t* _name)
{
	DescriptorMap::iterator pos = m_Descriptors.find(_name);
	if(pos != m_Descriptors.end() )
	{
		m_Descriptors.erase(pos);
	}
	return ;
}

void  xPropertyItem::__removeProperty(const wchar_t* _name)
{
	PropertyMap::iterator pos = m_mapChildren.find(_name);
	if(pos != m_mapChildren.end() )
	{
		xPropertyItem* pProperty = pos->second;
		m_mapChildren.erase(pos);
		for( PropertyItems::iterator it = m_vChildren.begin() ; it != m_vChildren.end() ; it ++)
		{
			if(*it == pProperty)
			{
				m_vChildren.erase(it);
				break;
			}
		}
		return ;
	}
	return ;
}

void  xPropertyItem::__removeProperty(xPropertyItem* pProperty)
{
       const wchar_t* _name = pProperty->name();
	   PropertyMap::iterator pos = m_mapChildren.find(_name);
	   if(pos != m_mapChildren.end() )
	   {
		   m_mapChildren.erase(pos);
		   for( PropertyItems::iterator it = m_vChildren.begin() ; it != m_vChildren.end() ; it ++)
		   {
			   if(*it == pProperty)
			   {
				   m_vChildren.erase(it);
				   break;
			   }
		   }
		   return ;
	   }
	   return ;

}

void  xPropertyItem::__insertProperty(const wchar_t* _name , xPropertyItem* pProperty)
{
      m_mapChildren.insert(PropertyMap::value_type(_name , pProperty) );
	  m_vChildren.push_back(pProperty);
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name ,  size_t dataLen ,  void* pData)
{
	xPropertyItem*  pProperty = get_property(_name);
	if(pProperty)
		return pProperty;
	pProperty = new xPropertyItem(_name , dataLen , pData);
	__insertProperty(_name , pProperty) ;
	return pProperty;
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name )
{
	xPropertyItem*  pProperty = get_property(_name);
	if(pProperty)
		return pProperty;
	pProperty = new xPropertyItem(_name);
	__insertProperty(_name , pProperty) ;
	return pProperty;
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name , IPropertyValue* _v)
{
	return t_add_property(_name , _v );
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name ,  float  _v)
{
	return t_add_property(_name , _v );
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name ,  int    _v)
{
	return t_add_property(_name , _v );
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name ,  bool   _v)
{
	return t_add_property(_name , _v );
}

xPropertyItem* xPropertyItem::add_property(const wchar_t* _name ,  const wchar_t* _v)
{
	return t_add_property(_name , _v );
}


//----------------
//属性
//----------------
xProperty* xPropertyItem::add_descriptor(const wchar_t* _name ,  size_t dataLen ,  void* pData)
{
    xProperty*  pProperty = descriptor(_name);
	if(pProperty)
		return pProperty;
	pProperty = new xProperty(_name , dataLen , pData);
	m_Descriptors.insert(DescriptorMap::value_type( std::ds_wstring(_name) , pProperty) );
	return pProperty;
}

xProperty* xPropertyItem::add_descriptor(const wchar_t* _name ,  const wchar_t* _value)
{
	xProperty*  pProperty = descriptor(_name);
	if(pProperty)
		return pProperty;
	pProperty = new xProperty(_name , new xPropertyTextValue(_value , 0) );
	m_Descriptors.insert(DescriptorMap::value_type(std::ds_wstring(_name) , pProperty) );
	return pProperty;
}

xProperty* xPropertyItem::add_descriptor(const wchar_t* _name ,  float  _v)
{
	return t_add_descriptor(_name , _v);
}

xProperty* xPropertyItem::add_descriptor(const wchar_t* _name ,  int    _v)
{
	return t_add_descriptor(_name , _v);
}

xProperty* xPropertyItem::add_descriptor(const wchar_t* _name ,  bool   _v)
{
	return t_add_descriptor(_name , _v);
}

void xPropertyItem::descriptor(const wchar_t* _name , const wchar_t* _v)
{
	xProperty*  pProperty =  add_descriptor(_name , (wcslen(_v) + 1) * sizeof(wchar_t) , NULL);
	pProperty->setValue(_v);
}

void xPropertyItem::descriptor(const wchar_t* _name , const void* _v , size_t dataLen)
{
	xProperty*  pProperty =  add_descriptor(_name , dataLen , NULL);
	pProperty->setValue(_v , dataLen);
}




xPropertyItem* xPropertyItem::get_property(const wchar_t* _name)
{
	//int hashName = xStringHash(_name);
	PropertyMap::iterator pos = m_mapChildren.find(_name);
	if(pos == m_mapChildren.end() )
		return NULL;

	xPropertyItem* pProperty = (xPropertyItem*)pos->second;
	return pProperty;
}
void xPropertyItem::get_property(xPropertyItem::PropertyItems& vOuts)
{
	PropertyItems::iterator pos = m_vChildren.begin();
	for(; pos != m_vChildren.end() ; pos ++)
	{
		vOuts.push_back( (xPropertyItem*)*pos);
	}
}
xPropertyItem::xPropertyItem(const wchar_t* _name ,size_t dataLen  , void* pData)
:xProperty(_name , dataLen , pData)
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}
xPropertyItem::xPropertyItem(const wchar_t* _name ):xProperty(_name)
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}

xPropertyItem::xPropertyItem(const wchar_t* _name , IPropertyValue* pValue):xProperty(_name , pValue)
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}
xPropertyItem::xPropertyItem(const wchar_t* _name , const wchar_t* pValue):xProperty(_name , new xPropertyTextValue(pValue , 0 ))
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}
xPropertyItem::xPropertyItem(const wchar_t* _name , float  pValue):xProperty(_name , new TPropertyValue<float>(pValue , 0 ))
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}
xPropertyItem::xPropertyItem(const wchar_t* _name , int    pValue):xProperty(_name , new TPropertyValue<int>(pValue , 0 ))
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}
xPropertyItem::xPropertyItem(const wchar_t* _name , bool   pValue):xProperty(_name , new TPropertyValue<bool>(pValue , 0 ))
{
	m_pEventHandler = NULL;
	m_pUserData     = NULL;
}

xPropertyItem::~xPropertyItem()
{
   clear();
}
void xPropertyItem::clear()
{
	{
		//清理子属性
		PropertyMap::iterator pos = m_mapChildren.begin();
		for(; pos != m_mapChildren.end() ; pos ++)
		{
			xPropertyItem* pItem = (xPropertyItem*)pos->second;
			delete pItem;
		}
		m_mapChildren.clear();
		m_vChildren.clear();
	}

	//清理描述
	{
		DescriptorMap::iterator pos = m_Descriptors.begin();
		for(; pos != m_Descriptors.end() ; pos ++)
		{
			xProperty* pItem = pos->second;
			delete pItem;
		}
		m_Descriptors.clear();
	}

}


//属性集
xPropertySet::xPropertySet(const wchar_t* name , const wchar_t* _desc):
xPropertyItem(name , _desc)
{
     descriptor(L"type" , L"string");
}

xPropertySet::~xPropertySet()
{
	clear();
}

bool xPropertySet::load(const wchar_t* fileName , const int8* buf , size_t len )
{


	return true;
}

bool xPropertySet::load(xXmlDocument* pXml)
{
     xXmlNode* pNode = pXml->root();
	 if(pNode == NULL)
		 return false;
	 xXmlNode::XmlNodes propertyNodes;
	 pNode->findNode(L"property" , propertyNodes);
	 size_t nNodes = propertyNodes.size();
	 for(size_t i = 0 ; i <  nNodes; i ++)
	 {
          add(propertyNodes[i] , this);
	 }
	 return true;
}

bool xPropertySet::add(xXmlNode* pXmlNode , xPropertyItem* pParent)
{
	xPropertyItem* pPropertyItem = NULL;
	std::ds_wstring  dataType = L"string";
	if(pXmlNode->value(L"data_type") )
	{
		dataType = pXmlNode->value(L"data_type");
	}
    if(dataType == L"string")
	{ 
		std::ds_wstring _value = pXmlNode->value(L"value");
		pPropertyItem = pParent->add_property(pXmlNode->value(L"name") , _value.c_str() );
	}
	else if(dataType == L"int")
	{ 
		int _value = pXmlNode->int_value(L"value");
		pPropertyItem = pParent->add_property(pXmlNode->value(L"name") , _value );
	}
	else if(dataType == L"float")
	{ 
		float _value = pXmlNode->float_value(L"value");
		pPropertyItem = pParent->add_property(pXmlNode->value(L"name") , _value );
	}
	else
	{ 
		std::ds_wstring _value = pXmlNode->value(L"value");
		pPropertyItem = pParent->add_property(pXmlNode->value(L"name") , _value.c_str() );
	}

	//-----------------------------------------------
    //加入描述
	xXmlNode::XmlValues _xmlValues;
	pXmlNode->findValue(_xmlValues);
	size_t nValues = _xmlValues.size();
	for(size_t i =0 ; i < nValues ; i ++)
	{
		xXmlValue* pValue = _xmlValues[i];
		if(pValue->name() != std::ds_wstring(L"name") && pValue->name() != std::ds_wstring(L"value") )
		{
			pPropertyItem->add_descriptor(pValue->name() , pValue->value() );
		}
	}

	xXmlNode::XmlNodes descNodes;
	pXmlNode->findNode(L"descriptor" , descNodes);
	
	size_t nDescNodes = descNodes.size();
	for(size_t i = 0 ; i <  nDescNodes; i ++)
	{
		xXmlNode* pDescNode = descNodes[i];
        dataType = L"string";
		if(pDescNode->value(L"data_type") )
		{
			dataType = pDescNode->value(L"data_type");
		}

		if(dataType == L"string")
		{ 
			std::ds_wstring _value = pDescNode->value(L"value");
			pPropertyItem->add_descriptor(pDescNode->value(L"name") , _value.c_str() );
		}
		else if(dataType == L"int")
		{ 
			int _value = pDescNode->int_value(L"value");
			pPropertyItem->add_descriptor(pDescNode->value(L"name") , _value );
		}
		else if(dataType == L"float")
		{ 
			float _value = pDescNode->float_value(L"value");
			pPropertyItem->add_descriptor(pDescNode->value(L"name") , _value );
		}
		else
		{ 
			std::ds_wstring _value = pDescNode->value(L"value");
			pPropertyItem->add_descriptor(pDescNode->value(L"name") , _value.c_str() );
		}
	}


	xXmlNode::XmlNodes propertyNodes;
	pXmlNode->findNode(L"property" , propertyNodes);
	size_t nPropertyNodes = propertyNodes.size() ;
	for(size_t i = 0 ; i < nPropertyNodes ;  i ++)
	{
		add(propertyNodes[i] , pPropertyItem);
	}
	return true;
}

bool xPropertySet::unload()
{
	return true;
}

END_NAMESPACE_XEVOL3D
