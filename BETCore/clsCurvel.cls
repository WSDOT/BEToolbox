VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsCurvel"
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
'local variable(s) to hold property value(s)
Private mvarInputString As String 'local copy
Private mvarOutputString As String 'local copy
Private mvarFileName As String 'local copy
'The following "filename" property is used to get the filename of a saved/loaded file and is used
' in the output screen to place the filename at the top of the output.
Public Property Let filename(ByVal vData As String)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.FileName = 5
    mvarFileName = vData
End Property


Public Property Get filename() As String
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.FileName
    filename = mvarFileName
End Property



    Private Sub IObjectSafety_GetInterfaceSafetyOptions(ByVal riid As _
              Long, pdwSupportedOptions As Long, pdwEnabledOptions As Long)

            IObjectSafety_GetInterfaceSafetyOptionsImplementation riid, _
                pdwSupportedOptions, pdwEnabledOptions
    End Sub

    Private Sub IObjectSafety_SetInterfaceSafetyOptions(ByVal riid As _
              Long, ByVal dwOptionsSetMask As Long, ByVal dwEnabledOptions As Long)
              
           IObjectSafety_SetInterfaceSafetyOptionsImplementation riid, dwOptionsSetMask, _
                dwEnabledOptions
    End Sub




Public Sub SaveDataFile()
    Dim intfilenumber As Integer
    Dim sfile As String
    Dim strTemp As String

    sfile = getFileName(Form1.dlgCommonDialog, "Curvel Files(*.Curvel)|*.Curvel")
    If sfile <> "" Then
        If Right$(sfile, 7) <> ".Curvel" Then
            sfile = sfile + ".Curvel"
        End If
        
        intfilenumber = FreeFile
        Open (sfile) For Output As intfilenumber

        Print #intfilenumber, InputString

        Close intfilenumber
        
        filename = sfile
        
    End If
End Sub

Public Sub Process()
    'Creates the "Input File" for the Fortran code
    ConvertText
    
    Dim strInfile As String * 254
    Dim strOutfile As String * 254
    Dim tmpDir As String

    tmpDir = strTempDirectory
    
    strInfile = tmpDir & "\ipf"
    strOutfile = tmpDir & "\opf"
    
    'delete temp files:
    If DoesFileExist(strInfile) = True Then
        Kill strInfile
    End If
    If DoesFileExist(strOutfile) = True Then
        Kill strOutfile
    End If
        
    'Creates the "Input File" for the Fortran code
    ConvertText
    
    'Call the fortran code
    Call Curvel(strInfile, strOutfile)

    'Convert the fortran output code to
    CreateOutString

    'delete temp files:
    If DoesFileExist(strInfile) = True Then
'        Kill strInfile
    End If
    If DoesFileExist(strOutfile) = True Then
'        Kill strOutfile
    End If

End Sub

Public Property Let OutputString(ByVal vData As String)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.OutputString = 5
    mvarOutputString = vData
End Property

Public Property Get OutputString() As String
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.OutputString
    OutputString = mvarOutputString
End Property

Public Function LoadDataFile() As String
    'Loads a boxgir data file and returns a string with the file data so that the
    '  html script can take it and do "whatever"
    Dim sfile As String
    Dim intfilenumber As Integer
 '   Dim lineString As String
    Dim fileString As String

    If sfile = "" Then
        With Form1.dlgCommonDialog
            .DialogTitle = "Open"
            .CancelError = True
            .Flags = cdlOFNHideReadOnly
            On Error GoTo OpenDialogError
            'ToDo: set the flags and attributes of the common dialog control
            .Filter = "Curvel Files (*.Curvel)|*.Curvel"
            .ShowOpen
            sfile = .filename
        End With
    End If

    If ((Len(sfile) = 0) Or (DoesFileExist(sfile) = False)) Then
        MsgBox "File does not exist.  No action will be taken.", vbInformation, "BE Toolbox"
        Exit Function
    End If

    intfilenumber = FreeFile

    Open sfile For Input As intfilenumber

    LoadDataFile = Input(LOF(intfilenumber), intfilenumber)  'read entire file

    Close intfilenumber

    filename = sfile

OpenDialogError:
    'Cancel pressed

End Function

Public Property Let InputString(ByVal vData As String)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.InputString = 5
    mvarInputString = vData
End Property

Public Property Get InputString() As String
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.InputString
    InputString = mvarInputString
End Property

Private Sub Class_Initialize()
    mvarInputString = ""
    mvarOutputString = ""
    
    If strTempDirectory = "" Then
        strTempDirectory = FindTempDirectory
    End If
   
End Sub

Private Sub ConvertText()
'Takes the "Input" data and creates an "Input" file for the Fortran dll.
    Dim intfilenumber As Integer
    intfilenumber = FreeFile
    Open (strTempDirectory & "\ipf") For Output As intfilenumber
    
    Print #intfilenumber, InputString
   
    Close intfilenumber
    
End Sub

Private Sub CreateOutString()
'Creates the "outstring" from the file created by the fortran dll

'Do I need this at all?  Why couldn't I just suck up the entire file into one
'  string?  Yes, sometimes that creates errors...but why am I adding linefeeds???
'  shouldn't those already be in the string?
    Dim fileString As String
    Dim intfilenumber As Integer
    intfilenumber = FreeFile
    Open strTempDirectory & "\opf" For Input As intfilenumber
    Dim lineString As String
    While Not EOF(intfilenumber)
        Line Input #intfilenumber, lineString
        If fileString = "" Then
            fileString = lineString + Chr$(13) + Chr$(10)
        Else
            fileString = fileString + Chr$(13) + Chr$(10) + lineString
        End If
    Wend

    Close intfilenumber
    
    OutputString = fileString
    fileString = ""
    lineString = ""
    
End Sub



