// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _AIS_Circle_HeaderFile
#define _AIS_Circle_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_AIS_Circle.hxx>

#include <Handle_Geom_Circle.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <AIS_InteractiveObject.hxx>
#include <PrsMgr_PresentationManager3d.hxx>
#include <Handle_Prs3d_Presentation.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Prs3d_Projector.hxx>
#include <Handle_Geom_Transformation.hxx>
#include <Handle_SelectMgr_Selection.hxx>
#include <AIS_KindOfInteractive.hxx>
#include <Quantity_NameOfColor.hxx>
class Geom_Circle;
class Prs3d_Presentation;
class Prs3d_Projector;
class Geom_Transformation;
class SelectMgr_Selection;
class Quantity_Color;


//! Constructs circle datums to be used in construction of
//! composite shapes.
class AIS_Circle : public AIS_InteractiveObject
{

public:

  
  //! Initializes this algorithm for constructing AIS circle
  //! datums initializes the circle aCircle
  Standard_EXPORT AIS_Circle(const Handle(Geom_Circle)& aCircle);
  
  //! Initializes this algorithm for constructing AIS circle datums.
  //! Initializes the circle theCircle, the arc
  //! starting point theUStart, the arc ending point theUEnd,
  //! and the type of sensitivity theIsFilledCircleSens.
  Standard_EXPORT AIS_Circle(const Handle(Geom_Circle)& theCircle, const Standard_Real theUStart, const Standard_Real theUEnd, const Standard_Boolean theIsFilledCircleSens = Standard_False);
  
  //! computes the presentation according to a point of view
  //! given by <aProjector>.
  //! To be Used when the associated degenerated Presentations
  //! have been transformed by <aTrsf> which is not a Pure
  //! Translation. The HLR Prs can't be deducted automatically
  //! WARNING :<aTrsf> must be applied
  //! to the object to display before computation  !!!
  Standard_EXPORT virtual   void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Geom_Transformation)& aTrsf, const Handle(Prs3d_Presentation)& aPresentation) ;
  
  //! Returns index 6 by default.
    virtual   Standard_Integer Signature()  const;
  
  //! Indicates that the type of Interactive Object is a datum.
    virtual   AIS_KindOfInteractive Type()  const;
  
  //! Returns the circle component defined in SetCircle.
     const  Handle(Geom_Circle)& Circle()  const;
  

  //! Constructs instances of the starting point and the end
  //! point parameters, u1 and u2.
      void Parameters (Standard_Real& u1, Standard_Real& u2)  const;
  
  //! Allows you to provide settings for the circle datum aCircle.
      void SetCircle (const Handle(Geom_Circle)& aCircle) ;
  
  //! Allows you to set the parameter u for the starting point of an arc.
      void SetFirstParam (const Standard_Real u) ;
  
  //! Allows you to provide the parameter u for the end point of an arc.
      void SetLastParam (const Standard_Real u) ;
  
  //! Assigns the color aColor to the solid line boundary of the circle datum.
  Standard_EXPORT   void SetColor (const Quantity_NameOfColor aColor) ;
  
  Standard_EXPORT   void SetColor (const Quantity_Color& aColor) ;
  
  //! Assigns the width aValue to the solid line boundary of the circle datum.
  Standard_EXPORT   void SetWidth (const Standard_Real aValue) ;
  
  //! Removes color from the solid line boundary of the circle datum.
  Standard_EXPORT   void UnsetColor() ;
  
  //! Removes width settings from the solid line boundary of the circle datum.
  Standard_EXPORT   void UnsetWidth() ;
  
  //! Returns the type of sensitivity for the circle;
      Standard_Boolean IsFilledCircleSens()  const;
  
  //! Sets the type of sensitivity for the circle. If theIsFilledCircleSens set to Standard_True
  //! then the whole circle will be detectable, otherwise only the boundary of the circle.
      void SetFilledCircleSens (const Standard_Boolean theIsFilledCircleSens) ;




  DEFINE_STANDARD_RTTI(AIS_Circle)

protected:




private: 

  
  Standard_EXPORT   void Compute (const Handle(PrsMgr_PresentationManager3d)& aPresentationManager, const Handle(Prs3d_Presentation)& aPresentation, const Standard_Integer aMode = 0) ;
  
  Standard_EXPORT   void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Prs3d_Presentation)& aPresentation) ;
  
  Standard_EXPORT   void ComputeSelection (const Handle(SelectMgr_Selection)& aSelection, const Standard_Integer aMode) ;
  
  Standard_EXPORT   void ComputeCircle (const Handle(Prs3d_Presentation)& aPresentation) ;
  
  Standard_EXPORT   void ComputeArc (const Handle(Prs3d_Presentation)& aPresentation) ;
  
  Standard_EXPORT   void ComputeCircleSelection (const Handle(SelectMgr_Selection)& aSelection) ;
  
  Standard_EXPORT   void ComputeArcSelection (const Handle(SelectMgr_Selection)& aSelection) ;

  Handle(Geom_Circle) myComponent;
  Standard_Real myUStart;
  Standard_Real myUEnd;
  Standard_Boolean myCircleIsArc;
  Standard_Boolean myIsFilledCircleSens;


};


#include <AIS_Circle.lxx>





#endif // _AIS_Circle_HeaderFile