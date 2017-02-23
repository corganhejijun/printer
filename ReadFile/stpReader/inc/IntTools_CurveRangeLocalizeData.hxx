// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IntTools_CurveRangeLocalizeData_HeaderFile
#define _IntTools_CurveRangeLocalizeData_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Integer.hxx>
#include <Standard_Real.hxx>
#include <IntTools_MapOfCurveSample.hxx>
#include <IntTools_DataMapOfCurveSampleBox.hxx>
#include <Standard_Boolean.hxx>
class IntTools_CurveRangeSample;
class Bnd_Box;
class IntTools_ListOfCurveRangeSample;



class IntTools_CurveRangeLocalizeData 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT IntTools_CurveRangeLocalizeData(const Standard_Integer theNbSample, const Standard_Real theMinRange);
  
      Standard_Integer GetNbSample()  const;
  
      Standard_Real GetMinRange()  const;
  
  Standard_EXPORT   void AddOutRange (const IntTools_CurveRangeSample& theRange) ;
  
  Standard_EXPORT   void AddBox (const IntTools_CurveRangeSample& theRange, const Bnd_Box& theBox) ;
  
  Standard_EXPORT   Standard_Boolean FindBox (const IntTools_CurveRangeSample& theRange, Bnd_Box& theBox)  const;
  
  Standard_EXPORT   Standard_Boolean IsRangeOut (const IntTools_CurveRangeSample& theRange)  const;
  
  Standard_EXPORT   void ListRangeOut (IntTools_ListOfCurveRangeSample& theList)  const;




protected:





private:



  Standard_Integer myNbSampleC;
  Standard_Real myMinRangeC;
  IntTools_MapOfCurveSample myMapRangeOut;
  IntTools_DataMapOfCurveSampleBox myMapBox;


};


#include <IntTools_CurveRangeLocalizeData.lxx>





#endif // _IntTools_CurveRangeLocalizeData_HeaderFile
