// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Aspect_AspectLine_HeaderFile
#define _Aspect_AspectLine_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Aspect_AspectLine.hxx>

#include <Quantity_Color.hxx>
#include <Aspect_TypeOfLine.hxx>
#include <Standard_Real.hxx>
#include <MMgt_TShared.hxx>
class Aspect_AspectLineDefinitionError;
class Quantity_Color;


//! This class allows the definition of a group
//! of attributes for the LINE primitive
//! The attributes are:
//! * Colour
//! * Type
//! * Thickness
//! When any value of the group is modified
//! all graphic objects using the group are modified.
class Aspect_AspectLine : public MMgt_TShared
{

public:

  
  //! Modifies the colour of <me>.
  Standard_EXPORT   void SetColor (const Quantity_Color& AColor) ;
  
  //! Modifies the type of <me>.
  Standard_EXPORT   void SetType (const Aspect_TypeOfLine AType) ;
  
  //! Modifies the thickness of <me>.
  //! Category: Methods to modify the class definition
  //! Warning: Raises AspectLineDefinitionError if the
  //! width is a negative value.
  Standard_EXPORT   void SetWidth (const Standard_Real AWidth) ;
  
  //! Returns the current values of the group <me>.
  Standard_EXPORT   void Values (Quantity_Color& AColor, Aspect_TypeOfLine& AType, Standard_Real& AWidth)  const;




  DEFINE_STANDARD_RTTI(Aspect_AspectLine)

protected:

  
  //! Initialise the constructor for Graphic3d_AspectLine3d.
  //!
  //! Default values :
  //! Color = Quantity_NOC_YELLOW;
  //! Type  = Aspect_TOL_SOLID;
  //! Width = 1.0;
  Standard_EXPORT Aspect_AspectLine();
  
  //! Initialise the values for the
  //! constructor of Graphic3d_AspectLine3d.
  Standard_EXPORT Aspect_AspectLine(const Quantity_Color& AColor, const Aspect_TypeOfLine AType, const Standard_Real AWidth);



private: 


  Quantity_Color MyColor;
  Aspect_TypeOfLine MyType;
  Standard_Real MyWidth;


};







#endif // _Aspect_AspectLine_HeaderFile