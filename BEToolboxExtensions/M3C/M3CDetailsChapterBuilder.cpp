///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "M3CDetailsChapterBuilder.h"
#include "BEToolboxColors.h"
#include <Reporter\Reporter.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CM3CMaterialDetailsChapterBuilder::CM3CMaterialDetailsChapterBuilder(CM3CDoc* pDoc)
{
   m_pDoc = pDoc;
}

CM3CMaterialDetailsChapterBuilder::~CM3CMaterialDetailsChapterBuilder(void)
{
}

LPCTSTR CM3CMaterialDetailsChapterBuilder::GetName() const
{
   return _T("M3C Material Details");
}

Uint16 CM3CMaterialDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CM3CMaterialDetailsChapterBuilder::Build(CReportSpecification* pRptSpec, Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   BuildConcreteModel(pChapter);
   BuildRebarModel(pChapter);
   BuildStrandModel(pChapter);
   return pChapter;
}

CChapterBuilder* CM3CMaterialDetailsChapterBuilder::Clone() const
{
   return new CM3CMaterialDetailsChapterBuilder(m_pDoc);
}

void CM3CMaterialDetailsChapterBuilder::BuildConcreteModel(rptChapter* pChapter) const
{
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   *pPara << _T("Confined Concrete: Mander's Model") << rptNewLine;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   *pPara << pLayoutTable << rptNewLine;

   (*pLayoutTable)(0,0) << rptRcImage(std::_tstring(rptStyleManager::GetImagePath()) + _T("MandersModel.png"));

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE(rptStressUnitValue, mod_e, pDisplayUnits->ModE, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dim, pDisplayUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, true);

   CComPtr<IStressStrain> ssConcrete;
   m_pDoc->GetConcreteModel(&ssConcrete);

   CComQIPtr<IManderModel> mander_model(ssConcrete);
   Float64 fco, eco;
   mander_model->get_fco(&fco);
   mander_model->get_eco(&eco);

   pPara = &((*pLayoutTable)(0, 1));

   *pPara << Sub2(_T("f'"),_T("co")) << _T(" = ") << stress.SetValue(fco) << rptNewLine;
   *pPara << Sub2(symbol(epsilon),_T("co")) << _T(" = ") << eco << rptNewLine;

   CComPtr<IManderModelSection> mander_model_section;
   mander_model->get_Section(&mander_model_section);

   CComQIPtr<ICircularManderSection> circular_model(mander_model_section);
   Float64 Asp, As, db, S, D, cover;
   circular_model->get_Asp(&Asp);
   circular_model->get_As(&As);
   circular_model->get_db(&db);
   circular_model->get_S(&S);
   circular_model->get_Diameter(&D);
   circular_model->get_Cover(&cover);

   *pPara << _T("Column Diameter (D) ") << _T(" = ") << dim.SetValue(D) << rptNewLine;
   *pPara << _T("Cover ") << _T(" = ") << dim.SetValue(cover) << rptNewLine;
   *pPara << Sub2(_T("A"),_T("s")) << _T(" = ") << area.SetValue(As) << rptNewLine;
   *pPara << Sub2(_T("A"),_T("sp")) << _T(" = ") << area.SetValue(Asp) << rptNewLine;
   *pPara << Sub2(_T("d"),_T("b")) << _T(" = ") << dim.SetValue(db) << rptNewLine;
   *pPara << _T("S") << _T(" = ") << dim.SetValue(S) << rptNewLine;


   *pPara << rptNewLine;

   Float64 fyh, ps, ke;
   mander_model_section->get_TransvYieldStrength(&fyh);
   mander_model_section->get_TransvReinforcementRatio(&ps);
   mander_model_section->get_ConfinementEffectivenessCoefficient(&ke);

   *pPara << Sub2(_T("f"),_T("yh")) << _T(" = ") << stress.SetValue(fyh) << rptNewLine;
   *pPara << Sub2(symbol(rho),_T("s")) << _T(" = ") << ps << rptNewLine;
   *pPara << Sub2(_T("k"),_T("e")) << _T(" = ") << ke << rptNewLine;


   *pPara << rptNewLine;

   Float64 fr, fcc, ecc;
   mander_model->GetConcreteParameters(&fr, &fcc, &ecc);
   *pPara << Sub2(_T("f'"),_T("l")) << _T(" = ") << stress.SetValue(fr) << rptNewLine;
   *pPara << Sub2(_T("f'"),_T("cc")) << _T(" = ") << stress.SetValue(fcc) << rptNewLine;
   *pPara << Sub2(symbol(epsilon), _T("cc")) << _T(" = ") << ecc << rptNewLine;

   Float64 Ec;
   ssConcrete->get_ModulusOfElasticity(&Ec);
   *pPara << Sub2(_T("E"),_T("c")) << _T(" = ") << mod_e.SetValue(Ec) << rptNewLine;
}

