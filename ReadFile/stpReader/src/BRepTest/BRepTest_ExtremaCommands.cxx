// Created on: 1995-09-08
// Created by: Modelistation
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

// modified by mps (juillet 96) : ajout de la commande distmini

#include <DBRep.hxx>
#include <BRepTest.hxx>
#include <BRepExtrema_Poly.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepLib_MakeEdge.hxx>
#include <BRepLib_MakeVertex.hxx>
#include <Draw.hxx>

//#ifdef WNT
#include <stdio.h>
//#endif

//=======================================================================
//function : distance
//purpose  : 
//=======================================================================

static Standard_Integer distance (Draw_Interpretor& di,
				  Standard_Integer n,
				  const char** a)
{
  if (n < 3) return 1;

  const char *name1 = a[1];
  const char *name2 = a[2];

  TopoDS_Shape S1 = DBRep::Get(name1);
  TopoDS_Shape S2 = DBRep::Get(name2);
  if (S1.IsNull() || S2.IsNull()) return 1;
  gp_Pnt P1,P2;
  Standard_Real D;
  if (!BRepExtrema_Poly::Distance(S1,S2,P1,P2,D)) return 1;
  //cout << " distance : " << D << endl;
  di << " distance : " << D << "\n";
  TopoDS_Edge E = (TopoDS_Edge) BRepLib_MakeEdge(P1,P2);
  DBRep::Set("distance",E);
  return 0;
}

static Standard_Integer distmini(Draw_Interpretor& di, Standard_Integer n, const char** a)
{ 
  Standard_Integer i1;
  //  gp_Pnt P;

  if (n != 4) return 1;

  const char *ns1 = (a[2]), *ns2 = (a[3]), *ns0 = (a[1]);
  TopoDS_Shape S1(DBRep::Get(ns1)), S2(DBRep::Get(ns2))  ;
  BRepExtrema_DistShapeShape dst(S1 ,S2 );


  if (dst.IsDone()) 
  { 
#ifdef OCCT_DEBUG
    //dst.Dump(cout);
    di << "*** Dump of \"BRepExtrema_DistShapeShape\" in DEBUG mode (begin) *****\n";
    Standard_SStream aSStream;
    dst.Dump(aSStream);
    di << aSStream;
    di << "*** Dump of \"BRepExtrema_DistShapeShape\" in DEBUG mode (end)   *****\n";
#endif

    di << "\"distmini\" command returns:\n";

    char named[100];
    Sprintf(named, "%s%s" ,ns0,"_val");
    char* tempd = named;
    Draw::Set(tempd,dst.Value());
    di << named << " ";

    for (i1=1; i1<= dst.NbSolution(); i1++)
    {
      gp_Pnt P1,P2;
      P1 = (dst.PointOnShape1(i1));
      P2 = (dst.PointOnShape2(i1));
      if (dst.Value()<=1.e-9) 
      {
        TopoDS_Vertex V = (TopoDS_Vertex) BRepLib_MakeVertex(P1);
        char namev[100];
        if (i1==1) 
          Sprintf(namev, "%s" ,ns0);
        else
          Sprintf(namev, "%s%d" ,ns0,i1);
        char* tempv = namev;
        DBRep::Set(tempv,V);
        di << namev << " ";
      }
      else
      {
        char name[100];
        TopoDS_Edge E = (TopoDS_Edge) BRepLib_MakeEdge (P1, P2);
        if (i1==1)
        {
          Sprintf(name,"%s",ns0);
        }
        else
        {
          Sprintf(name,"%s%d",ns0,i1);
        }
        
        char* temp = name;
        DBRep::Set(temp,E);
        di << name << " " ;
      }
    }

    di << "\nOutput is complete.\n";

  }
  
  else di << "probleme"<< "\n";
  //else cout << "probleme"<< endl;
  return 0;
}

//=======================================================================
//function : ExtremaCommands
//purpose  : 
//=======================================================================

void BRepTest::ExtremaCommands (Draw_Interpretor& theCommands)
{
  static const char*      aGroup = "TOPOLOGY Extrema commands";
  static Standard_Boolean isDone = Standard_False;
  if (isDone)
  {
    return;
  }
  isDone = Standard_True;

  theCommands.Add ("dist",
                   "dist Shape1 Shape2",
                   __FILE__,
                   distance,
                   aGroup);

  theCommands.Add ("distmini",
                   "distmini name Shape1 Shape2",
                   __FILE__,
                   distmini,
                   aGroup);
}
