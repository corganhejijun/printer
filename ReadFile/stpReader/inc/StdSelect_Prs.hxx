// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StdSelect_Prs_HeaderFile
#define _StdSelect_Prs_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StdSelect_Prs.hxx>

#include <Handle_Graphic3d_StructureManager.hxx>
#include <Prs3d_Presentation.hxx>
class Graphic3d_StructureManager;


//! allows entities owners to be hilighted
//! independantly from PresentableObjects
class StdSelect_Prs : public Prs3d_Presentation
{

public:

  
  Standard_EXPORT StdSelect_Prs(const Handle(Graphic3d_StructureManager)& aStructureManager);
  
     const  Handle(Graphic3d_StructureManager)& Manager()  const;




  DEFINE_STANDARD_RTTI(StdSelect_Prs)

protected:




private: 


  Handle(Graphic3d_StructureManager) myManager;


};


#include <StdSelect_Prs.lxx>





#endif // _StdSelect_Prs_HeaderFile
