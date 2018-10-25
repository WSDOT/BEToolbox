Attribute VB_Name = "mdlRegistry"
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

'NOTE from Robert:
'This code, as mentioned below, is written by Dave Scarmozzino from www.TheScarms.com.
'  This code was obtained from the website, which contains code that may be used
'  with the only requirement being: mentioning that it came from TheScarms.com
'  With that being done, I would also like to point out that only two functions of all
'  the included functions are used.  I have not deleted the other functions out of
'  respect for Dave Scarmozzino and www.TheScarms.com.  I also have made no modifications
'  to the code.  His code does not fall under the Alternate Route Project license.

'
' Written by Dave Scarmozzino  www.TheScarms.com
'
Type SECURITY_ATTRIBUTES
    nLength              As Long
    lpSecurityDescriptor As Long
    bInheritHandle       As Boolean
End Type

Public Const MAX_SIZE = 2048
Public Const MAX_INISIZE = 8192

' Constants for Registry top-level keys
Public Const HKEY_CURRENT_USER = &H80000001
Public Const HKEY_LOCAL_MACHINE = &H80000002
Public Const HKEY_USERS = &H80000003
Public Const HKEY_DYN_DATA = &H80000006
Public Const HKEY_CURRENT_CONFIG = &H80000005
Public Const HKEY_CLASSES_ROOT = &H80000000

' Return values
Public Const ERROR_SUCCESS = 0&
Public Const ERROR_FILE_NOT_FOUND = 2&
Public Const ERROR_MORE_DATA = 234
Public Const ERROR_NO_MORE_ITEMS = 259&

' RegCreateKeyEx options
Public Const REG_OPTION_NON_VOLATILE = 0

' RegCreateKeyEx Disposition
Public Const REG_CREATED_NEW_KEY = &H1
Public Const REG_OPENED_EXISTING_KEY = &H2

' Registry data types
Public Const REG_NONE = 0
Public Const REG_SZ = 1
Public Const REG_BINARY = 3
Public Const REG_DWORD = 4

' Registry security attributes
Public Const KEY_QUERY_VALUE = &H1
Public Const KEY_SET_VALUE = &H2
Public Const KEY_CREATE_SUB_KEY = &H4
Public Const KEY_ENUMERATE_SUB_KEYS = &H8

Declare Function RegEnumValue Lib "advapi32.dll" _
        Alias "RegEnumValueA" (ByVal hKey As Long, ByVal dwIndex As Long, _
        ByVal lpValueName As String, lpcbValueName As Long, ByVal lpReserved As Long, _
        lpType As Long, lpData As Any, lpcbData As Long) As Long

Declare Function RegDeleteValue Lib "advapi32.dll" _
        Alias "RegDeleteValueA" _
        (ByVal hKey As Long, ByVal lpValueName As String) _
        As Long

Declare Function RegDeleteKey Lib "advapi32.dll" _
        Alias "RegDeleteKeyA" _
        (ByVal hKey As Long, ByVal lpSubKey As String) As Long

Declare Function RegOpenKeyEx Lib "advapi32.dll" _
        Alias "RegOpenKeyExA" _
        (ByVal hKey As Long, ByVal lpSubKey As String, _
        ByVal ulOptions As Long, ByVal samDesired As Long, _
        phkResult As Long) As Long

Declare Function RegCreateKeyEx Lib "advapi32.dll" _
        Alias "RegCreateKeyExA" _
        (ByVal hKey As Long, ByVal lpSubKey As String, _
        ByVal Reserved As Long, ByVal lpClass As String, _
        ByVal dwOptions As Long, ByVal samDesired As Long, _
        lpSecurityAttributes As SECURITY_ATTRIBUTES, phkResult As Long, _
        lpdwDisposition As Long) As Long

Declare Function RegQueryValueEx Lib "advapi32.dll" _
        Alias "RegQueryValueExA" _
        (ByVal hKey As Long, ByVal lpszValueName As String, _
        ByVal lpdwReserved As Long, lpdwType As Long, _
        lpData As Any, lpcbData As Long) As Long

Declare Function RegSetValueEx Lib "advapi32.dll" _
        Alias "RegSetValueExA" _
        (ByVal hKey As Long, ByVal lpValueName As String, _
        ByVal Reserved As Long, ByVal dwType As Long, _
        lpData As Any, ByVal cbData As Long) As Long