void CM3CMaterialDetailsChapterBuilder::BuildRebarModel(rptChapter* pChapter) const
{
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   *pPara << _T("Mild Reinforcement: Elastic-Plastic with Strain Hardening") << rptNewLine;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   *pPara << pLayoutTable << rptNewLine;

   (*pLayoutTable)(0,0) << rptRcImage(std::_tstring(rptStyleManager::GetImagePath()) + _T("StrainHardenedRebarModel.png")) << rptNewLine;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE(rptStressUnitValue, mod_e, pDisplayUnits->ModE, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dim, pDisplayUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, true);

   CComPtr<IStressStrain> ssRebar;
   m_pDoc->GetRebarModel(&ssRebar);

   CComQIPtr<IStrainHardenedRebarModel> shRebar(ssRebar);

   Float64 fy, fu, Es, esh, efr;
   shRebar->get_fy(&fy);
   shRebar->get_fu(&fu);
   shRebar->get_Es(&Es);
   shRebar->get_esh(&esh);
   shRebar->get_efr(&efr);

   pPara = &((*pLayoutTable)(0, 1));

   *pPara << Sub2(_T("f"),_T("y")) << _T(" = ") << stress.SetValue(fy) << rptNewLine;
   *pPara << Sub2(_T("f"),_T("u")) << _T(" = ") << stress.SetValue(fu) << rptNewLine;
   *pPara << Sub2(_T("E"),_T("s")) << _T(" = ") << mod_e.SetValue(Es) << rptNewLine;
   *pPara << Sub2(symbol(epsilon),_T("sh")) << _T(" = ") << esh << rptNewLine;
   *pPara << Sub2(symbol(epsilon),_T("fr")) << _T(" = ") << efr << rptNewLine;

}

void CM3CMaterialDetailsChapterBuilder::BuildStrandModel(rptChapter* pChapter) const
{
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   *pPara << _T("Unbonded Tendon: Power Formula") << rptNewLine;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   *pPara << pLayoutTable << rptNewLine;

   (*pLayoutTable)(0, 0) << rptRcImage(std::_tstring(rptStyleManager::GetImagePath()) + _T("PowerFormula.png")) << rptNewLine;

   //CEAFApp* pApp = EAFGetApp();
   //const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   //INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   //INIT_UV_PROTOTYPE(rptStressUnitValue, mod_e, pDisplayUnits->ModE, true);
   //INIT_UV_PROTOTYPE(rptLengthUnitValue, dim, pDisplayUnits->ComponentDim, true);
   //INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, true);

   //CComPtr<IStressStrain> ssRebar;
   //m_pDoc->GetRebarModel(&ssRebar);

   //CComQIPtr<IStrainHardenedRebarModel> shRebar(ssRebar);

   //Float64 fy, fu, Es, esh, efr;
   //shRebar->get_fy(&fy);
   //shRebar->get_fu(&fu);
   //shRebar->get_Es(&Es);
   //shRebar->get_esh(&esh);
   //shRebar->get_efr(&efr);

   //pPara = &((*pLayoutTable)(0, 1));

   //*pPara << Sub2(_T("f"), _T("y")) << _T(" = ") << stress.SetValue(fy) << rptNewLine;
   //*pPara << Sub2(_T("f"), _T("u")) << _T(" = ") << stress.SetValue(fu) << rptNewLine;
   //*pPara << Sub2(_T("E"), _T("s")) << _T(" = ") << mod_e.SetValue(Es) << rptNewLine;
   //*pPara << Sub2(symbol(epsilon), _T("sh")) << _T(" = ") << esh << rptNewLine;
   //*pPara << Sub2(symbol(epsilon), _T("fr")) << _T(" = ") << efr << rptNewLine;

}
