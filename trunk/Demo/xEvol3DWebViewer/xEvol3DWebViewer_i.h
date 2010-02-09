

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __xEvol3DWebViewer_i_h__
#define __xEvol3DWebViewer_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICxEvol3DViewer_FWD_DEFINED__
#define __ICxEvol3DViewer_FWD_DEFINED__
typedef interface ICxEvol3DViewer ICxEvol3DViewer;
#endif 	/* __ICxEvol3DViewer_FWD_DEFINED__ */


#ifndef __CxEvol3DViewer_FWD_DEFINED__
#define __CxEvol3DViewer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CxEvol3DViewer CxEvol3DViewer;
#else
typedef struct CxEvol3DViewer CxEvol3DViewer;
#endif /* __cplusplus */

#endif 	/* __CxEvol3DViewer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICxEvol3DViewer_INTERFACE_DEFINED__
#define __ICxEvol3DViewer_INTERFACE_DEFINED__

/* interface ICxEvol3DViewer */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICxEvol3DViewer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8E1B0FDB-4F99-4693-B4B7-EF9A00CD6BB2")
    ICxEvol3DViewer : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICxEvol3DViewerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICxEvol3DViewer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICxEvol3DViewer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICxEvol3DViewer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICxEvol3DViewer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICxEvol3DViewer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICxEvol3DViewer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICxEvol3DViewer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICxEvol3DViewerVtbl;

    interface ICxEvol3DViewer
    {
        CONST_VTBL struct ICxEvol3DViewerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxEvol3DViewer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICxEvol3DViewer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICxEvol3DViewer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICxEvol3DViewer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICxEvol3DViewer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICxEvol3DViewer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICxEvol3DViewer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICxEvol3DViewer_INTERFACE_DEFINED__ */



#ifndef __xEvol3DWebViewerLib_LIBRARY_DEFINED__
#define __xEvol3DWebViewerLib_LIBRARY_DEFINED__

/* library xEvol3DWebViewerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_xEvol3DWebViewerLib;

EXTERN_C const CLSID CLSID_CxEvol3DViewer;

#ifdef __cplusplus

class DECLSPEC_UUID("189C05E3-2961-4C64-87C1-886B04F4145B")
CxEvol3DViewer;
#endif
#endif /* __xEvol3DWebViewerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


