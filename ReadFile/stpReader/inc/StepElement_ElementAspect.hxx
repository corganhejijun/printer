// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepElement_ElementAspect_HeaderFile
#define _StepElement_ElementAspect_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <StepData_SelectType.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Standard_Transient.hxx>
#include <Handle_StepData_SelectMember.hxx>
#include <StepElement_ElementVolume.hxx>
#include <StepElement_CurveEdge.hxx>
class Standard_Transient;
class StepData_SelectMember;


//! Representation of STEP SELECT type ElementAspect
class StepElement_ElementAspect  : public StepData_SelectType
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Empty constructor
  Standard_EXPORT StepElement_ElementAspect();
  
  //! Recognizes a kind of ElementAspect select type
  //! return 0
  Standard_EXPORT   Standard_Integer CaseNum (const Handle(Standard_Transient)& ent)  const;
  
  //! Recognizes a items of select member ElementAspectMember
  //! 1 -> ElementVolume
  //! 2 -> Volume3dFace
  //! 3 -> Volume2dFace
  //! 4 -> Volume3dEdge
  //! 5 -> Volume2dEdge
  //! 6 -> Surface3dFace
  //! 7 -> Surface2dFace
  //! 8 -> Surface3dEdge
  //! 9 -> Surface2dEdge
  //! 10 -> CurveEdge
  //! 0 else
  Standard_EXPORT virtual   Standard_Integer CaseMem (const Handle(StepData_SelectMember)& ent)  const;
  
  //! Returns a new select member the type ElementAspectMember
  Standard_EXPORT virtual   Handle(StepData_SelectMember) NewMember()  const;
  
  //! Set Value for ElementVolume
  Standard_EXPORT   void SetElementVolume (const StepElement_ElementVolume aVal) ;
  
  //! Returns Value as ElementVolume (or Null if another type)
  Standard_EXPORT   StepElement_ElementVolume ElementVolume()  const;
  
  //! Set Value for Volume3dFace
  Standard_EXPORT   void SetVolume3dFace (const Standard_Integer aVal) ;
  
  //! Returns Value as Volume3dFace (or Null if another type)
  Standard_EXPORT   Standard_Integer Volume3dFace()  const;
  
  //! Set Value for Volume2dFace
  Standard_EXPORT   void SetVolume2dFace (const Standard_Integer aVal) ;
  
  //! Returns Value as Volume2dFace (or Null if another type)
  Standard_EXPORT   Standard_Integer Volume2dFace()  const;
  
  //! Set Value for Volume3dEdge
  Standard_EXPORT   void SetVolume3dEdge (const Standard_Integer aVal) ;
  
  //! Returns Value as Volume3dEdge (or Null if another type)
  Standard_EXPORT   Standard_Integer Volume3dEdge()  const;
  
  //! Set Value for Volume2dEdge
  Standard_EXPORT   void SetVolume2dEdge (const Standard_Integer aVal) ;
  
  //! Returns Value as Volume2dEdge (or Null if another type)
  Standard_EXPORT   Standard_Integer Volume2dEdge()  const;
  
  //! Set Value for Surface3dFace
  Standard_EXPORT   void SetSurface3dFace (const Standard_Integer aVal) ;
  
  //! Returns Value as Surface3dFace (or Null if another type)
  Standard_EXPORT   Standard_Integer Surface3dFace()  const;
  
  //! Set Value for Surface2dFace
  Standard_EXPORT   void SetSurface2dFace (const Standard_Integer aVal) ;
  
  //! Returns Value as Surface2dFace (or Null if another type)
  Standard_EXPORT   Standard_Integer Surface2dFace()  const;
  
  //! Set Value for Surface3dEdge
  Standard_EXPORT   void SetSurface3dEdge (const Standard_Integer aVal) ;
  
  //! Returns Value as Surface3dEdge (or Null if another type)
  Standard_EXPORT   Standard_Integer Surface3dEdge()  const;
  
  //! Set Value for Surface2dEdge
  Standard_EXPORT   void SetSurface2dEdge (const Standard_Integer aVal) ;
  
  //! Returns Value as Surface2dEdge (or Null if another type)
  Standard_EXPORT   Standard_Integer Surface2dEdge()  const;
  
  //! Set Value for CurveEdge
  Standard_EXPORT   void SetCurveEdge (const StepElement_CurveEdge aVal) ;
  
  //! Returns Value as CurveEdge (or Null if another type)
  Standard_EXPORT   StepElement_CurveEdge CurveEdge()  const;




protected:





private:





};







#endif // _StepElement_ElementAspect_HeaderFile