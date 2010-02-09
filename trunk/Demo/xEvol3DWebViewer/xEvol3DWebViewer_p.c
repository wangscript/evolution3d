

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Feb 08 17:23:35 2010
 */
/* Compiler settings for .\xEvol3DWebViewer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "xEvol3DWebViewer_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _xEvol3DWebViewer_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } xEvol3DWebViewer_MIDL_TYPE_FORMAT_STRING;

typedef struct _xEvol3DWebViewer_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } xEvol3DWebViewer_MIDL_PROC_FORMAT_STRING;

typedef struct _xEvol3DWebViewer_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } xEvol3DWebViewer_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const xEvol3DWebViewer_MIDL_TYPE_FORMAT_STRING xEvol3DWebViewer__MIDL_TypeFormatString;
extern const xEvol3DWebViewer_MIDL_PROC_FORMAT_STRING xEvol3DWebViewer__MIDL_ProcFormatString;
extern const xEvol3DWebViewer_MIDL_EXPR_FORMAT_STRING xEvol3DWebViewer__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICxEvol3DViewer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICxEvol3DViewer_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const xEvol3DWebViewer_MIDL_PROC_FORMAT_STRING xEvol3DWebViewer__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const xEvol3DWebViewer_MIDL_TYPE_FORMAT_STRING xEvol3DWebViewer__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ICxEvol3DViewer, ver. 0.0,
   GUID={0x8E1B0FDB,0x4F99,0x4693,{0xB4,0xB7,0xEF,0x9A,0x00,0xCD,0x6B,0xB2}} */

#pragma code_seg(".orpc")
static const unsigned short ICxEvol3DViewer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ICxEvol3DViewer_ProxyInfo =
    {
    &Object_StubDesc,
    xEvol3DWebViewer__MIDL_ProcFormatString.Format,
    &ICxEvol3DViewer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICxEvol3DViewer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    xEvol3DWebViewer__MIDL_ProcFormatString.Format,
    &ICxEvol3DViewer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ICxEvol3DViewerProxyVtbl = 
{
    0,
    &IID_ICxEvol3DViewer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ICxEvol3DViewer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ICxEvol3DViewerStubVtbl =
{
    &IID_ICxEvol3DViewer,
    &ICxEvol3DViewer_ServerInfo,
    7,
    &ICxEvol3DViewer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    xEvol3DWebViewer__MIDL_TypeFormatString.Format,
    0, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _xEvol3DWebViewer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICxEvol3DViewerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _xEvol3DWebViewer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICxEvol3DViewerStubVtbl,
    0
};

PCInterfaceName const _xEvol3DWebViewer_InterfaceNamesList[] = 
{
    "ICxEvol3DViewer",
    0
};

const IID *  _xEvol3DWebViewer_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _xEvol3DWebViewer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _xEvol3DWebViewer, pIID, n)

int __stdcall _xEvol3DWebViewer_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_xEvol3DWebViewer_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo xEvol3DWebViewer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _xEvol3DWebViewer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _xEvol3DWebViewer_StubVtblList,
    (const PCInterfaceName * ) & _xEvol3DWebViewer_InterfaceNamesList,
    (const IID ** ) & _xEvol3DWebViewer_BaseIIDList,
    & _xEvol3DWebViewer_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

