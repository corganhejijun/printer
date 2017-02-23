// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _HLRBRep_ThePolygonOfInterCSurf_HeaderFile
#define _HLRBRep_ThePolygonOfInterCSurf_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Bnd_Box.hxx>
#include <Standard_Real.hxx>
#include <Standard_Integer.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_TColStd_HArray1OfReal.hxx>
class TColStd_HArray1OfReal;
class Standard_OutOfRange;
class gp_Lin;
class HLRBRep_LineTool;
class TColStd_Array1OfReal;
class Bnd_Box;
class gp_Pnt;



class HLRBRep_ThePolygonOfInterCSurf 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT HLRBRep_ThePolygonOfInterCSurf(const gp_Lin& Curve, const Standard_Integer NbPnt);
  
  Standard_EXPORT HLRBRep_ThePolygonOfInterCSurf(const gp_Lin& Curve, const Standard_Real U1, const Standard_Real U2, const Standard_Integer NbPnt);
  
  Standard_EXPORT HLRBRep_ThePolygonOfInterCSurf(const gp_Lin& Curve, const TColStd_Array1OfReal& Upars);
  
     const  Bnd_Box& Bounding()  const;
  
      Standard_Real DeflectionOverEstimation()  const;
  
      void SetDeflectionOverEstimation (const Standard_Real x) ;
  
      void Closed (const Standard_Boolean clos) ;
  
      Standard_Boolean Closed()  const;
  
      Standard_Integer NbSegments()  const;
  
     const  gp_Pnt& BeginOfSeg (const Standard_Integer Index)  const;
  
     const  gp_Pnt& EndOfSeg (const Standard_Integer Index)  const;
  
      Standard_Real InfParameter()  const;
  
      Standard_Real SupParameter()  const;
  
  Standard_EXPORT   Standard_Real ApproxParamOnCurve (const Standard_Integer Index, const Standard_Real ParamOnLine)  const;
  
  Standard_EXPORT   void Dump()  const;




protected:

  
  Standard_EXPORT   void Init (const gp_Lin& Curve) ;
  
  Standard_EXPORT   void Init (const gp_Lin& Curve, const TColStd_Array1OfReal& Upars) ;




private:



  Bnd_Box TheBnd;
  Standard_Real TheDeflection;
  Standard_Integer NbPntIn;
  TColgp_Array1OfPnt ThePnts;
  Standard_Boolean ClosedPolygon;
  Standard_Real Binf;
  Standard_Real Bsup;
  Handle(TColStd_HArray1OfReal) myParams;


};

#define TheCurve gp_Lin
#define TheCurve_hxx <gp_Lin.hxx>
#define TheCurveTool HLRBRep_LineTool
#define TheCurveTool_hxx <HLRBRep_LineTool.hxx>
#define IntCurveSurface_Polygon HLRBRep_ThePolygonOfInterCSurf
#define IntCurveSurface_Polygon_hxx <HLRBRep_ThePolygonOfInterCSurf.hxx>

#include <IntCurveSurface_Polygon.lxx>

#undef TheCurve
#undef TheCurve_hxx
#undef TheCurveTool
#undef TheCurveTool_hxx
#undef IntCurveSurface_Polygon
#undef IntCurveSurface_Polygon_hxx




#endif // _HLRBRep_ThePolygonOfInterCSurf_HeaderFile
