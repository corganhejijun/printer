// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepDS_DataMapOfShapeState_HeaderFile
#define _TopOpeBRepDS_DataMapOfShapeState_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMap.hxx>
#include <TopAbs_State.hxx>
#include <Handle_TopOpeBRepDS_DataMapNodeOfDataMapOfShapeState.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Address.hxx>
class Standard_DomainError;
class Standard_NoSuchObject;
class TopoDS_Shape;
class TopTools_ShapeMapHasher;
class TopOpeBRepDS_DataMapNodeOfDataMapOfShapeState;
class TopOpeBRepDS_DataMapIteratorOfDataMapOfShapeState;



class TopOpeBRepDS_DataMapOfShapeState  : public TCollection_BasicMap
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TopOpeBRepDS_DataMapOfShapeState(const Standard_Integer NbBuckets = 1);
  
  Standard_EXPORT   TopOpeBRepDS_DataMapOfShapeState& Assign (const TopOpeBRepDS_DataMapOfShapeState& Other) ;
  TopOpeBRepDS_DataMapOfShapeState& operator = (const TopOpeBRepDS_DataMapOfShapeState& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT   void ReSize (const Standard_Integer NbBuckets) ;
  
  Standard_EXPORT   void Clear() ;
~TopOpeBRepDS_DataMapOfShapeState()
{
  Clear();
}
  
  Standard_EXPORT   Standard_Boolean Bind (const TopoDS_Shape& K, const TopAbs_State& I) ;
  
  Standard_EXPORT   Standard_Boolean IsBound (const TopoDS_Shape& K)  const;
  
  Standard_EXPORT   Standard_Boolean UnBind (const TopoDS_Shape& K) ;
  
  Standard_EXPORT  const  TopAbs_State& Find (const TopoDS_Shape& K)  const;
 const  TopAbs_State& operator() (const TopoDS_Shape& K)  const
{
  return Find(K);
}
  
  Standard_EXPORT   TopAbs_State& ChangeFind (const TopoDS_Shape& K) ;
  TopAbs_State& operator() (const TopoDS_Shape& K) 
{
  return ChangeFind(K);
}
  
  Standard_EXPORT   Standard_Address Find1 (const TopoDS_Shape& K)  const;
  
  Standard_EXPORT   Standard_Address ChangeFind1 (const TopoDS_Shape& K) ;




protected:





private:

  
  Standard_EXPORT TopOpeBRepDS_DataMapOfShapeState(const TopOpeBRepDS_DataMapOfShapeState& Other);




};







#endif // _TopOpeBRepDS_DataMapOfShapeState_HeaderFile
