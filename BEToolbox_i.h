

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Jan 19 13:40:54 2016
 */
/* Compiler settings for .\BEToolbox.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
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


#ifndef __BEToolbox_i_h__
#define __BEToolbox_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __BEToolboxPlugin_FWD_DEFINED__
#define __BEToolboxPlugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class BEToolboxPlugin BEToolboxPlugin;
#else
typedef struct BEToolboxPlugin BEToolboxPlugin;
#endif /* __cplusplus */

#endif 	/* __BEToolboxPlugin_FWD_DEFINED__ */


#ifndef __BEToolboxComponentInfo_FWD_DEFINED__
#define __BEToolboxComponentInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class BEToolboxComponentInfo BEToolboxComponentInfo;
#else
typedef struct BEToolboxComponentInfo BEToolboxComponentInfo;
#endif /* __cplusplus */

#endif 	/* __BEToolboxComponentInfo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __BEToolboxLib_LIBRARY_DEFINED__
#define __BEToolboxLib_LIBRARY_DEFINED__

/* library BEToolboxLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BEToolboxLib;

EXTERN_C const CLSID CLSID_BEToolboxPlugin;

#ifdef __cplusplus

class DECLSPEC_UUID("EA33DB99-86FB-4678-9442-AB1D334456E4")
BEToolboxPlugin;
#endif

EXTERN_C const CLSID CLSID_BEToolboxComponentInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("71B4631C-5AB5-468e-A1AD-5764A728E419")
BEToolboxComponentInfo;
#endif
#endif /* __BEToolboxLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


