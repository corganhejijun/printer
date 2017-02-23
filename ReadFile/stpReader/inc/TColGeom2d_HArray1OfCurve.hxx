// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColGeom2d_HArray1OfCurve_HeaderFile
#define _TColGeom2d_HArray1OfCurve_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TColGeom2d_HArray1OfCurve.hxx>

#include <TColGeom2d_Array1OfCurve.hxx>
#include <MMgt_TShared.hxx>
#include <Handle_Geom2d_Curve.hxx>
#include <Standard_Integer.hxx>
class Standard_RangeError;
class Standard_DimensionMismatch;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class Geom2d_Curve;
class TColGeom2d_Array1OfCurve;



class TColGeom2d_HArray1OfCurve : public MMgt_TShared
{

public:

  
    TColGeom2d_HArray1OfCurve(const Standard_Integer Low, const Standard_Integer Up);
  
    TColGeom2d_HArray1OfCurve(const Standard_Integer Low, const Standard_Integer Up, const Handle(Geom2d_Curve)& V);
  
      void Init (const Handle(Geom2d_Curve)& V) ;
  
      Standard_Integer Length()  const;
  
      Standard_Integer Lower()  const;
  
      Standard_Integer Upper()  const;
  
      void SetValue (const Standard_Integer Index, const Handle(Geom2d_Curve)& Value) ;
  
     const  Handle(Geom2d_Curve)& Value (const Standard_Integer Index)  const;
  
      Handle(Geom2d_Curve)& ChangeValue (const Standard_Integer Index) ;
  
     const  TColGeom2d_Array1OfCurve& Array1()  const;
  
      TColGeom2d_Array1OfCurve& ChangeArray1() ;




  DEFINE_STANDARD_RTTI(TColGeom2d_HArray1OfCurve)

protected:




private: 


  TColGeom2d_Array1OfCurve myArray;


};

#define ItemHArray1 Handle(Geom2d_Curve)
#define ItemHArray1_hxx <Geom2d_Curve.hxx>
#define TheArray1 TColGeom2d_Array1OfCurve
#define TheArray1_hxx <TColGeom2d_Array1OfCurve.hxx>
#define TCollection_HArray1 TColGeom2d_HArray1OfCurve
#define TCollection_HArray1_hxx <TColGeom2d_HArray1OfCurve.hxx>
#define Handle_TCollection_HArray1 Handle_TColGeom2d_HArray1OfCurve
#define TCollection_HArray1_Type_() TColGeom2d_HArray1OfCurve_Type_()

#include <TCollection_HArray1.lxx>

#undef ItemHArray1
#undef ItemHArray1_hxx
#undef TheArray1
#undef TheArray1_hxx
#undef TCollection_HArray1
#undef TCollection_HArray1_hxx
#undef Handle_TCollection_HArray1
#undef TCollection_HArray1_Type_




#endif // _TColGeom2d_HArray1OfCurve_HeaderFile
