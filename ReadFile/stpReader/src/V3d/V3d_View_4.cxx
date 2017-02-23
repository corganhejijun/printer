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

#include <V3d_View.jxx>

#include <TColStd_Array2OfReal.hxx>

#include <gp_Dir2d.hxx>

#include <Aspect.hxx>

#include <V3d_RectangularGrid.hxx>
#include <V3d_CircularGrid.hxx>

#define MYEPSILON1 0.0001		// Comparison with 0.0
#define MYEPSILON2 M_PI / 180.	// Delta between 2 angles

/*----------------------------------------------------------------------*/

void V3d_View::SetGrid (const gp_Ax3& aPlane, const Handle(Aspect_Grid)& aGrid)
{
  MyPlane	= aPlane;
  MyGrid	= aGrid;

  Standard_Real xl, yl, zl;
  Standard_Real xdx, xdy, xdz;
  Standard_Real ydx, ydy, ydz;
  Standard_Real dx, dy, dz;
  aPlane.Location ().Coord (xl, yl, zl);
  aPlane.XDirection ().Coord (xdx, xdy, xdz);
  aPlane.YDirection ().Coord (ydx, ydy, ydz);
  aPlane.Direction ().Coord (dx, dy, dz);

  Standard_Real CosAlpha = Cos (MyGrid->RotationAngle ());
  Standard_Real SinAlpha = Sin (MyGrid->RotationAngle ());

  TColStd_Array2OfReal Trsf1 (1, 4, 1, 4);
  Trsf1 (4, 4) = 1.0;
  Trsf1 (4, 1) = Trsf1 (4, 2) = Trsf1 (4, 3) = 0.0;
  // Translation
  Trsf1 (1, 4) = xl,
  Trsf1 (2, 4) = yl,
  Trsf1 (3, 4) = zl;
  // Transformation change of marker
  Trsf1 (1, 1) = xdx,
  Trsf1 (2, 1) = xdy,
  Trsf1 (3, 1) = xdz,
  Trsf1 (1, 2) = ydx,
  Trsf1 (2, 2) = ydy,
  Trsf1 (3, 2) = ydz,
  Trsf1 (1, 3) = dx,
  Trsf1 (2, 3) = dy,
  Trsf1 (3, 3) = dz;

  TColStd_Array2OfReal Trsf2 (1, 4, 1, 4);
  Trsf2 (4, 4) = 1.0;
  Trsf2 (4, 1) = Trsf2 (4, 2) = Trsf2 (4, 3) = 0.0;
  // Translation of the origin
  Trsf2 (1, 4) = -MyGrid->XOrigin (),
  Trsf2 (2, 4) = -MyGrid->YOrigin (),
  Trsf2 (3, 4) = 0.0;
  // Rotation Alpha around axis -Z
  Trsf2 (1, 1) = CosAlpha,
  Trsf2 (2, 1) = -SinAlpha,
  Trsf2 (3, 1) = 0.0,
  Trsf2 (1, 2) = SinAlpha,
  Trsf2 (2, 2) = CosAlpha,
  Trsf2 (3, 2) = 0.0,
  Trsf2 (1, 3) = 0.0,
  Trsf2 (2, 3) = 0.0,
  Trsf2 (3, 3) = 1.0;

  Standard_Real valuetrsf;
  Standard_Real valueoldtrsf;
  Standard_Real valuenewtrsf;
  Standard_Integer i, j, k;
  // Calculation of the product of matrices
  for (i=1; i<=4; i++)
      for (j=1; j<=4; j++) {
    MyTrsf (i, j) = 0.0;
    for (k=1; k<=4; k++) {
        valueoldtrsf = Trsf1 (i, k);
        valuetrsf	 = Trsf2 (k, j);
        valuenewtrsf = MyTrsf (i, j) + valueoldtrsf * valuetrsf;
        MyTrsf (i, j) = valuenewtrsf;
    }
     }
}

void V3d_View::SetGridActivity (const Standard_Boolean AFlag)
{
  if (AFlag) MyGrid->Activate ();
  else MyGrid->Deactivate ();
}

void V3d_View::SetGridGraphicValues (const Handle(Aspect_Grid)& )
{
}


