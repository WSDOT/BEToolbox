VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsGeneral"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
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

Option Explicit

Implements IObjectSafety
Private Const SND_ASYNC = &H1             '  play asynchronously
Private Const SND_NODEFAULT = &H2         '  silence not default, if sound not found
Private Const SND_MEMORY = &H4            '  lpszSoundName points to a memory file
Private Declare Function PlaySoundData Lib "winmm.dll" Alias "PlaySoundA" (lpData As Any, ByVal hModule As Long, ByVal dwFlags As Long) As Long

Private bLoadSound() As Byte
      Private Sub IObjectSafety_GetInterfaceSafetyOptions(ByVal riid As _
              Long, pdwSupportedOptions As Long, pdwEnabledOptions As Long)

                  Dim Rc      As Long
                  Dim rClsId  As udtGUID
                  Dim IID     As String
                  Dim bIID()  As Byte

                  pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER Or _
                                        INTERFACESAFE_FOR_UNTRUSTED_DATA

                  If (riid <> 0) Then
                      CopyMemory rClsId, ByVal riid, Len(rClsId)

                      bIID = String$(MAX_GUIDLEN, 0)
                      Rc = StringFromGUID2(rClsId, VarPtr(bIID(0)), MAX_GUIDLEN)
                      Rc = InStr(1, bIID, vbNullChar) - 1
                      IID = Left$(UCase(bIID), Rc)

                      Select Case IID
                          Case IID_IDispatch
                              pdwEnabledOptions = IIf(m_fSafeForScripting, _
                            INTERFACESAFE_FOR_UNTRUSTED_CALLER, 0)
                              Exit Sub
                          Case IID_IPersistStorage, IID_IPersistStream, _
                             IID_IPersistPropertyBag
                              pdwEnabledOptions = IIf(m_fSafeForInitializing, _
                            INTERFACESAFE_FOR_UNTRUSTED_DATA, 0)
                              Exit Sub
                          Case Else
                              Err.Raise E_NOINTERFACE
                              Exit Sub
                      End Select
                  End If
              End Sub

              Private Sub IObjectSafety_SetInterfaceSafetyOptions(ByVal riid As _
              Long, ByVal dwOptionsSetMask As Long, ByVal dwEnabledOptions As Long)
                  Dim Rc          As Long
                  Dim rClsId      As udtGUID
                  Dim IID         As String
                  Dim bIID()      As Byte

                  If (riid <> 0) Then
                      CopyMemory rClsId, ByVal riid, Len(rClsId)

                      bIID = String$(MAX_GUIDLEN, 0)
                      Rc = StringFromGUID2(rClsId, VarPtr(bIID(0)), MAX_GUIDLEN)
                      Rc = InStr(1, bIID, vbNullChar) - 1
                      IID = Left$(UCase(bIID), Rc)

                      Select Case IID
                          Case IID_IDispatch
                              If ((dwEnabledOptions And dwOptionsSetMask) <> _
                           INTERFACESAFE_FOR_UNTRUSTED_CALLER) Then
                                  Err.Raise E_FAIL
                                  Exit Sub
                              Else
                                  If Not m_fSafeForScripting Then
                                      Err.Raise E_FAIL
                                  End If
                                  Exit Sub
                              End If

                          Case IID_IPersistStorage, IID_IPersistStream, _
                        IID_IPersistPropertyBag
                              If ((dwEnabledOptions And dwOptionsSetMask) <> _
                            INTERFACESAFE_FOR_UNTRUSTED_DATA) Then
                                  Err.Raise E_FAIL
                                  Exit Sub
                              Else
                                  If Not m_fSafeForInitializing Then
                                      Err.Raise E_FAIL
                                  End If
                                  Exit Sub
                              End If

                          Case Else
                            'Must be commented out for it to work on anything but WinNT4.0
'                              Err.Raise E_NOINTERFACE
'                              Exit Sub
                      End Select
                  End If
              End Sub



Public Sub SoundDoor()
   'play sound
   
   'Commented out because it irritates people:
'   Const Flags = SND_MEMORY Or SND_ASYNC Or SND_NODEFAULT
'   bLoadSound = LoadResData(200, "SOUND")
'   PlaySoundData bLoadSound(0), 0, Flags
   
End Sub


