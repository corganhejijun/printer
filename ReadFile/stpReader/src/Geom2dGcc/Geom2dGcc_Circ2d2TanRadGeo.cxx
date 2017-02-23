// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#include <Geom2dGcc_Circ2d2TanRadGeo.ixx>

#include <ElCLib.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Lin2d.hxx>
#include <Standard_NegativeValue.hxx>
#include <Standard_OutOfRange.hxx>
#include <StdFail_NotDone.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <GccEnt_BadQualifier.hxx>
#include <IntRes2d_Domain.hxx>
#include <IntRes2d_IntersectionPoint.hxx>

#include <Geom2dGcc_CurveTool.hxx>
#include <Adaptor3d_OffsetCurve.hxx>
#include <Geom2dAdaptor_HCurve.hxx>
#include <Geom2dGcc_CurveToolGeo.hxx>
#include <Geom2dInt_GInter.hxx>

// circulaire tant a une courbe et une droite ,de rayon donne
//==============================================================

//========================================================================
// On initialise WellDone a false.                                       +
// On recupere la courbe Cu2 et la droite L1.                            +
// On sort en erreur dans les cas ou la construction est impossible.     +
// On fait la parallele a Cu2 dans le bon sens.                          +
// On fait la parallele a L1 dans le bon sens.                           +
// On intersecte les paralleles ==> point de centre de la solution.      +
// On cree la solution qu on ajoute aux solutions deja trouvees.         +
// On remplit les champs.                                                +
//========================================================================

Geom2dGcc_Circ2d2TanRadGeo::
Geom2dGcc_Circ2d2TanRadGeo (const GccEnt_QualifiedLin&  Qualified1,
                            const Geom2dGcc_QCurve&     Qualified2,
                            const Standard_Real         Radius    ,
                            const Standard_Real         Tolerance ):

//========================================================================
// initialisation des champs.                                            +
//========================================================================

cirsol(1,16)   ,
qualifier1(1,16),
qualifier2(1,16),
TheSame1(1,16) ,
TheSame2(1,16) ,
pnttg1sol(1,16),
pnttg2sol(1,16),
par1sol(1,16)  ,
par2sol(1,16)  ,
pararg1(1,16)  ,
pararg2(1,16)  
{

  //========================================================================
  // Traitement.                                                           +
  //========================================================================

  Standard_Real Tol = Abs(Tolerance);
  Standard_Real thefirst = -100000.;
  Standard_Real thelast  =  100000.;
  Standard_Real firstparam;
  Standard_Real lastparam;
  gp_Dir2d dirx(1.,0.);
  TColStd_Array1OfReal cote1(1,2);
  TColStd_Array1OfReal cote2(1,2);
  Standard_Integer nbrcote1=0;
  Standard_Integer nbrcote2=0;
  WellDone = Standard_False;
  NbrSol = 0;
  if (!(Qualified1.IsEnclosed() ||
    Qualified1.IsOutside() || Qualified1.IsUnqualified()) ||
    !(Qualified2.IsEnclosed() || Qualified2.IsEnclosing() || 
    Qualified2.IsOutside() || Qualified2.IsUnqualified())) {

      GccEnt_BadQualifier::Raise();
      return;
  }
  gp_Lin2d L1 = Qualified1.Qualified();
  Standard_Real x1dir = (L1.Direction()).X();
  Standard_Real y1dir = (L1.Direction()).Y();
  Standard_Real lxloc = (L1.Location()).X();
  Standard_Real lyloc = (L1.Location()).Y();
  gp_Pnt2d origin1(lxloc,lyloc);
  gp_Dir2d normL1(-y1dir,x1dir);
  Geom2dAdaptor_Curve Cu2= Qualified2.Qualified();
  if (Radius < 0.0) { Standard_NegativeValue::Raise(); }
  else {
    if (Qualified1.IsEnclosed() && Qualified2.IsEnclosed()) {
      //   =======================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsEnclosed() && Qualified2.IsOutside()) {
      //   ==========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = -Radius;
    }
    else if (Qualified1.IsOutside() && Qualified2.IsEnclosed()) {
      //   ===========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsOutside()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = -Radius;
    }
    if(Qualified1.IsEnclosed() && Qualified2.IsUnqualified()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsEnclosed()) {
      //   =========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsUnqualified()) {
      //   =============================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsOutside()) {
      //   ========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = -Radius;
    }
    else if(Qualified1.IsUnqualified() && Qualified2.IsUnqualified()) {
      //   =================================================================
      nbrcote1 = 2;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    gp_Dir2d Dir(-y1dir,x1dir);
    for (Standard_Integer jcote1 = 1 ; jcote1 <= nbrcote1 ; jcote1++) {
      gp_Pnt2d Point(L1.Location().XY()+cote1(jcote1)*Dir.XY());
      gp_Lin2d Line(Point,L1.Direction()); // ligne avec deport.
      IntRes2d_Domain D1;
      for (Standard_Integer jcote2 = 1 ; jcote2 <= nbrcote2 ; jcote2++) {
        Handle(Geom2dAdaptor_HCurve) HCu2 = new Geom2dAdaptor_HCurve(Cu2);
        Adaptor3d_OffsetCurve C2(HCu2,cote2(jcote2));
        firstparam = Max(Geom2dGcc_CurveToolGeo::FirstParameter(C2),thefirst);
        lastparam  = Min(Geom2dGcc_CurveToolGeo::LastParameter(C2),thelast);
        IntRes2d_Domain D2(Geom2dGcc_CurveToolGeo::Value(C2,firstparam),firstparam,Tol,
          Geom2dGcc_CurveToolGeo::Value(C2,lastparam),lastparam,Tol);
        Geom2dInt_TheIntConicCurveOfGInter Intp(Line,D1,C2,D2,Tol,Tol);
        if (Intp.IsDone()) {
          if (!Intp.IsEmpty()) {
            for (Standard_Integer i = 1 ; i <= Intp.NbPoints() ; i++) {
              NbrSol++;
              gp_Pnt2d Center(Intp.Point(i).Value());
              cirsol(NbrSol) = gp_Circ2d(gp_Ax2d(Center,dirx),Radius);
              //             =======================================================
              gp_Dir2d dc1(origin1.XY()-Center.XY());
              qualifier2(NbrSol) = Qualified2.Qualifier();
              if (!Qualified1.IsUnqualified()) { 
                qualifier1(NbrSol) = Qualified1.Qualifier();
              }
              else if (dc1.Dot(normL1) > 0.0) {
                qualifier1(NbrSol) = GccEnt_outside;
              }
              else { qualifier1(NbrSol) = GccEnt_enclosed; }
              TheSame1(NbrSol) = 0;
              TheSame2(NbrSol) = 0;
              pararg1(NbrSol) = Intp.Point(i).ParamOnFirst();
              pararg2(NbrSol) = Intp.Point(i).ParamOnSecond();
              pnttg1sol(NbrSol) = ElCLib::Value(pararg1(NbrSol),L1);
              pnttg2sol(NbrSol) = Geom2dGcc_CurveTool::Value(Cu2,pararg2(NbrSol));
              par1sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg1sol(NbrSol));
              par2sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg2sol(NbrSol));
            }
          }
          WellDone = Standard_True;
        }
      }
    }
  }
}

