#ifndef _XEVOL3D_RENDER_API_FW_DECL_H_
#define _XEVOL3D_RENDER_API_FW_DECL_H_

#include "../BaseLib/xEvol3DAPI.h"
#include "../BaseLib/xResourceMgr.h"
BEGIN_NAMESPACE_XEVOL3D
//Forword decl baselib
class _XEVOL_BASE_API_  xXmlNode;

//===================================
class _XEVOL_BASE_API_  IRenderApiObject;
class _XEVOL_BASE_API_  IRenderApiResource;
class _XEVOL_BASE_API_  IRenderApiState;
class _XEVOL_BASE_API_  ISamplerState;
class _XEVOL_BASE_API_  IBlenderState;
class _XEVOL_BASE_API_  IRasterizerState;
class _XEVOL_BASE_API_  IDepthStencilState;
class _XEVOL_BASE_API_  ILightingEnv;

class _XEVOL_BASE_API_  IHeplerRenderApi;
class _XEVOL_BASE_API_  IRenderApi;
class _XEVOL_BASE_API_  IBaseTexture ;
class _XEVOL_BASE_API_  IRenderTarget;
struct _XEVOL_BASE_API_  xRTSampleDesc;
class _XEVOL_BASE_API_  IRenderCamera;
class _XEVOL_BASE_API_  xRenderAPIDefInfo;
class _XEVOL_BASE_API_  IRenderApiCreator;
class _XEVOL_BASE_API_  xRendererAPIManager;
class _XEVOL_BASE_API_  ITextureLoader;
class _XEVOL_BASE_API_  xTextureLdrMgr;
struct _XEVOL_BASE_API_ xTextureName;

class _XEVOL_BASE_API_    I2DRectObject;
class  _XEVOL_BASE_API_   xInputElementTypeInfo;
class  _XEVOL_BASE_API_   IInputBuffer;
class  _XEVOL_BASE_API_   xInputLayoutDesc;
class  _XEVOL_BASE_API_   IInputAssembler;
class  _XEVOL_BASE_API_   IVertexStream ;
class  _XEVOL_BASE_API_   IInputBufferReflection;
struct xInputElement;
struct xShaderVarInfo;
struct xInputBufferDesc;
struct xTextureLockArea;
struct xTextureDesc;
class _XEVOL_BASE_API_  IBaseShader;
class _XEVOL_BASE_API_  IGpuProgram;
class _XEVOL_BASE_API_  IGpuProgramParamTable;
class _XEVOL_BASE_API_  IShaderConstBinder;
class _XEVOL_BASE_API_  IShaderParamTable;

struct xShaderVarInfo;
class _XEVOL_BASE_API_  xShaderVarParser;
class _XEVOL_BASE_API_  xShaderVariable;
class _XEVOL_BASE_API_  xShaderPin;
class _XEVOL_BASE_API_  xShaderNodeSocket;
class _XEVOL_BASE_API_  xShaderNodeSlot;
class _XEVOL_BASE_API_  xShaderCodeSection;
class _XEVOL_BASE_API_  xShaderCodeNode;
class _XEVOL_BASE_API_  xShaderSemanticTable;
class _XEVOL_BASE_API_  IShaderCodeGenerator;

class _XEVOL_BASE_API_  xBaseShaderLoader;
class _XEVOL_BASE_API_  xGpuProgramManager;
class _XEVOL_BASE_API_  xGpuProgNameParser;
class _XEVOL_BASE_API_  xGpuProgramName;
class _XEVOL_BASE_API_  IShaderConstantReflection;
class _XEVOL_BASE_API_  IShaderCodeGenerator;

typedef TResHandle   <IGpuProgram, xGpuProgramName , xGpuProgramManager > HGpuProgram;
typedef TSimpleResMgr<IGpuProgram, xGpuProgramName , xGpuProgramManager > xGpuProgramMgrImp;
typedef TResHandle   <IBaseShader , std::ds_wstring , xBaseShaderLoader>  HBaseShader;
typedef TSimpleResMgr<IBaseShader , std::ds_wstring , xBaseShaderLoader>  xBaseShaderMgr;

class _XEVOL_BASE_API_ IRenderView;
class _XEVOL_BASE_API_ IRenderTarget;

enum eShaderParamSemantic;
enum eShaderType;
enum eLockPolicy;
enum ePrimtiveType;
enum eMatrixMode;
enum eShaderVarSemantic;
enum eShaderVarDataType;
enum eShaderVarType;
enum eInputPerVertUsage;
enum eResourceAccessFlage;
enum eResourceUsage;


//Shader的语义定义
#define SHADER_SEMANTIC_POSITION      (wchar_t*)L"POSITION"
#define SHADER_SEMANTIC_NORMAL        (wchar_t*)L"NORMAL"
#define SHADER_SEMANTIC_WEIGHT        (wchar_t*)L"BLENDWEIGHT"
#define SHADER_SEMANTIC_BLENDINDICES  (wchar_t*)L"BLENDINDICES"
#define SHADER_SEMANTIC_TANGENT       (wchar_t*)L"TANGENT"
#define SHADER_SEMANTIC_BINORMAL      (wchar_t*)L"BINORMAL"
#define SHADER_SEMANTIC_TEXCOORD      (wchar_t*)L"TEXCOORD"
#define SHADER_SEMANTIC_COLOR         (wchar_t*)L"COLOR"
#define SHADER_SEMANTIC_ANY           (wchar_t*)L"ANY"


#include "xRenderApiEnum.h"

END_NAMESPACE_XEVOL3D

#endif
