// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ShapeUpgrade_FixSmallCurves_HeaderFile
#define _ShapeUpgrade_FixSmallCurves_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_ShapeUpgrade_FixSmallCurves.hxx>

#include <Standard_Integer.hxx>
#include <Handle_ShapeUpgrade_SplitCurve3d.hxx>
#include <Handle_ShapeUpgrade_SplitCurve2d.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <ShapeUpgrade_Tool.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_Geom_Curve.hxx>
#include <Handle_Geom2d_Curve.hxx>
#include <Standard_Real.hxx>
#include <ShapeExtend_Status.hxx>
class ShapeUpgrade_SplitCurve3d;
class ShapeUpgrade_SplitCurve2d;
class TopoDS_Edge;
class TopoDS_Face;
class Geom_Curve;
class Geom2d_Curve;



class ShapeUpgrade_FixSmallCurves : public ShapeUpgrade_Tool
{

public:

  
  Standard_EXPORT ShapeUpgrade_FixSmallCurves();
  
  Standard_EXPORT   void Init (const TopoDS_Edge& theEdge, const TopoDS_Face& theFace) ;
  
  Standard_EXPORT virtual   Standard_Boolean Approx (Handle(Geom_Curve)& Curve3d, Handle(Geom2d_Curve)& Curve2d, Handle(Geom2d_Curve)& Curve2dR, Standard_Real& First, Standard_Real& Last) ;
  
  //! Sets the tool for splitting 3D curves.
  Standard_EXPORT   void SetSplitCurve3dTool (const Handle(ShapeUpgrade_SplitCurve3d)& splitCurve3dTool) ;
  
  //! Sets the tool for splitting pcurves.
  Standard_EXPORT   void SetSplitCurve2dTool (const Handle(ShapeUpgrade_SplitCurve2d)& splitCurve2dTool) ;
  
  //! Queries the status of last call to Perform
  //! OK   :
  //! DONE1:
  //! DONE2:
  //! FAIL1:
  Standard_EXPORT   Standard_Boolean Status (const ShapeExtend_Status status)  const;




  DEFINE_STANDARD_RTTI(ShapeUpgrade_FixSmallCurves)

protected:

  
  Standard_EXPORT virtual   Handle(ShapeUpgrade_SplitCurve3d) GetSplitCurve3dTool()  const;
  
  //! Returns the tool for splitting pcurves.
  Standard_EXPORT virtual   Handle(ShapeUpgrade_SplitCurve2d) GetSplitCurve2dTool()  const;

  Standard_Integer myStatus;
  Handle(ShapeUpgrade_SplitCurve3d) mySplitCurve3dTool;
  Handle(ShapeUpgrade_SplitCurve2d) mySplitCurve2dTool;
  TopoDS_Edge myEdge;
  TopoDS_Face myFace;


private: 




};







#endif // _ShapeUpgrade_FixSmallCurves_HeaderFile