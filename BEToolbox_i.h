

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jun 13 12:54:14 2017
 */
/* Compiler settings for BEToolbox.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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


#ifndef __CurvelExporter_FWD_DEFINED__
#define __CurvelExporter_FWD_DEFINED__

#ifdef __cplusplus
typedef class CurvelExporter CurvelExporter;
#else
typedef struct CurvelExporter CurvelExporter;
#endif /* __cplusplus */

#endif 	/* __CurvelExporter_FWD_DEFINED__ */


#ifndef __CurvelImporter_FWD_DEFINED__
#define __CurvelImporter_FWD_DEFINED__

#ifdef __cplusplus
typedef class CurvelImporter CurvelImporter;
#else
typedef struct CurvelImporter CurvelImporter;
#endif /* __cplusplus */

#endif 	/* __CurvelImporter_FWD_DEFINED__ */


#ifndef __GenCompExporter_FWD_DEFINED__
#define __GenCompExporter_FWD_DEFINED__

#ifdef __cplusplus
typedef class GenCompExporter GenCompExporter;
#else
typedef struct GenCompExporter GenCompExporter;
#endif /* __cplusplus */

#endif 	/* __GenCompExporter_FWD_DEFINED__ */


#ifndef __PGStableExporter_FWD_DEFINED__
#define __PGStableExporter_FWD_DEFINED__

#ifdef __cplusplus
typedef class PGStableExporter PGStableExporter;
#else
typedef struct PGStableExporter PGStableExporter;
#endif /* __cplusplus */

#endif 	/* __PGStableExporter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "PGSuperIEPlugin.h"

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

class DECLSPEC_UUID("B5E128D4-78F3-48e0-970E-812C2281EE4E")
BEToolboxPlugin;
#endif

EXTERN_C const CLSID CLSID_BEToolboxComponentInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("8BC4CCD2-35D5-4f2c-AA18-0E2D58FF5C3A")
BEToolboxComponentInfo;
#endif

EXTERN_C const CLSID CLSID_CurvelExporter;

#ifdef __cplusplus

class DECLSPEC_UUID("E89CC143-53C9-43df-B1B4-E522769DE29B")
CurvelExporter;
#endif

EXTERN_C const CLSID CLSID_CurvelImporter;

#ifdef __cplusplus

class DECLSPEC_UUID("E56801AC-B583-4e9e-A2D8-2AA64079BE15")
CurvelImporter;
#endif

EXTERN_C const CLSID CLSID_GenCompExporter;

#ifdef __cplusplus

class DECLSPEC_UUID("16A21688-25B4-4118-ABF4-DD6D4983A2CD")
GenCompExporter;
#endif

EXTERN_C const CLSID CLSID_PGStableExporter;

#ifdef __cplusplus

class DECLSPEC_UUID("FF96408C-27B3-4288-8A44-6F80652BE187")
PGStableExporter;
#endif
#endif /* __BEToolboxLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


