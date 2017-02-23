// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TNaming_OldShapeIterator_HeaderFile
#define _TNaming_OldShapeIterator_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TNaming_PtrNode.hxx>
#include <Standard_Integer.hxx>
#include <Handle_TNaming_UsedShapes.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_TNaming_NamedShape.hxx>
class Standard_NoMoreObject;
class Standard_NoSuchObject;
class TNaming_Tool;
class TNaming_Localizer;
class TNaming_Naming;
class TopoDS_Shape;
class TNaming_UsedShapes;
class TDF_Label;
class TNaming_Iterator;
class TNaming_NamedShape;


//! Iterates on all the ascendants of a shape
class TNaming_OldShapeIterator 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TNaming_OldShapeIterator(const TopoDS_Shape& aShape, const Standard_Integer Transaction, const TDF_Label& access);
  
  Standard_EXPORT TNaming_OldShapeIterator(const TopoDS_Shape& aShape, const TDF_Label& access);
  
  //! Iterates from the current Shape in <anIterator>
  Standard_EXPORT TNaming_OldShapeIterator(const TNaming_OldShapeIterator& anIterator);
  
  //! Iterates from the current Shape in <anIterator>
  Standard_EXPORT TNaming_OldShapeIterator(const TNaming_Iterator& anIterator);
  
      Standard_Boolean More()  const;
  
  Standard_EXPORT   void Next() ;
  
  Standard_EXPORT   TDF_Label Label()  const;
  
  Standard_EXPORT   Handle(TNaming_NamedShape) NamedShape()  const;
  
  Standard_EXPORT  const  TopoDS_Shape& Shape()  const;
  
  //! True if the  new  shape is a modification  (split,
  //! fuse,etc...) of the old shape.
  Standard_EXPORT   Standard_Boolean IsModification()  const;


friend class TNaming_Tool;
friend class TNaming_Localizer;
friend class TNaming_Naming;


protected:





private:

  
  Standard_EXPORT TNaming_OldShapeIterator(const TopoDS_Shape& aShape, const Standard_Integer Transaction, const Handle(TNaming_UsedShapes)& Shapes);
  
  Standard_EXPORT TNaming_OldShapeIterator(const TopoDS_Shape& aShape, const Handle(TNaming_UsedShapes)& Shapes);


  TNaming_PtrNode myNode;
  Standard_Integer myTrans;


};


#include <TNaming_OldShapeIterator.lxx>





#endif // _TNaming_OldShapeIterator_HeaderFile