#ifndef _XSHADER_MANGER_H_
#define _XSHADER_MANGER_H_
#include <vector>

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xResPackageMgr.h"

#include "xShaderSemantic.h"
#include "xShaderLexer.h"
#include "xShaderNode.h"
#include "xBaseShader.h"
#include "xShaderName.h"


BEGIN_NAMESPACE_XEVOL3D

//这里是主角: xGpuProgramManager
typedef TResHandle   <IGpuProgram, xGpuProgramName , xGpuProgramManager > HGpuProgram;
typedef TSimpleResMgr<IGpuProgram, xGpuProgramName , xGpuProgramManager > xGpuProgramMgrImp;

class _XEVOL_BASE_API_  xGpuProgramManager : public xResPathManager
{
public:
	void            setRenderApi(IRenderApi* pRenderApi , const wchar_t* _defExt);
	IBaseShader*    loadShader(const wchar_t* _vsName , eShaderType _shaderType);
	HGpuProgram     loadEffect(const wchar_t* _effName);
	HGpuProgram     load(const wchar_t* _programName);
	HGpuProgram     load(const wchar_t* _vsName , const wchar_t* _psName , const wchar_t* _gsName);
	HGpuProgram     load(const xGpuProgramName& _programName);
	void            clear();
    void            unload();
	xShaderCodeNodeMgr* codeManager(){return &m_CodeMgr;}
public:
	//Path Manager;
	void            clearAllSearchPath();
	bool            addPackage(xcomdoc& package,const wchar_t* ResDir);
	bool            addPackage(const wchar_t* package_file,const wchar_t* ResDir);
	bool            addPath(const wchar_t* ResDir);

public:
	virtual ~xGpuProgramManager();
	xGpuProgramManager();
protected:
	unsigned int        _getResSize(IGpuProgram* pRes);
	virtual bool        _isResLoaded(IGpuProgram* pRes);
	virtual bool        _loadResource(const xGpuProgramName&  strResName  , IGpuProgram* & pRes , int& ResSize, unsigned int arg);
	virtual bool        _loadResource(const wchar_t* strResName, IGpuProgram* & pRes , int& ResSize, unsigned int arg);
	virtual bool        _unloadResource(const xGpuProgramName& strResName , IGpuProgram* & pRes , unsigned int& TotalResSize);
	virtual void        _deleteResource(const xGpuProgramName& strResName , IGpuProgram*& pRes);
protected:
    IRenderApi*         m_pRenderApi;
	xShaderCodeNodeMgr  m_CodeMgr;
	ds_wstring          m_defExt;
};

class  _XEVOL_BASE_API_  xBaseShaderLoader
{
	xShaderCodeNodeMgr*  m_pCodeMgr;
	IRenderApi*          m_pRenderApi;
	bool                 m_bPreGen;
	std::ds_wstring      m_preGenFolder;
	ds_wstring           m_defExt;
public:	
	xBaseShaderLoader();
	void         setRenderApi(IRenderApi* pRenderApi , xShaderCodeNodeMgr*  pCodeMgr , const wchar_t* defExt);
	void         setOutputForlder(const wchar_t* _path);
	void         enablePreGenerator(bool bEnable);
protected:
	virtual bool _isResLoaded(IBaseShader* pRes);
	virtual bool _loadResource(const std::ds_wstring& strResName   , IBaseShader* & pRes , int& ResSize, unsigned int arg);
	virtual bool _unloadResource(const std::ds_wstring& strResName , IBaseShader* & pRes , unsigned int& TotalResSize);
	virtual void _deleteResource(const std::ds_wstring& strResName , IBaseShader* pRes);
};

typedef TResHandle   <IBaseShader , std::ds_wstring , xBaseShaderLoader> HBaseShader;
typedef TSimpleResMgr<IBaseShader , std::ds_wstring , xBaseShaderLoader> xBaseShaderMgr;


END_NAMESPACE_XEVOL3D
#endif
