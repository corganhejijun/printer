// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepDS_ListOfInterference_HeaderFile
#define _TopOpeBRepDS_ListOfInterference_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Address.hxx>
#include <Handle_TopOpeBRepDS_Interference.hxx>
#include <Handle_TopOpeBRepDS_ListNodeOfListOfInterference.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>
class Standard_NoSuchObject;
class TopOpeBRepDS_ListIteratorOfListOfInterference;
class TopOpeBRepDS_Interference;
class TopOpeBRepDS_ListNodeOfListOfInterference;



class TopOpeBRepDS_ListOfInterference 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TopOpeBRepDS_ListOfInterference();
  
  Standard_EXPORT TopOpeBRepDS_ListOfInterference(const TopOpeBRepDS_ListOfInterference& Other);
  
  Standard_EXPORT   void Assign (const TopOpeBRepDS_ListOfInterference& Other) ;
  void operator= (const TopOpeBRepDS_ListOfInterference& Other) 
{
  Assign(Other);
}
  
  Standard_EXPORT   Standard_Integer Extent()  const;
  
  Standard_EXPORT   void Clear() ;
~TopOpeBRepDS_ListOfInterference()
{
  Clear();
}
  
      Standard_Boolean IsEmpty()  const;
  
  Standard_EXPORT   void Prepend (const Handle(TopOpeBRepDS_Interference)& I) ;
  
  Standard_EXPORT   void Prepend (const Handle(TopOpeBRepDS_Interference)& I, TopOpeBRepDS_ListIteratorOfListOfInterference& theIt) ;
  
  Standard_EXPORT   void Prepend (TopOpeBRepDS_ListOfInterference& Other) ;
  
  Standard_EXPORT   void Append (const Handle(TopOpeBRepDS_Interference)& I) ;
  
  Standard_EXPORT   void Append (const Handle(TopOpeBRepDS_Interference)& I, TopOpeBRepDS_ListIteratorOfListOfInterference& theIt) ;
  
  Standard_EXPORT   void Append (TopOpeBRepDS_ListOfInterference& Other) ;
  
  Standard_EXPORT   Handle(TopOpeBRepDS_Interference)& First()  const;
  
  Standard_EXPORT   Handle(TopOpeBRepDS_Interference)& Last()  const;
  
  Standard_EXPORT   void RemoveFirst() ;
  
  Standard_EXPORT   void Remove (TopOpeBRepDS_ListIteratorOfListOfInterference& It) ;
  
  Standard_EXPORT   void InsertBefore (const Handle(TopOpeBRepDS_Interference)& I, TopOpeBRepDS_ListIteratorOfListOfInterference& It) ;
  
  Standard_EXPORT   void InsertBefore (TopOpeBRepDS_ListOfInterference& Other, TopOpeBRepDS_ListIteratorOfListOfInterference& It) ;
  
  Standard_EXPORT   void InsertAfter (const Handle(TopOpeBRepDS_Interference)& I, TopOpeBRepDS_ListIteratorOfListOfInterference& It) ;
  
  Standard_EXPORT   void InsertAfter (TopOpeBRepDS_ListOfInterference& Other, TopOpeBRepDS_ListIteratorOfListOfInterference& It) ;


friend class TopOpeBRepDS_ListIteratorOfListOfInterference;


protected:





private:



  Standard_Address myFirst;
  Standard_Address myLast;


};

#define Item Handle(TopOpeBRepDS_Interference)
#define Item_hxx <TopOpeBRepDS_Interference.hxx>
#define TCollection_ListNode TopOpeBRepDS_ListNodeOfListOfInterference
#define TCollection_ListNode_hxx <TopOpeBRepDS_ListNodeOfListOfInterference.hxx>
#define TCollection_ListIterator TopOpeBRepDS_ListIteratorOfListOfInterference
#define TCollection_ListIterator_hxx <TopOpeBRepDS_ListIteratorOfListOfInterference.hxx>
#define Handle_TCollection_ListNode Handle_TopOpeBRepDS_ListNodeOfListOfInterference
#define TCollection_ListNode_Type_() TopOpeBRepDS_ListNodeOfListOfInterference_Type_()
#define TCollection_List TopOpeBRepDS_ListOfInterference
#define TCollection_List_hxx <TopOpeBRepDS_ListOfInterference.hxx>

#include <TCollection_List.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx




#endif // _TopOpeBRepDS_ListOfInterference_HeaderFile