Public Declare Function RegEnumKey Lib "advapi32.dll" Alias _
        "RegEnumKeyA" (ByVal hKey As Long, ByVal dwIndex As Long, _
        ByVal lpName As String, ByVal cbName As Long) As Long

Declare Function RegCloseKey Lib "advapi32.dll" _
        (ByVal hKey As Long) As Long

Declare Function GetPrivateProfileSection Lib "kernel32" _
        Alias "GetPrivateProfileSectionA" (ByVal lpAppName As String, _
        ByVal lpReturnedString As String, ByVal nSize As Long, ByVal _
        lpFileName As String) As Long
        
Declare Function GetPrivateProfileString Lib "kernel32" _
        Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, _
        ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString _
        As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
        
Declare Function WritePrivateProfileString Lib "kernel32" _
        Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, _
        ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) _
        As Long

Declare Function GetPrivateProfileInt Lib "kernel32" _
        Alias "GetPrivateProfileIntA" (ByVal lpApplicationName As String, _
        ByVal lpKeyName As String, ByVal nDefault As Long, ByVal lpFileName _
        As String) As Long

Public Function fDeleteKey(ByVal sTopKey As String, _
    ByVal sSubKey As String, ByVal sKeyName As String) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to:
'   -   Delete a registry key.
'
' sTopKey
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"}
'
' sSubKey
'   -   A registry subkey.
'
' sKeyName
'   -   The name of the key to delete.
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example
'   lResult = fDeleteKey("HKCU", "Software\YourKey\...\YourApp", "KeyToDelete")
'   Call fDeleteKey("HKCU", "Software\YourKey\...\YourApp", "KeyToDelete")
'
' NOTE:
'   The key to be deleted cannot be a top-level key
'   and cannot have any sub-keys.
'
Dim lTopKey As Long
Dim lHandle As Long
Dim lResult As Long

On Error GoTo fDeleteKeyError
lResult = 99
lTopKey = fTopKey(sTopKey)
If lTopKey = 0 Then GoTo fDeleteKeyError

lResult = RegOpenKeyEx(lTopKey, sSubKey, 0, KEY_CREATE_SUB_KEY, lHandle)
If lResult = ERROR_SUCCESS Then
    lResult = RegDeleteKey(lHandle, sKeyName)
End If

If lResult = ERROR_SUCCESS Or lResult = ERROR_FILE_NOT_FOUND Then
    fDeleteKey = ERROR_SUCCESS
Else
    fDeleteKey = lResult
End If
Exit Function

fDeleteKeyError:
    fDeleteKey = lResult
End Function

Public Function fDeleteValue(ByVal sTopKeyOrFile As String, _
    ByVal sSubKeyOrSection As String, ByVal sValueName As String) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to:
'   -   Delete a registry value.
'   -   Delete an .ini file value.
'
' sTopKeyOrIniFile
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"} or
'   -   The full path of an .ini file (ex. "C:\Windows\MyFile.ini")
'
' sSubKeyOrSection
'   -   A registry subkey or
'   -   An .ini file section name
'
' sValueName
'   -   A registry entry or
'   -   An .ini file entry
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example 1   -   Delete a registry value.
'   lResult = fDeleteValue("HKCU", "Software\YourKey\LastKey\YourApp", "EntryToDelete")
'
' Example 2   -   Delete an .ini file value.
'   lResult = fDeleteValue("C:\Windows\Myfile.ini", "SectionName", "EntryToDelete")
'
Dim lTopKey As Long
Dim lHandle As Long
Dim lResult As Long

On Error GoTo fDeleteValueError
lResult = 99
lTopKey = fTopKey(sTopKeyOrFile)
If lTopKey = 0 Then GoTo fDeleteValueError

If lTopKey = 1 Then
    lResult = WritePrivateProfileString(sSubKeyOrSection, sValueName, "", sTopKeyOrFile)