// circulaire tant a une courbe et un cercle ,de rayon donne
//=============================================================

//========================================================================
// On initialise WellDone a false.                                       +
// On recupere la courbe Cu2 et le cercle C1.                            +
// On sort en erreur dans les cas ou la construction est impossible.     +
// On fait la parallele a Cu2 dans le bon sens.                          +
// On fait la parallele a C1 dans le bon sens.                           +
// On intersecte les paralleles ==> point de centre de la solution.      +
// On cree la solution qu on ajoute aux solutions deja trouvees.         +
// On remplit les champs.                                                +
//========================================================================

Geom2dGcc_Circ2d2TanRadGeo::
Geom2dGcc_Circ2d2TanRadGeo (const GccEnt_QualifiedCirc& Qualified1,
                            const Geom2dGcc_QCurve&     Qualified2,
                            const Standard_Real         Radius    ,
                            const Standard_Real         Tolerance ):

//========================================================================
// initialisation des champs.                                            +
//========================================================================

cirsol(1,16)   ,
qualifier1(1,16),
qualifier2(1,16),
TheSame1(1,16) ,
TheSame2(1,16) ,
pnttg1sol(1,16),
pnttg2sol(1,16),
par1sol(1,16)  ,
par2sol(1,16)  ,
pararg1(1,16)  ,
pararg2(1,16)  
{

  //========================================================================
  // Traitement.                                                           +
  //========================================================================

  Standard_Real Tol = Abs(Tolerance);
  Standard_Real thefirst = -100000.;
  Standard_Real thelast  =  100000.;
  Standard_Real firstparam;
  Standard_Real lastparam;
  gp_Dir2d dirx(1.,0.);
  TColStd_Array1OfReal cote1(1,2);
  TColStd_Array1OfReal cote2(1,2);
  Standard_Integer nbrcote1=0;
  Standard_Integer nbrcote2=0;
  WellDone = Standard_False;
  NbrSol = 0;
  if (!(Qualified1.IsEnclosed() || Qualified1.IsEnclosing() || 
    Qualified1.IsOutside() || Qualified1.IsUnqualified()) ||
    !(Qualified2.IsEnclosed() || Qualified2.IsEnclosing() || 
    Qualified2.IsOutside() || Qualified2.IsUnqualified())) {
      GccEnt_BadQualifier::Raise();
      return;
  }
  gp_Circ2d C1 = Qualified1.Qualified();
  gp_Pnt2d center1(C1.Location());
  Geom2dAdaptor_Curve Cu2 = Qualified2.Qualified();
  if (Radius < 0.0) { Standard_NegativeValue::Raise(); }
  else {
    if (Qualified1.IsEnclosed() && Qualified2.IsEnclosed()) {
      //   =======================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsEnclosed() && Qualified2.IsOutside()) {
      //   ==========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = -Radius;
    }
    else if (Qualified1.IsOutside() && Qualified2.IsEnclosed()) {
      //   ===========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsOutside()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = -Radius;
    }
    if(Qualified1.IsEnclosed() && Qualified2.IsUnqualified()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsEnclosed()) {
      //   =========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsUnqualified()) {
      //   =============================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsOutside()) {
      //   ========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = -Radius;
    }
    else if(Qualified1.IsUnqualified() && Qualified2.IsUnqualified()) {
      //   =================================================================
      nbrcote1 = 2;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    Standard_Real R1 = C1.Radius();
    Geom2dInt_TheIntConicCurveOfGInter Intp;
    for (Standard_Integer jcote1 = 1 ; jcote1 <= nbrcote1 ; jcote1++) {
      gp_Circ2d Circ(C1.XAxis(),R1+cote1(jcote1));
      IntRes2d_Domain D1(ElCLib::Value(0.,Circ),   0.,Tol,
        ElCLib::Value(2.*M_PI,Circ),2.*M_PI,Tol);
      D1.SetEquivalentParameters(0.,2.*M_PI);
      for (Standard_Integer jcote2 = 1 ; jcote2 <= nbrcote2 ; jcote2++) {
        Handle(Geom2dAdaptor_HCurve) HCu2 = new Geom2dAdaptor_HCurve(Cu2);
        Adaptor3d_OffsetCurve C2(HCu2,cote2(jcote2));
        firstparam = Max(Geom2dGcc_CurveToolGeo::FirstParameter(C2),thefirst);
        lastparam  = Min(Geom2dGcc_CurveToolGeo::LastParameter(C2),thelast);
        IntRes2d_Domain D2(Geom2dGcc_CurveToolGeo::Value(C2,firstparam),firstparam,Tol,
          Geom2dGcc_CurveToolGeo::Value(C2,lastparam),lastparam,Tol);
        Intp.Perform(Circ,D1,C2,D2,Tol,Tol);
        if (Intp.IsDone()) {
          if (!Intp.IsEmpty()) {
            for (Standard_Integer i = 1 ; i <= Intp.NbPoints() ; i++) {
              NbrSol++;
              gp_Pnt2d Center(Intp.Point(i).Value());
              cirsol(NbrSol) = gp_Circ2d(gp_Ax2d(Center,dirx),Radius);
              //             =======================================================
#ifdef OCCT_DEBUG
              gp_Dir2d dir1(Center.XY()-center1.XY());
#else
              Center.XY() ;
              center1.XY() ;
#endif
              Standard_Real distcc1 = Center.Distance(center1);
              if (!Qualified1.IsUnqualified()) { 
                qualifier1(NbrSol) = Qualified1.Qualifier();
              }
              else if (Abs(distcc1+Radius-R1) < Tol) {
                qualifier1(NbrSol) = GccEnt_enclosed;
              }
              else if (Abs(distcc1-R1-Radius) < Tol) {
                qualifier1(NbrSol) = GccEnt_outside;
              }
              else { qualifier1(NbrSol) = GccEnt_enclosing; }
              qualifier2(NbrSol) = Qualified2.Qualifier();
              TheSame1(NbrSol) = 0;
              TheSame2(NbrSol) = 0;
              pararg1(NbrSol) = Intp.Point(i).ParamOnFirst();
              pararg2(NbrSol) = Intp.Point(i).ParamOnSecond();
              pnttg1sol(NbrSol) = ElCLib::Value(pararg1(NbrSol),C1);
              pnttg2sol(NbrSol) = Geom2dGcc_CurveTool::Value(Cu2,pararg2(NbrSol));
              par1sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg1sol(NbrSol));
              par2sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg2sol(NbrSol));
            }
          }
          WellDone = Standard_True;
        }
      }
    }
  }
}

