VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Shape"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"No"
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

'local variable to hold collection
Private mCol As Collection
'local variable(s) to hold property value(s)
Private mvarfc As Variant 'local copy



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


Public Property Let fc(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.fc = 5
    mvarfc = vData
End Property


Public Property Set fc(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.fc = Form1
    Set mvarfc = vData
End Property


Public Property Get fc() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.fc
    If IsObject(mvarfc) Then
        Set fc = mvarfc
    Else
        fc = mvarfc
    End If
End Property




Public Function Add(X As Variant, Y As Variant, Optional sKey As String) As Points
    'create a new object
    Dim objNewMember As Points
    Set objNewMember = New Points


    'set the properties passed into the method
    If IsObject(X) Then
        Set objNewMember.X = X
    Else
        objNewMember.X = X
    End If
    If IsObject(Y) Then
        Set objNewMember.Y = Y
    Else
        objNewMember.Y = Y
    End If
    If Len(sKey) = 0 Then
        mCol.Add objNewMember
    Else
        mCol.Add objNewMember, sKey
    End If


    'return the object created
    Set Add = objNewMember
    Set objNewMember = Nothing


End Function

Public Property Get Item(vntIndexKey As Variant) As Points
    'used when referencing an element in the collection
    'vntIndexKey contains either the Index or Key to the collection,
    'this is why it is declared as a Variant
    'Syntax: Set foo = x.Item(xyz) or Set foo = x.Item(5)
  Set Item = mCol(vntIndexKey)
End Property



Public Property Get Count() As Long
    'used when retrieving the number of elements in the
    'collection. Syntax: Debug.Print x.Count
    Count = mCol.Count
End Property


Public Sub Remove(vntIndexKey As Variant)
    'used when removing an element from the collection
    'vntIndexKey contains either the Index or Key, which is why
    'it is declared as a Variant
    'Syntax: x.Remove(xyz)


    mCol.Remove vntIndexKey
End Sub


Public Property Get NewEnum() As IUnknown
    'this property allows you to enumerate
    'this collection with the For...Each syntax
    Set NewEnum = mCol.[_NewEnum]
End Property


Private Sub Class_Initialize()
    'creates the collection when this class is created
    Set mCol = New Collection
End Sub


Private Sub Class_Terminate()
    'destroys collection when this class is terminated
    Set mCol = Nothing
End Sub




