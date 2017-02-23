// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _SelectMgr_ViewerSelector_HeaderFile
#define _SelectMgr_ViewerSelector_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_SelectMgr_ViewerSelector.hxx>

#include <SelectMgr_DataMapOfIntegerSensitive.hxx>
#include <SelectMgr_DataMapOfSelectionActivation.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Real.hxx>
#include <SelectBasics_SortAlgo.hxx>
#include <Bnd_Box2d.hxx>
#include <Standard_Integer.hxx>
#include <SelectMgr_IndexedDataMapOfOwnerCriterion.hxx>
#include <Handle_TColStd_HArray1OfInteger.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <SelectBasics_PickArgs.hxx>
#include <MMgt_TShared.hxx>
#include <Handle_SelectMgr_Selection.hxx>
#include <Handle_SelectMgr_SelectableObject.hxx>
#include <Handle_SelectMgr_EntityOwner.hxx>
#include <SelectMgr_StateOfSelection.hxx>
#include <Standard_OStream.hxx>
#include <Handle_SelectBasics_SensitiveEntity.hxx>
class TColStd_HArray1OfInteger;
class SelectMgr_SelectionManager;
class SelectMgr_Selection;
class SelectMgr_SelectableObject;
class Bnd_Box2d;
class TColgp_Array1OfPnt2d;
class SelectMgr_EntityOwner;
class TColStd_ListOfInteger;
class TCollection_AsciiString;
class SelectBasics_SensitiveEntity;
class SelectMgr_DataMapOfIntegerSensitive;
class gp_Lin;


//! A framework to define finding, sorting the sensitive
//! primitives in a view. Services are also provided to
//! define the return of the owners of those primitives
//! selected. The primitives are sorted by criteria such
//! as priority of the primitive or its depth in the view
//! relative to that of other primitives.
//! This framework is undefined for either 2D or 3D,
//! and is consequently used by both
//! StdSelect_ViewerSelector2d and
//! StdSelect_ViewerSelector3d, which inherit it, and
//! which in turn, return 2D and 3D owners of sensitive
//! primitives respectively.
//! Note that in 3D, the inheriting framework
//! StdSelect_ViewerSelector3d   is only to be used
//! if you do not want to use the services provided by
//! AIS. In 2D, you will, however, need the services
//! provided by the StdSelect_ViewerSelector2d.
//! Two tools are available to find and select objects
//! found at a given position in the view. If you want to
//! select the owners of all the objects detected at
//! point x,y, you use the Init - More - Next - Picked
//! loop. If, on the other hand, you want to select only
//! one object detected at that point, you use the Init -
//! More - OnePicked loop. In this iteration, More is
//! used to see if an object was picked and
//! OnePicked, to get the object closest to the pick position.
//! Viewer selectors are driven by
//! SelectMgr_SelectionManager, and manipulate
//! the SelectMgr_Selection objects given to them by
//! the selection manager.
class SelectMgr_ViewerSelector : public MMgt_TShared
{

public:

  
  //! to be redefined if conversion is necessary for SensitiveEntities...
  Standard_EXPORT virtual   void Convert (const Handle(SelectMgr_Selection)& aSelection) ;
  
  //! Empties all the tables, removes all selections...
  Standard_EXPORT   void Clear() ;
  
  //! converts all the sensitive entities ;
  Standard_EXPORT   void UpdateConversion() ;
  
  //! changes the Sensitivity of picking
  //! Input value is Real.
  Standard_EXPORT   void SetSensitivity (const Standard_Real aTol) ;
  
  //! returns the Sensitivity of picking
      Standard_Real Sensitivity()  const;
  
  //! sets the clipping limits of dynamic picking
  //! input value are Real
  Standard_EXPORT   void SetClipping (const Standard_Real Xc, const Standard_Real Yc, const Standard_Real Height, const Standard_Real Width) ;
  
  //! sets the clipping limits of dynamic picking
  //! input value are Real
  Standard_EXPORT   void SetClipping (const Bnd_Box2d& aRectangle) ;
  
  //! Performs a pick action. Xr, Yr   are the real 2D mouse
  //! coordinates in the view. The selector looks for areas
  //! and owners that are touched.
  Standard_EXPORT   void InitSelect (const Standard_Real Xr, const Standard_Real Yr) ;
  
  //! Performs a pick action. aRect is a Box2d (real
  //! coordinates) for the selection. The selector looks for
  //! areas and owners that are touched.
  Standard_EXPORT   void InitSelect (const Bnd_Box2d& aRect) ;
  
  //! Performs a pick action
  //! -   Xmin, Ymin define the coordinates of the minimum
  //! point in the lower left hand corner of the selection
  //! box, and XMax, YMax define the coordinates of
  //! the maximum point in the upper right hand corner
  //! of the selection box. The selector looks for areas
  //! and owners that are touched.
  Standard_EXPORT   void InitSelect (const Standard_Real Xmin, const Standard_Real Ymin, const Standard_Real Xmax, const Standard_Real Ymax) ;
  