// circulaire tant a une courbe et un point ,de rayon donne
//============================================================

//========================================================================
// On initialise WellDone a false.                                       +
// On recupere la courbe Cu1 et le point P2.                             +
// On sort en erreur dans les cas ou la construction est impossible.     +
// On fait la parallele a Cu1 dans le bon sens.                          +
// On fait la parallele a P2 dans le bon sens.                           +
// On intersecte les paralleles ==> point de centre de la solution.      +
// On cree la solution qu on ajoute aux solutions deja trouvees.         +
// On remplit les champs.                                                +
//========================================================================

Geom2dGcc_Circ2d2TanRadGeo::
Geom2dGcc_Circ2d2TanRadGeo (const Geom2dGcc_QCurve& Qualified1,
                            const gp_Pnt2d&     Point2    ,
                            const Standard_Real Radius    ,
                            const Standard_Real Tolerance ):

//========================================================================
// initialisation des champs.                                            +
//========================================================================

cirsol(1,16)   ,
qualifier1(1,16),
qualifier2(1,16),
TheSame1(1,16) ,
TheSame2(1,16) ,
pnttg1sol(1,16),
pnttg2sol(1,16),
par1sol(1,16)  ,
par2sol(1,16)  ,
pararg1(1,16)  ,
pararg2(1,16)  
{

  //========================================================================
  // Traitement.                                                           +
  //========================================================================

  Standard_Real Tol = Abs(Tolerance);
  Standard_Real thefirst = -100000.;
  Standard_Real thelast  =  100000.;
  Standard_Real firstparam;
  Standard_Real lastparam;
  gp_Dir2d dirx(1.,0.);
  TColStd_Array1OfReal cote1(1,2);
  Standard_Integer nbrcote1=0;
  WellDone = Standard_False;
  NbrSol = 0;
  if (!(Qualified1.IsEnclosed() || Qualified1.IsEnclosing() || 
    Qualified1.IsOutside() || Qualified1.IsUnqualified())) {
      GccEnt_BadQualifier::Raise();
      return;
  }
  Geom2dAdaptor_Curve Cu1 = Qualified1.Qualified();
  if (Radius < 0.0) { Standard_NegativeValue::Raise(); }
  else {
    if (Qualified1.IsEnclosed()) {
      //    ===========================
      nbrcote1 = 1;
      cote1(1) = Radius;
    }
    else if(Qualified1.IsOutside()) {
      //   ===============================
      nbrcote1 = 1;
      cote1(1) = -Radius;
    }
    else if(Qualified1.IsUnqualified()) {
      //   ===================================
      nbrcote1 = 2;
      cote1(1) = Radius;
      cote1(2) = -Radius;
    }
    gp_Circ2d Circ(gp_Ax2d(Point2,gp_Dir2d(1.,0.)),Radius);
    IntRes2d_Domain D1(ElCLib::Value(0.,Circ),   0.,Tol,
      ElCLib::Value(M_PI+M_PI,Circ),M_PI+M_PI,Tol);
    D1.SetEquivalentParameters(0.,M_PI+M_PI);
    Geom2dInt_TheIntConicCurveOfGInter Intp;
    for (Standard_Integer jcote1 = 1 ; jcote1 <= nbrcote1 ; jcote1++) {
      Handle(Geom2dAdaptor_HCurve) HCu1 = new Geom2dAdaptor_HCurve(Cu1);
      Adaptor3d_OffsetCurve Cu2(HCu1,cote1(jcote1));
      firstparam = Max(Geom2dGcc_CurveToolGeo::FirstParameter(Cu2),thefirst);
      lastparam  = Min(Geom2dGcc_CurveToolGeo::LastParameter(Cu2),thelast);
      IntRes2d_Domain D2(Geom2dGcc_CurveToolGeo::Value(Cu2,firstparam),firstparam,Tol,
        Geom2dGcc_CurveToolGeo::Value(Cu2,lastparam),lastparam,Tol);
      Intp.Perform(Circ,D1,Cu2,D2,Tol,Tol);
      if (Intp.IsDone()) {
        if (!Intp.IsEmpty()) {
          for (Standard_Integer i = 1 ; i <= Intp.NbPoints() ; i++) {
            NbrSol++;
            gp_Pnt2d Center(Intp.Point(i).Value());
            cirsol(NbrSol) = gp_Circ2d(gp_Ax2d(Center,dirx),Radius);
            //           =======================================================
            qualifier1(NbrSol) = Qualified1.Qualifier();
            qualifier2(NbrSol) = GccEnt_noqualifier;
            TheSame1(NbrSol) = 0;
            TheSame2(NbrSol) = 0;
            pararg1(NbrSol) = Intp.Point(i).ParamOnSecond();
            pararg2(NbrSol) = 0.;
            pnttg1sol(NbrSol) = Geom2dGcc_CurveTool::Value(Cu1,pararg1(NbrSol));
            pnttg2sol(NbrSol) = Point2;
            par1sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
              pnttg1sol(NbrSol));
            par2sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
              pnttg2sol(NbrSol));
          }
        }
        WellDone = Standard_True;
      }
    }
  }
}

