#ifndef _XSHADER_NAME_H_
#define _XSHADER_NAME_H_
#include <vector>

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xResPackageMgr.h"

#include "xShaderSemantic.h"
#include "xShaderLexer.h"
#include "xShaderNode.h"
#include "xBaseShader.h"



BEGIN_NAMESPACE_XEVOL3D



class _XEVOL_BASE_API_  xGpuProgramName
{
public:
#ifdef _MSC_VER
	size_t hash_value() const;
#endif
	void           clear();
	void           set(const wchar_t* vs = NULL, const wchar_t* ps = NULL , const wchar_t* gs = NULL);
	void           set(const wchar_t* shaderNames[eShader_Max] );
	void           set(const wchar_t* _shaderName, bool bLoadFromeFile);
	bool           hasShader() const;
	operator size_t(){return (size_t)m_hash1 ; }
	bool           toStdName(const wchar_t* shaderName[]) const;
	bool           toName(std::ds_wstring& name) const;
	xGpuProgramName(const wchar_t* vs = NULL, const wchar_t* ps = NULL , const wchar_t* gs = NULL);
	xGpuProgramName(const wchar_t* shaderNames[eShader_Max]);
	xGpuProgramName(const wchar_t* _shaderName, bool bLoadFromeFile);
public:
	typedef std::pair<std::ds_wstring , eShaderType> ShaderNameValue;
	typedef ds_vector(ShaderNameValue)               vShaders;

public:
	vShaders m_Shaders;
	bool     m_bLoadFromFile;
	int      m_hash1;
};

inline bool operator == (const xGpuProgramName& lhv , const xGpuProgramName& rhv)
{
	return lhv.m_hash1 == rhv.m_hash1;
}

inline bool operator < (const xGpuProgramName& lhv , const xGpuProgramName& rhv)
{
	return lhv.m_hash1 < rhv.m_hash1;
}

inline size_t hash_value(const xGpuProgramName& _Keyval)
{
	return ( _Keyval.hash_value());
}
//====================================================================================================
//Shader 名字四种方式
//1. Simple.shader。 这个.shader文件为一描述文件，这样的Shader文件名可以用xGpuProgNameParser来解析，
//   并可以在.shader里附带参数表。这种shader不能带扩展 Node。
//2. 用三个文件名来指定，这种shader不能带扩展 Node。
//3. 前两种方式用xGpuProgNameParser辅助，可以生成带扩展Node的Shader. 第一种方式的.Shader文件中可以指定使用那些Node
//3. 用三个标准Shader名来指定，标准ShaderName格式为 SHADERNAME<[SOCKET_ID|SOCKECT_NAME]:NODE,NODE... ; [SOCKET_ID|SOCKECT_NAME]:NODE,NODE... ....>
//            例如 Glow<VertexTrans:Skin,Morph; 1:Glow>
//                 Video<0:YUVMosic,VideoParam,Gray>
//            都是标准的ShaderName
//====================================================================================================

class _XEVOL_BASE_API_  xGpuProgNameParser
{
public:
	struct xShaderSocketInfo;
	struct xShaderName;
	typedef ds_vector(xShaderSocketInfo)  xShaderSocketInfos;
public:
	xGpuProgNameParser(xGpuProgramName& _name);
	xGpuProgNameParser();
	//===========================================
	bool          parse(const xGpuProgramName& _name);
	bool          parse(const char* _shaderNameString);//从.shader文件里解析
	void          toName(xGpuProgramName& _name);
	bool          setShaderName(eShaderType _shaderType , const wchar_t* _shaderName);//_shaderName为标准Shader名
	void          addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName, const wchar_t* _socketName   );
	void          addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName, size_t _socketIdx           = 0  );
	bool          generateCode(eShaderType _shaderType  , xShaderCodeNodeMgr* pNodeMgr , ds_string& _code);
	xShaderName*  getShaderName(eShaderType _shaderType){return _getShader(_shaderType) ; }

public:
	struct xShaderSocketInfo
	{
		//可以使用Name也可以使用idx
		ds_wstring              m_name;//使用idx时候m_name = ""
		int                     m_idx;//使用name时候m_idx= -1;
		std::ds_wstring_vector  m_attachNodes;
	};
	struct xShaderName
	{
		ds_wstring         m_strName;
		xShaderSocketInfos m_Sokects; 
		xShaderSocketInfo* findSocket(const wchar_t* socketName);
		xShaderSocketInfo* findSocket(const size_t   socketIdx);
		void               toName(ds_wstring& strName);
	};
protected:
	xShaderName*  _getShader(eShaderType _shaderType);
	bool          _generateDecl(HShaderCodeNode hCodeNode , ds_string& _code);
	bool          _generateSlotCode(HShaderCodeNode hCodeNode , ds_string& _code);
	bool          _generateSocketCode(xShaderCodeNodeMgr* pNodeMgr , xShaderNodeSocket* pSocket , xShaderSocketInfo* info , ds_string& _code);
protected:


protected:
	xShaderName m_VertexShader;
	xShaderName m_PixelShader;
	xShaderName m_GeomShader;
};


END_NAMESPACE_XEVOL3D
#endif
