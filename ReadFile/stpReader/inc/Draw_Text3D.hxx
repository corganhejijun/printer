// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Draw_Text3D_HeaderFile
#define _Draw_Text3D_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Draw_Text3D.hxx>

#include <gp_Pnt.hxx>
#include <Draw_Color.hxx>
#include <TCollection_AsciiString.hxx>
#include <Standard_Real.hxx>
#include <Draw_Drawable3D.hxx>
#include <Standard_CString.hxx>
class gp_Pnt;
class Draw_Color;
class Draw_Display;



class Draw_Text3D : public Draw_Drawable3D
{

public:

  
  Standard_EXPORT Draw_Text3D(const gp_Pnt& p, const Standard_CString T, const Draw_Color& col);
  
  Standard_EXPORT Draw_Text3D(const gp_Pnt& p, const Standard_CString T, const Draw_Color& col, const Standard_Real moveX, const Standard_Real moveY);
  
  Standard_EXPORT   void SetPnt (const gp_Pnt& p) ;
  
  Standard_EXPORT   void DrawOn (Draw_Display& dis)  const;




  DEFINE_STANDARD_RTTI(Draw_Text3D)

protected:




private: 


  gp_Pnt myPoint;
  Draw_Color myColor;
  TCollection_AsciiString myText;
  Standard_Real mymoveX;
  Standard_Real mymoveY;


};







#endif // _Draw_Text3D_HeaderFile
