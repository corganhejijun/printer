// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepShape_GeometricSet_HeaderFile
#define _StepShape_GeometricSet_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StepShape_GeometricSet.hxx>

#include <Handle_StepShape_HArray1OfGeometricSetSelect.hxx>
#include <StepGeom_GeometricRepresentationItem.hxx>
#include <Handle_TCollection_HAsciiString.hxx>
#include <Standard_Integer.hxx>
class StepShape_HArray1OfGeometricSetSelect;
class TCollection_HAsciiString;
class StepShape_GeometricSetSelect;



class StepShape_GeometricSet : public StepGeom_GeometricRepresentationItem
{

public:

  
  //! Returns a GeometricSet
  Standard_EXPORT StepShape_GeometricSet();
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName) ;
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName, const Handle(StepShape_HArray1OfGeometricSetSelect)& aElements) ;
  
  Standard_EXPORT   void SetElements (const Handle(StepShape_HArray1OfGeometricSetSelect)& aElements) ;
  
  Standard_EXPORT   Handle(StepShape_HArray1OfGeometricSetSelect) Elements()  const;
  
  Standard_EXPORT   StepShape_GeometricSetSelect ElementsValue (const Standard_Integer num)  const;
  
  Standard_EXPORT   Standard_Integer NbElements()  const;




  DEFINE_STANDARD_RTTI(StepShape_GeometricSet)

protected:




private: 


  Handle(StepShape_HArray1OfGeometricSetSelect) elements;


};







#endif // _StepShape_GeometricSet_HeaderFile
