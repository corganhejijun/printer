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

#include <Prs3d_Drawer.ixx>

// =======================================================================
// function : Prs3d_Drawer
// purpose  :
// =======================================================================
Prs3d_Drawer::Prs3d_Drawer()
: myNbPoints (30),
  myIsoOnPlane (Standard_False),
  myFreeBoundaryDraw (Standard_True),
  myUnFreeBoundaryDraw (Standard_True),
  myWireDraw (Standard_True),
  myShadingAspect (new Prs3d_ShadingAspect()),
  myShadingAspectGlobal (Standard_True),
  myChordialDeviation (0.0001),
  myTypeOfDeflection (Aspect_TOD_RELATIVE),
  myMaximalParameterValue (500000.),
  myDeviationCoefficient (0.001),
  myHLRDeviationCoefficient (0.02),
  myDeviationAngle (12.0 * M_PI / 180.0),
  myHLRAngle (20.0 * M_PI / 180.0),
  myVertexDrawMode (Prs3d_VDM_Isolated),
  myLineDrawArrow (Standard_False),
  myDrawHiddenLine (Standard_False),
  myFaceBoundaryDraw (Standard_False),
  myTypeOfHLR (Prs3d_TOH_PolyAlgo)
{
  myDimensionModelUnits.SetLengthUnits ("m");
  myDimensionModelUnits.SetAngleUnits ("rad");
  myDimensionDisplayUnits.SetLengthUnits ("m");
  myDimensionDisplayUnits.SetAngleUnits ("deg");
}

void Prs3d_Drawer::SetTypeOfDeflection(const Aspect_TypeOfDeflection aTypeOfDeflection){

  myTypeOfDeflection = aTypeOfDeflection;}

Aspect_TypeOfDeflection Prs3d_Drawer::TypeOfDeflection() const {

  return myTypeOfDeflection;

}

 
void Prs3d_Drawer::SetIsoOnPlane(const Standard_Boolean OnOff)
{
  myIsoOnPlane = OnOff;
}

 
Standard_Boolean Prs3d_Drawer::IsoOnPlane()const
{
  return myIsoOnPlane;
}


Standard_Integer Prs3d_Drawer::Discretisation() const
{
  return myNbPoints;
}


void Prs3d_Drawer::SetDiscretisation(const Standard_Integer d) 
{
  myNbPoints = d;
}


void Prs3d_Drawer::SetMaximalChordialDeviation(
                               const Quantity_Length aChordialDeviation) {
  myChordialDeviation = aChordialDeviation;
}

Quantity_Length Prs3d_Drawer::MaximalChordialDeviation() const {
  return myChordialDeviation;
}

//
//=======================================================================
//function : SetDeviationCoefficient
//purpose  : 
//=======================================================================

void Prs3d_Drawer::SetDeviationCoefficient (const Standard_Real aCoefficient) {

  myDeviationCoefficient = aCoefficient;
}

//=======================================================================
//function : DeviationCoefficient
//purpose  : 
//=======================================================================

Standard_Real Prs3d_Drawer::DeviationCoefficient () const {

  return myDeviationCoefficient;
}

//=======================================================================
//function : SetHLRDeviationCoefficient
//purpose  : 
//=======================================================================

void Prs3d_Drawer::SetHLRDeviationCoefficient (const Standard_Real aCoefficient) {

  myHLRDeviationCoefficient = aCoefficient;
}

//=======================================================================
//function : HLRDeviationCoefficient
//purpose  : 
//=======================================================================

Standard_Real Prs3d_Drawer::HLRDeviationCoefficient () const {

  return myHLRDeviationCoefficient;
}

//=======================================================================
//function : SetHLRAngle
//purpose  : 
//=======================================================================

void Prs3d_Drawer::SetHLRAngle (const Standard_Real anAngle) {

  myHLRAngle = anAngle;
}

//=======================================================================
//function : HLRAngle
//purpose  : 
//=======================================================================

Standard_Real Prs3d_Drawer::HLRAngle () const {

  return myHLRAngle;
}

//=======================================================================
//function : SetDeviationAngle
//purpose  : 
//=======================================================================

void Prs3d_Drawer::SetDeviationAngle (const Standard_Real anAngle)
{
  myDeviationAngle = anAngle;
}

//=======================================================================
//function : DeviationAngle
//purpose  : 
//=======================================================================

Standard_Real Prs3d_Drawer::DeviationAngle () const 
{
  return myDeviationAngle;
}


void Prs3d_Drawer::SetMaximalParameterValue (const Standard_Real Value) {
  myMaximalParameterValue = Value;
}

