// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TNaming_Identifier_HeaderFile
#define _TNaming_Identifier_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TDF_Label.hxx>
#include <TopoDS_Shape.hxx>
#include <Standard_Boolean.hxx>
#include <TNaming_NameType.hxx>
#include <Handle_TNaming_NamedShape.hxx>
#include <TNaming_ListOfNamedShape.hxx>
#include <TopTools_ListOfShape.hxx>
class TNaming_NamedShape;
class TDF_Label;
class TopoDS_Shape;
class TNaming_Localizer;



class TNaming_Identifier 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TNaming_Identifier(const TDF_Label& Lab, const TopoDS_Shape& S, const TopoDS_Shape& Context, const Standard_Boolean Geom);
  
  Standard_EXPORT TNaming_Identifier(const TDF_Label& Lab, const TopoDS_Shape& S, const Handle(TNaming_NamedShape)& ContextNS, const Standard_Boolean Geom);
  
  Standard_EXPORT   Standard_Boolean IsDone()  const;
  
  Standard_EXPORT   TNaming_NameType Type()  const;
  
  Standard_EXPORT   Standard_Boolean IsFeature() ;
  
  Standard_EXPORT   Handle(TNaming_NamedShape) Feature()  const;
  
  Standard_EXPORT   void InitArgs() ;
  
  Standard_EXPORT   Standard_Boolean MoreArgs()  const;
  
  Standard_EXPORT   void NextArg() ;
  
  Standard_EXPORT   Standard_Boolean ArgIsFeature()  const;
  
  Standard_EXPORT   Handle(TNaming_NamedShape) FeatureArg() ;
  
  Standard_EXPORT   TopoDS_Shape ShapeArg() ;
  
  Standard_EXPORT   TopoDS_Shape ShapeContext()  const;
  
  Standard_EXPORT   Handle(TNaming_NamedShape) NamedShapeOfGeneration()  const;
  
  Standard_EXPORT   void AncestorIdentification (TNaming_Localizer& Localizer, const TopoDS_Shape& Context) ;
  
  Standard_EXPORT   void PrimitiveIdentification (TNaming_Localizer& Localizer, const Handle(TNaming_NamedShape)& NS) ;
  
  Standard_EXPORT   void GeneratedIdentification (TNaming_Localizer& Localizer, const Handle(TNaming_NamedShape)& NS) ;
  
  Standard_EXPORT   void Identification (TNaming_Localizer& Localizer, const Handle(TNaming_NamedShape)& NS) ;




protected:





private:

  
  Standard_EXPORT   void Init (const TopoDS_Shape& Context) ;


  TDF_Label myTDFAcces;
  TopoDS_Shape myShape;
  Standard_Boolean myDone;
  Standard_Boolean myIsFeature;
  TNaming_NameType myType;
  Handle(TNaming_NamedShape) myFeature;
  TNaming_ListOfNamedShape myPrimitiveArgs;
  TopTools_ListOfShape myShapeArgs;
  Handle(TNaming_NamedShape) myNSContext;


};







#endif // _TNaming_Identifier_HeaderFile
