#ifndef _XGIS_IMAGE_BASEDEF_H_
#define _XGIS_IMAGE_BASEDEF_H_
#include "Application/xPluginMgr.h"
#include "BaseLib/xStringTable.h"
#include "xScene/xSceneGraph.h"
#include "xScene/xSceneNode.h"
#include "xScene/xSceneObject.h"
#include "xScene/xSceneDrawable.h"
#include "xScene/xScenePropertyTool.h"
#include "xScene/xBaseTerrain.h"
#include "Renderer/xRenderer.h"
#include "BaseLib/xarray.h"
#include "BaseLib/stl_dllalloc.h"
#include "BaseLib/xBaseMath.h"
#ifdef  GISTERRAIN_EXPORTS
#define _GISIMAGE_API_  __declspec(dllexport)
#else 
#define _GISIMAGE_API_  __declspec(dllimport)
#endif


BEGIN_NAMESPACE_XEVOL3D
class _GISIMAGE_API_ xPDEMImage;
class _GISIMAGE_API_ xGisTerrainLoDCtrller;
class _GISIMAGE_API_ xGisTerrainMeshObject;
class _GISIMAGE_API_ xGisTerrainScene;


END_NAMESPACE_XEVOL3D

#endif
