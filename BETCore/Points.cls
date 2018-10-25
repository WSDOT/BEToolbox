VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Points"
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

'local variable(s) to hold property value(s)
Private mvarX As Variant 'local copy
Private mvarY As Variant 'local copy



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


Public Property Let Y(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.Y = 5
    mvarY = vData
End Property


Public Property Set Y(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.Y = Form1
    Set mvarY = vData
End Property


Public Property Get Y() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.Y
    If IsObject(mvarY) Then
        Set Y = mvarY
    Else
        Y = mvarY
    End If
End Property



Public Property Let X(ByVal vData As Variant)
'used when assigning a value to the property, on the left side of an assignment.
'Syntax: X.X = 5
    mvarX = vData
End Property


Public Property Set X(ByVal vData As Variant)
'used when assigning an Object to the property, on the left side of a Set statement.
'Syntax: Set x.X = Form1
    Set mvarX = vData
End Property


Public Property Get X() As Variant
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.X
    If IsObject(mvarX) Then
        Set X = mvarX
    Else
        X = mvarX
    End If
End Property






