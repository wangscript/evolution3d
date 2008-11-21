#ifndef __XEVOL_BASE_SHADER_H__
#define __XEVOL_BASE_SHADER_H__
#include "xRenderAPI.h"
#include "xShaderSemantic.h"
#include "xShaderNode.h"
#include "xShaderDefine.h"
#include "../XMathLib/XMathLib.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
/*
第一部分：
  shader系统包含的shader组成部分：
  一: 完整的SHader代码，这样的Shader代码独立可执行。不需要shader修改器一类的。
  二: shader基础代码。这样的shader代码可以增加修改器，安装默认修改器代码后可独立执行。
  三: shader修改器代码。类似于一个函数。
  四: shader的输入模块。这样的shader模块，表示一个模块接受什么样的输入参数，
    这样的系统还将对输入数据进行基本处理。
    Vertex Shader通常是一个对应于Input Assembler的系统
    Pixel Shader则对应如何抓取纹理数据和接受GS/VS传递下来的数据。
    举例说，一个视频处理的ps的输入模块可能是一个转换YUV为RGB，并进行亮度，对比度调节的输入模块。
    输入模块和输出模块，修改器系统一起组成一个完整的shader.
    
    shader输入系统除了要对输入参数描述外，还需要对这个系统的输出数据进行定义。这样的结构叫输入描述符
    基础shader代码会包含一个输入描述符

  五:shader输出模块:
    这样的shader模块，表示一个shader处理完后能输出什么样的数据。
    VS的输出模块要和gs/ps的输入系统匹配。
    ps的输出模块则可以定义输出多少个RenderTarget等。
    输出模块的输出参数需要进行描述，以便与对应的输入模块进行匹配。
    基础shader代码会包含一个输出描述符
    
  六:shader的控制文件。
    用来描述用以上描述的5个部分中的那些数据来组成一个完整可用的shader。
    

第二部分：
  shader修改器系统的组成
  (以下各部分均为和具体的shader api有关，比如)

  shader基础代码:
  shader基础代码如果不使用其修改器。则会调用默认的例程，
  这可能用#ifdef来控制。也可能由Shader系统用默认的插入代码。
  shader的修改器描述符：
  定义shader修改器的类型，参数等，还可能定义出默认的插入代码
  shader的修改器代码:
  多个修改器代码组成一个完整的修改器。
  一个修改器代码加入到shader修改器之前需要匹配参数。
  shader修改器堆栈:
  一个用来描述如何用修改器代码组成修改器，并加入到shader代码里形成新shader的结构。

*/

/*
达到的目的：
    1:  这个用到IGpuProgram的Object，可以提交自己的 IGpuConstantTable
	2:  每个IGpuProgram可以有自己的IGpuConstantTable，也可以没有。
	3:  IGpuConstantTable包含了一个IGpuProgram所有的constant table , VertexShader , Pixel Shader , 
	4:  IGpuConstantTable有能力自己设置那些渲染器绑定的Shader参数.
*/


/*
用来更新那些需要自动更新的变量
比如变换矩阵什么的.
*/
class _XEVOL_BASE_API_  IShaderConstBinder : public IRefCountObject
{
public:
	virtual ~IShaderConstBinder(){};
	IShaderConstBinder(){}
	virtual void                  setDirty() = 0;
	//返回true，表示变量被更新了
	virtual bool                  updateConstant(IShaderConstantReflection* pConst) = 0;
	virtual IShaderConstBinder*   createInstance() = 0 ;
};

struct xShaderConstantDesc
{
public:
	unsigned  int           m_bufOffset;//这个Constant在Const Buffer里的偏移位置.
	eShaderVarType          m_varType;//什么用途，向量？颜色？还是矩阵？
	eShaderParamSemantic    m_semantic;//语义？用来定义这个Param是干什么的
	unsigned int            m_element;//如果是数组，则有多长
	std::ds_wstring         m_strName;//常量的名字
	int                     m_hashName;//常量的Hash名字
};

class _XEVOL_BASE_API_  IShaderConstantReflection : public xShaderConstantDesc
{
public:
	IShaderConstBinder*     m_pBinder;//绑定后能自动更新参数.
public:
	PRIVATE virtual  bool setData(void* _data , int dataLen) = 0;
	PRIVATE virtual  bool updateData();
};

class _XEVOL_BASE_API_ IInputBufferReflection : public IRefCountObject
{
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(IInputBufferReflection);
public:
	IInputBufferReflection(){};
	virtual ~IInputBufferReflection(){}

	virtual size_t                 lenght() = 0;
	virtual size_t                 nConstant() = 0;
	virtual IShaderConstantReflection*   constantDesc(size_t idx) = 0;
	virtual bool                   setData(int _off , void* _data , int dataLen) = 0;
};

