// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger_HeaderFile
#define _MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMapIterator.hxx>
#include <MeshVS_TwoColors.hxx>
#include <Handle_MeshVS_DataMapNodeOfDataMapOfTwoColorsMapOfInteger.hxx>
class Standard_NoSuchObject;
class TColStd_MapOfInteger;
class MeshVS_TwoColorsHasher;
class MeshVS_DataMapOfTwoColorsMapOfInteger;
class MeshVS_DataMapNodeOfDataMapOfTwoColorsMapOfInteger;



class MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger  : public TCollection_BasicMapIterator
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger();
  
  Standard_EXPORT MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger(const MeshVS_DataMapOfTwoColorsMapOfInteger& aMap);
  
  Standard_EXPORT   void Initialize (const MeshVS_DataMapOfTwoColorsMapOfInteger& aMap) ;
  
  Standard_EXPORT  const  MeshVS_TwoColors& Key()  const;
  
  Standard_EXPORT  const  TColStd_MapOfInteger& Value()  const;




protected:





private:





};







#endif // _MeshVS_DataMapIteratorOfDataMapOfTwoColorsMapOfInteger_HeaderFile