void toPolarCoords (const Standard_Real theX, const Standard_Real theY, 
                          Standard_Real& theR, Standard_Real& thePhi)
{
  theR = Sqrt (theX * theX + theY * theY);
  thePhi = ATan2 (theY, theX);  
}

void toCartesianCoords (const Standard_Real theR, const Standard_Real thePhi, 
                              Standard_Real& theX, Standard_Real& theY)
{
  theX = theR * Cos (thePhi);
  theY = theR * Sin (thePhi);
}

Graphic3d_Vertex V3d_View::Compute (const Graphic3d_Vertex & AVertex) const
{
  Graphic3d_Vertex CurPoint, NewPoint;
  Standard_Real X1, Y1, Z1, X2, Y2, Z2;
  Standard_Real XPp, YPp;

  gp_Dir aRefPlane = myCamera->Direction().Reversed();
  X1 = aRefPlane.X(); Y1 = aRefPlane.Y(); Z1 = aRefPlane.Z();
  MyPlane.Direction ().Coord (X2, Y2, Z2);

  gp_Dir VPN (X1, Y1, Z1);
  gp_Dir GPN (X2, Y2, Z2);

  AVertex.Coord (X1, Y1, Z1);
  Project (X1, Y1, Z1, XPp, YPp);

  // Casw when the plane of the grid and the plane of the view
  // are perpendicular to MYEPSILON2 close radians
  if (Abs (VPN.Angle (GPN) - M_PI / 2.) < MYEPSILON2) {
    NewPoint.SetCoord (X1, Y1, Z1);
    return NewPoint;
  }

  Standard_Boolean IsRectangular = 
    MyGrid->IsKind (STANDARD_TYPE (Aspect_RectangularGrid));

  Graphic3d_Vertex P1;

  Standard_Real x0, y0, z0, x1, y1, z1, x2, y2, z2;
    
  P1.SetCoord (0.0, 0.0, 0.0);
  CurPoint = V3d_View::TrsPoint (P1, MyTrsf);
  CurPoint.Coord (x0, y0, z0);
    
  // get grid axes in world space
  P1.SetCoord (1.0, 0.0, 0.0);
  CurPoint = V3d_View::TrsPoint (P1, MyTrsf);
  CurPoint.Coord (x1, y1, z1);
  gp_Vec aGridX (gp_Pnt (x0, y0, z0), gp_Pnt (x1, y1, z1));
  aGridX.Normalize();

  P1.SetCoord (0.0, 1.0, 0.0);
  CurPoint = V3d_View::TrsPoint (P1, MyTrsf);
  CurPoint.Coord (x2, y2, z2);
  gp_Vec aGridY (gp_Pnt (x0, y0, z0), gp_Pnt (x2, y2, z2));
  aGridY.Normalize();

  // get grid normal
  MyPlane.Direction().Coord (x2, y2, z2);
  gp_Vec aPlaneNormal (x2, y2, z2);

  gp_Vec aPointOnPlane = gp_Vec (0.0, 0.0, 0.0);

  AVertex.Coord (x1, y1, z1);
    
  // project ray from camera onto grid plane
  if (!myCamera->IsOrthographic())
  {
    gp_Vec aPointFromCamera = gp_Vec (myCamera->Eye(), gp_Pnt (x1, y1, z1));
    aPointFromCamera.Normalize();

    Standard_Real aT = - gp_Vec (myCamera->Eye().XYZ()).Dot (aPlaneNormal) / 
                                  aPointFromCamera.Dot (aPlaneNormal);
    aPointOnPlane = gp_Vec (myCamera->Eye().XYZ()) + aPointFromCamera * aT;
  } else
  {
    gp_Vec aPointFromCamera (myCamera->Direction());
    gp_Vec aPointOnCamera (gp_Vec (x1, y1, z1) - aPointFromCamera);

    Standard_Real aT = - aPointOnCamera.Dot (aPlaneNormal) / 
                          aPointFromCamera.Dot (aPlaneNormal);
    aPointOnPlane = aPointOnCamera + aPointFromCamera * aT;
  }

  if (IsRectangular) {
    Standard_Real XS, YS;
    Handle(Aspect_RectangularGrid) theGrid =
      *(Handle(Aspect_RectangularGrid) *) &MyGrid;
    XS = theGrid->XStep (), YS = theGrid->YStep ();

    // project point on plane to grid local space
    gp_Vec aToPoint (gp_Pnt (x0, y0, z0), 
                     gp_Pnt (aPointOnPlane.X(), aPointOnPlane.Y(), aPointOnPlane.Z()));
    Standard_Real anXSteps = Round (aGridX.Dot (aToPoint) / XS);
    Standard_Real anYSteps = Round (aGridY.Dot (aToPoint) / YS);

    // clamp point to grid
    gp_Vec aResult = aGridX * anXSteps * XS + aGridY * anYSteps * YS + gp_Vec (x0, y0, z0);
    NewPoint.SetCoord (aResult.X(), aResult.Y(), aResult.Z());

  } 
  else // IsCircular
  {
    Standard_Real RS;
    Standard_Integer DN;
    Standard_Real Alpha;
    Handle(Aspect_CircularGrid) theGrid =
      *(Handle(Aspect_CircularGrid) *) &MyGrid;
    RS = theGrid->RadiusStep ();
    DN = theGrid->DivisionNumber ();
    Alpha = M_PI / Standard_Real (DN);

    // project point on plane to grid local space
    gp_Vec aToPoint (gp_Pnt (x0, y0, z0), 
                     gp_Pnt (aPointOnPlane.X(), aPointOnPlane.Y(), aPointOnPlane.Z()));

    Standard_Real anR = 0.0, aPhi = 0.0;
    Standard_Real aLocalX = aGridX.Dot (aToPoint);
    Standard_Real aLocalY = aGridY.Dot (aToPoint);
    toPolarCoords (aLocalX, aLocalY, anR, aPhi);

    // clamp point to grid
    Standard_Real anRSteps  = Round (anR / RS);
    Standard_Real aPhiSteps = Round (aPhi / Alpha);
    toCartesianCoords (anRSteps * RS, aPhiSteps * Alpha, aLocalX, aLocalY);

    gp_Vec aResult = aGridX * aLocalX + aGridY * aLocalY + gp_Vec (x0, y0, z0);
    NewPoint.SetCoord (aResult.X(), aResult.Y(), aResult.Z());
  }

  return NewPoint;
}