  //! pick action  - input  values of a polyline selection for selection.
  Standard_EXPORT   void InitSelect (const TColgp_Array1OfPnt2d& Polyline) ;
  
  //! Sorts the detected entites by priority and distance.
  //! to be redefined if other criterion are used...
  Standard_EXPORT virtual   void SortResult() ;
  
  //! Begins an iteration scanning for the owners detected at a position in the view.
      void Init() ;
  
  //! Continues the interation scanning for the owners
  //! detected at a position in the view, or
  //! -   continues the iteration scanning for the owner
  //! closest to the position in the view.
  Standard_EXPORT   Standard_Boolean More() ;
  
  //! Returns the next owner found in the iteration. This is
  //! a scan for the owners detected at a position in the view.
      void Next() ;
  
  //! Returns the current selected entity detected by the selector;
  Standard_EXPORT   Handle(SelectMgr_EntityOwner) Picked()  const;
  
  //! Returns the picked element with the highest priority,
  //! and which is the closest to the last successful mouse position.
  Standard_EXPORT   Handle(SelectMgr_EntityOwner) OnePicked() ;
  
  //! Set preference of selecting one object for OnePicked() method:
  //! - If True, objects with less depth (distance fron the view plane) are
  //! preferred regardless of priority (priority is used then to choose among
  //! objects with similar depth),
  //! - If False, objects with higher priority are preferred regardless of the
  //! depth which is used to choose among objects of the same priority.
      void SetPickClosest (const Standard_Boolean preferClosest) ;
  
  //! Returns the number of owners found at a position in
  //! the view by the Init - More - Next - Picked iteration.
  Standard_EXPORT   Standard_Integer NbPicked()  const;
  
  //! Returns the  entity which is at rank <aRank>
  //! in the list of stored ones.
  Standard_EXPORT   Handle(SelectMgr_EntityOwner) Picked (const Standard_Integer aRank)  const;
  
  //! Returns True if a successful pick was stored,
  //! i.e. LastPosition method means something...
  Standard_EXPORT   Standard_Boolean HasStored() ;
  
  //! Gives the last successful pick position;
  //! is useful to get objects really picked
  Standard_EXPORT   void LastPosition (Standard_Real& Xr, Standard_Real& Yr)  const;
  
  Standard_EXPORT   Standard_Boolean Contains (const Handle(SelectMgr_SelectableObject)& aSelectableObject)  const;
  
  //! Returns the list of selection modes ModeList found in
  //! this selector for the selectable object aSelectableObject.
  //! Returns true if aSelectableObject is referenced inside
  //! this selector; returns false if the object is not present
  //! in this selector.
  Standard_EXPORT   Standard_Boolean Modes (const Handle(SelectMgr_SelectableObject)& aSelectableObject, TColStd_ListOfInteger& ModeList, const SelectMgr_StateOfSelection WantedState = SelectMgr_SOS_Any)  const;
  
  //! Returns true if the selectable object
  //! aSelectableObject having the selection mode aMode
  //! is active in this selector.
  Standard_EXPORT   Standard_Boolean IsActive (const Handle(SelectMgr_SelectableObject)& aSelectableObject, const Standard_Integer aMode)  const;
  
  //! Returns true if the selectable object
  //! aSelectableObject having the selection mode aMode
  //! is in this selector.
  Standard_EXPORT   Standard_Boolean IsInside (const Handle(SelectMgr_SelectableObject)& aSelectableObject, const Standard_Integer aMode)  const;
  
  //! Returns the selection status Status of the selection aSelection.
  Standard_EXPORT   SelectMgr_StateOfSelection Status (const Handle(SelectMgr_Selection)& aSelection)  const;
  
  Standard_EXPORT   void Dump (Standard_OStream& S)  const;
  
  Standard_EXPORT   TCollection_AsciiString Status (const Handle(SelectMgr_SelectableObject)& aSelectableObject)  const;
  
  //! gives general information about the Selector
  Standard_EXPORT   TCollection_AsciiString Status()  const;
  
  Standard_EXPORT   void UpdateSort() ;
  
  Standard_EXPORT   Handle(SelectBasics_SensitiveEntity) Primitive (const Standard_Integer Rank)  const;
  
     const  SelectMgr_DataMapOfIntegerSensitive& Primitives()  const;
  
  Standard_EXPORT   void SetUpdateSortPossible (const Standard_Boolean possible) ;
  
  Standard_EXPORT   Standard_Boolean IsUpdateSortPossible()  const;
  
     const  SelectBasics_PickArgs& LastPickingArguments()  const;


friend class SelectMgr_SelectionManager;