static void PrecRoot(const Adaptor3d_OffsetCurve& theC1,
                     const Adaptor3d_OffsetCurve& theC2,
                     const Standard_Real theU0,
                     const Standard_Real theV0,
                     const Standard_Real theUmin,
                     const Standard_Real theUmax,
                     const Standard_Real theVmin,
                     const Standard_Real theVmax,
                     Standard_Real& theUfinal,
                     Standard_Real& theVfinal)
{
  const Standard_Real aInitStepU = (theUmax - theUmin)/2.0,
    aInitStepV = (theVmax - theVmin)/2.0;

  Standard_Real aStepU = aInitStepU, aStepV = aInitStepV;

  const Standard_Real aTol = Precision::PConfusion() * Precision::PConfusion();
  const Standard_Integer aNbIterMax = 100;

  gp_Pnt2d aP1, aP2;
  gp_Vec2d aD1, aD2;

  Geom2dGcc_CurveToolGeo::D1(theC1, theU0, aP1, aD1);
  Geom2dGcc_CurveToolGeo::D1(theC2, theV0, aP2, aD2);

  gp_Vec2d vP12(aP1.XY() - aP2.XY());

  Standard_Real aU = theU0, aV = theV0;
  theUfinal = theU0;
  theVfinal = theV0;

  Standard_Real aSQDistPrev = aP1.SquareDistance(aP2);

  Standard_Integer aNbIter = 1;

  do
  {
    Standard_Real aDetH = aD1.Y()*aD2.X() - aD1.X()*aD2.Y();
    if(aDetH == 0.0)
      break;

    aU += aStepU*(aD2.Y() * vP12.X() - aD2.X()*vP12.Y())/aDetH;
    aV += aStepV*(aD1.Y() * vP12.X() - aD1.X()*vP12.Y())/aDetH;

    if(Abs(aU - theUmin) > 1000.0)
      //method diverges
      return;

    if(Abs(aU - theUmax) > 1000.0)
      //method diverges
      return;

    if(Abs(aV - theVmin) > 1000.0)
      //method diverges
      return;

    if(Abs(aV - theVmax) > 1000.0)
      //method diverges
      return;

    Geom2dGcc_CurveToolGeo::D1(theC1, aU, aP1, aD1);
    Geom2dGcc_CurveToolGeo::D1(theC2, aV, aP2, aD2);
    const Standard_Real aSQDist = aP1.SquareDistance(aP2);

    if(Precision::IsInfinite(aSQDist))
      //method diverges
      return;

    vP12.SetXY(aP1.XY() - aP2.XY());

    if(aSQDist < aSQDistPrev)
    {
      aSQDistPrev = aSQDist;
      aStepU = aInitStepU;
      aStepV = aInitStepV;
      theUfinal = aU;
      theVfinal = aV;
    }
    else
    {
      aStepU /= 2.0;
      aStepV /= 2.0;
    }
  }
  while((aNbIter++ < aNbIterMax) && ((aStepU > aTol) || (aStepV > aTol)));

  Standard_Boolean isInBound = Standard_True;
  if(theUfinal < theUmin)
  {
    aU = theUfinal;
    aV = theVfinal;

    theUfinal = theUmin;
    isInBound = Standard_False;
  }

  if(theUfinal > theUmax)
  {
    aU = theUfinal;
    aV = theVfinal;

    theUfinal = theUmax;
    isInBound = Standard_False;
  }

  if(!isInBound)
  {
    Geom2dGcc_CurveToolGeo::D1(theC1, aU, aP1, aD1);
    Geom2dGcc_CurveToolGeo::D1(theC2, aV, aP2, aD2);
    Standard_Real aV1 = (aD2.X() == 0.0) ? aV :((theUfinal - aU)*aD1.X() + aV*aD2.X() + (aP1.X() - aP2.X()))/aD2.X();
    Standard_Real aV2 = (aD2.Y() == 0.0) ? aV :((theUfinal - aU)*aD1.Y() + aV*aD2.Y() + (aP1.Y() - aP2.Y()))/aD2.Y();

    if(aV1 < theVmin)
      aV1 = theVmin;

    if(aV1 > theVmax)
      aV1 = theVmax;

    if(aV2 < theVmin)
      aV2 = theVmin;

    if(aV2 > theVmax)
      aV2 = theVmax;

    aP1 = Geom2dGcc_CurveToolGeo::Value(theC1,theUfinal);
    aP2 = Geom2dGcc_CurveToolGeo::Value(theC2,aV1);

    Standard_Real aSQ1 = aP1.SquareDistance(aP2);

    aP2 = Geom2dGcc_CurveToolGeo::Value(theC2,aV2);
    Standard_Real aSQ2 = aP1.SquareDistance(aP2);

    if(aSQ1 < aSQ2)
      theVfinal = aV1;
    else
      theVfinal = aV2;

    return;
  }

  if(theVfinal < theVmin)
  {
    aU = theUfinal;
    aV = theVfinal;

    theVfinal = theVmin;
    isInBound = Standard_False;
  }

  if(theVfinal > theVmax)
  {
    aU = theUfinal;
    aV = theVfinal;

    theVfinal = theVmax;
    isInBound = Standard_False;
  }

  if(isInBound)
    return;

  Geom2dGcc_CurveToolGeo::D1(theC1, aU, aP1, aD1);
  Geom2dGcc_CurveToolGeo::D1(theC2, aV, aP2, aD2);
  Standard_Real aU1 = (aD1.X() == 0.0) ? aU :((theVfinal - aV)*aD2.X() + aU*aD1.X() + (aP2.X() - aP1.X()))/aD1.X();
  Standard_Real aU2 = (aD1.Y() == 0.0) ? aU :((theVfinal - aV)*aD2.Y() + aU*aD1.Y() + (aP2.Y() - aP1.Y()))/aD1.Y();

  if(aU1 < theUmin)
    aU1 = theUmin;

  if(aU1 > theUmax)
    aU1 = theUmax;

  if(aU2 < theUmin)
    aU2 = theUmin;

  if(aU2 > theUmax)
    aU2 = theUmax;

  aP2 = Geom2dGcc_CurveToolGeo::Value(theC2,theVfinal);
  aP1 = Geom2dGcc_CurveToolGeo::Value(theC1,aU1);

  Standard_Real aSQ1 = aP1.SquareDistance(aP2);

  aP1 = Geom2dGcc_CurveToolGeo::Value(theC1,aU2);
  Standard_Real aSQ2 = aP1.SquareDistance(aP2);

  if(aSQ1 < aSQ2)
    theUfinal = aU1;
  else
    theUfinal = aU2;  
}

