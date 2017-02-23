// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _GeomInt_LineTool_HeaderFile
#define _GeomInt_LineTool_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Integer.hxx>
#include <Handle_IntPatch_Line.hxx>
#include <Standard_Real.hxx>
class IntPatch_Line;
class IntPatch_Point;



class GeomInt_LineTool 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT static   Standard_Integer NbVertex (const Handle(IntPatch_Line)& L) ;
  
  Standard_EXPORT static  const  IntPatch_Point& Vertex (const Handle(IntPatch_Line)& L, const Standard_Integer I) ;
  
  Standard_EXPORT static   Standard_Real FirstParameter (const Handle(IntPatch_Line)& L) ;
  
  Standard_EXPORT static   Standard_Real LastParameter (const Handle(IntPatch_Line)& L) ;




protected:





private:





};







#endif // _GeomInt_LineTool_HeaderFile