// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepShape_OrientedFace_HeaderFile
#define _StepShape_OrientedFace_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StepShape_OrientedFace.hxx>

#include <Handle_StepShape_Face.hxx>
#include <Standard_Boolean.hxx>
#include <StepShape_Face.hxx>
#include <Handle_TCollection_HAsciiString.hxx>
#include <Handle_StepShape_HArray1OfFaceBound.hxx>
#include <Handle_StepShape_FaceBound.hxx>
#include <Standard_Integer.hxx>
class StepShape_Face;
class TCollection_HAsciiString;
class StepShape_HArray1OfFaceBound;
class StepShape_FaceBound;



class StepShape_OrientedFace : public StepShape_Face
{

public:

  
  //! Returns a OrientedFace
  Standard_EXPORT StepShape_OrientedFace();
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName, const Handle(StepShape_HArray1OfFaceBound)& aBounds) ;
  
  Standard_EXPORT virtual   void Init (const Handle(TCollection_HAsciiString)& aName, const Handle(StepShape_Face)& aFaceElement, const Standard_Boolean aOrientation) ;
  
  Standard_EXPORT   void SetFaceElement (const Handle(StepShape_Face)& aFaceElement) ;
  
  Standard_EXPORT   Handle(StepShape_Face) FaceElement()  const;
  
  Standard_EXPORT   void SetOrientation (const Standard_Boolean aOrientation) ;
  
  Standard_EXPORT   Standard_Boolean Orientation()  const;
  
  Standard_EXPORT virtual   void SetBounds (const Handle(StepShape_HArray1OfFaceBound)& aBounds) ;
  
  Standard_EXPORT virtual   Handle(StepShape_HArray1OfFaceBound) Bounds()  const;
  
  Standard_EXPORT virtual   Handle(StepShape_FaceBound) BoundsValue (const Standard_Integer num)  const;
  
  Standard_EXPORT virtual   Standard_Integer NbBounds()  const;




  DEFINE_STANDARD_RTTI(StepShape_OrientedFace)

protected:




private: 


  Handle(StepShape_Face) faceElement;
  Standard_Boolean orientation;


};







#endif // _StepShape_OrientedFace_HeaderFile