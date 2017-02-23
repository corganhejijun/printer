// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _MAT_DataMapIteratorOfDataMapOfIntegerNode_HeaderFile
#define _MAT_DataMapIteratorOfDataMapOfIntegerNode_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMapIterator.hxx>
#include <Standard_Integer.hxx>
#include <Handle_MAT_Node.hxx>
#include <Handle_MAT_DataMapNodeOfDataMapOfIntegerNode.hxx>
class Standard_NoSuchObject;
class MAT_Node;
class TColStd_MapIntegerHasher;
class MAT_DataMapOfIntegerNode;
class MAT_DataMapNodeOfDataMapOfIntegerNode;



class MAT_DataMapIteratorOfDataMapOfIntegerNode  : public TCollection_BasicMapIterator
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT MAT_DataMapIteratorOfDataMapOfIntegerNode();
  
  Standard_EXPORT MAT_DataMapIteratorOfDataMapOfIntegerNode(const MAT_DataMapOfIntegerNode& aMap);
  
  Standard_EXPORT   void Initialize (const MAT_DataMapOfIntegerNode& aMap) ;
  
  Standard_EXPORT  const  Standard_Integer& Key()  const;
  
  Standard_EXPORT  const  Handle(MAT_Node)& Value()  const;




protected:





private:





};







#endif // _MAT_DataMapIteratorOfDataMapOfIntegerNode_HeaderFile
