#include "../xStdPch.h"
#include  "xRenderApi.h"
#include  "xBaseShader.h"
#include  "xShaderName.h"
#include  "xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D

class xRenderCameraEvent : public IRenderCamera::xCameraChangeEvent
{
	IRenderCamera* m_pRenderCamera;
public:
	bool onChange()
	{
		return m_pRenderCamera->onChange();
	}
	xRenderCameraEvent(IRenderCamera* pCamera)
	{
		m_pRenderCamera = pCamera;
	}
};

IRenderCamera::IRenderCamera()
{
	m_RefCount = 1;
	m_pPropertySet = NULL;
	m_pEventHanlder = new xRenderCameraEvent(this);
}

IRenderCamera::~IRenderCamera()
{
	delete m_pEventHanlder;
}

IRenderApi::IRenderApi()
{
}

IRenderApi::~IRenderApi()
{
}

ICameraPropertySet*  IRenderCamera::getPropertySet()
{
	if(m_pPropertySet == NULL)
	{
		m_pPropertySet = new ICameraPropertySet( L"CameraParam", L"Camera Param");
		initPropertySet();
	}

	return m_pPropertySet;
}



void IRenderCamera::initPropertySet()
{
	m_pPropertySet->clear();
	class xCameraPropertyValueChangeHandler : public xPropertyItem::xEventHanlder
	{
	public:
		bool onChange(void* pUserData)
		{
			IRenderCamera* pCamera = (IRenderCamera*)pUserData;
			pCamera->dirty();
			return true;
		}
	};
	static xCameraPropertyValueChangeHandler g_CameraChangeHandler;

#define ADD_CAMERA_DESCRIPTOR(name , _value)                   pPropertyItem->add_descriptor(name    , _value);
#define ADD_CAMERA_EVENTHANLDER(handler , _userData)           pPropertyItem->setEventHanlder(handler, _userData);
#define DECL_ADD_CAMERA_PROPERTY_REF(name, type , member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_pPropertySet->add_property(name , new TPropertyValueRef<type>(member , 0) );
#define DECL_ADD_CAMERA_PROPERTY_VALUE(name, type , pValue){ xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_PropertySet->add_property(name , pValue );
#define DECL_ADD_CAMERA_PROPERTY_REF_STRING(name, type, member)  { xPropertyItem* pPropertyItem = NULL; pPropertyItem = m_PropertySet->add_property(name , new xPropertyStringValue<type>(member , 0)    );
#define END_ADD_CAMERA_PROPERTY(name)  pPropertyItem = NULL; }

	DECL_ADD_CAMERA_PROPERTY_REF(L"相机参数" , IRenderCamera, *this );
	ADD_CAMERA_DESCRIPTOR(L"data_type"    , L"cameraParam");
	ADD_CAMERA_DESCRIPTOR(L"control_type" , L"cameraParam");
	ADD_CAMERA_EVENTHANLDER(&g_CameraChangeHandler , (void*)this);
	END_ADD_CAMERA_PROPERTY(L"相机参数");

}



void IRenderCamera::toTopView()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float fLen = dir.len();

	m_Eye.m_EyePos.x = m_Eye.m_EyeTarget.x;
	m_Eye.m_EyePos.y = m_Eye.m_EyeTarget.y;
	m_Eye.m_EyePos.z = m_Eye.m_EyeTarget.z + fLen;

	m_Eye.m_Up.x = 0.0f;
	m_Eye.m_Up.y = 1.0f;
	m_Eye.m_Up.z = 0.0f;


	dirty();

}

void IRenderCamera::toLeftView()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float fLen = dir.len();

	m_Eye.m_EyePos.x = m_Eye.m_EyeTarget.x - fLen;
	m_Eye.m_EyePos.y = m_Eye.m_EyeTarget.y;
	m_Eye.m_EyePos.z = m_Eye.m_EyeTarget.z;
	m_Eye.m_Up.x = 0.0f;
	m_Eye.m_Up.y = 0.0f;
	m_Eye.m_Up.z = 1.0f;
	dirty();

}

void IRenderCamera::toRightView()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float fLen = dir.len();

	m_Eye.m_EyePos.x = m_Eye.m_EyeTarget.x + fLen;
	m_Eye.m_EyePos.y = m_Eye.m_EyeTarget.y;
	m_Eye.m_EyePos.z = m_Eye.m_EyeTarget.z;
	m_Eye.m_Up.x = 0.0f;
	m_Eye.m_Up.y = 0.0f;
	m_Eye.m_Up.z = 1.0f;
	dirty();

}

void IRenderCamera::toBackView()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float fLen = dir.len();

	m_Eye.m_EyePos.x = m_Eye.m_EyeTarget.x;
	m_Eye.m_EyePos.y = m_Eye.m_EyeTarget.y + fLen;
	m_Eye.m_EyePos.z = m_Eye.m_EyeTarget.z;
	m_Eye.m_Up.x = 0.0f;
	m_Eye.m_Up.y = 0.0f;
	m_Eye.m_Up.z = 1.0f;
	dirty();

}

void IRenderCamera::toFrontView()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float fLen = dir.len();

	m_Eye.m_EyePos.x = m_Eye.m_EyeTarget.x;
	m_Eye.m_EyePos.y = m_Eye.m_EyeTarget.y - fLen;
	m_Eye.m_EyePos.z = m_Eye.m_EyeTarget.z;
	m_Eye.m_Up.x = 0.0f;
	m_Eye.m_Up.y = 0.0f;
	m_Eye.m_Up.z = 1.0f;

	dirty();
}

END_NAMESPACE_XEVOL3D
