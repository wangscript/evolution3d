#ifndef __XEVOL_PROPERTIES_H__
#define __XEVOL_PROPERTIES_H__
#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <map>
BEGIN_NAMESPACE_XEVOL3D
//--------------------------------
//属性系统
//不同的属性的构造函数带一个RefCount，
//如果=0。外部不需要释放。 完全交给内部管理
//如果=1，那么外部需要释放
//--------------------------------
class _XEVOL_BASE_API_ xXmlDocument;
class _XEVOL_BASE_API_ xXmlNode;
class _XEVOL_BASE_API_ IPropertyValue : public IRefCountObject
{
public:
	virtual const void*  getValuePtr() = 0 ;
	virtual size_t       getValueDataLen() = 0 ;
	virtual bool         setValulePtr(const void* pData , size_t dataLen , int _offset = 0) = 0;

	//如果一个Value是固定大小的，那么这个函数可以返回false。而不需要改变任何大小
	virtual bool         reAlloc(size_t iData) = 0;
};


class _XEVOL_BASE_API_ xPropertyValue : public IPropertyValue
{
	DECL_REFCOUNT_OBJECT_INTERFACE(xPropertyValue)
public:
	virtual const void*  getValuePtr();
	virtual size_t       getValueDataLen();
	virtual bool         setValulePtr(const void* pData , size_t dataLen , int _offset = 0);
	virtual bool         reAlloc(size_t iData) ;

	xPropertyValue(size_t iDataLen , const void* pInitData , int RefCount = 1);
	virtual ~xPropertyValue();
protected:
	size_t         m_DataLen;
	void*          m_pData;
};

class _XEVOL_BASE_API_ xPropertyPtrValue : public IPropertyValue
{
	DECL_REFCOUNT_OBJECT_INTERFACE(xPropertyPtrValue)
public:
	virtual const void*  getValuePtr();
	virtual size_t       getValueDataLen();
	virtual bool         setValulePtr(const void* pData , size_t dataLen , int _offset = 0);
	virtual bool         reAlloc(size_t iData) ;

	xPropertyPtrValue(size_t iDataLen , void* pData , int RefCount = 1);
	virtual~xPropertyPtrValue();
protected:
	size_t         m_DataLen;
	void*          m_pData;
};

//整数，浮点数，boolean型的定义。
template <typename T> class TPropertyValueRef : public IPropertyValue
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(TPropertyValueRef)
public:
	virtual const void*  getValuePtr()
	{
		return &m_value;
	}
	virtual size_t getValueDataLen()
	{
		return sizeof(m_value);
	}
	virtual bool   setValulePtr(const void* pData , size_t dataLen , int _offset = 0)
	{
         if(sizeof(m_value) != dataLen )
	     {
			 unsigned int8* pDstData = (unsigned int8*)&m_value + _offset;
             memcpy(pDstData , pData , dataLen);
			 return true;
		 }
		 m_value = *(const T*)pData;
		 return true;
	}

	virtual bool   reAlloc(size_t iData)
	{
		return false;
	}
	TPropertyValueRef(T& _value , int RefCount = 1) :m_value(_value) {m_RefCount = RefCount ;}
	virtual ~TPropertyValueRef(){}
protected:
	T&           m_value;
};


template <typename T> class TPropertyValue : public IPropertyValue
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(TPropertyValue)
public:
	virtual const void*  getValuePtr()
	{
		return &m_value;
	}
	virtual size_t getValueDataLen()
	{
		return sizeof(m_value);
	}
	virtual bool   setValulePtr(const void* pData , size_t dataLen , int _offset = 0)
	{
		if(sizeof(m_value) != dataLen )
		{
			unsigned int8* pDstData = (unsigned int8*)&m_value + _offset;
			memcpy(pDstData , pData , dataLen);
			return true;
		}
		m_value = *(const T*)pData;
		return true;
	}

	virtual bool   reAlloc(size_t iData)
	{
		return false;
	}
	TPropertyValue(T _value , int RefCount = 1) :m_value(_value) {m_RefCount = RefCount ;}
	virtual ~TPropertyValue(){}
protected:
	T           m_value;
};

template <typename TSTRING> class xPropertyStringValue : public IPropertyValue
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xPropertyStringValue)
public:
	virtual const void*  getValuePtr()
	{
		return (const void*)m_value.c_str();
	}
	virtual size_t getValueDataLen()
	{
		return (m_value.size() + 1) * sizeof(TSTRING::value_type);
	}
	virtual bool   setValulePtr(const void* pData , size_t dataLen , int _offset = 0)
	{
		m_value = (const typename TSTRING::value_type*)pData;
		return true;
	}

	virtual bool   reAlloc(size_t iData)
	{
		m_value.resize(iData);
		return true;
	}
	xPropertyStringValue(TSTRING& _value , int RefCount = 1) :m_value(_value) { m_RefCount = RefCount ; }
	virtual ~xPropertyStringValue(){}
