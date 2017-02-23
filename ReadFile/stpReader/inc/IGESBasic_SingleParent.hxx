// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESBasic_SingleParent_HeaderFile
#define _IGESBasic_SingleParent_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_IGESBasic_SingleParent.hxx>

#include <Standard_Integer.hxx>
#include <Handle_IGESData_IGESEntity.hxx>
#include <Handle_IGESData_HArray1OfIGESEntity.hxx>
#include <IGESData_SingleParentEntity.hxx>
class IGESData_IGESEntity;
class IGESData_HArray1OfIGESEntity;
class Standard_OutOfRange;


//! defines SingleParent, Type <402> Form <9>
//! in package IGESBasic
//! It defines a logical structure of one independent
//! (parent) entity and one or more subordinate (children)
//! entities
class IGESBasic_SingleParent : public IGESData_SingleParentEntity
{

public:

  
  Standard_EXPORT IGESBasic_SingleParent();
  
  //! This method is used to set the fields of the class
  //! SingleParent
  //! - nbParentEntities : Indicates number of Parents, always = 1
  //! - aParentEntity    : Used to hold the Parent Entity
  //! - allChildren      : Used to hold the children
  Standard_EXPORT   void Init (const Standard_Integer nbParentEntities, const Handle(IGESData_IGESEntity)& aParentEntity, const Handle(IGESData_HArray1OfIGESEntity)& allChildren) ;
  
  //! returns the number of Parent Entities, which should be 1
  Standard_EXPORT   Standard_Integer NbParentEntities()  const;
  
  //! Returns the Parent Entity (inherited method)
  Standard_EXPORT   Handle(IGESData_IGESEntity) SingleParent()  const;
  
  //! returns the number of children of the Parent
  Standard_EXPORT   Standard_Integer NbChildren()  const;
  
  //! returns the specific child as indicated by Index
  //! raises exception if Index <= 0 or Index > NbChildren()
  Standard_EXPORT   Handle(IGESData_IGESEntity) Child (const Standard_Integer Index)  const;




  DEFINE_STANDARD_RTTI(IGESBasic_SingleParent)

protected:




private: 


  Standard_Integer theNbParentEntities;
  Handle(IGESData_IGESEntity) theParentEntity;
  Handle(IGESData_HArray1OfIGESEntity) theChildren;


};







#endif // _IGESBasic_SingleParent_HeaderFile
