// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepGeom_CompositeCurve_HeaderFile
#define _StepGeom_CompositeCurve_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StepGeom_CompositeCurve.hxx>

#include <Handle_StepGeom_HArray1OfCompositeCurveSegment.hxx>
#include <StepData_Logical.hxx>
#include <StepGeom_BoundedCurve.hxx>
#include <Handle_TCollection_HAsciiString.hxx>
#include <Handle_StepGeom_CompositeCurveSegment.hxx>
#include <Standard_Integer.hxx>
class StepGeom_HArray1OfCompositeCurveSegment;
class TCollection_HAsciiString;
class StepGeom_CompositeCurveSegment;



class StepGeom_CompositeCurve : public StepGeom_BoundedCurve
{

public:

  
  //! Returns a CompositeCurve
  Standard_EXPORT StepGeom_CompositeCurve();
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName) ;
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName, const Handle(StepGeom_HArray1OfCompositeCurveSegment)& aSegments, const StepData_Logical aSelfIntersect) ;
  
  Standard_EXPORT   void SetSegments (const Handle(StepGeom_HArray1OfCompositeCurveSegment)& aSegments) ;
  
  Standard_EXPORT   Handle(StepGeom_HArray1OfCompositeCurveSegment) Segments()  const;
  
  Standard_EXPORT   Handle(StepGeom_CompositeCurveSegment) SegmentsValue (const Standard_Integer num)  const;
  
  Standard_EXPORT   Standard_Integer NbSegments()  const;
  
  Standard_EXPORT   void SetSelfIntersect (const StepData_Logical aSelfIntersect) ;
  
  Standard_EXPORT   StepData_Logical SelfIntersect()  const;




  DEFINE_STANDARD_RTTI(StepGeom_CompositeCurve)

protected:




private: 


  Handle(StepGeom_HArray1OfCompositeCurveSegment) segments;
  StepData_Logical selfIntersect;


};







#endif // _StepGeom_CompositeCurve_HeaderFile