Else
    lResult = RegOpenKeyEx(lTopKey, sSubKeyOrSection, 0, KEY_SET_VALUE, lHandle)
    If lResult = ERROR_SUCCESS Then
        lResult = RegDeleteValue(lHandle, sValueName)
    End If
    
    If lResult = ERROR_SUCCESS Or lResult = ERROR_FILE_NOT_FOUND Then
        fDeleteValue = ERROR_SUCCESS
    Else
        fDeleteValue = lResult
    End If
End If
Exit Function

fDeleteValueError:
    fDeleteValue = lResult
End Function

Public Function fEnumKey(ByVal sTopKey As String, _
    ByVal sSubKey As String, sValues As String) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to:
'   -   Enumerate the subkeys of a registry key.
'
' sTopKey
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"}
'
' sSubKey
'   -   A registry subkey
'
' sValues
'   -   A returned string of the form:
'           SubKeyName|SubKeyName|.... SubKeyName||
'
'           Where - "|" equals vbNullChar (chr(0)).
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example 1
'   lResult = fEnumKey("HKLM", "Software\Microsoft", sValues)
'
Dim bDone    As Boolean
Dim lTopKey  As Long
Dim lHandle  As Long
Dim lResult  As Long
Dim lIndex   As Long
Dim sKeyName As String

On Error GoTo fEnumKeyError
lResult = 99
lTopKey = fTopKey(sTopKey)
If lTopKey = 0 Then GoTo fEnumKeyError
'
' Open the registry SubKey.
'
lResult = RegOpenKeyEx(lTopKey, sSubKey, 0, KEY_ENUMERATE_SUB_KEYS, lHandle)
If lResult <> ERROR_SUCCESS Then GoTo fEnumKeyError
'
' Get all subkeys until ERROR_NO_MORE_ITEMS or an error occurs.
'
Do While Not bDone
    sKeyName = Space$(MAX_SIZE)
    lResult = RegEnumKey(lHandle, lIndex, sKeyName, MAX_SIZE)
    
    If lResult = ERROR_SUCCESS Then
        sValues = sValues & Trim$(sKeyName)
        lIndex = lIndex + 1
    Else
        bDone = True
    End If
Loop
sValues = sValues & vbNullChar
If Len(sValues) = 1 Then sValues = sValues & vbNullChar
'
' Close the key.
'
fEnumKey = RegCloseKey(lHandle)
Exit Function
'
' Error processing.
'
fEnumKeyError:
    fEnumKey = lResult
End Function
Public Function fEnumValue(ByVal sTopKeyOrIniFile As String, _
    ByVal sSubKeyOrSection As String, sValues As String) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to:
'   -   Enumerate the values of a registry key or
'   -   Enumerate all entries in a particular section of an .ini file.
'
' sTopKeyOrIniFile
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"} or
'   -   The full path of an .ini file (ex. "C:\Windows\MyFile.ini")
'
' sSubKeyOrSection
'   -   A registry subkey or
'   -   An .ini file section name
'
' sValues
'   -   A returned string of the form:
'           EntryName=Value|EntryName=Value|.... EntryName=Value||
'
'           Where - Value can be a string or binary value.
'           and   - "|" equals vbNullChar (chr(0)).
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example 1
'   lResult = fEnumValue("HKCU", "Software\YourKey\LastKey\YourApp", sValues)
'
' Example 2
'   lResult = fEnumValue("C:\Windows\Myfile.ini", "SectionName", sValues)
'
' NOTE:
'   When enumerating registry values, only string, dword and binary values
'   with a length under 2 bytes (which allows for true/false values) are returned.
'
Dim lTopKey    As Long
Dim lHandle    As Long
Dim lResult    As Long
Dim lValueLen  As Long
Dim lIndex     As Long
Dim lValue     As Long
Dim lValueType As Long
Dim lData      As Long
Dim lDataLen   As Long
Dim bDone      As Boolean
Dim sValueName As String
Dim sValue     As String

On Error GoTo fEnumValueError
lResult = 99
lTopKey = fTopKey(sTopKeyOrIniFile)
If lTopKey = 0 Then GoTo fEnumValueError

If lTopKey = 1 Then
    '
    ' Enumerate an .ini file section.
    '
    sValues = Space$(MAX_INISIZE)
    lResult = GetPrivateProfileSection(sSubKeyOrSection, sValues, Len(sValues), sTopKeyOrIniFile)
