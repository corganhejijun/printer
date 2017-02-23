// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TPrsStd_PointDriver_HeaderFile
#define _TPrsStd_PointDriver_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TPrsStd_PointDriver.hxx>

#include <TPrsStd_Driver.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_AIS_InteractiveObject.hxx>
class TDF_Label;
class AIS_InteractiveObject;


//! An implementation of TPrsStd_Driver for points.
class TPrsStd_PointDriver : public TPrsStd_Driver
{

public:

  
  //! Constructs an empty point driver.
  Standard_EXPORT TPrsStd_PointDriver();
  
  //! Build the AISObject (if null) or update it.
  //! No compute is done.
  //! Returns <True> if informations was found
  //! and AISObject updated.
  Standard_EXPORT virtual   Standard_Boolean Update (const TDF_Label& aLabel, Handle(AIS_InteractiveObject)& anAISObject) ;




  DEFINE_STANDARD_RTTI(TPrsStd_PointDriver)

protected:




private: 




};







#endif // _TPrsStd_PointDriver_HeaderFile