// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepPrimAPI_MakeCylinder_HeaderFile
#define _BRepPrimAPI_MakeCylinder_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <BRepPrim_Cylinder.hxx>
#include <BRepPrimAPI_MakeOneAxis.hxx>
#include <Standard_Real.hxx>
#include <Standard_Address.hxx>
class Standard_DomainError;
class gp_Ax2;
class BRepPrim_Cylinder;


//! Describes functions to build cylinders or portions of  cylinders.
//! A MakeCylinder object provides a framework for:
//! -   defining the construction of a cylinder,
//! -   implementing the construction algorithm, and
//! -   consulting the result.
class BRepPrimAPI_MakeCylinder  : public BRepPrimAPI_MakeOneAxis
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Make a cylinder of radius R and length H.
  Standard_EXPORT BRepPrimAPI_MakeCylinder(const Standard_Real R, const Standard_Real H);
  
  //! Make a cylinder   of  radius R  and  length H with
  //! angle  H.
  Standard_EXPORT BRepPrimAPI_MakeCylinder(const Standard_Real R, const Standard_Real H, const Standard_Real Angle);
  
  //! Make a cylinder of radius R and length H.
  Standard_EXPORT BRepPrimAPI_MakeCylinder(const gp_Ax2& Axes, const Standard_Real R, const Standard_Real H);
  
  //! Make a cylinder   of  radius R  and  length H with
  //! angle  H.
  //! Constructs
  //! -   a cylinder of radius R and height H, or
  //! -   a portion of cylinder of radius R and height H, and of
  //! the angle Angle defining the missing portion of the cylinder.
  //! The cylinder is constructed about the "Z Axis" of either:
  //! -   the global coordinate system, or
  //! -   the local coordinate system Axes.
  //! It is limited in this coordinate system as follows:
  //! -   in the v parametric direction (the Z axis), by the two
  //! parameter values 0 and H,
  //! -   and in the u parametric direction (the rotation angle
  //! around the Z Axis), in the case of a portion of a
  //! cylinder, by the two parameter values 0 and Angle.
  //! Angle is given in radians.
  //! The resulting shape is composed of:
  //! -   a lateral cylindrical face,
  //! -   two planar faces in the planes z = 0 and z = H
  //! (in the case of a complete cylinder, these faces are circles), and
  //! -   in case of a portion of a cylinder, two additional
  //! planar faces to close the shape.(two rectangles in the
  //! planes u = 0 and u = Angle).
  //! Exceptions Standard_DomainError if:
  //! -   R is less than or equal to Precision::Confusion(), or
  //! -   H is less than or equal to Precision::Confusion().
  Standard_EXPORT BRepPrimAPI_MakeCylinder(const gp_Ax2& Axes, const Standard_Real R, const Standard_Real H, const Standard_Real Angle);
  
  //! Returns the algorithm.
  Standard_EXPORT   Standard_Address OneAxis() ;
  
  //! Returns the algorithm.
  Standard_EXPORT   BRepPrim_Cylinder& Cylinder() ;




protected:





private:



  BRepPrim_Cylinder myCylinder;


};







#endif // _BRepPrimAPI_MakeCylinder_HeaderFile
