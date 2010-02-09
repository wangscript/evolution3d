#ifndef _XSCENE_PROPERTY_TOOL_H_
#define _XSCENE_PROPERTY_TOOL_H_

//声明一个属性集合, 这一组宏用来需要声明一个属性变量的类
#define BEGIN_SCENE_NODE_PROPERTY_SET(PropClass , BassClass)                                                             \
protected:                                                                                                   \
	virtual ISceneNodePropertySet* getPropertySet() { this->initPropertySet() ; return & m_PropertySet ; }   \
protected:                                                                                                   \
	PropClass                   m_PropertySet;                                                               \
	bool    initPropertySet() {                                                                              \
	if(m_bPropertySetInited == true)                                                                            \
	return true;                                                                                             \
	m_bPropertySetInited = true;                                                                                \
	BassClass::initPropertySet();                                                                            \



#define END_SCENE_NODE_PROPERTY_SET(BassClass)   return true; }

#define DECL_SCENE_PROPERTY_REF(name, type , member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_PropertySet.add_property(name , new TPropertyValueRef<type>(member , 0) );
#define DECL_SCENE_PROPERTY_VALUE(name, pValue){ xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_PropertySet.add_property(name , pValue );
#define DECL_SCENE_PROPERTY_REF_STRING(name, type , member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_PropertySet.add_property(name , new xPropertyStringValue<type>(member , 0)    );
#define END_SCENE_PROPERTY(name)  pPropertyItem = NULL; }

#define ADD_DESCRIPTOR(name , _value)                   pPropertyItem->add_descriptor(name    , _value);
#define ADD_EVENTHANLDER(handler , _userData)           pPropertyItem->setEventHanlder(handler, _userData);
//#define BEGIN_DESCRIPTOR(name , _value)                 {  xPropertyItem* pDescriptor = pPropertyItem->add_descriptor(name    , _value);
//#define ADD_SUB_DESCRIPTOR(name , _value)                     pDescriptor->add_descriptor(name    , _value);
//#define END_DESCRIPTOR(name)                   pDescriptor = NULL ; }


//这一组宏适合不需要声明属性变量的类
#define BEGIN_INIT_SCENE_NODE_PROPERTY(BassClass)                                              \
	protected:                                                                                 \
	bool    initPropertySet() {                                                                \
		BassClass::initPropertySet();                                                          \

#define END_INIT_SCENE_NODE_PROPERTY_SET(BassClass)   return true; }

#define DECL_ADD_SCENE_PROPERTY_REF(name, type , member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = getPropertySet()->add_property(name , new TPropertyValueRef<type>(member , 0) );
#define DECL_ADD_SCENE_PROPERTY_VALUE(name, type , pValue){ xPropertyItem* pPropertyItem = NULL; pPropertyItem = getPropertySet()->add_property(name , pValue );
#define DECL_ADD_SCENE_PROPERTY_REF_STRING(name, type, member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = getPropertySet()->add_property(name , new xPropertyStringValue<type>(member , 0)    );
#define END_ADD_SCENE_PROPERTY(name)  pPropertyItem = NULL; }


//在这个类里使用PropertySet对象。如果是在这个类里声明了m_pPropertySet对象，就用调用这个宏。
//这个宏设置父类的m_pPropertySet对象的指针。
#define USE_THISCLASS_PROPERTY() m_pPropertySet = &m_PropertySet; m_pPropertySet->AddRef();  
#endif

