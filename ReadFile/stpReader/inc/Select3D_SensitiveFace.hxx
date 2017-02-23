// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Select3D_SensitiveFace_HeaderFile
#define _Select3D_SensitiveFace_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Select3D_SensitiveFace.hxx>

#include <Select3D_TypeOfSensitivity.hxx>
#include <Select3D_SensitivePoly.hxx>
#include <Handle_SelectBasics_EntityOwner.hxx>
#include <Handle_TColgp_HArray1OfPnt.hxx>
#include <Standard_Boolean.hxx>
#include <SelectBasics_PickArgs.hxx>
#include <Standard_Real.hxx>
#include <Standard_OStream.hxx>
#include <Handle_Select3D_SensitiveEntity.hxx>
class Standard_ConstructionError;
class Standard_OutOfRange;
class SelectBasics_EntityOwner;
class TColgp_Array1OfPnt;
class TColgp_HArray1OfPnt;
class TColgp_Array1OfPnt2d;
class Bnd_Box2d;
class gp_Lin;
class Select3D_SensitiveEntity;
class TopLoc_Location;


//! Sensitive Entity to make a face selectable.
//! In some cases this class can raise Standard_ConstructionError and
//! Standard_OutOfRange exceptions. For more details see Select3D_SensitivePoly.
class Select3D_SensitiveFace : public Select3D_SensitivePoly
{

public:

  
  //! Constructs a sensitive face object defined by the
  //! owner OwnerId, the array of points ThePoints, and
  //! the sensitivity type Sensitivity.
  //! The array of points is the outer polygon of the geometric face.
  Standard_EXPORT Select3D_SensitiveFace(const Handle(SelectBasics_EntityOwner)& OwnerId, const TColgp_Array1OfPnt& ThePoints, const Select3D_TypeOfSensitivity Sensitivity = Select3D_TOS_INTERIOR);
  
  //! Constructs a sensitive face object defined by the
  //! owner OwnerId, the array of points ThePoints, and
  //! the sensitivity type Sensitivity.
  //! The array of points is the outer polygon of the geometric face.
  Standard_EXPORT Select3D_SensitiveFace(const Handle(SelectBasics_EntityOwner)& OwnerId, const Handle(TColgp_HArray1OfPnt)& ThePoints, const Select3D_TypeOfSensitivity Sensitivity = Select3D_TOS_INTERIOR);
  
  //! Checks whether the sensitive entity matches the picking
  //! detection area (close to the picking line).
  //! For details please refer to base class declaration.
  Standard_EXPORT virtual   Standard_Boolean Matches (const SelectBasics_PickArgs& thePickArgs, Standard_Real& theMatchDMin, Standard_Real& theMatchDepth) ;
  
  Standard_EXPORT virtual   Standard_Boolean Matches (const Standard_Real XMin, const Standard_Real YMin, const Standard_Real XMax, const Standard_Real YMax, const Standard_Real aTol) ;
  
  Standard_EXPORT virtual   Standard_Boolean Matches (const TColgp_Array1OfPnt2d& Polyline, const Bnd_Box2d& aBox, const Standard_Real aTol) ;
  
  //! Computes the depth values for all 3D points defining this face and returns
  //! the minimal value among them.
  //! If the "minimal depth" approach is not suitable and gives wrong detection results
  //! in some particular case, a custom sensitive face class can redefine this method.
  Standard_EXPORT virtual   Standard_Real ComputeDepth (const gp_Lin& thePickLine, const Standard_Real theDepthMin, const Standard_Real theDepthMax)  const;
  
  Standard_EXPORT virtual   void Dump (Standard_OStream& S, const Standard_Boolean FullDump = Standard_True)  const;
  
  //! Returns the copy of this
  Standard_EXPORT virtual   Handle(Select3D_SensitiveEntity) GetConnected (const TopLoc_Location& theLocation) ;




  DEFINE_STANDARD_RTTI(Select3D_SensitiveFace)

protected:




private: 

  
  //! Warning: Obsolete.
  //! Use newer version of the method with min, max limits passed as arguments.
  Standard_EXPORT   void ComputeDepth (const gp_Lin& EyeLine)  const;

  Select3D_TypeOfSensitivity mytype;


};







#endif // _Select3D_SensitiveFace_HeaderFile