class _XEVOL_BASE_API_  IShaderParamTable  : public IRefCountObject
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(IShaderParamTable)
public:
	virtual ~IShaderParamTable();
	IShaderParamTable();
	PUBLIC  virtual size_t                 nConstant() = 0;
	PUBLIC  virtual IShaderConstantReflection*   getConstDesc(size_t idx) = 0;
	PUBLIC  virtual IBaseShader*           getShader() = 0;
	
	/*
	自动设置渲染API的状态：
	包括: 
	变换矩阵:          World , View , WorldView , Normal , MVP , InvWorld , InvView , InvWorldView .
	灯光: 
	fog:               Arg , Color.
	TextureStage Arg:  纹理大小
	*/

	PRIVATE virtual bool                   updateRenderContext() = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float  _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int    _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4& _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4& _value) = 0;

	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0) = 0;
};

class _XEVOL_BASE_API_  IBaseShader : public IRenderResource
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IBaseShader)
public:
    IBaseShader(IRenderApi* pOwner);
	virtual ~IBaseShader();
	PRIVATE virtual IShaderParamTable*       createShaderParamTable() = 0;
	PUBLIC  virtual bool                   load(const wchar_t* fileName , unsigned long  arg) = 0;
	PUBLIC  virtual bool                   load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	PUBLIC  virtual bool                   isLoaded()  = 0;
	PUBLIC  virtual bool                   unload() = 0;
	PUBLIC  virtual unsigned long          memUsage() = 0;
	//PUBLIC  virtual IShaderNodePin*      inputPin() = 0;
	//PUBLIC  virtual IShaderNodePin*      outPin() = 0;
	PUBLIC  virtual eResourceType          res_type(){ return RESOURCE_SHADER ; }

public:
	PUBLIC  virtual eShaderType            type() = 0;
};


class _XEVOL_BASE_API_  IGpuProgramParamTable  : public IRefCountObject
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD10GpuProgram)
public:
	virtual ~IGpuProgramParamTable();
	IGpuProgramParamTable();
	PUBLIC  virtual IGpuProgram*           getGpuProgram() = 0;
	PUBLIC  virtual IShaderParamTable*       getShaderParamTab(eShaderType shType) = 0;

	/*
	自动设置渲染API的状态：
	包括: 
	      变换矩阵:          World , View , WorldView , Normal , MVP , InvWorld , InvView , InvWorldView .
	      灯光: 
		  fog:               Arg , Color.
		  TextureStage Arg:  纹理大小

	*/
	//给渲染器使用

	PRIVATE virtual bool                   updateRenderContext() = 0;
	PRIVATE virtual bool                   commit() = 0;

	PUBLIC  virtual bool                   setTexture(const wchar_t* texName     , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTexture(const int texName          , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;

    PUBLIC  virtual bool                   load(xCfgNode* pXMLNode) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float  _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int    _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4& _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4& _value , eShaderType shType = eShader_None) = 0;

	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
};


class _XEVOL_BASE_API_  IGpuProgram : public IRenderResource
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IGpuProgram)
public:
	IGpuProgram(IRenderApi* pOwner);
	virtual ~IGpuProgram();
	PUBLIC virtual bool                    load(const wchar_t* fileName , unsigned long  arg) = 0;
	PUBLIC virtual bool                    load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	PUBLIC virtual bool                    load(const wchar_t* vsName , const wchar_t* psName, const wchar_t* gsName , unsigned long arg) = 0;
	PUBLIC virtual bool                    isLoaded()  = 0;
	PUBLIC virtual bool                    unload() = 0;
	PUBLIC virtual unsigned long           memUsage() = 0;
	PUBLIC virtual eResourceType           res_type(){ return RESOURCE_SHADER ; }
public:
	/*
	GpuProgramConstTable的管理
	*/
	PUBLIC  virtual IGpuProgramParamTable*   createParamTable(bool bLoadDef) = 0;
	PUBLIC  virtual IGpuProgramParamTable*   getParamTable() = 0;
	PUBLIC  virtual void                   setParamTable(IGpuProgramParamTable* pTable) = 0;

	//GpuProgram 对纹理的管理
	PUBLIC  virtual bool                   setTexture(const wchar_t* textureName , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
    PUBLIC  virtual bool                   setTexture(const int texName          , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
	//给渲染器使用
	PRIVATE virtual IGpuProgramParamTable*  getDefaultParamTable() = 0;
	PRIVATE virtual void                  commit() = 0 ;
	
};

END_NAMESPACE_XEVOL3D
#endif
