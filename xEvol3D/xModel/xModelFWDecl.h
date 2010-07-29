#ifndef __XMODEL_FW_DECL_H__
#define __XMODEL_FW_DECL_H__
#include "../BaseLib/xEvol3DAPI.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xResourceMgr.h"
BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_ xCoreActionFrame;
class  _XEVOL_BASE_API_ xBoneHiberarchy;
class  _XEVOL_BASE_API_ xCoreSkeleton;
class  _XEVOL_BASE_API_ xBaseAction;
class  _XEVOL_BASE_API_ xCoreActionLoader;
class  _XEVOL_BASE_API_ xCoreActionName;

class _XEVOL_BASE_API_ xBaseModel;
class _XEVOL_BASE_API_ xBaseModelLoader;
class _XEVOL_BASE_API_ xCoreMeshName;

class _XEVOL_BASE_API_ xCoreMesh;
class _XEVOL_BASE_API_ xCoreMeshLoader;

typedef std::vector<xCoreMesh*   , dllsafe_alloc<xCoreMesh*>   > vCoreMeshs;

typedef TResHandle<xBaseAction , xCoreActionName , xCoreActionLoader>          HCoreAction;
typedef TLRUPolicyResMgr<xBaseAction , xCoreActionName , xCoreActionLoader>    xCoreActionMgr;

typedef TResHandle<xBaseModel , std::ds_wstring , xBaseModelLoader>          HBaseModel;
typedef TLRUPolicyResMgr<xBaseModel , std::ds_wstring , xBaseModelLoader>    xBaseModelMgr;

typedef TResHandle<xCoreMesh        , xCoreMeshName , xCoreMeshLoader>       HCoreMesh;
typedef TLRUPolicyResMgr<xCoreMesh  , xCoreMeshName , xCoreMeshLoader>       xCoreMeshMgr;

typedef std::vector<HCoreMesh , dllsafe_alloc<HCoreMesh>       >       vHCoreMeshs;

END_NAMESPACE_XEVOL3D
#endif