protected:
	TSTRING&           m_value;
};

//前面那个类是带引用的。这个不是引用
class _XEVOL_BASE_API_ xPropertyTextValue : public IPropertyValue
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xPropertyTextValue)
public:
	virtual const void*  getValuePtr()
	{
		return (const void*)m_value.c_str();
	}
	virtual size_t getValueDataLen()
	{
		return (m_value.size() + 1) * sizeof(wchar_t);
	}
	virtual bool   setValulePtr(const void* pData , size_t dataLen , int _offset = 0)
	{
		m_value = (const wchar_t*)pData;
		return true;
	}

	virtual bool   reAlloc(size_t iData)
	{
		m_value.resize(iData);
		return true;
	}
	xPropertyTextValue(const wchar_t* _value , int RefCount = 1) :m_value(_value) { m_RefCount = RefCount ; }
	xPropertyTextValue(const std::ds_wstring& _value , int RefCount = 1) :m_value(_value) { m_RefCount = RefCount ; }
	xPropertyTextValue(int RefCount = 1) :m_value(L"") { m_RefCount = RefCount ; }
	virtual ~xPropertyTextValue(){}
protected:
	std::ds_wstring           m_value;
};

class  _XEVOL_BASE_API_   xProperty : public IRefCountObject
{
    DECL_REFCOUNT_OBJECT_INTERFACE(xProperty);
public:
	~xProperty();
	xProperty(const wchar_t* _name , size_t dataLen, void* pData);
	xProperty(const wchar_t* _name);
	xProperty(const wchar_t* _name , IPropertyValue* pValue);

	template<typename T>    const T&   value()       { return *(const T*)m_pValue->getValuePtr() ; }
	template<typename T>    const T*   pointer_cast(){ return  (const T*)m_pValue->getValuePtr() ; } 

	template<typename T>               operator T*(){ return (T*)m_pValue->getValuePtr() ; } 
	template<typename T>               operator const T*(){ return (const T*)m_pValue->getValuePtr() ; } 
	template<typename T>        void   operator = (const T& _v ) { setValue(_v) ;                        } 
	template<typename T>   xProperty&  operator = (      T& _v ) { setValue(_v) ;  return *this ; } 
                                void   operator = (const wchar_t* _v ) { setValue(_v) ; } 

	const wchar_t*                     name()         { return m_Name.c_str() ; }
	int                                int_value()    { return value<int>() ; }
	float                              float_value()  { return value<float>()   ; }
	bool                               bool_value()   { return value<bool>()   ; }
	double                             double_value() { return value<double>()   ; }
	const  void*                       value()  const ;
	const wchar_t*                     string_value() const ;

	template <typename T> void         setValue(const T& _v , int _offset = 0)   {  if(m_pValue) { m_pValue->setValulePtr(&_v , sizeof(T) , _offset) ; }  }
	void                               setValue(const wchar_t* _v , int _offset = 0);
	void                               setValue(const void* _v , size_t dataLen , int _offset = 0)  ;
    IPropertyValue*                    getValueObject(){ return m_pValue ; }
	virtual        bool                onValueChange(){ return true ;}
protected:

	IPropertyValue*  m_pValue;
	std::ds_wstring  m_Name;
};


//属性Item。这个类有更强的属性描述能力，比如描述该属性的一些参数，更适用UI对属性的显示
class _XEVOL_BASE_API_ xPropertyItem : public xProperty
{
public:
	typedef std::ds_wstring                   HASHVALUE;
	typedef ds_map(HASHVALUE, xPropertyItem*) PropertyMap;
	typedef ds_map(HASHVALUE, xProperty*)     DescriptorMap;
	typedef ds_vector(xPropertyItem*)         PropertyItems;
	class xEventHanlder
	{
	public:
		virtual  bool onChange(void* pUserData) = 0;
		virtual  bool onUpdateValue(void* pUserData) { return false ; }
	};
public:
	void                                  setEventHanlder(xEventHanlder*   pHandler , void* pUserData);
	bool                                  onValueChange();
	bool                                  updateValue();
	///属性的操作
	xProperty*                            descriptor(const wchar_t* _name);
	xProperty*                            add_descriptor(const wchar_t* _name ,  size_t dataLen ,  void* pData);
	xProperty*                            add_descriptor(const wchar_t* _name ,  const wchar_t* _value);
	xProperty*                            add_descriptor(const wchar_t* _name ,  float  _v);
	xProperty*                            add_descriptor(const wchar_t* _name ,  int    _v);
	xProperty*                            add_descriptor(const wchar_t* _name ,  bool   _v);
	