// circulaire tant a deux courbes ,de rayon donne
//==================================================

//========================================================================
// On initialise WellDone a false.                                       +
// On recupere les courbes Cu1 et Cu2.                                   +
// On sort en erreur dans les cas ou la construction est impossible.     +
// On fait la parallele a Cu1 dans le bon sens.                          +
// On fait la parallele a Cu2 dans le bon sens.                          +
// On intersecte les paralleles ==> point de centre de la solution.      +
// On cree la solution qu on ajoute aux solutions deja trouvees.         +
// On remplit les champs.                                                +
//========================================================================

Geom2dGcc_Circ2d2TanRadGeo::
Geom2dGcc_Circ2d2TanRadGeo (const Geom2dGcc_QCurve& Qualified1,
                            const Geom2dGcc_QCurve& Qualified2,
                            const Standard_Real Radius    ,
                            const Standard_Real Tolerance ):

//========================================================================
// initialisation des champs.                                            +
//========================================================================

cirsol(1,16)   ,
qualifier1(1,16),
qualifier2(1,16),
TheSame1(1,16) ,
TheSame2(1,16) ,
pnttg1sol(1,16),
pnttg2sol(1,16),
par1sol(1,16)  ,
par2sol(1,16)  ,
pararg1(1,16)  ,
pararg2(1,16)  
{

  //========================================================================
  // Traitement.                                                           +
  //========================================================================

  Standard_Real Tol = Abs(Tolerance);
  Standard_Real thefirst = -100000.;
  Standard_Real thelast  =  100000.;
  Standard_Real firstparam;
  Standard_Real lastparam;
  gp_Dir2d dirx(1.,0.);
  TColStd_Array1OfReal cote1(1,2);
  TColStd_Array1OfReal cote2(1,2);
  Standard_Integer nbrcote1=0;
  Standard_Integer nbrcote2=0;
  WellDone = Standard_False;
  NbrSol = 0;
  if (!(Qualified1.IsEnclosed() || Qualified1.IsEnclosing() || 
    Qualified1.IsOutside() || Qualified1.IsUnqualified()) ||
    !(Qualified2.IsEnclosed() || Qualified2.IsEnclosing() || 
    Qualified2.IsOutside() || Qualified2.IsUnqualified())) {
      GccEnt_BadQualifier::Raise();
      return;
  }
  Geom2dAdaptor_Curve Cu1 = Qualified1.Qualified();
  Geom2dAdaptor_Curve Cu2 = Qualified2.Qualified();
  if (Radius < 0.0) { Standard_NegativeValue::Raise(); }
  else {
    if (Qualified1.IsEnclosed() && Qualified2.IsEnclosed()) {
      //   =======================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsEnclosed() && Qualified2.IsOutside()) {
      //   ==========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote2(1) = -Radius;
    }
    else if (Qualified1.IsOutside() && Qualified2.IsEnclosed()) {
      //   ===========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsOutside()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 1;
      cote1(1) = -Radius;
      cote2(1) = -Radius;
    }
    if(Qualified1.IsEnclosed() && Qualified2.IsUnqualified()) {
      //   =========================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsEnclosed()) {
      //   =========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
    }
    else if(Qualified1.IsOutside() && Qualified2.IsUnqualified()) {
      //   =============================================================
      nbrcote1 = 1;
      nbrcote2 = 2;
      cote1(1) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    if(Qualified1.IsUnqualified() && Qualified2.IsOutside()) {
      //   ========================================================
      nbrcote1 = 2;
      nbrcote2 = 1;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = -Radius;
    }
    else if(Qualified1.IsUnqualified() && Qualified2.IsUnqualified()) {
      //   =================================================================
      nbrcote1 = 2;
      nbrcote2 = 2;
      cote1(1) = Radius;
      cote1(2) = -Radius;
      cote2(1) = Radius;
      cote2(2) = -Radius;
    }
    Geom2dInt_GInter Intp;
    for (Standard_Integer jcote1 = 1 ; jcote1 <= nbrcote1 ; jcote1++) {
      Handle(Geom2dAdaptor_HCurve) HCu1 = new Geom2dAdaptor_HCurve(Cu1); 
      Adaptor3d_OffsetCurve C1(HCu1,cote1(jcote1));
      firstparam = Max(Geom2dGcc_CurveToolGeo::FirstParameter(C1),thefirst);
      lastparam  = Min(Geom2dGcc_CurveToolGeo::LastParameter(C1),thelast);
#ifdef OCCT_DEBUG
      IntRes2d_Domain D2(Geom2dGcc_CurveToolGeo::Value(C1,firstparam),firstparam,Tol,
        Geom2dGcc_CurveToolGeo::Value(C1,lastparam),lastparam,Tol);
#else
      Geom2dGcc_CurveToolGeo::Value(C1,firstparam);
      Geom2dGcc_CurveToolGeo::Value(C1,lastparam);
#endif
      for (Standard_Integer jcote2 = 1 ; jcote2 <= nbrcote2 ; jcote2++) {
        Handle(Geom2dAdaptor_HCurve) HCu2 = new Geom2dAdaptor_HCurve(Cu2);
        Adaptor3d_OffsetCurve C2(HCu2,cote2(jcote2));
        firstparam = Max(Geom2dGcc_CurveToolGeo::FirstParameter(C2),thefirst);
        lastparam  = Min(Geom2dGcc_CurveToolGeo::LastParameter(C2),thelast);
#ifdef OCCT_DEBUG
        IntRes2d_Domain D2(Geom2dGcc_CurveToolGeo::Value(C2,firstparam),firstparam,Tol,
          Geom2dGcc_CurveToolGeo::Value(C2,lastparam),lastparam,Tol);
#else
        Geom2dGcc_CurveToolGeo::Value(C2,firstparam);
        Geom2dGcc_CurveToolGeo::Value(C2,lastparam);
#endif
        Intp.Perform(C1,C2,Tol,Tol);
        if (Intp.IsDone()) {
          if (!Intp.IsEmpty()) {
            for (Standard_Integer i = 1 ; i <= Intp.NbPoints() ; i++)
            {
              Standard_Real aU0 = Intp.Point(i).ParamOnFirst();
              Standard_Real aV0 = Intp.Point(i).ParamOnSecond();

              Standard_Real aU1 = aU0-Precision::PApproximation();
              Standard_Real aV1 = aV0-Precision::PApproximation();

              Standard_Real aU2 = aU0+Precision::PApproximation();
              Standard_Real aV2 = aV0+Precision::PApproximation();

              gp_Pnt2d P11 = Geom2dGcc_CurveToolGeo::Value(C1,aU1);
              gp_Pnt2d P12 = Geom2dGcc_CurveToolGeo::Value(C2,aV1);
              gp_Pnt2d P21 = Geom2dGcc_CurveToolGeo::Value(C1,aU2);
              gp_Pnt2d P22 = Geom2dGcc_CurveToolGeo::Value(C2,aV2);

              Standard_Real aDist1112 = P11.Distance(P12);
              Standard_Real aDist1122 = P11.Distance(P22);

              Standard_Real aDist1221 = P12.Distance(P21);
              Standard_Real aDist2122 = P21.Distance(P22);

              if( Min(aDist1112, aDist1122) <= Precision::Approximation() &&
                Min(aDist1221, aDist2122) <= Precision::Approximation())
              {
                PrecRoot(C1, C2, aU0, aV0,
                  Max(Geom2dGcc_CurveToolGeo::FirstParameter(C1), aU0 - 10.0),
                  Min(Geom2dGcc_CurveToolGeo::LastParameter(C1), aU0 + 10.0),
                  Max(Geom2dGcc_CurveToolGeo::FirstParameter(C2), aV0 - 10.0),
                  Min(Geom2dGcc_CurveToolGeo::LastParameter(C2), aV0 + 10.0),
                  aU0, aV0);
              }

              NbrSol++;
              gp_Pnt2d Center(Geom2dGcc_CurveToolGeo::Value(C1, aU0));
              cirsol(NbrSol) = gp_Circ2d(gp_Ax2d(Center,dirx),Radius);
              //             =======================================================
              qualifier1(NbrSol) = Qualified1.Qualifier();
              qualifier1(NbrSol) = Qualified1.Qualifier();
              TheSame1(NbrSol) = 0;
              TheSame2(NbrSol) = 0;
              pararg1(NbrSol) = Intp.Point(i).ParamOnFirst();
              pararg2(NbrSol) = Intp.Point(i).ParamOnSecond();
              pnttg1sol(NbrSol) = Geom2dGcc_CurveTool::Value(Cu1,pararg1(NbrSol));
              pnttg2sol(NbrSol) = Geom2dGcc_CurveTool::Value(Cu2,pararg2(NbrSol));
              par1sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg1sol(NbrSol));
              par2sol(NbrSol)=ElCLib::Parameter(cirsol(NbrSol),
                pnttg2sol(NbrSol));
            }
          }

          WellDone = Standard_True;
        }
      }
    }
  }
}

