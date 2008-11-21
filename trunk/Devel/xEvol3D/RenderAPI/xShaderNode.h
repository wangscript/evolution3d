#ifndef _XSHADER_NODE_H_
#define _XSHADER_NODE_H_
#include "xRenderAPI.h"
#include "xShaderSemantic.h"
#include "../BaseLib/xResPackageMgr.h"
#include "xShaderLexer.h"
BEGIN_NAMESPACE_XEVOL3D



//=================================================
typedef std::vector<xShaderCodeSection* , std::_dllsafe_alloc<xShaderCodeSection*> > xShaderCodeSections;
typedef std::vector<xShaderNodeSocket * , std::_dllsafe_alloc<xShaderNodeSocket *> > xShaderNodeSockets;

class _XEVOL_BASE_API_  xShaderCodeNode
{
	xShaderCodeSections        m_vSections;
	std::ds_string             m_code;
	xShaderNodeSockets         m_vSockets;
	xShaderNodeSlot*           m_pSlot;
	ds_wstring_vector          m_ImportNode;
	std::ds_wstring            m_nodeName;
	int                        load_Action(const char* _code);
public:
	//Resource interface
	virtual bool                 load(const wchar_t* nodeName ,unsigned  int arg);
	virtual bool                 load(const wchar_t* nodeName , const unsigned int8* buf , unsigned int len , unsigned int arg );
	virtual bool                 unload();
	virtual int                  memUsage();
	


	//============================================
	virtual ds_wstring_vector&   importLibs(){return m_ImportNode ; };
	virtual xShaderNodeSockets&  sockets();
	virtual xShaderNodeSlot*     slot();
	virtual bool                 isSkeletonNode();
	virtual bool                 isSlotNode();
	virtual xShaderNodeSocket*   findSocket(const wchar_t* _name);
	virtual xShaderNodeSocket*   findSocket(int idx);
	virtual xShaderNodeSocket*   findSocket(int idx , const wchar_t* _name);
	virtual int                  findSocketIdx(const wchar_t* _name);
	virtual xShaderNodeSlot*     getSlotInfo();
	virtual xShaderCodeSections& sections();
	virtual const wchar_t*       name(){return m_nodeName.c_str();}

	//============================================
	virtual xShaderCodeSection*  createSection(const char* _typeName , const char* _name);
	virtual bool                 aferSectionLoad(xShaderCodeSection* pSection);
								 
	virtual bool                 generateCode(std::ds_string& code);
	virtual const char*          code(){ return m_code.c_str() ; };                 

	static xShaderCodeNode*      newInstance(const wchar_t* strResName);
	static void                  deleteInstance(xShaderCodeNode* pRes);
	//							 
	xShaderCodeNode();			 
	~xShaderCodeNode();

  




};

//支持从打包器里读取ShaderCode
typedef TResHandle<xShaderCodeNode      , std::ds_wstring , TResPkgLoader2<xShaderCodeNode> > HShaderCodeNode;
typedef TSimpleResMgr<xShaderCodeNode   , std::ds_wstring , TResPkgLoader2<xShaderCodeNode> > xShaderCodeNodeMgr;
typedef vector<HShaderCodeNode>  vHShaderCodeNodes;
END_NAMESPACE_XEVOL3D
#endif
