// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Graphic3d_Texture1Dsegment_HeaderFile
#define _Graphic3d_Texture1Dsegment_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Graphic3d_Texture1Dsegment.hxx>

#include <Standard_ShortReal.hxx>
#include <Graphic3d_Texture1D.hxx>
#include <Graphic3d_NameOfTexture1D.hxx>
#include <Image_PixMap_Handle.hxx>
class TCollection_AsciiString;


//! This class provides the implementation
//! of a 1D texture applyable along a segment.
//! You might use the SetSegment() method
//! to set the way the texture is "streched" on facets.
class Graphic3d_Texture1Dsegment : public Graphic3d_Texture1D
{

public:

  
  //! Creates a texture from a file
  Standard_EXPORT Graphic3d_Texture1Dsegment(const TCollection_AsciiString& theFileName);
  
  //! Creates a texture from a predefined texture name set.
  Standard_EXPORT Graphic3d_Texture1Dsegment(const Graphic3d_NameOfTexture1D theNOT);
  
  //! Creates a texture from the pixmap.
  Standard_EXPORT Graphic3d_Texture1Dsegment(const Image_PixMap_Handle& thePixMap);
  
  //! Sets the texture application bounds. Defines the way
  //! the texture is stretched across facets.
  //! Default values are <0.0, 0.0, 0.0> , <0.0, 0.0, 1.0>
  Standard_EXPORT   void SetSegment (const Standard_ShortReal theX1, const Standard_ShortReal theY1, const Standard_ShortReal theZ1, const Standard_ShortReal theX2, const Standard_ShortReal theY2, const Standard_ShortReal theZ2) ;
  
  //! Returns the values of the current segment X1, Y1, Z1 , X2, Y2, Z2.
  Standard_EXPORT   void Segment (Standard_ShortReal& theX1, Standard_ShortReal& theY1, Standard_ShortReal& theZ1, Standard_ShortReal& theX2, Standard_ShortReal& theY2, Standard_ShortReal& theZ2)  const;




  DEFINE_STANDARD_RTTI(Graphic3d_Texture1Dsegment)

protected:




private: 


  Standard_ShortReal myX1;
  Standard_ShortReal myY1;
  Standard_ShortReal myZ1;
  Standard_ShortReal myX2;
  Standard_ShortReal myY2;
  Standard_ShortReal myZ2;


};







#endif // _Graphic3d_Texture1Dsegment_HeaderFile