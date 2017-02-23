// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TransferBRep_BinderOfShape_HeaderFile
#define _TransferBRep_BinderOfShape_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TransferBRep_BinderOfShape.hxx>

#include <TopoDS_Shape.hxx>
#include <Transfer_Binder.hxx>
#include <Handle_Standard_Type.hxx>
#include <Standard_CString.hxx>
class Transfer_TransferFailure;
class TopoDS_Shape;
class Standard_Type;


//! Allows direct binding between a starting Object and the Result
//! of its transfer when it is Unique.
//! The Result itself is defined as a formal parameter <Shape from TopoDS>
//! Warning : While it is possible to instantiate BinderOfShape with any Type
//! for the Result, it is not advisable to instantiate it with
//! Transient Classes, because such Results are directly known and
//! managed by TransferProcess & Co, through
//! SimpleBinderOfTransient : this class looks like instantiation
//! of BinderOfShape, but its method ResultType
//! is adapted (reads DynamicType of the Result)
class TransferBRep_BinderOfShape : public Transfer_Binder
{

public:

  
  //! normal standard constructor, creates an empty BinderOfShape
  Standard_EXPORT TransferBRep_BinderOfShape();
  
  //! constructor which in the same time defines the result
  //! Returns True if a starting object is bound with SEVERAL
  //! results : Here, returns allways False
  //! But it can have next results
  Standard_EXPORT TransferBRep_BinderOfShape(const TopoDS_Shape& res);
  
  //! Returns the Type permitted for the Result, i.e. the Type
  //! of the Parameter Class <Shape from TopoDS> (statically defined)
  Standard_EXPORT   Handle(Standard_Type) ResultType()  const;
  
  //! Returns the Type Name computed for the Result (dynamic)
  Standard_EXPORT   Standard_CString ResultTypeName()  const;
  
  //! Defines the Result
  Standard_EXPORT   void SetResult (const TopoDS_Shape& res) ;
  
  //! Returns the defined Result, if there is one
  Standard_EXPORT  const  TopoDS_Shape& Result()  const;
  
  //! Returns the defined Result, if there is one, and allows to
  //! change it (avoids Result + SetResult).
  //! Admits that Result can be not yet defined
  //! Warning : a call to CResult causes Result to be known as defined
  Standard_EXPORT   TopoDS_Shape& CResult() ;




  DEFINE_STANDARD_RTTI(TransferBRep_BinderOfShape)

protected:




private: 


  TopoDS_Shape theres;


};







#endif // _TransferBRep_BinderOfShape_HeaderFile