// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StdSelect_Shape_HeaderFile
#define _StdSelect_Shape_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StdSelect_Shape.hxx>

#include <TopoDS_Shape.hxx>
#include <PrsMgr_PresentableObject.hxx>
#include <PrsMgr_PresentationManager3d.hxx>
#include <Handle_Prs3d_Presentation.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Prs3d_Projector.hxx>
#include <Handle_Geom_Transformation.hxx>
class TopoDS_Shape;
class Prs3d_Presentation;
class Prs3d_Projector;
class Geom_Transformation;


//! Presentable shape only for purpose of display for BRepOwner...
class StdSelect_Shape : public PrsMgr_PresentableObject
{

public:

  
  Standard_EXPORT StdSelect_Shape(const TopoDS_Shape& Sh);
  
  Standard_EXPORT   void Compute (const Handle(PrsMgr_PresentationManager3d)& aPresentationManager, const Handle(Prs3d_Presentation)& aPresentation, const Standard_Integer aMode = 0) ;
  
  //! computes the presentation according to a point of view
  //! given by <aProjector>.
  //! To be Used when the associated degenerated Presentations
  //! have been transformed by <aTrsf> which is not a Pure
  //! Translation. The HLR Prs can't be deducted automatically
  //! WARNING :<aTrsf> must be applied
  //! to the object to display before computation  !!!
  Standard_EXPORT virtual   void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Geom_Transformation)& aTrsf, const Handle(Prs3d_Presentation)& aPresentation) ;
  
     const  TopoDS_Shape& Shape()  const;
  
      void Shape (const TopoDS_Shape& sh) ;




  DEFINE_STANDARD_RTTI(StdSelect_Shape)

protected:




private: 

  
  Standard_EXPORT   void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Prs3d_Presentation)& aPresentation) ;

  TopoDS_Shape mysh;


};


#include <StdSelect_Shape.lxx>





#endif // _StdSelect_Shape_HeaderFile
