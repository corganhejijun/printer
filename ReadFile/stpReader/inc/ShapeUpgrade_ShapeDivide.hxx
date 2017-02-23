// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ShapeUpgrade_ShapeDivide_HeaderFile
#define _ShapeUpgrade_ShapeDivide_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Handle_ShapeUpgrade_FaceDivide.hxx>
#include <Handle_ShapeBuild_ReShape.hxx>
#include <TopoDS_Shape.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>
#include <ShapeExtend_Status.hxx>
class ShapeUpgrade_FaceDivide;
class ShapeBuild_ReShape;
class TopoDS_Shape;


//! Divides a all faces in shell with given criteria Shell.
class ShapeUpgrade_ShapeDivide 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT ShapeUpgrade_ShapeDivide();
  
  //! Initialize by a Shape.
  Standard_EXPORT ShapeUpgrade_ShapeDivide(const TopoDS_Shape& S);
  
  //! Initialize by a Shape.
  Standard_EXPORT   void Init (const TopoDS_Shape& S) ;
  
  Standard_EXPORT virtual   void Delete() ;
Standard_EXPORT virtual ~ShapeUpgrade_ShapeDivide(){Delete();}
  
  //! Defines the spatial precision used for splitting
  Standard_EXPORT   void SetPrecision (const Standard_Real Prec) ;
  
  //! Sets maximal allowed tolerance
  Standard_EXPORT   void SetMaxTolerance (const Standard_Real maxtol) ;
  
  //! Sets minimal allowed tolerance
  Standard_EXPORT   void SetMinTolerance (const Standard_Real mintol) ;
  
  //! Purpose sets mode for trimming (segment) surface by
  //! wire UV bounds.
  Standard_EXPORT   void SetSurfaceSegmentMode (const Standard_Boolean Segment) ;
  
  //! Performs splitting and computes the resulting shape
  //! If newContext is True (default), the internal context
  //! will be cleared at start, else previous substitutions
  //! will be acting.
  Standard_EXPORT virtual   Standard_Boolean Perform (const Standard_Boolean newContext = Standard_True) ;
  
  //! Gives the resulting Shape, or Null shape if not done.
  Standard_EXPORT   TopoDS_Shape Result()  const;
  
  //! Returns context with all the modifications made during
  //! last call(s) to Perform() recorded
  Standard_EXPORT   Handle(ShapeBuild_ReShape) GetContext()  const;
  
  //! Sets context with recorded modifications to be applied
  //! during next call(s) to Perform(shape,Standard_False)
  Standard_EXPORT   void SetContext (const Handle(ShapeBuild_ReShape)& context) ;
  
  //! Queries the status of last call to Perform
  //! OK   : no splitting was done (or no call to Perform)
  //! DONE1: some edges were splitted
  //! DONE2: surface was splitted
  //! FAIL1: some errors occured
  Standard_EXPORT   Standard_Boolean Status (const ShapeExtend_Status status)  const;
  
  //! Sets the tool for splitting faces.
  Standard_EXPORT   void SetSplitFaceTool (const Handle(ShapeUpgrade_FaceDivide)& splitFaceTool) ;
  
  //! Sets mode for splitting 3d curves from edges.
  //! 0 - only curve 3d from free edges.
  //! 1 - only curve 3d from shared edges.
  //! 2 -  all curve 3d.
  Standard_EXPORT   void SetEdgeMode (const Standard_Integer aEdgeMode) ;




protected:

  
  //! Returns the tool for splitting faces.
  Standard_EXPORT virtual   Handle(ShapeUpgrade_FaceDivide) GetSplitFaceTool()  const;


  Handle(ShapeBuild_ReShape) myContext;
  TopoDS_Shape myShape;
  TopoDS_Shape myResult;
  Standard_Real myPrecision;
  Standard_Real myMinTol;
  Standard_Real myMaxTol;
  Standard_Boolean mySegmentMode;
  Standard_Integer myStatus;
  Standard_Integer myEdgeMode;


private:



  Handle(ShapeUpgrade_FaceDivide) mySplitFaceTool;


};







#endif // _ShapeUpgrade_ShapeDivide_HeaderFile