//=========================================================================

Standard_Boolean Geom2dGcc_Circ2d2TanRadGeo::
IsDone () const { return WellDone; }

Standard_Integer Geom2dGcc_Circ2d2TanRadGeo::
NbSolutions () const { return NbrSol; }

gp_Circ2d Geom2dGcc_Circ2d2TanRadGeo::
ThisSolution (const Standard_Integer Index) const 
{
  if (!WellDone) { StdFail_NotDone::Raise(); }
  if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }
  return cirsol(Index);
}

void Geom2dGcc_Circ2d2TanRadGeo::
WhichQualifier(const Standard_Integer Index   ,
               GccEnt_Position& Qualif1 ,
               GccEnt_Position& Qualif2 ) const
{
  if (!WellDone) { StdFail_NotDone::Raise(); }
  else if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }
  else {
    Qualif1 = qualifier1(Index);
    Qualif2 = qualifier2(Index);
  }
}

void Geom2dGcc_Circ2d2TanRadGeo::
Tangency1 (const Standard_Integer Index,
           Standard_Real&   ParSol,
           Standard_Real&   ParArg,
           gp_Pnt2d&        PntSol) const{
             if (!WellDone) { StdFail_NotDone::Raise(); }
             else if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }
             else {
               if (TheSame1(Index) == 0) {
                 ParSol = par1sol(Index);
                 ParArg = pararg1(Index);
                 PntSol = gp_Pnt2d(pnttg1sol(Index));
               }
               else { StdFail_NotDone::Raise(); }
             }
}