Standard_Real Prs3d_Drawer::MaximalParameterValue () const {
  return myMaximalParameterValue;
}

Handle (Prs3d_IsoAspect) Prs3d_Drawer::UIsoAspect (){

  if (myUIsoAspect.IsNull()) 
    myUIsoAspect = new Prs3d_IsoAspect
                   (Quantity_NOC_GRAY75,Aspect_TOL_SOLID,0.5,1);

  return myUIsoAspect;
}

void Prs3d_Drawer::SetUIsoAspect ( const Handle(Prs3d_IsoAspect)& anAspect) {
 myUIsoAspect = anAspect;
}

Handle (Prs3d_IsoAspect) Prs3d_Drawer::VIsoAspect () {
  if (myVIsoAspect.IsNull()) 
    myVIsoAspect = new Prs3d_IsoAspect
      (Quantity_NOC_GRAY75,Aspect_TOL_SOLID,0.5,1);
      
  return myVIsoAspect;
}

void Prs3d_Drawer::SetVIsoAspect ( const Handle(Prs3d_IsoAspect)& anAspect) {
 myVIsoAspect = anAspect;
}

Handle (Prs3d_LineAspect) Prs3d_Drawer::FreeBoundaryAspect () {
  if (myFreeBoundaryAspect.IsNull())
    myFreeBoundaryAspect = new Prs3d_LineAspect
      (Quantity_NOC_GREEN,Aspect_TOL_SOLID,1.);

  return myFreeBoundaryAspect;
}

void Prs3d_Drawer::SetFreeBoundaryAspect (const Handle(Prs3d_LineAspect)& anAspect) {
 myFreeBoundaryAspect = anAspect;
}

void Prs3d_Drawer::SetFreeBoundaryDraw ( const Standard_Boolean OnOff ) {
  myFreeBoundaryDraw = OnOff;
}

Standard_Boolean Prs3d_Drawer::FreeBoundaryDraw () const {
return myFreeBoundaryDraw;
}

Handle (Prs3d_LineAspect) Prs3d_Drawer::UnFreeBoundaryAspect (){
  if  (myUnFreeBoundaryAspect.IsNull())
    myUnFreeBoundaryAspect = new Prs3d_LineAspect
      (Quantity_NOC_YELLOW,Aspect_TOL_SOLID,1.);
  return myUnFreeBoundaryAspect;
}

void Prs3d_Drawer::SetUnFreeBoundaryAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 myUnFreeBoundaryAspect = anAspect;
}

void Prs3d_Drawer::SetUnFreeBoundaryDraw ( const Standard_Boolean OnOff ) {
  myUnFreeBoundaryDraw = OnOff;
}

Standard_Boolean Prs3d_Drawer::UnFreeBoundaryDraw () const {
  return myUnFreeBoundaryDraw;
}

Handle (Prs3d_LineAspect) Prs3d_Drawer::WireAspect ()  {
  if (myWireAspect.IsNull())
   myWireAspect = new Prs3d_LineAspect(Quantity_NOC_RED,Aspect_TOL_SOLID,1.);

  return myWireAspect;
}

void Prs3d_Drawer::SetWireAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 myWireAspect = anAspect;
}
void Prs3d_Drawer::SetWireDraw ( const Standard_Boolean OnOff ) {
  myWireDraw = OnOff;
}

Standard_Boolean Prs3d_Drawer::WireDraw () const {
return myWireDraw;
}


Handle (Prs3d_LineAspect) Prs3d_Drawer::LineAspect ()  {
  if (myLineAspect.IsNull())
    myLineAspect = new Prs3d_LineAspect
      (Quantity_NOC_YELLOW,Aspect_TOL_SOLID,1.);
  return myLineAspect;
}

void Prs3d_Drawer::SetLineAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 myLineAspect = anAspect;
}

Handle (Prs3d_TextAspect) Prs3d_Drawer::TextAspect ()  {
  if (myTextAspect.IsNull())
    myTextAspect = new Prs3d_TextAspect();

  return myTextAspect;
}

void Prs3d_Drawer::SetTextAspect ( const Handle(Prs3d_TextAspect)& anAspect) {
 myTextAspect = anAspect;
}

Handle (Prs3d_ShadingAspect) Prs3d_Drawer::ShadingAspect ()  {
  if (myShadingAspect.IsNull())
  myShadingAspect = new Prs3d_ShadingAspect();

  return myShadingAspect;
}

void Prs3d_Drawer::SetShadingAspect ( const Handle(Prs3d_ShadingAspect)& anAspect) {
 myShadingAspect = anAspect;
}

