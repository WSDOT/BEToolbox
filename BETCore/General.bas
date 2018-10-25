Attribute VB_Name = "General"
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

'Public Declare Function GetTempPath Lib "kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long

Public Declare Sub BoxGir Lib "BoxGir.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub ComGir Lib "ComGir.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub Curvel Lib "Curvel.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub GirComp Lib "GirComp.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub GPile Lib "GPile.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub PSGDES Lib "PSGDES.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub TORSION Lib "TORSION.dll" (ByVal INFILE As String, ByVal OUTFILE As String, ByVal BDS As String)
Public Declare Sub Trssmem Lib "Trssmem.dll" (ByVal INFILE As String, ByVal OUTFILE As String)
Public Declare Sub Ult2Ax Lib "Ult2Ax.dll" (ByVal INFILE As String, ByVal OUTFILE As String)

Public strTempDirectory As String
'Public dlgCommonDialog As CommonDialog


Public Function DoesFileExist(filename As String) As Boolean
    'check if file exists:
    Dim FileExists As String
    FileExists = Dir$(filename)
    If FileExists > "" Then
        'The File Exists
        DoesFileExist = True
    Else
        DoesFileExist = False
    End If
    
End Function


Public Function getFileName(ComDialog As Object, strFilter As String) As String
     
     Dim sfile As String
     
     With ComDialog
         .DialogTitle = "Save As"
         .Flags = cdlOFNHideReadOnly
'         .DefaultExt = strExt
         On Error GoTo SaveCancel
         .CancelError = True
         '.Filter = "All Files(*.*)|*.*"
         .Filter = strFilter
         .ShowSave
         If Len(.filename) = 0 Then
             MsgBox "File not saved.", vbInformation
             Exit Function
         End If
         sfile = .filename
         If DoesFileExist(sfile) Then
             Dim Answer As Integer
             Answer = MsgBox(sfile & " already exists. " + Chr$(13) + Chr$(10) & _
                         "Do you want to replace it?", vbYesNo, "Save As")
             If Answer = 7 Then
                 getFileName = ""
                 Exit Function
             Else
                 'continue
             End If
         End If
     End With
    getFileName = GetRidOfSpaces(sfile)
    
     Exit Function
     
'Save was cancelled:
SaveCancel:
    getFileName = ""

End Function

'Each class has its own copy of this, so that the HTML code can call it
' (better than cluttering up the HTML with it)

Private Function GetRidOfSpaces(text As String) As String
    Dim Count As Integer
    Dim length As Integer
    Dim startOfChar As Integer
    Dim endOfChar As Integer
    Dim char As String
    Dim parsedString As String
    
    length = Len(text)
    
    'find start of characters within string:
    For Count = 1 To length
        char = Mid$(text, Count, 1)
        If Not char = " " Then
            startOfChar = Count
            Exit For
        End If
    Next
    
    'find end of characters within string:
    For Count = length To 1 Step -1
        char = Mid$(text, Count, 1)
        If Not char = " " Then
            endOfChar = Count
            Exit For
        End If
    Next
    
    Dim count2 As Integer
    Dim truelength As Integer
    truelength = ((endOfChar - startOfChar) + 1)
    count2 = 1
    parsedString = ""
    For Count = startOfChar To endOfChar
        If startOfChar = 0 And endOfChar = 0 Then Exit For
        parsedString = parsedString + Mid$(text, Count, 1)
        
    Next
    
    GetRidOfSpaces = parsedString
    
End Function

Public Function GetTextBoxName(number As Integer) As String
    Dim strNumber As String
    
    strNumber = GetRidOfSpaces(Str(number))
    GetTextBoxName = "X" + strNumber

End Function

Public Function FindTempDirectory() As String
'    'Read tmp directory from registry (using function in modRegistry)
'    Dim strSystemDrive As String
'    Dim strPathString As String
'
'    fReadValue "HKCU", "Environment", "TEMP", "S", "", strPathString
'
'    If Not strPathString = "" Then
'        'Get that horrid %SystemDrive% out of the string:
'        Dim Count As Integer
'        Dim PercentMarkCount As Integer
'        Dim tmpString As String
'        Dim bNoSystemDrive As Boolean   'tells us if there is a "system drive" in use.
'        bNoSystemDrive = False
'        Count = 1               'just an index
'        PercentMarkCount = 0    'keeps track of how many "%" we've seen
'        While PercentMarkCount <> 2
'            'count also keeps track of where you are in the string...
'            'If you exceed the string length, then it
'            'obviously isn't set the the "system" drive,
'            'so just set it to whatever the string is.
'            tmpString = Mid$(strPathString, Count, 1)
'            If tmpString = "%" Then PercentMarkCount = PercentMarkCount + 1
'            If PercentMarkCount = 2 Then
'                strPathString = Mid$(strPathString, Count + 1, Len(strPathString) - Count)
'            End If
'            Count = Count + 1
'            If Count = Len(strPathString) And PercentMarkCount <> 2 Then
'                PercentMarkCount = 2 'allows us to exit loop
'                bNoSystemDrive = True
'                'leave the strPathString alone, as it contains the full path to temp directory.
'            End If
'
'        Wend
'
'        If bNoSystemDrive = False Then
'            strSystemDrive = Environ("SystemDrive")
'        Else
'            strSystemDrive = ""
'        End If
'
'        'merge the two string (system drive and the path) together and return:
'
'        FindTempDirectory = strSystemDrive + strPathString
'    Else
'        Dim nBufferLength As Long
'        Dim lpBuffer As String
'
'        nBufferLength = 254
'        GetTempPath nBufferLength, lpBuffer
    
'        FindTempDirectory = lpBuffer

        Dim strPathString As String
        strPathString = Environ$("temp")
        If strPathString = "" Then
            strPathString = Environ$("tmp")
'        End If

    End If
    
    FindTempDirectory = strPathString
    
End Function


Public Function IsItAValidPath(strPathName As String)
    Dim fso As FileSystemObject
    Dim Answer As Boolean
    Dim tmpPath As String
    
    tmpPath = strPathName
    
    'Get rid of filename:
    Dim Count
    Dim count2
    For Count = Len(tmpPath) To 1 Step -1
        count2 = count2 + 1
        If Mid$(tmpPath, Count, 1) = "\" Then
            tmpPath = Mid$(tmpPath, 1, Len(tmpPath) - count2)
            Exit For
        End If
    Next

    Set fso = New FileSystemObject
    Answer = fso.FolderExists(tmpPath)
    
    IsItAValidPath = Answer
    Set fso = Nothing
    
End Function


Function Author() As String
    Author = "BEToolbox, written by: Robert S. Lotze"

End Function


Public Sub IObjectSafety_GetInterfaceSafetyOptionsImplementation(ByVal riid As _
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

    Public Sub IObjectSafety_SetInterfaceSafetyOptionsImplementation(ByVal riid As _
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
                    'This code must be commented out for this code to work
                    '  on anything other than NT4.0
'                    Err.Raise E_NOINTERFACE
'                    Exit Sub
            End Select
        End If
    End Sub


