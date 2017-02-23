// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Bisector_BisecCC_HeaderFile
#define _Bisector_BisecCC_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Bisector_BisecCC.hxx>

#include <Handle_Geom2d_Curve.hxx>
#include <Standard_Real.hxx>
#include <TColStd_SequenceOfReal.hxx>
#include <Standard_Integer.hxx>
#include <Bisector_PolyBis.hxx>
#include <Standard_Boolean.hxx>
#include <gp_Pnt2d.hxx>
#include <Bisector_Curve.hxx>
#include <Handle_Geom2d_Geometry.hxx>
#include <GeomAbs_Shape.hxx>
class Geom2d_Curve;
class Standard_DomainError;
class Standard_RangeError;
class gp_Pnt2d;
class Geom2d_Geometry;
class gp_Trsf2d;
class gp_Vec2d;
class Bisector_PolyBis;
class TColStd_SequenceOfReal;


//! Construct the bisector between two curves.
//! The curves can intersect only in their extremities.
class Bisector_BisecCC : public Bisector_Curve
{

public:

  
  Standard_EXPORT Bisector_BisecCC();
  
  //! Constructs  the bisector  between the  curves <Cu1>
  //! and <Cu2>.
  //!
  //! <Side1>  (resp <Side2>) = 1   if the
  //! bisector curve is on the left of <Cu1> (resp <Cu2>)
  //! else <Side1> (resp <Side2>) = -1.
  //!
  //! the Bisector is trimmed by the Point <Origin>.
  //! <DistMax> is used to trim the bisector.The distance
  //! between the points of the bisector and <Cu> is smaller
  //! than <DistMax>.
  Standard_EXPORT Bisector_BisecCC(const Handle(Geom2d_Curve)& Cu1, const Handle(Geom2d_Curve)& Cu2, const Standard_Real Side1, const Standard_Real Side2, const gp_Pnt2d& Origin, const Standard_Real DistMax = 500);
  
  //! Computes the bisector  between the  curves <Cu1>
  //! and <Cu2>.
  //!
  //! <Side1>  (resp <Side2>) = 1   if the
  //! bisector curve is on the left of <Cu1> (resp <Cu2>)
  //! else <Side1> (resp <Side2>) = -1.
  //!
  //! the Bisector is trimmed by the Point <Origin>.
  //!
  //! <DistMax> is used to trim the bisector.The distance
  //! between the points of the bisector and <Cu> is smaller
  //! than <DistMax>.
  Standard_EXPORT   void Perform (const Handle(Geom2d_Curve)& Cu1, const Handle(Geom2d_Curve)& Cu2, const Standard_Real Side1, const Standard_Real Side2, const gp_Pnt2d& Origin, const Standard_Real DistMax = 500) ;
  
  Standard_EXPORT   Standard_Boolean IsExtendAtStart()  const;
  
  Standard_EXPORT   Standard_Boolean IsExtendAtEnd()  const;
  
  Standard_EXPORT   void Reverse() ;
  
  Standard_EXPORT   Standard_Real ReversedParameter (const Standard_Real U)  const;
  
  //! Returns the order of continuity of the curve.
  //! Raised if N < 0.
  Standard_EXPORT   Standard_Boolean IsCN (const Standard_Integer N)  const;
  
  //! The parameter  on <me> is linked to  the parameter
  //! on the first curve. This method creates the same bisector
  //! where the curves are inversed.
  Standard_EXPORT   Handle(Bisector_BisecCC) ChangeGuide()  const;
  
  Standard_EXPORT   Handle(Geom2d_Geometry) Copy()  const;
  

  //! Transformation of a geometric object. This tansformation
  //! can be a translation, a rotation, a symmetry, a scaling
  //! or a complex transformation obtained by combination of
  //! the previous elementaries transformations.
  Standard_EXPORT   void Transform (const gp_Trsf2d& T) ;
  
  Standard_EXPORT   Standard_Real FirstParameter()  const;
  
  Standard_EXPORT   Standard_Real LastParameter()  const;
  
  Standard_EXPORT   GeomAbs_Shape Continuity()  const;
  
  //! If necessary,  breaks the  curve in  intervals  of
  //! continuity  <C1>.    And  returns   the number   of
  //! intervals.
  Standard_EXPORT   Standard_Integer NbIntervals()  const;
  
  //! Returns  the  first  parameter    of  the  current
  //! interval.
  Standard_EXPORT   Standard_Real IntervalFirst (const Standard_Integer Index)  const;
  
  //! Returns  the  last  parameter    of  the  current
  //! interval.
  Standard_EXPORT   Standard_Real IntervalLast (const Standard_Integer Index)  const;
  
  Standard_EXPORT   GeomAbs_Shape IntervalContinuity()  const;
  
  Standard_EXPORT   Standard_Boolean IsClosed()  const;
  
  Standard_EXPORT   Standard_Boolean IsPeriodic()  const;
  
  //! Returns the point of parameter U.
  //! Computes the distance between the current point and
  //! the two curves I separate.
  //! Computes the parameters on each curve corresponding
  //! of the projection of the current point.
  Standard_EXPORT   gp_Pnt2d ValueAndDist (const Standard_Real U, Standard_Real& U1, Standard_Real& U2, Standard_Real& Distance)  const;
  
