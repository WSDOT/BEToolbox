

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_BEToolboxLib,0xB00EE67C,0x9FBF,0x42ef,0x8B,0x80,0xCE,0xCD,0x5A,0xE5,0xCC,0x23);


MIDL_DEFINE_GUID(CLSID, CLSID_BEToolboxPlugin,0xB5E128D4,0x78F3,0x48e0,0x97,0x0E,0x81,0x2C,0x22,0x81,0xEE,0x4E);


MIDL_DEFINE_GUID(CLSID, CLSID_BEToolboxComponentInfo,0x8BC4CCD2,0x35D5,0x4f2c,0xAA,0x18,0x0E,0x2D,0x58,0xFF,0x5C,0x3A);


MIDL_DEFINE_GUID(CLSID, CLSID_CurvelExporter,0xE89CC143,0x53C9,0x43df,0xB1,0xB4,0xE5,0x22,0x76,0x9D,0xE2,0x9B);


MIDL_DEFINE_GUID(CLSID, CLSID_CurvelImporter,0xE56801AC,0xB583,0x4e9e,0xA2,0xD8,0x2A,0xA6,0x40,0x79,0xBE,0x15);


MIDL_DEFINE_GUID(CLSID, CLSID_GenCompExporter,0x16A21688,0x25B4,0x4118,0xAB,0xF4,0xDD,0x6D,0x49,0x83,0xA2,0xCD);


MIDL_DEFINE_GUID(CLSID, CLSID_PGStableExporter,0xFF96408C,0x27B3,0x4288,0x8A,0x44,0x6F,0x80,0x65,0x2B,0xE1,0x87);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