  DEFINE_STANDARD_RTTI(SelectMgr_ViewerSelector)

protected:

  
  Standard_EXPORT SelectMgr_ViewerSelector();
  
  Standard_EXPORT virtual   void LoadResult() ;
  
  Standard_EXPORT virtual   void LoadResult (const Bnd_Box2d& aBox) ;
  
  Standard_EXPORT virtual   void LoadResult (const TColgp_Array1OfPnt2d& Polyline) ;
  
  //! Returns picking line along which the depth value should be
  //! computed. Override this method to compute picking line by the same
  //! which is used for projecting sensitive entities to selection space.
  //! @param theX [in] the x picking coordinate.
  //! @param theY [in] the y picking coordinate.
  //! @return picking line.
  Standard_EXPORT virtual   gp_Lin PickingLine (const Standard_Real theX, const Standard_Real theY)  const;
  
  //! Returns global depth clipping limits applied to every sensitive.
  //! Override this method to convert clippings defined by application into
  //! selection space for mouse picking detection.
  //! Default implementation returns infinite clip limits (no clipping).
  //! @param theX [in] the x picking coordinate.
  //! @param theY [in] the y picking coordinate.
  //! @param theMin [out] the minimum depth. Default is RealFirst()
  //! @param theMax [out] the maximum depth. Default is RealLast()
  Standard_EXPORT virtual   void DepthClipping (const Standard_Real theX, const Standard_Real theY, Standard_Real& theMin, Standard_Real& theMax)  const;
  
  //! Returns depth clipping limits applied to sensitives of
  //! entity owner. Override this method to convert clippings defined by
  //! application owners into selection space for mouse picking detection.
  //! Default implementation returns infinite clip limits (no clipping).
  //! @param theX [in] the x picking coordinate.
  //! @param theY [in] the y picking coordinate.
  //! @param theOwner [in] the sensitive owner.
  //! @param theMin [out] the minimum depth. Default is RealFirst()
  //! @param theMax [out] the maximum depth. Default is RealLast()
  Standard_EXPORT virtual   void DepthClipping (const Standard_Real theX, const Standard_Real theY, const Handle(SelectMgr_EntityOwner)& theOwner, Standard_Real& theMin, Standard_Real& theMax)  const;
  
  //! Returns True if the owner provides clipping by depth
  //! for its sensitives. Override this method to tell the selector
  //! to use the DepthClipping method for the owner.
  //! Default implementation returns False for every owner.
  //! @param theOwner [in] the onwer to check.
  //! @return True if owner provides depth limits for sensitive clipping.
  Standard_EXPORT virtual   Standard_Boolean HasDepthClipping (const Handle(SelectMgr_EntityOwner)& theOwner)  const;

  SelectMgr_DataMapOfIntegerSensitive myentities;
  SelectMgr_DataMapOfSelectionActivation myselections;
  Standard_Boolean toupdate;
  Standard_Boolean tosort;
  Standard_Boolean preferclosest;
  Standard_Real mytolerance;
  SelectBasics_SortAlgo myselector;
  Bnd_Box2d myclip;
  SelectMgr_IndexedDataMapOfOwnerCriterion mystored;


private: 

  
  Standard_EXPORT   void Activate (const Handle(SelectMgr_Selection)& aSelection, const Standard_Boolean AutomaticProj = Standard_True) ;
  
  Standard_EXPORT   void Deactivate (const Handle(SelectMgr_Selection)& aSelection) ;
  
  //! Desactivates all the objects of the view;
  //! no object in this view will be selectable;
  Standard_EXPORT   void Sleep() ;
  
  //! reactivates all the selection which were sleeping....
  Standard_EXPORT   void Awake (const Standard_Boolean AutomaticProj = Standard_True) ;
  
  Standard_EXPORT   void Sleep (const Handle(SelectMgr_SelectableObject)& anObject) ;
  
  Standard_EXPORT   void Awake (const Handle(SelectMgr_SelectableObject)& anObject, const Standard_Boolean AutomaticProj = Standard_True) ;
  
  //! removes a Selection from the Selector
  Standard_EXPORT   void Remove (const Handle(SelectMgr_Selection)& aSelection) ;
  
  Standard_EXPORT   Standard_Integer NbBoxes() ;

  Standard_Integer myactivenb;
  Handle(TColStd_HArray1OfInteger) myIndexes;
  TColStd_SequenceOfInteger myprim;
  Standard_Integer myCurRank;
  SelectBasics_PickArgs myLastPickArgs;
  Standard_Real lastx;
  Standard_Real lasty;
  Standard_Boolean myUpdateSortPossible;


};


#include <SelectMgr_ViewerSelector.lxx>





#endif // _SelectMgr_ViewerSelector_HeaderFile
