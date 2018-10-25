VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTrssmen"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True

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
                              Err.Raise E_NOINTERFACE
                              Exit Sub
                      End Select
                  End If
              End Sub

Public Sub SaveDataFile()
    Dim intfilenumber As Integer
    Dim sfile As String
    Dim strTemp As String

    sfile = getFileName(Form1.dlgCommonDialog, "Trssmen Files (*.Trssmen)|*.Trssmen")
    
    If Not sfile = "" Then
        If Right$(sfile, 7) <> ".Trssmen" Then
            sfile = sfile + ".Trssmen"
        End If
        
        intfilenumber = FreeFile
        Open (sfile) For Output As intfilenumber

        Print #intfilenumber, InputString

        Close intfilenumber
            
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
    
    'Call the fortran code
    Call Trssmen(strInfile, strOutfile)

    'Convert the fortran output code to
    CreateOutString

    'delete temp files:
    Kill strInfile
    Kill strOutfile

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
            .Filter = "Trssmen Files (*.Trssmen)|*.Trssmen"
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
'  Possible answer, though I haven't researched it: The programs were originally VAX
'    programs, and they do not use either linefeeds or carraige returns, so perhaps
'    that is why.
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



