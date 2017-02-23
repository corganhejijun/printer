// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepToGeom_MakeSurfaceOfRevolution_HeaderFile
#define _StepToGeom_MakeSurfaceOfRevolution_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Boolean.hxx>
#include <Handle_StepGeom_SurfaceOfRevolution.hxx>
#include <Handle_Geom_SurfaceOfRevolution.hxx>
class StepGeom_SurfaceOfRevolution;
class Geom_SurfaceOfRevolution;


//! This class implements the mapping between class
//! SurfaceOfRevolution from StepGeom which describes a
//! surface_of_revolution from Prostep and SurfaceOfRevolution
//! from Geom
class StepToGeom_MakeSurfaceOfRevolution 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT static   Standard_Boolean Convert (const Handle(StepGeom_SurfaceOfRevolution)& SS, Handle(Geom_SurfaceOfRevolution)& CS) ;




protected:





private:





};







#endif // _StepToGeom_MakeSurfaceOfRevolution_HeaderFile
