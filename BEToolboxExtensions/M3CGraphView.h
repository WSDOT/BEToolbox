///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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
#pragma once

// CM3CGraphView view

#include <UnitMgt\IndirectMeasure.h>

interface IStressStrain;

class CM3CGraphView : public CView
{
	DECLARE_DYNAMIC(CM3CGraphView)

protected:
   CM3CGraphView();           // protected constructor used by dynamic creation
	virtual ~CM3CGraphView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
   void DoPrint(CDC* pDC, CPrintInfo* pInfo);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   bool m_bIsPrinting;
   CRect m_PrintingRect;
   CRect GetDrawingRect();

   unitmgtScalar m_Scalar;

   virtual void GetMaterialModel(IStressStrain** ppMaterial) = 0;
   virtual LPCTSTR GetGraphTitle() = 0;
   virtual LPCTSTR GetGraphSubtitle() = 0;
   virtual std::vector<Float64> GetSpecialPoints() { return std::vector<Float64>(); }
   virtual void GetSign(Float64* pX, Float64* pY) { *pX = 1.0; *pY = 1.0; }
   DECLARE_MESSAGE_MAP()
};

class CM3CConcreteGraphView : public CM3CGraphView
{
   DECLARE_DYNCREATE(CM3CConcreteGraphView)
protected:
   virtual void GetMaterialModel(IStressStrain** ppMaterial) override;
   virtual LPCTSTR GetGraphTitle() override { return _T("Concrete"); }
   virtual LPCTSTR GetGraphSubtitle() override { return _T("Mander's Model"); }
   virtual void GetSign(Float64* pX, Float64* pY) { *pX = -1.0; *pY = -1.0; }
};

class CM3CRebarGraphView : public CM3CGraphView
{
   DECLARE_DYNCREATE(CM3CRebarGraphView)
protected:
   virtual void GetMaterialModel(IStressStrain** ppMaterial) override;
   virtual LPCTSTR GetGraphTitle() override { return _T("Longitudinal Reinforcement"); }
   virtual LPCTSTR GetGraphSubtitle() override { return _T("Elastic-Plastic with Strain Hardening"); }
   virtual std::vector<Float64> GetSpecialPoints() override;
};

class CM3CStrandGraphView : public CM3CGraphView
{
   DECLARE_DYNCREATE(CM3CStrandGraphView)
protected:
   virtual void GetMaterialModel(IStressStrain** ppMaterial) override;
   virtual LPCTSTR GetGraphTitle() override { return _T("Unbonded Tendons"); }
   virtual LPCTSTR GetGraphSubtitle() override { return _T("Power Formula"); }
};
