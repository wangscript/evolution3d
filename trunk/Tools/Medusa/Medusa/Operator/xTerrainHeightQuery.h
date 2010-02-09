#ifndef _MEDUSA_TERRAIN_BLOCK_SELECTOR_
#define _MEDUSA_TERRAIN_BLOCK_SELECTOR_
#include <RenderApi/xRenderApi.h>
#include "RenderAPI/xBaseShader.h"
#include "../xMedusaEditor.h"
#include <string>
#include <vector>
BEGIN_NAMESPACE_XEVOL3D

class MEDUSA_API xTerrainHeightSelector : public IColorSelector
{
	HBaseShader                m_hColorSelShader;
	IColorSelector*            m_pBasicSelector;
	IRenderApi*                m_pRenderApi;
	IRenderView*               m_SelectRenderView;
	ePIXEL_FORMAT              m_fmt;
	IBlenderState*             m_ColorSelBlender;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xTerrainHeightSelector);
public:
	xTerrainHeightSelector(IRenderApi* pApi , IColorSelector* pBasicSelect);
	~xTerrainHeightSelector();
	IRenderApi* renderApi() { return m_pRenderApi ; }
	bool setRenderObjectID(int id1 , int id2);
	bool setRenderObjMasterID(int _id);
	bool setRenderObjSlaveID(int _id);
	bool begin();
	bool end();
	bool beginPrimitive();
	bool endPrimitive();
	bool getSelectID(int x , int y , int w , int h, void* _data);
	bool create(xXmlNode* pNode);
	bool resize(int w , int h );
};

END_NAMESPACE_XEVOL3D

#endif