void Geom2dGcc_Circ2d2TanRadGeo::
Tangency2 (const Standard_Integer Index,
           Standard_Real&   ParSol,
           Standard_Real&   ParArg,
           gp_Pnt2d&        PntSol) const{
             if (!WellDone) { StdFail_NotDone::Raise(); }
             else if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }
             else {
               if (TheSame2(Index) == 0) {
                 ParSol = par2sol(Index);
                 ParArg = pararg2(Index);
                 PntSol = gp_Pnt2d(pnttg2sol(Index));
               }
               else { StdFail_NotDone::Raise(); }
             }
}

Standard_Boolean Geom2dGcc_Circ2d2TanRadGeo::
IsTheSame1 (const Standard_Integer Index) const
{
  if (!WellDone) { StdFail_NotDone::Raise(); }
  if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }

  if (TheSame1(Index) == 0) { return Standard_False; }
  return Standard_True;
}

Standard_Boolean Geom2dGcc_Circ2d2TanRadGeo::
IsTheSame2 (const Standard_Integer Index) const
{
  if (!WellDone) { StdFail_NotDone::Raise(); }
  if (Index <= 0 ||Index > NbrSol) { Standard_OutOfRange::Raise(); }

  if (TheSame2(Index) == 0) { return Standard_False; }
  return Standard_True;
}

