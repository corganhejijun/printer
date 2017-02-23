// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StlAPI_HeaderFile
#define _StlAPI_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_CString.hxx>
#include <Standard_Boolean.hxx>
class TopoDS_Shape;
class StlAPI_Writer;
class StlAPI_Reader;


//! Offers the API for STL data manipulation.
class StlAPI 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Convert and write shape to STL format.
  //! file is written in binary if aAsciiMode is False
  //! otherwise it is written in Ascii (by default)
  Standard_EXPORT static   void Write (const TopoDS_Shape& aShape, const Standard_CString aFile, const Standard_Boolean aAsciiMode = Standard_True) ;
  
  //! Create a shape from a STL format.
  Standard_EXPORT static   void Read (TopoDS_Shape& aShape, const Standard_CString aFile) ;




protected:





private:




friend class StlAPI_Writer;
friend class StlAPI_Reader;

};







#endif // _StlAPI_HeaderFile
