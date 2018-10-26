///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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
#include "BEToolboxUtilities.h"
#include <EAF\EAFApp.h>

rptRcTable* CreateTable(ColumnIndexType nColumns)
{
   rptRcTable* pTable = new rptRcTable(nColumns,0.);

   pTable->SetTableHeaderStyle( _T("ColumnHeading") );
   pTable->SetOutsideBorderStyle( rptRiStyle::HAIR_THICK );
   pTable->SetInsideBorderStyle( rptRiStyle::NOBORDER );
   pTable->SetCellPad( 0.03125 );

   pTable->EnableRowStriping(true);

   ColumnIndexType numColumns = pTable->GetNumberOfColumns();

   for ( ColumnIndexType i = 0; i < numColumns; i++ )
   {
      pTable->SetColumnStyle( i, _T("NormalRow") );
      pTable->SetStripeRowColumnStyle( i, _T("StripedRow") );
   }

   return pTable;
}

std::_tstring g_strImagePath;
const std::_tstring& GetImagePath()
{
   if ( g_strImagePath.empty() )
   {
      TCHAR szBuff[_MAX_PATH];
      ::GetModuleFileName(::GetModuleHandle(NULL), szBuff, _MAX_PATH);
      std::_tstring filename(szBuff);
      make_upper( filename.begin(), filename.end() );

      CEAFApp* pApp = EAFGetApp();
      // find first Occurrence of the application name
      std::_tstring strAppName(pApp->m_pszAppName);
      make_upper( strAppName.begin(), strAppName.end() );
      std::_tstring::size_type loc = filename.find(strAppName);
      if ( loc != std::_tstring::npos )
      {
         loc += strAppName.length();
      }
      else
      {
         // something is wrong... that find should have succeeded
         // hard code the default install location so that there is a remote chance of success
         TCHAR szNativeProgramFilesFolder[MAX_PATH];
         ExpandEnvironmentStrings(_T("%ProgramW6432%"),szNativeProgramFilesFolder,ARRAYSIZE(szNativeProgramFilesFolder));
         filename = _T("\\") + std::_tstring(szNativeProgramFilesFolder) + _T("\\WSDOT\\") + strAppName;
         loc = filename.length();
      }

      filename.replace(filename.begin()+loc,filename.end(),_T("\\IMAGES\\"));
      g_strImagePath = filename;
   }

   return g_strImagePath;
}

Float64 GetOffset(CString strOffset,const unitmgtLengthData& displayUnit)
{
   strOffset.MakeUpper();
   strOffset.Trim();
   TCHAR cDir = (strOffset.GetLength() == 0 ? 'R' : strOffset.GetAt(strOffset.GetLength()-1));
   Float64 sign = (cDir == 'L' ? -1 : 1);
   Float64 value = _tstof(strOffset);
   value *= sign;
   value = ::ConvertToSysUnits(value,displayUnit.UnitOfMeasure);
   return value;
}

CString GetOffset(Float64 value,const unitmgtLengthData& displayUnit)
{
   CString strOffset = FormatOffset(value,displayUnit.UnitOfMeasure,false);
   return strOffset;
}