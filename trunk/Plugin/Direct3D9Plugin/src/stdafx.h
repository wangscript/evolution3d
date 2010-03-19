#pragma  once
#ifdef _DEBUG
//#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <D3d9types.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <deque>
#include <new>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include "RenderApi/xRenderApi.h"
#include "RenderApi/xRenderApiImp.h"
#include "BaseLib/xStringHash.h"
#include "RenderApi/xShaderManager.h"
#include "RenderApi/xBaseShader.h"
#include "RenderApi/xRenderStateObjManager.h"
#include "RenderApi/xRenderApiManager.h"
#include "RenderApi/xRenderView.h"
#include "RenderApi/xBaseTexture.h"
#include "BaseLib/xI18N.h"
#include "BaseLib/xBaseMath.h"
#include "OperationSys/xOperationSys.h"

inline void RESET_D3DPOOL(D3DPOOL& d3dPool)
{
    if(d3dPool == D3DPOOL_DEFAULT) d3dPool = D3DPOOL_MANAGED;
}
