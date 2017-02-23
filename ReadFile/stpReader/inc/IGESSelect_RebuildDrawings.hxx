// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESSelect_RebuildDrawings_HeaderFile
#define _IGESSelect_RebuildDrawings_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_IGESSelect_RebuildDrawings.hxx>

#include <IGESSelect_ModelModifier.hxx>
#include <Handle_IGESData_IGESModel.hxx>
class IFSelect_ContextModif;
class IGESData_IGESModel;
class Interface_CopyTool;
class TCollection_AsciiString;


//! Rebuilds Drawings which were bypassed to produce new models.
//! If a set of entities, all put into a same IGESModel, were
//! attached to a same Drawing in the starting Model, this Modifier
//! rebuilds the original Drawing, but only with the transferred
//! entities. This includes that all its views are kept too, but
//! empty; and annotations are not kept. Drawing Name is renewed.
//!
//! If the Input Selection is present, tries to rebuild Drawings
//! only for the selected entities. Else, tries to rebuild
//! Drawings for all the transferred entities.
class IGESSelect_RebuildDrawings : public IGESSelect_ModelModifier
{

public:

  
  //! Creates an RebuildDrawings, which uses the system Date
  Standard_EXPORT IGESSelect_RebuildDrawings();
  
  //! Specific action : Rebuilds the original Drawings
  Standard_EXPORT   void Performing (IFSelect_ContextModif& ctx, const Handle(IGESData_IGESModel)& target, Interface_CopyTool& TC)  const;
  
  //! Returns a text which is
  //! "Rebuild Drawings"
  Standard_EXPORT   TCollection_AsciiString Label()  const;




  DEFINE_STANDARD_RTTI(IGESSelect_RebuildDrawings)

protected:




private: 




};







#endif // _IGESSelect_RebuildDrawings_HeaderFile