#ifndef __XEVOL_D9_SHADER_CONSTANT_BUFFER_H__
#define __XEVOL_D9_SHADER_CONSTANT_BUFFER_H__
#include <RenderAPI/xBaseShader.h>
#include <vector>
#include <map>
#include <string>
#include "xD3DInputBuffer.h"
#include "xD3D9InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D
class  xD3D9ConstBufferReflection;
class xD3D9ConstantBuffer: public IInputBuffer
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9ConstantBuffer);
    xD3D9RenderApi*             m_pD3D9RenderAPI;
    char *                      m_pBuffer;

    size_t                      m_nElement;
    size_t                      m_nBytePerVertex;  
    size_t                      m_iBufLen;
    xD3D9ConstBufferReflection* m_pReflection;
    bool _destory() ; 
public:
    void* handle(){ return m_pBuffer ; }
    xD3D9ConstantBuffer(xD3D9RenderApi* pAPI  );
    ~xD3D9ConstantBuffer();
    bool create(size_t nElement , size_t bytePerVertex ,const xInputBufferDesc* pDesc , void* pInitData  ) ;
    bool create(size_t iBufLen , const xInputBufferDesc* pDesc , void* pInitData  ) ;
    bool lock(eLockPolicy lockPolicy , void** pData);
    bool unlock();
    bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    IInputBufferReflection* reflection();
    IInputBufferReflection* createReflection(bool bSysBuffe);
    virtual size_t          nElement(){ return m_nElement ; }
    virtual size_t          bufLen(){ return m_iBufLen ; }
};

class  xD3D9ConstBufferReflection;
class xD3D9ShaderConstReflection : public IShaderConstantReflection
{
public:
    eD3DResourceBindType        m_bindType;
    xD3D9ConstBufferReflection* m_pBufferReflection;
    std::wstring                m_Name;
    D3DXHANDLE                  m_hConstHandle;
    ID3DXConstantTable*         m_pConstTable;
    int                         m_Bytes;
    xD3D9RenderApi*             m_pD3D9Api;
public:
    xD3D9ShaderConstReflection(const wchar_t* _name);
    ~xD3D9ShaderConstReflection();
    bool setData(int _offset , void* _data , int dataLen);
    bool setData(void* _data , int dataLen);
};

typedef ds_vector(xD3D9ShaderConstReflection*)     vecD3D9ShaderConstantDescs;
typedef ds_map(int , xD3D9ShaderConstReflection*)  mapD3D9ShaderConstantDescs;

class xD3D9ConstBufferReflection : public IInputBufferReflection
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9ConstBufferReflection);
public:
    xD3D9ConstBufferReflection(IInputBuffer* pBuf , bool bSysBuffer , int bufLen);
    ~xD3D9ConstBufferReflection();
    size_t                       lenght();
    size_t                       nConstant();
    IShaderConstantReflection*   constantDesc(size_t idx);
    //void                         addConstant(xD3D9ShaderConstReflection* cd);
    xD3D9ShaderConstReflection*  addConstant(const wchar_t* _name);
    void*                        systemBuffer();
    void                         setDirty(bool bDirty = true) {m_bDirty = bDirty; }
    bool                         dirty(){return m_bDirty ; }
public:
    bool                         setData(int _off , void* _data , int dataLen);
    bool                         flushToBuffer();
protected:
    vecD3D9ShaderConstantDescs   m_vConstant;
    int8*                        m_pSysMem;
    int                          m_bindPoint;
    bool                         m_bDirty;
    size_t                       m_BufLen;
    IInputBuffer*                m_pInputBuffer;
};

END_NAMESPACE_XEVOL3D

#endif
