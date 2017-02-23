// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _HLRTest_DrawablePolyEdgeTool_HeaderFile
#define _HLRTest_DrawablePolyEdgeTool_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_HLRTest_DrawablePolyEdgeTool.hxx>

#include <Handle_HLRBRep_PolyAlgo.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>
#include <HLRBRep_ListOfBPoint.hxx>
#include <Draw_Drawable3D.hxx>
class HLRBRep_PolyAlgo;
class Draw_Display;


//! Used to display the results.
class HLRTest_DrawablePolyEdgeTool : public Draw_Drawable3D
{

public:

  
  Standard_EXPORT HLRTest_DrawablePolyEdgeTool(const Handle(HLRBRep_PolyAlgo)& Alg, const Standard_Integer ViewId, const Standard_Boolean Debug = Standard_False);
  
      void Show() ;
  
      void Hide() ;
  
      void DisplayRg1Line (const Standard_Boolean B) ;
  
      Standard_Boolean DisplayRg1Line()  const;
  
      void DisplayRgNLine (const Standard_Boolean B) ;
  
      Standard_Boolean DisplayRgNLine()  const;
  
      void DisplayHidden (const Standard_Boolean B) ;
  
      Standard_Boolean DisplayHidden()  const;
  
  Standard_EXPORT   void DrawOn (Draw_Display& D)  const;
  
      Standard_Boolean Debug()  const;
  
      void Debug (const Standard_Boolean B) ;




  DEFINE_STANDARD_RTTI(HLRTest_DrawablePolyEdgeTool)

protected:




private: 


  Handle(HLRBRep_PolyAlgo) myAlgo;
  Standard_Boolean myDispRg1;
  Standard_Boolean myDispRgN;
  Standard_Boolean myDispHid;
  Standard_Integer myViewId;
  HLRBRep_ListOfBPoint myBiPntVis;
  HLRBRep_ListOfBPoint myBiPntHid;
  Standard_Boolean myDebug;
  Standard_Boolean myHideMode;


};


#include <HLRTest_DrawablePolyEdgeTool.lxx>





#endif // _HLRTest_DrawablePolyEdgeTool_HeaderFile
