///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BEToolbox_i.h"
#include "BEToolboxApp.h"
#include "BEToolboxComponentInfo.h"
#include "resource.h"
#include <MFCTools\VersionInfo.h>
#include <EAF\EAFApp.h>
#include <EAF\EAFUtilities.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HRESULT CBEToolboxComponentInfo::FinalConstruct()
{
   return S_OK;
}

void CBEToolboxComponentInfo::FinalRelease()
{
}

BOOL CBEToolboxComponentInfo::Init(CEAFApp* pApp)
{
   return TRUE;
}

void CBEToolboxComponentInfo::Terminate()
{
}

CString CBEToolboxComponentInfo::GetName()
{
   return _T("BEToolbox");
}

CString CBEToolboxComponentInfo::GetDescription()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxApp* pApp = (CBEToolboxApp*)AfxGetApp();
   CString strExe = pApp->m_pszExeName;
   strExe += _T(".dll");

   CVersionInfo verInfo;
   verInfo.Load(strExe);
   CString strVersion = verInfo.GetFileVersionAsString();
   CString strCopyright = verInfo.GetLegalCopyright();

   CString strDesc;
   strDesc.Format(_T("Bridge Engineer's Toolbox\nVersion %s\n%s"),strVersion,strCopyright);
   return strDesc;
}

HICON CBEToolboxComponentInfo::GetIcon()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return AfxGetApp()->LoadIcon(IDR_BETOOLBOX);
}

bool CBEToolboxComponentInfo::HasMoreInfo()
{
   return false;
}

void CBEToolboxComponentInfo::OnMoreInfo()
{
}