	void                                  descriptor(const wchar_t* _name , const wchar_t* _v);
	void                                  descriptor(const wchar_t* _name , const void* _v , size_t dataLen);  
	template<typename T> void             descriptor(const wchar_t* _name , const T& _v) 
	{
		xProperty*  pProperty = add_descriptor(_name , sizeof(T) , NULL);
		pProperty->setValue(_v);
	}

	//父子关系的操作
    xPropertyItem*                        add_property(const wchar_t* _name ,  size_t dataLen ,  void* pData);
	xPropertyItem*                        add_property(const wchar_t* _name );
	xPropertyItem*                        add_property(const wchar_t* _name , IPropertyValue* pValue);
	xPropertyItem*                        add_property(const wchar_t* _name ,  const wchar_t* _v);
	xPropertyItem*                        add_property(const wchar_t* _name ,  float  _v);
	xPropertyItem*                        add_property(const wchar_t* _name ,  int    _v);
	xPropertyItem*                        add_property(const wchar_t* _name ,  bool   _v);

	xPropertyItem*                        get_property(const wchar_t* _name);
    void                                  get_property(PropertyItems& vOuts);
	template<typename T> void             set_property(const wchar_t* _name , T _v) 
	{
		xPropertyItem*  pProperty = get_property(_name);
		if(pProperty == NULL) { add_property(_name , _v) ; }
		else { pProperty->setValue(_v); }
		
	}
    

	template<typename T> T               get_property_value(const wchar_t* _name) 
	{
		xPropertyItem*  pProperty = get_property(_name );
		pProperty->value<T>();
	}
    

	xPropertyItem(const wchar_t* _name ,  size_t dataLen ,  void* pData);
	xPropertyItem(const wchar_t* _name );
	xPropertyItem(const wchar_t* _name , const wchar_t* pValue);
	xPropertyItem(const wchar_t* _name , float  pValue);
	xPropertyItem(const wchar_t* _name , int    pValue);
	xPropertyItem(const wchar_t* _name , bool   pValue);
	xPropertyItem(const wchar_t* _name , IPropertyValue* pValue);
	void                 clear();
	void                 remove_property(const wchar_t* _Name)  { return __removeProperty(_Name)   ; } 
	void                 remove_property(xPropertyItem* pItem) 	{ return __removeProperty(pItem)   ; }     
	void                 remove_descriptor(const wchar_t* _Name){ return __removeDescriptor(_Name) ; } 
	void                 remove_descriptor(xProperty* pItem)    { return __removeDescriptor(pItem) ; }                              
	~xPropertyItem();

protected:
	template<typename T> xProperty* t_add_descriptor(const wchar_t* _name ,  const T& _value)
	{
		xProperty*  pProperty = descriptor(_name);
		if(pProperty)
			return pProperty;
		pProperty = new xProperty(_name , new TPropertyValue<T>(_value , 0) );
		m_Descriptors.insert(DescriptorMap::value_type(std::ds_wstring(_name) , pProperty) );
		return pProperty;
	}

	template<typename T> xPropertyItem* t_add_property(const wchar_t* _name ,  T   _v)
	{
		xPropertyItem*  pProperty = get_property(_name);
		if(pProperty)
			return pProperty;
		pProperty = new xPropertyItem(_name , _v);
		__insertProperty(_name , pProperty) ;
		return pProperty;
	}
protected:
	DescriptorMap                         m_Descriptors;	
	PropertyMap                           m_mapChildren;
    PropertyItems                         m_vChildren;
	xEventHanlder *                       m_pEventHandler;
	void*                                 m_pUserData;
protected:
	void  __insertProperty(const wchar_t* _name , xPropertyItem* pItem);
	void  __removeProperty(const wchar_t* _name);
	void  __removeProperty(xPropertyItem* pProperty);
	void  __removeDescriptor(xProperty* pDescritor);
	void  __removeDescriptor(const wchar_t* _name);
};

class _XEVOL_BASE_API_ xPropertySet : public xPropertyItem
{
public:
	xPropertySet(const wchar_t* name , const wchar_t* _desc);
	~xPropertySet();
	bool load(const wchar_t* fileName , const int8* buf , size_t len ) ;
	bool load(xXmlDocument* pXml);
    bool load(xXmlNode* pNode);
	bool add(xXmlNode* pXmlNode, xPropertyItem* pParent);
	bool unload();
};

END_NAMESPACE_XEVOL3D

#endif