  //! Returns the point of parameter U.
  //! Computes the distance between the current point and
  //! the two curves I separate.
  //! Computes the parameters on each curve corresponding
  //! of the projection of the current point.
  Standard_EXPORT   gp_Pnt2d ValueByInt (const Standard_Real U, Standard_Real& U1, Standard_Real& U2, Standard_Real& Distance)  const;
  
  Standard_EXPORT   void D0 (const Standard_Real U, gp_Pnt2d& P)  const;
  
  Standard_EXPORT   void D1 (const Standard_Real U, gp_Pnt2d& P, gp_Vec2d& V)  const;
  
  Standard_EXPORT   void D2 (const Standard_Real U, gp_Pnt2d& P, gp_Vec2d& V1, gp_Vec2d& V2)  const;
  
  Standard_EXPORT   void D3 (const Standard_Real U, gp_Pnt2d& P, gp_Vec2d& V1, gp_Vec2d& V2, gp_Vec2d& V3)  const;
  
  Standard_EXPORT   gp_Vec2d DN (const Standard_Real U, const Standard_Integer N)  const;
  
  Standard_EXPORT   Standard_Boolean IsEmpty()  const;
  
  //! Returns the parameter on the curve1 of the projection
  //! of the point of parameter U on <me>.
  Standard_EXPORT   Standard_Real LinkBisCurve (const Standard_Real U)  const;
  
  //! Returns the reciproque of LinkBisCurve.
  Standard_EXPORT   Standard_Real LinkCurveBis (const Standard_Real U)  const;
  
  Standard_EXPORT   Standard_Real Parameter (const gp_Pnt2d& P)  const;
  
  Standard_EXPORT   Handle(Geom2d_Curve) Curve (const Standard_Integer IndCurve)  const;
  
  Standard_EXPORT  const  Bisector_PolyBis& Polygon()  const;
  
  Standard_EXPORT   void Dump (const Standard_Integer Deep = 0, const Standard_Integer Offset = 0)  const;




  DEFINE_STANDARD_RTTI(Bisector_BisecCC)

protected:




private: 

  
  Standard_EXPORT   void Values (const Standard_Real U, const Standard_Integer N, gp_Pnt2d& P, gp_Vec2d& V1, gp_Vec2d& V2, gp_Vec2d& V3)  const;
  
  Standard_EXPORT   void SupLastParameter() ;
  
  Standard_EXPORT   gp_Pnt2d Extension (const Standard_Real U, Standard_Real& U1, Standard_Real& U2, Standard_Real& Dist, gp_Vec2d& T1)  const;
  
  Standard_EXPORT   Standard_Real SearchBound (const Standard_Real U1, const Standard_Real U2)  const;
  
  Standard_EXPORT   void ComputePointEnd() ;
  
  Standard_EXPORT   void Curve (const Standard_Integer Index, const Handle(Geom2d_Curve)& C) ;
  
  Standard_EXPORT   void Sign (const Standard_Integer Index, const Standard_Real Sign) ;
  
  Standard_EXPORT   void Polygon (const Bisector_PolyBis& Poly) ;
  
  Standard_EXPORT   void DistMax (const Standard_Real DistMax) ;
  
  Standard_EXPORT   void IsConvex (const Standard_Integer Index, const Standard_Boolean IsConvex) ;
  
  Standard_EXPORT   void IsEmpty (const Standard_Boolean IsEmpty) ;
  
  Standard_EXPORT   void ExtensionStart (const Standard_Boolean ExtensionStart) ;
  
  Standard_EXPORT   void ExtensionEnd (const Standard_Boolean ExtensionEnd) ;
  
  Standard_EXPORT   void PointStart (const gp_Pnt2d& Point) ;
  
  Standard_EXPORT   void PointEnd (const gp_Pnt2d& Point) ;
  
  Standard_EXPORT   void StartIntervals (const TColStd_SequenceOfReal& StartIntervals) ;
  
  Standard_EXPORT   void EndIntervals (const TColStd_SequenceOfReal& EndIntervals) ;
  
  Standard_EXPORT   void FirstParameter (const Standard_Real U1) ;
  
  Standard_EXPORT   void LastParameter (const Standard_Real U1) ;

  Handle(Geom2d_Curve) curve1;
  Handle(Geom2d_Curve) curve2;
  Standard_Real sign1;
  Standard_Real sign2;
  TColStd_SequenceOfReal startIntervals;
  TColStd_SequenceOfReal endIntervals;
  Standard_Integer currentInterval;
  Bisector_PolyBis myPolygon;
  Standard_Real shiftParameter;
  Standard_Real distMax;
  Standard_Boolean isEmpty;
  Standard_Boolean isConvex1;
  Standard_Boolean isConvex2;
  Standard_Boolean extensionStart;
  Standard_Boolean extensionEnd;
  gp_Pnt2d pointStart;
  gp_Pnt2d pointEnd;


};







#endif // _Bisector_BisecCC_HeaderFile
