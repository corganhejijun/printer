// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _ShapeUpgrade_ConvertCurve3dToBezier_HeaderFile
#define _ShapeUpgrade_ConvertCurve3dToBezier_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_ShapeUpgrade_ConvertCurve3dToBezier.hxx>

#include <Handle_TColGeom_HSequenceOfCurve.hxx>
#include <Handle_TColStd_HSequenceOfReal.hxx>
#include <Standard_Boolean.hxx>
#include <ShapeUpgrade_SplitCurve3d.hxx>
class TColGeom_HSequenceOfCurve;
class TColStd_HSequenceOfReal;


//! converts/splits a 3d curve of any type to a list of beziers
class ShapeUpgrade_ConvertCurve3dToBezier : public ShapeUpgrade_SplitCurve3d
{

public:

  
  //! Empty constructor
  Standard_EXPORT ShapeUpgrade_ConvertCurve3dToBezier();
  
  //! Sets mode for conversion Geom_Line to bezier.
      void SetLineMode (const Standard_Boolean mode) ;
  
  //! Returns the Geom_Line conversion mode.
      Standard_Boolean GetLineMode()  const;
  
  //! Sets mode for conversion Geom_Circle to bezier.
      void SetCircleMode (const Standard_Boolean mode) ;
  
  //! Returns the Geom_Circle conversion mode.
      Standard_Boolean GetCircleMode()  const;
  
  //! Returns the Geom_Conic conversion mode.
      void SetConicMode (const Standard_Boolean mode) ;
  
  //! Performs converting and computes the resulting shape.
      Standard_Boolean GetConicMode()  const;
  
  //! Converts curve into a list of beziers, and stores the
  //! splitting parameters on original curve.
  Standard_EXPORT virtual   void Compute() ;
  
  //! Splits a list of beziers computed by Compute method according
  //! the split values and splitting parameters.
  Standard_EXPORT virtual   void Build (const Standard_Boolean Segment) ;
  
  //! Returns the list of splitted parameters in original curve
  //! parametrisation.
  Standard_EXPORT   Handle(TColStd_HSequenceOfReal) SplitParams()  const;




  DEFINE_STANDARD_RTTI(ShapeUpgrade_ConvertCurve3dToBezier)

protected:




private: 

  
  //! Returns the list of bezier curves correspondent to original
  //! curve.
  Standard_EXPORT   Handle(TColGeom_HSequenceOfCurve) Segments()  const;

  Handle(TColGeom_HSequenceOfCurve) mySegments;
  Handle(TColStd_HSequenceOfReal) mySplitParams;
  Standard_Boolean myLineMode;
  Standard_Boolean myCircleMode;
  Standard_Boolean myConicMode;


};


#include <ShapeUpgrade_ConvertCurve3dToBezier.lxx>





#endif // _ShapeUpgrade_ConvertCurve3dToBezier_HeaderFile
