// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepBasic_ProductOrFormationOrDefinition_HeaderFile
#define _StepBasic_ProductOrFormationOrDefinition_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <StepData_SelectType.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Standard_Transient.hxx>
#include <Handle_StepBasic_Product.hxx>
#include <Handle_StepBasic_ProductDefinitionFormation.hxx>
#include <Handle_StepBasic_ProductDefinition.hxx>
class Standard_Transient;
class StepBasic_Product;
class StepBasic_ProductDefinitionFormation;
class StepBasic_ProductDefinition;


//! Representation of STEP SELECT type ProductOrFormationOrDefinition
class StepBasic_ProductOrFormationOrDefinition  : public StepData_SelectType
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Empty constructor
  Standard_EXPORT StepBasic_ProductOrFormationOrDefinition();
  
  //! Recognizes a kind of ProductOrFormationOrDefinition select type
  //! 1 -> Product from StepBasic
  //! 2 -> ProductDefinitionFormation from StepBasic
  //! 3 -> ProductDefinition from StepBasic
  //! 0 else
  Standard_EXPORT   Standard_Integer CaseNum (const Handle(Standard_Transient)& ent)  const;
  
  //! Returns Value as Product (or Null if another type)
  Standard_EXPORT   Handle(StepBasic_Product) Product()  const;
  
  //! Returns Value as ProductDefinitionFormation (or Null if another type)
  Standard_EXPORT   Handle(StepBasic_ProductDefinitionFormation) ProductDefinitionFormation()  const;
  
  //! Returns Value as ProductDefinition (or Null if another type)
  Standard_EXPORT   Handle(StepBasic_ProductDefinition) ProductDefinition()  const;




protected:





private:





};







#endif // _StepBasic_ProductOrFormationOrDefinition_HeaderFile