void Prs3d_Drawer::SetShadingAspectGlobal(const Standard_Boolean aValue) {
 myShadingAspectGlobal = aValue;
}

Standard_Boolean Prs3d_Drawer::ShadingAspectGlobal() const { 
 
 return myShadingAspectGlobal;

}
 
void Prs3d_Drawer::SetLineArrowDraw ( const Standard_Boolean OnOff ) {
  myLineDrawArrow = OnOff;
}

Standard_Boolean Prs3d_Drawer::LineArrowDraw () const {
return myLineDrawArrow;
}

Handle (Prs3d_ArrowAspect) Prs3d_Drawer::ArrowAspect() {
  if (myArrowAspect.IsNull())
    myArrowAspect = new Prs3d_ArrowAspect;

  return myArrowAspect;
}

void Prs3d_Drawer::SetArrowAspect ( const Handle(Prs3d_ArrowAspect)& anAspect) {
 myArrowAspect = anAspect;
}

Handle (Prs3d_PointAspect) Prs3d_Drawer::PointAspect() {
  if (myPointAspect.IsNull())
    myPointAspect = new Prs3d_PointAspect 
      (Aspect_TOM_PLUS,Quantity_NOC_YELLOW,1.);

  return myPointAspect;
}

void Prs3d_Drawer::SetPointAspect ( const Handle(Prs3d_PointAspect)& anAspect) {
 myPointAspect = anAspect;
}

void Prs3d_Drawer::SetVertexDrawMode (const Prs3d_VertexDrawMode theMode)
{
  // Prs3d_VDM_Inherited value is allowed at AIS_Drawer level.
  // Replacing it by Prs3d_VDM_Isolated to avoid unpredictable behavior.
  myVertexDrawMode = (theMode == Prs3d_VDM_Inherited ? Prs3d_VDM_Isolated : theMode);
}

Prs3d_VertexDrawMode Prs3d_Drawer::VertexDrawMode () const
{
  return myVertexDrawMode;
}

Standard_Boolean Prs3d_Drawer::DrawHiddenLine () const {return myDrawHiddenLine;}

void Prs3d_Drawer::EnableDrawHiddenLine () {myDrawHiddenLine=Standard_True;}

void Prs3d_Drawer::DisableDrawHiddenLine () {myDrawHiddenLine=Standard_False;}

Handle (Prs3d_LineAspect) Prs3d_Drawer::HiddenLineAspect ()  {
 if (myHiddenLineAspect.IsNull())
   myHiddenLineAspect = new Prs3d_LineAspect
     (Quantity_NOC_YELLOW,Aspect_TOL_DASH,0.5);

  return myHiddenLineAspect;
}

void Prs3d_Drawer::SetHiddenLineAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 myHiddenLineAspect = anAspect;
}

Handle (Prs3d_LineAspect) Prs3d_Drawer::SeenLineAspect ()  {
  if (mySeenLineAspect.IsNull())
    mySeenLineAspect = new Prs3d_LineAspect
      (Quantity_NOC_YELLOW,Aspect_TOL_SOLID,1.);

  return mySeenLineAspect;
}

void Prs3d_Drawer::SetSeenLineAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 mySeenLineAspect = anAspect;
}

Handle (Prs3d_LineAspect) Prs3d_Drawer::VectorAspect ()  {
  if (myVectorAspect.IsNull())
    myVectorAspect = new Prs3d_LineAspect
      (Quantity_NOC_SKYBLUE,Aspect_TOL_SOLID,1.);

  return myVectorAspect;
}

void Prs3d_Drawer::SetVectorAspect ( const Handle(Prs3d_LineAspect)& anAspect) {
 myVectorAspect = anAspect;
}

Handle (Prs3d_DatumAspect) Prs3d_Drawer::DatumAspect () {
  if (myDatumAspect.IsNull())
    myDatumAspect = new Prs3d_DatumAspect;

  return myDatumAspect;
}

void Prs3d_Drawer::SetDatumAspect ( const Handle(Prs3d_DatumAspect)& anAspect) {
 myDatumAspect = anAspect;
}

Handle (Prs3d_PlaneAspect) Prs3d_Drawer::PlaneAspect () {
  if (myPlaneAspect.IsNull())
    myPlaneAspect = new Prs3d_PlaneAspect;

  return myPlaneAspect;
}

void Prs3d_Drawer::SetPlaneAspect ( const Handle(Prs3d_PlaneAspect)& anAspect) {
  myPlaneAspect = anAspect;
}

