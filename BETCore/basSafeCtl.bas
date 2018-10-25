Attribute VB_Name = "basSafeCtl"
'///////////////////////////////////////////////////////////////////////
'// BEToolbox - Bridge Engineering Toolbox
'// Copyright (C) 2000, Washington State Department of Transportation
'//                     Bridge and Structures Office
'//
'// This software was developed as part of the Alternate Route Project
'//
'// This program is free software; you can redistribute it and/or modify
'// it under the terms of the Alternate Route Open Source License as
'// published by the Washington State Department of Transportation,
'// Bridge and Structures Office.
'//
'// This program is distributed in the hope that it will be useful,
'// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
'// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
'// PURPOSE.  See the Alternate Route Open Source License for more details.
'//
'// You should have received a copy of the Alternate Open Source License
'// along with this program; if not, write to the Washington State
'// Department of Transportation, Bridge and Structures Office,
'// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
'// Bridge_Support@wsdot.wa.gov
'///////////////////////////////////////////////////////////////////////

'This code taken from MSDN:
'http://support.microsoft.com/support/kb/articles/Q182/5/98.asp
      
      
      Option Explicit

              Public Const IID_IDispatch = "{00020400-0000-0000-C000-000000000046}"
              Public Const IID_IPersistStorage = _
                "{0000010A-0000-0000-C000-000000000046}"
              Public Const IID_IPersistStream = _
                "{00000109-0000-0000-C000-000000000046}"
              Public Const IID_IPersistPropertyBag = _
                "{37D84F60-42CB-11CE-8135-00AA004BB851}"

              Public Const INTERFACESAFE_FOR_UNTRUSTED_CALLER = &H1
              Public Const INTERFACESAFE_FOR_UNTRUSTED_DATA = &H2
              Public Const E_NOINTERFACE = &H80004002
              Public Const E_FAIL = &H80004005
              Public Const MAX_GUIDLEN = 40

              Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
                 (pDest As Any, pSource As Any, ByVal ByteLen As Long)
              Public Declare Function StringFromGUID2 Lib "ole32.dll" (rguid As _
                 Any, ByVal lpstrClsId As Long, ByVal cbMax As Integer) As Long

              Public Type udtGUID
                  Data1 As Long
                  Data2 As Integer
                  Data3 As Integer
                  Data4(7) As Byte
              End Type

              Public m_fSafeForScripting As Boolean
              Public m_fSafeForInitializing As Boolean

              Sub Main()
                  m_fSafeForScripting = True
                  m_fSafeForInitializing = True
              End Sub

