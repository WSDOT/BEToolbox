///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include "..\resource.h"
#include "SpectraChildFrame.h"
#include "SpectraDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool DoesFileExist(const CString& filename)
{
   if (filename.IsEmpty())
   {
      return false;
   }
   else
   {
      CFileFind finder;
      BOOL is_file;
      is_file = finder.FindFile(filename);
      return (is_file!=0);
   }
}


IMPLEMENT_DYNCREATE(CSpectraChildFrame, CEAFChildFrame)

CSpectraChildFrame::CSpectraChildFrame(void)
{
}

CSpectraChildFrame::~CSpectraChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CSpectraChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
   ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_SITE_CLASS, OnSiteClassChanged)
   ON_CBN_SELCHANGE(IDC_SPECIFICATION, OnSpecificationChanged)
   ON_EN_CHANGE(IDC_LATITUDE, OnLatitudeChanged)
   ON_EN_CHANGE(IDC_LONGITUDE, OnLongitudeChanged)
END_MESSAGE_MAP()

LRESULT CSpectraChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_SPECTRA );
   return TRUE;
}

BOOL CSpectraChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   
   // I have no idea why, but creating this window fail the second time a BEToolbox document
   // is created in a single BridgeLink run. For some reason, MFC determines the wrong
   // class name. lpszClass is nullptr the first time this window is created successfully, so
   // we'll force it to nullptr here so it works on subsequent creation attempts
   cs.lpszClass = nullptr;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CSpectraChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_DlgBar.Create(this,IDD_SPECTRA,CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_DlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_SPECTRA),TRUE);
   }

   return 0;
}

void CSpectraChildFrame::OnUpdate()
{
   m_DlgBar.UpdateData(TRUE);

   CSpectraDoc* pDoc = (CSpectraDoc*)GetActiveDocument();
   if ( pDoc )
   {
      pDoc->SetModifiedFlag();
      pDoc->UpdateAllViews(nullptr);
   }
}

void CSpectraChildFrame::OnExport()
{
   USES_CONVERSION;

   CSpectraDoc* pDoc = (CSpectraDoc*)GetActiveDocument();
   
   Float64 lat,lng;
   pDoc->GetLocation(&lat,&lng);
   SiteClass siteClass = pDoc->GetSiteClass();
   SpecificationType specType = pDoc->GetSpecification();

   CResponseSpectra spectra;
   Uint32 result = pDoc->GetResponseSpectra(specType,lat,lng,siteClass,&spectra);
   if ( result != SPECTRA_OK )
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      CString strMsg;
      strMsg.Format(_T("Response spectra could not be exported. %s"),pDoc->GetSpectraResultExplaination(result));
      AfxMessageBox(strMsg,MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   CString strDefaultFileName(_T("Spectra.txt"));

	CFileDialog fileDlg(FALSE,_T("txt"),strDefaultFileName,OFN_HIDEREADONLY,_T("Text Files (*.txt)|*.txt"));
   if ( fileDlg.DoModal() == IDOK )
   {
		CString file_path = fileDlg.GetPathName();

      // See if the file currently exists
		if (DoesFileExist(file_path))
		{
		   CString msg;
         msg.Format(_T("The file: %s exists. Overwrite it?"),file_path);
   
         AFX_MANAGE_STATE(AfxGetStaticModuleState());
		   if ( AfxMessageBox(msg,MB_YESNO | MB_ICONQUESTION) != IDYES )
         {
            return;
         }
		}

      std::_tstring strSiteClass[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

      std::ofstream ofile(file_path);
      WBFL::System::Date date;
      ofile << "$ This response spectrum created by BridgeLink-BEToolbox-Spectra, " << T2A(date.AsString().c_str()) << std::endl;
      ofile << "$ Site Coordinates: " << lat << "° N, " << -lng << "° W" << std::endl;
      ofile << "$ Site Soil Classification: Site Class " << T2A(strSiteClass[siteClass].c_str()) << " - " << T2A(pDoc->GetSiteClassDescription(siteClass)) << std::endl;
      ofile << "$ Frequecy (Hz)   Accerlation (g)" << std::endl;

      std::vector<std::pair<Float64,Float64>> values(spectra.GetSpectraValues(SPECTRUM_MAX_PERIOD,SPECTRUM_STEP_SIZE));
      std::vector<std::pair<Float64,Float64>>::reverse_iterator iter(values.rbegin());
      std::vector<std::pair<Float64,Float64>>::reverse_iterator end(values.rend());
      for ( ; iter != end; iter++ )
      {
         Float64 t = iter->first;
         Float64 f = (t == 0 ? DBL_MAX : 1/t);

         Float64 Sa = iter->second;
         ofile << f << '\t' << Sa << std::endl;
      }

      ofile.close();
   }
}

#if defined _DEBUG
void CSpectraChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CSpectraChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if ( !CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
   {
      return FALSE;
   }

   return TRUE;
}

void CSpectraChildFrame::UpdateData(BOOL bUpdate)
{
   m_DlgBar.UpdateData(bUpdate);
}

void CSpectraChildFrame::OnSiteClassChanged()
{
   OnUpdate();
   m_DlgBar.UpdateSiteClassDescription();
}

void CSpectraChildFrame::OnLatitudeChanged()
{
   OnUpdate();
}

void CSpectraChildFrame::OnLongitudeChanged()
{
   OnUpdate();
}

void CSpectraChildFrame::OnSpecificationChanged()
{
   OnUpdate();
}