Else
    '
    ' Open the registry SubKey.
    '
    lResult = RegOpenKeyEx(lTopKey, sSubKeyOrSection, 0, KEY_QUERY_VALUE, lHandle)
    If lResult <> ERROR_SUCCESS Then GoTo fEnumValueError
    '
    ' Get all values until ERROR_NO_MORE_ITEMS or an error occurs.
    '
    Do While Not bDone
        lDataLen = MAX_SIZE
        lValueLen = lDataLen
        sValueName = Space$(lDataLen)
        
        lResult = RegEnumValue(lHandle, lIndex, sValueName, lValueLen, 0, lValueType, ByVal lData, lDataLen)
        If lResult = ERROR_SUCCESS Then
            Select Case lValueType
                Case REG_SZ
                    sValue = Space$(lDataLen)
                    sValueName = Left$(sValueName, lValueLen)
                    lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_SZ, ByVal sValue, lDataLen)
                    If lResult = ERROR_SUCCESS Then
                        sValues = sValues & sValueName & "=" & sValue
                    Else
                        GoTo fEnumValueError
                    End If
                Case REG_DWORD
                    lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_NONE, lValue, lDataLen)
                    If lResult = ERROR_SUCCESS Then
                        sValueName = Left$(sValueName, lValueLen)
                        sValues = sValues & sValueName & "=" & lValue & vbNullChar
                    Else
                        GoTo fEnumValueError
                    End If
                Case REG_BINARY
                    If lDataLen <= 2 Then
                        lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_NONE, lValue, lDataLen)
                        If lResult = ERROR_SUCCESS Then
                            sValueName = Left$(sValueName, lValueLen)
                            sValues = sValues & sValueName & "=" & lValue & vbNullChar
                        Else
                            GoTo fEnumValueError
                        End If
                    End If
                Case Else
            End Select
            lIndex = lIndex + 1
        Else
            bDone = True
        End If
    Loop
    sValues = sValues & vbNullChar
    If Len(sValues) = 1 Then sValues = sValues & vbNullChar
    '
    ' Close the key.
    '
    lResult = RegCloseKey(lHandle)
    fEnumValue = lResult
End If
Exit Function
'
' Error processing.
'
fEnumValueError:
    fEnumValue = lResult
End Function






Public Function fReadIniFuzzy(ByVal sIniFile As String, _
    sSection As String, ByVal sIniEntry As String, _
    ByVal sDefault As String, sValue As String) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to:
'   -   Read a string value from an .ini file when you do not know the exact
'       name of the section the value is in.
'
' sIniFile
'   -   The full path of an .ini file (ex. "C:\Windows\MyFile.ini")
'
' sSection
'   -   Any complete part of the .ini file section name.
'       Ex:   [ABC DEF GHI JKL]
'       sSection Name can be "ABC" or "DEF" or "GHI" or "JKL" but not
'       a partial value such as "AB" or "HI".
'
'       NOTE: if sSection is passed as a variable and not as the actual
'             string value, sSection will be populated with the
'             complete section name.
'
' sEntry
'   -   An .ini file entry
'
' sDefault
'   -   The default value to return.
'
' sValue
'   -   The string value read.
'   -   sDefault if unsuccessful.
'
' Return Value
'   -   0 if sEntry was found, non-zero otherwise.
'
' Example 1   -   Read a string value from an .ini file.
'       Ex:   [ABC DEF GHI JKL]
'             AppName="My App"
'
'   sEntry = "AppName"
'   lResult = fReadIniFuzzy("C:\Windows\Myfile.ini", "DEF", sEntry, sValue)
'
'   Upon completion:
'       lResult  = 0
'       sSection = "ABC DEF GHI JKL"
'       sValue   = "My App"
'
Dim sNextChar    As String
Dim sLine        As String
Dim sEntry       As String
Dim sSectionName As String
Dim iLen         As Integer
Dim iLocOfEq     As Integer
Dim iFnum        As Integer
Dim bDone        As Boolean
Dim bFound       As Boolean
Dim bNewSection  As Boolean

On Error GoTo fReadIniFuzzyError
fReadIniFuzzy = 99
bDone = False
sValue = sDefault
sEntry = UCase$(sIniEntry)
sSection = UCase$(sSection)
iLen = Len(sSection)