// Triedron methods : the Triedron is a non-zoomable object.

void V3d_View::ZBufferTriedronSetup(const Quantity_NameOfColor XColor,
                                    const Quantity_NameOfColor YColor,
                                    const Quantity_NameOfColor ZColor,
                                    const Standard_Real        SizeRatio,
                                    const Standard_Real        AxisDiametr,
                                    const Standard_Integer     NbFacettes)
{
  MyView->ZBufferTriedronSetup(XColor, YColor, ZColor, SizeRatio, AxisDiametr, NbFacettes);
}

void V3d_View::TriedronDisplay (const Aspect_TypeOfTriedronPosition APosition,
 const Quantity_NameOfColor AColor, const Standard_Real AScale, const V3d_TypeOfVisualization AMode )
{
  MyView->TriedronDisplay (APosition, AColor, AScale, (AMode == V3d_WIREFRAME));
}

void V3d_View::TriedronErase ( )
{
  MyView->TriedronErase ( );
}

void V3d_View::TriedronEcho (const Aspect_TypeOfTriedronEcho AType )
{
  MyView->TriedronEcho (AType);
}

void V3d_View::GetGraduatedTrihedron(/* Names of axes */
                                     TCollection_ExtendedString &xname,
                                     TCollection_ExtendedString &yname,
                                     TCollection_ExtendedString &zname,
                                     /* Draw names */
                                     Standard_Boolean& xdrawname, 
                                     Standard_Boolean& ydrawname, 
                                     Standard_Boolean& zdrawname,
                                     /* Draw values */
                                     Standard_Boolean& xdrawvalues, 
                                     Standard_Boolean& ydrawvalues, 
                                     Standard_Boolean& zdrawvalues,
                                     /* Draw grid */
                                     Standard_Boolean& drawgrid,
                                     /* Draw axes */
                                     Standard_Boolean& drawaxes,
                                     /* Number of splits along axes */
                                     Standard_Integer& nbx, 
                                     Standard_Integer& nby, 
                                     Standard_Integer& nbz,
                                     /* Offset for drawing values */
                                     Standard_Integer& xoffset, 
                                     Standard_Integer& yoffset, 
                                     Standard_Integer& zoffset,
                                     /* Offset for drawing names of axes */
                                     Standard_Integer& xaxisoffset, 
                                     Standard_Integer& yaxisoffset, 
                                     Standard_Integer& zaxisoffset,
                                     /* Draw tickmarks */
                                     Standard_Boolean& xdrawtickmarks, 
                                     Standard_Boolean& ydrawtickmarks, 
                                     Standard_Boolean& zdrawtickmarks,
                                     /* Length of tickmarks */
                                     Standard_Integer& xtickmarklength, 
                                     Standard_Integer& ytickmarklength, 
                                     Standard_Integer& ztickmarklength,
                                     /* Grid color */
                                     Quantity_Color& gridcolor,
                                     /* X name color */
                                     Quantity_Color& xnamecolor,
                                     /* Y name color */
                                     Quantity_Color& ynamecolor,
                                     /* Z name color */
                                     Quantity_Color& znamecolor,
                                     /* X color of axis and values */
                                     Quantity_Color& xcolor,
                                     /* Y color of axis and values */
                                     Quantity_Color& ycolor,
                                     /* Z color of axis and values */
                                     Quantity_Color& zcolor,
                                     /* Name of font for names of axes */
                                     TCollection_AsciiString &fontOfNames,
                                     /* Style of names of axes */
                                     Font_FontAspect& styleOfNames,
                                     /* Size of names of axes */
                                     Standard_Integer& sizeOfNames,
                                     /* Name of font for values */
                                     TCollection_AsciiString &fontOfValues,
                                     /* Style of values */
                                     Font_FontAspect& styleOfValues,
                                     /* Size of values */
                                     Standard_Integer& sizeOfValues) const
{
    MyView->GetGraduatedTrihedron(/* Names of axes */
                                  xname, 
                                  yname, 
                                  zname,
                                  /* Draw names */
                                  xdrawname, 
                                  ydrawname, 
                                  zdrawname,
                                  /* Draw values */
                                  xdrawvalues, 
                                  ydrawvalues, 
                                  zdrawvalues,
                                  /* Draw grid */
                                  drawgrid,
                                  /* Draw axes */
                                  drawaxes,
                                  /* Number of splits along axes */
                                  nbx, 
                                  nby, 
                                  nbz,
                                  /* Offset for drawing values */
                                  xoffset, 
                                  yoffset, 
                                  zoffset,
                                  /* Offset for drawing names of axes */
                                  xaxisoffset, 
                                  yaxisoffset, 
                                  zaxisoffset,
                                  /* Draw tickmarks */
                                  xdrawtickmarks, 
                                  ydrawtickmarks, 
                                  zdrawtickmarks,
                                  /* Length of tickmarks */
                                  xtickmarklength, 
                                  ytickmarklength, 
                                  ztickmarklength,
                                  /* Grid color */
                                  gridcolor,
                                  /* X name color */
                                  xnamecolor,
                                  /* Y name color */
                                  ynamecolor,
                                  /* Z name color */
                                  znamecolor,
                                  /* X color of axis and values */
                                  xcolor,
                                  /* Y color of axis and values */
                                  ycolor,
                                  /* Z color of axis and values */
                                  zcolor,
                                  /* Name of font for names of axes */
                                  fontOfNames,
                                  /* Style of names of axes */
                                  styleOfNames,
                                  /* Size of names of axes */
                                  sizeOfNames,
                                  /* Name of font for values */
                                  fontOfValues,
                                  /* Style of values */
                                  styleOfValues,
                                  /* Size of values */
                                  sizeOfValues);
}