// =======================================================================
// function : DimensionAspect
// purpose  :
// =======================================================================
Handle(Prs3d_DimensionAspect) Prs3d_Drawer::DimensionAspect()
{
  if (myDimensionAspect.IsNull())
  {
    myDimensionAspect = new Prs3d_DimensionAspect;
  }

  return myDimensionAspect;
}

// =======================================================================
// function : SetDimensionAspect
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetDimensionAspect (const Handle(Prs3d_DimensionAspect)& theAspect)
{
  myDimensionAspect = theAspect;
}

// =======================================================================
// function : SetDimLengthModelUnits
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetDimLengthModelUnits (const TCollection_AsciiString& theUnits)
{
  myDimensionModelUnits.SetLengthUnits (theUnits);
}

// =======================================================================
// function : SetDimAngleModelUnits
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetDimAngleModelUnits (const TCollection_AsciiString& theUnits)
{
  myDimensionModelUnits.SetAngleUnits (theUnits);
}

// =======================================================================
// function : DimLengthModelUnits
// purpose  :
// =======================================================================
const TCollection_AsciiString& Prs3d_Drawer::DimLengthModelUnits() const
{
  return myDimensionModelUnits.GetLengthUnits();
}

// =======================================================================
// function : DimAngleModelUnits
// purpose  :
// =======================================================================
const TCollection_AsciiString& Prs3d_Drawer::DimAngleModelUnits() const
{
  return myDimensionModelUnits.GetAngleUnits();
}

// =======================================================================
// function : SetDimLengthDisplayUnits
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetDimLengthDisplayUnits (const TCollection_AsciiString& theUnits)
{
  myDimensionDisplayUnits.SetLengthUnits (theUnits);
}

// =======================================================================
// function : SetDimAngleDisplayUnits
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetDimAngleDisplayUnits (const TCollection_AsciiString& theUnits)
{
  myDimensionDisplayUnits.SetAngleUnits (theUnits);
}

// =======================================================================
// function : DimLengthDisplayUnits
// purpose  :
// =======================================================================
const TCollection_AsciiString& Prs3d_Drawer::DimLengthDisplayUnits() const
{
  return myDimensionDisplayUnits.GetLengthUnits();
}

// =======================================================================
// function : DimAngleDisplayUnits
// purpose  :
// =======================================================================
const TCollection_AsciiString& Prs3d_Drawer::DimAngleDisplayUnits() const
{
  return myDimensionDisplayUnits.GetAngleUnits();
}

// =======================================================================
// function : SectionAspect
// purpose  :
// =======================================================================
Handle (Prs3d_LineAspect) Prs3d_Drawer::SectionAspect()
{
  if (mySectionAspect.IsNull())
  {
    mySectionAspect = new Prs3d_LineAspect (Quantity_NOC_ORANGE, Aspect_TOL_SOLID, 1.0);
  }

  return mySectionAspect;
}

// =======================================================================
// function : SetSectionAspect
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetSectionAspect (const Handle(Prs3d_LineAspect)& theAspect)
{
  mySectionAspect = theAspect;
}

// =======================================================================
// function : SetFaceBoundaryDraw
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetFaceBoundaryDraw (const Standard_Boolean theIsEnabled)
{
  myFaceBoundaryDraw = theIsEnabled;
}

// =======================================================================
// function : IsFaceBoundaryDraw
// purpose  :
// =======================================================================
Standard_Boolean Prs3d_Drawer::IsFaceBoundaryDraw () const
{
  return myFaceBoundaryDraw;
}

// =======================================================================
// function : SetFaceBoundaryAspect
// purpose  :
// =======================================================================
void Prs3d_Drawer::SetFaceBoundaryAspect (const Handle(Prs3d_LineAspect)& theAspect)
{
  myFaceBoundaryAspect = theAspect;
}

// =======================================================================
// function : FaceBoundaryAspect
// purpose  :
// =======================================================================
Handle(Prs3d_LineAspect) Prs3d_Drawer::FaceBoundaryAspect ()
{
  if (myFaceBoundaryAspect.IsNull ())
  {
    myFaceBoundaryAspect = 
      new Prs3d_LineAspect (Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.0);
  }

  return myFaceBoundaryAspect;
}

// =======================================================================
// function : SetTypeOfHLR
// purpose  : set type of HLR algorithm
// =======================================================================
void Prs3d_Drawer::SetTypeOfHLR ( const Prs3d_TypeOfHLR theTypeOfHLR) 
{
  myTypeOfHLR = theTypeOfHLR;
}

// =======================================================================
// function : TypeOfHLR
// purpose  : gets type of HLR algorithm
// =======================================================================
Prs3d_TypeOfHLR Prs3d_Drawer::TypeOfHLR ( ) const
{
  return myTypeOfHLR;
}