iFnum = FreeFile
Open sIniFile For Input Access Read As iFnum

Line Input #iFnum, sLine
Do While Not EOF(iFnum) And Not bDone
    sLine = UCase$(Trim$(sLine))
    bNewSection = False
    '
    ' See if line is a section heading.
    '
    If Left$(sLine, 1) = "[" Then
        '
        ' See if section heading contains desired value.
        '
        sSectionName = sLine
        Dim iPos As Integer
        iPos = InStr(1, sLine, sSection)
        If iPos > 0 Then
            '
            ' Be sure the value is not part of a larger value.
            '
            sNextChar = Mid$(sLine, iPos + iLen, 1)
            If sNextChar = " " Or sNextChar = "]" Then
                '
                ' Search this section for the entry.
                '
                Line Input #iFnum, sLine
                bFound = False
                bNewSection = False
                Do While Not EOF(iFnum) And Not bFound
                    '
                    ' If we hit a new section, stop.
                    '
                    sLine = UCase$(Trim$(sLine))
                    If Left$(sLine, 1) = "[" Then
                        bNewSection = True
                        Exit Do
                    End If
                    '
                    ' Entry must start in column 1 to avoid comment lines.
                    '
                    If InStr(1, sLine, sEntry) = 1 Then
                        '
                        ' If entry found and line is not incomplete, get value.
                        '
                        iLocOfEq = InStr(1, sLine, "=")
                        If iLocOfEq <> 0 Then
                            sValue = Mid$(sLine, iLocOfEq + 1)
                            sSection = Mid$(sSectionName, 2, InStr(1, sSectionName, "]") - 2)
                            bFound = True
                            bDone = True
                            fReadIniFuzzy = 0
                        End If
                    End If
                    If Not bFound Then
                        Line Input #iFnum, sLine
                    End If
                Loop
                If EOF(iFnum) Then bDone = True
                sSection = Mid$(sSectionName, 2, InStr(1, sSectionName, "]") - 2)
            End If
        End If
    End If
    If Not bNewSection And Not bDone Then
        Line Input #iFnum, sLine
    End If
Loop
Close iFnum
Exit Function

fReadIniFuzzyError:
    fReadIniFuzzy = 99
End Function
Public Function fReadValue(ByVal sTopKeyOrFile As String, _
    ByVal sSubKeyOrSection As String, ByVal sValueName As String, _
    ByVal sValueType As String, ByVal vDefault As Variant, _
    vValue As Variant) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to read a:
'   -   String, 16-bit binary (True|False), 32-bit integer registry value or
'   -   String or integer value from an .ini file.
'
' sTopKeyOrIniFile
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"} or
'   -   The full path of an .ini file (ex. "C:\Windows\MyFile.ini")
'
' sSubKeyOrSection
'   -   A registry subkey or
'   -   An .ini file section name
'
' sValueName
'   -   A registry entry or
'   -   An .ini file entry
'
' sValueType
'   -   "S" to read a string value or
'   -   "B" to read a 16-bit binary value (applies to registry use only) or
'   -   "D" to read a 32-bit number value (applies to registry use only).
'
' vDefault
'   -   The default value to return. It can be a string or boolean.
'
' vValue
'   -   The value read. It can be a string or boolean.
'   -   vDefault if unsuccessful (0 when reading an integer from an .ini file)
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example 1   -   Read a string value from the registry.
'   lResult = fReadValue("HKCU", "Software\YourKey\LastKey\YourApp", "AppName", "S", "", sValue)
'
' Example 2   -   Read a boolean (True|False) value from the registry.
'   lResult = fReadValue("HKCU", "Software\YourKey\LastKey\YourApp", "AutoHide", "B", False, bValue)
'
' Example 3   -   Read an integer value from the registry.
'   lResult = fReadValue("C:\Windows\Myfile.ini", "SectionName", "NumApps", "D", 12345, lValue)
'
' Example 4   -   Read a string value from an .ini file.
'   lResult = fReadValue("C:\Windows\Myfile.ini", "SectionName", "AppName", "S", "", sValue)
'
' Example 5   -   Read an integer value from an .ini file.
'   lResult = fReadValue("C:\Windows\Myfile.ini", "SectionName", "NumApps", "B", "0", iValue)
'
Dim lTopKey     As Long
Dim lHandle     As Long
Dim lLenData    As Long
Dim lResult     As Long
Dim lDefault    As Long
Dim lValue      As Long
Dim sValue      As String
Dim sSubKeyPath As String
Dim sDefaultStr As String
Dim bValue      As Boolean

