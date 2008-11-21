#ifndef _XEVOL_D3D10_BASE_COLOR_SELECTOR_
#define _XEVOL_D3D10_BASE_COLOR_SELECTOR_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include <d3d10.h>
#include <string>
#include <vector>
#include "xD10RenderView.h"
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;

union xPixelSelectID
{
	struct 
	{
		short	sLoWord;
		short	sHiWord;
	};
	struct 
	{
		unsigned short	uLoWord;
		unsigned short	uHiWord;
	};
	unsigned int     _id;
};

class xPixelSelectIDConstBinder : public IShaderConstBinder
{
protected:
	xPixelSelectID* m_id;
public:
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xPixelSelectIDConstBinder);
public:
	xPixelSelectIDConstBinder(){};
	void                  setDirty(){m_id = NULL ; }
	virtual void           setID(xPixelSelectID* _id){m_id = _id ; }
	virtual bool          updateConstant(IShaderConstantReflection* pConst) = 0;
	IShaderConstBinder*   createInstance(){return this ; }
};

class xPixelSelectIDConstBinderS : public xPixelSelectIDConstBinder
{
public:
	bool updateConstant(IShaderConstantReflection* pConst);
};

class xPixelSelectIDConstBinderU : public xPixelSelectIDConstBinder
{
public:
	bool updateConstant(IShaderConstantReflection* pConst);
};

class xD10BaseColorSelector : public IColorSelector
{
	xPixelSelectIDConstBinderS m_ColorSelBinderS;
	xPixelSelectIDConstBinderU m_ColorSelBinderU;
	IBlenderState*             m_ColorSelBlender;
	xD10RenderView*            m_SelectRenderView;
	IBaseShader*               m_pColorSelShader;
	xPixelSelectID             m_SelectID[2];
	xD3D10RenderApi*           m_pD10Api;
	ePIXEL_FORMAT              m_Format;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD10BaseColorSelector);
public:
	xD10BaseColorSelector(xD3D10RenderApi* pD10Api);
	~xD10BaseColorSelector();
	bool setRenderObjectID(int id1 , int id2);
	bool setRenderObjMasterID(int _id);
	bool setRenderObjSlaveID(int _id);
	bool begin();
	bool end();
	bool beginPrimitive();
	bool endPrimitive();
	bool getSelectID(int x , int y , int w , int h, void* _data);
	bool create(xCfgNode* pNode);
	bool resize(int w , int h );
};

END_NAMESPACE_XEVOL3D

#endif