void V3d_View::GraduatedTrihedronDisplay(/* Names of axes */
                                         const TCollection_ExtendedString &xname, 
                                         const TCollection_ExtendedString &yname, 
                                         const TCollection_ExtendedString &zname,
                                         /* Draw names */
                                         const Standard_Boolean xdrawname, 
                                         const Standard_Boolean ydrawname, 
                                         const Standard_Boolean zdrawname,
                                         /* Draw values */
                                         const Standard_Boolean xdrawvalues, 
                                         const Standard_Boolean ydrawvalues, 
                                         const Standard_Boolean zdrawvalues,
                                         /* Draw grid */
                                         const Standard_Boolean drawgrid,
                                         /* Draw axes */
                                         const Standard_Boolean drawaxes,
                                         /* Number of splits along axes */
                                         const Standard_Integer nbx, 
                                         const Standard_Integer nby, 
                                         const Standard_Integer nbz,
                                         /* Offset for drawing values */
                                         const Standard_Integer xoffset, 
                                         const Standard_Integer yoffset, 
                                         const Standard_Integer zoffset,
                                         /* Offset for drawing names of axes */
                                         const Standard_Integer xaxisoffset, 
                                         const Standard_Integer yaxisoffset, 
                                         const Standard_Integer zaxisoffset,
                                         /* Draw tickmarks */
                                         const Standard_Boolean xdrawtickmarks, 
                                         const Standard_Boolean ydrawtickmarks, 
                                         const Standard_Boolean zdrawtickmarks,
                                         /* Length of tickmarks */
                                         const Standard_Integer xtickmarklength, 
                                         const Standard_Integer ytickmarklength, 
                                         const Standard_Integer ztickmarklength,
                                         /* Grid color */
                                         const Quantity_Color& gridcolor,
                                         /* X name color */
                                         const Quantity_Color& xnamecolor,
                                         /* Y name color */
                                         const Quantity_Color& ynamecolor,
                                         /* Z name color */
                                         const Quantity_Color& znamecolor,
                                         /* X color of axis and values */
                                         const Quantity_Color& xcolor,
                                         /* Y color of axis and values */
                                         const Quantity_Color& ycolor,
                                         /* Z color of axis and values */
                                         const Quantity_Color& zcolor,
                                         /* Name of font for names of axes */
                                         const TCollection_AsciiString &fontOfNames,
                                         /* Style of names of axes */
                                         const Font_FontAspect styleOfNames,
                                         /* Size of names of axes */
                                         const Standard_Integer sizeOfNames,
                                         /* Name of font for values */
                                         const TCollection_AsciiString &fontOfValues,
                                         /* Style of values */
                                         const Font_FontAspect styleOfValues,
                                         /* Size of values */
                                         const Standard_Integer sizeOfValues)
{
    MyView->GraduatedTrihedronDisplay(/* Names of axes */
                                      xname, 
                                      yname, 
                                      zname,
                                      /* Draw names */
                                      xdrawname, 
                                      ydrawname, 
                                      zdrawname,
                                      /* Draw values */
                                      xdrawvalues, 
                                      ydrawvalues, 
                                      zdrawvalues,
                                      /* Draw grid */
                                      drawgrid,
                                      /* Draw axes */
                                      drawaxes,
                                      /* Number of splits along axes */
                                      nbx, 
                                      nby, 
                                      nbz,
                                      /* Offset for drawing values */
                                      xoffset, 
                                      yoffset, 
                                      zoffset,
                                      /* Offset for drawing names of axes */
                                      xaxisoffset, 
                                      yaxisoffset, 
                                      zaxisoffset,
                                      /* Draw tickmarks */
                                      xdrawtickmarks, 
                                      ydrawtickmarks, 
                                      zdrawtickmarks,
                                      /* Length of tickmarks */
                                      xtickmarklength, 
                                      ytickmarklength, 
                                      ztickmarklength,
                                      /* Grid color */
                                      gridcolor,
                                      /* X name color */
                                      xnamecolor,
                                      /* Y name color */
                                      ynamecolor,
                                      /* Z name color */
                                      znamecolor,
                                      /* X color of axis and values */
                                      xcolor,
                                      /* Y color of axis and values */
                                      ycolor,
                                      /* Z color of axis and values */
                                      zcolor,
                                      /* Name of font for names of axes */
                                      fontOfNames,
                                      /* Style of names of axes */
                                      styleOfNames,
                                      /* Size of names of axes */
                                      sizeOfNames,
                                      /* Name of font for values */
                                      fontOfValues,
                                      /* Style of values */
                                      styleOfValues,
                                      /* Size of values */
                                      sizeOfValues);
}

void V3d_View::GraduatedTrihedronErase()
{
    MyView->GraduatedTrihedronErase();
}