On Error GoTo fReadValueError
lResult = 99
vValue = vDefault
lTopKey = fTopKey(sTopKeyOrFile)
If lTopKey = 0 Then GoTo fReadValueError

If lTopKey = 1 Then
    '
    ' Read the .ini file value.
    '
    If UCase$(sValueType) = "S" Then
        lLenData = 255
        sDefaultStr = vDefault
        sValue = Space$(lLenData)
        lResult = GetPrivateProfileString(sSubKeyOrSection, sValueName, sDefaultStr, sValue, lLenData, sTopKeyOrFile)
        vValue = Left$(sValue, lResult)
    Else
        lDefault = 0
        lResult = GetPrivateProfileInt(sSubKeyOrSection, sValueName, lDefault, sTopKeyOrFile)
    End If
Else
    '
    ' Open the registry SubKey.
    '
    lResult = RegOpenKeyEx(lTopKey, sSubKeyOrSection, 0, KEY_QUERY_VALUE, lHandle)
    If lResult <> ERROR_SUCCESS Then
        fReadValue = lResult
        Exit Function
    End If
    '
    ' Get the actual value.
    '
    Select Case UCase$(sValueType)
        Case "S"
            '
            ' String value. The first query gets the string length. The second
            ' gets the string value.
            '
            lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_SZ, "", lLenData)
            If lResult = ERROR_MORE_DATA Then
                sValue = Space(lLenData)
                lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_SZ, ByVal sValue, lLenData)
            End If
            If lResult = ERROR_SUCCESS Then  'Remove null character.
                vValue = Left$(sValue, lLenData - 1)
            Else
                GoTo fReadValueError
            End If
        Case "B"
            lLenData = Len(bValue)
            lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_BINARY, bValue, lLenData)
            If lResult = ERROR_SUCCESS Then
                vValue = bValue
            Else
                GoTo fReadValueError
            End If
        Case "D"
            lLenData = 32
            lResult = RegQueryValueEx(lHandle, sValueName, 0, REG_DWORD, lValue, lLenData)
            If lResult = ERROR_SUCCESS Then
                vValue = lValue
            Else
                GoTo fReadValueError
            End If
    End Select
    '
    ' Close the key.
    '
    lResult = RegCloseKey(lHandle)
    fReadValue = lResult
End If
Exit Function
'
' Error processing.
'
fReadValueError:
    fReadValue = lResult
End Function


Private Function fTopKey(ByVal sTopKeyOrFile As String) As Long
Dim sDir   As String
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' This function returns:
'   -   the numeric value of a top level registry key or
'   -   1 if sTopKey is a valid .ini file or
'   -   0 otherwise.
'
On Error GoTo fTopKeyError
fTopKey = 0
Select Case UCase$(sTopKeyOrFile)
    Case "HKCU"
        fTopKey = HKEY_CURRENT_USER
    Case "HKLM"
        fTopKey = HKEY_LOCAL_MACHINE
    Case "HKU"
        fTopKey = HKEY_USERS
    Case "HKDD"
        fTopKey = HKEY_DYN_DATA
    Case "HKCC"
        fTopKey = HKEY_CURRENT_CONFIG
    Case "HKCR"
        fTopKey = HKEY_CLASSES_ROOT
    Case Else
        On Error Resume Next
        sDir = Dir$(sTopKeyOrFile)
        If Err.number = 0 And sDir <> "" Then fTopKey = 1
End Select
Exit Function

fTopKeyError:
End Function

Public Function fWriteValue(ByVal sTopKeyOrFile As String, _
    ByVal sSubKeyOrSection As String, ByVal sValueName As String, _
    ByVal sValueType As String, ByVal vValue As Variant) As Long
'
' Written by Dave Scarmozzino  www.TheScarms.com
'
' Use this function to write a:
'   -   String, 16-bit binary (True|False), 32-bit integer registry value or
'   -   String value to an .ini file.
'
' sTopKeyOrIniFile
'   -   A top level registry key abbreviation {"HKCU","HKLM","HKU","HKDD","HKCC","HKCR"} or
'   -   The full path of an .ini file (ex. "C:\Windows\MyFile.ini")
'
' sSubKeyOrSection
'   -   A registry subkey or
'   -   An .ini file section name
'
' sValueName
'   -   A registry entry or
'   -   An .ini file entry
'
' sValueType
'   -   "S" to write a string value or
'   -   "B" to write a 16-bit binary value (applies to registry use only) or
'   -   "D" to write a 32-bit number value (applies to registry use only).
'
' vValue
'   -   The value to write. It can be a string, binary or integer.
'
' Return Value
'   -   0 if successful, non-zero otherwise.
'
' Example 1   -   Write a string value to the registry.
'   lResult = fWriteValue("HKCU", "Software\YourKey\LastKey\YourApp", "AppName", "S", "MyApp")
'
' Example 2   -   Write a True|False value to the registry.
'   lResult = fWriteValue("HKCU", "Software\YourKey\LastKey\YourApp", "AutoHide", "B", True)
'
' Example 3   -   Write an integer value to the registry.
'   lResult = fWriteValue("HKCU", "Software\YourKey\LastKey\YourApp", "NumOfxxx", "D", 12345)
'
' Example 4   -   Write a string value to an .ini file.
'   lResult = fWriteValue("C:\Windows\Myfile.ini", "SectionName", "AppName", "S", "MyApp")
'
' NOTE:
'   This function cannot write a non-string value to an .ini file.
'
Dim hKey                As Long
Dim lTopKey             As Long
Dim lOptions            As Long
Dim lsamDesired         As Long
Dim lHandle             As Long
Dim lDisposition        As Long
Dim lLenData            As Long
Dim lResult             As Long
Dim lValue              As Long
Dim sClass              As String
Dim sValue              As String
Dim sSubKeyPath         As String
Dim bValue              As Boolean
Dim tSecurityAttributes As SECURITY_ATTRIBUTES

On Error GoTo fWriteValueError
lResult = 99
lTopKey = fTopKey(sTopKeyOrFile)
If lTopKey = 0 Then GoTo fWriteValueError

If lTopKey = 1 Then
    '
    ' Read the .ini file value.
    '
    If UCase$(sValueType) = "S" Then
        sValue = vValue
        lResult = WritePrivateProfileString(sSubKeyOrSection, sValueName, sValue, sTopKeyOrFile)
    Else
        GoTo fWriteValueError
    End If
Else
    sClass = ""
    lOptions = REG_OPTION_NON_VOLATILE
    lsamDesired = KEY_CREATE_SUB_KEY Or KEY_SET_VALUE
    '
    ' Create the SubKey or open it if it exists. Return its handle.
    ' lDisposition will be REG_CREATED_NEW_KEY if the key did not exist.
    '
    lResult = RegCreateKeyEx(lTopKey, sSubKeyOrSection, 0, sClass, lOptions, _
                  lsamDesired, tSecurityAttributes, lHandle, lDisposition)
    If lResult <> ERROR_SUCCESS Then GoTo fWriteValueError
    '
    ' Set the actual value.
    '
    Select Case UCase$(sValueType)
        Case "S"
            sValue = vValue
            lLenData = Len(sValue) + 1
            lResult = RegSetValueEx(lHandle, sValueName, 0, REG_SZ, ByVal sValue, lLenData)
        Case "B"
            bValue = vValue
            lLenData = Len(bValue)
            lResult = RegSetValueEx(lHandle, sValueName, 0, REG_BINARY, bValue, lLenData)
        Case "D"
            lValue = CInt(vValue)
            lLenData = 4
            lResult = RegSetValueEx(lHandle, sValueName, 0, REG_DWORD, lValue, lLenData)
    End Select
    '
    ' Close the key.
    '
    If lResult = ERROR_SUCCESS Then
        lResult = RegCloseKey(lHandle)
        fWriteValue = lResult
        Exit Function
    End If
End If
Exit Function
'
' Error processing.
'
fWriteValueError:
    fWriteValue = lResult
End Function





