// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepToIGES_BREntity_HeaderFile
#define _BRepToIGES_BREntity_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Handle_IGESData_IGESModel.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_Transfer_FinderProcess.hxx>
#include <Handle_IGESData_IGESEntity.hxx>
#include <Standard_CString.hxx>
#include <Handle_Standard_Transient.hxx>
class IGESData_IGESModel;
class Transfer_FinderProcess;
class IGESData_IGESEntity;
class TopoDS_Shape;
class Standard_Transient;


//! provides methods to transfer BRep entity from CASCADE to IGES.
class BRepToIGES_BREntity 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Creates a tool BREntity
  Standard_EXPORT BRepToIGES_BREntity();
  
  //! Initializes the field of the tool BREntity with
  //! default creating values.
  Standard_EXPORT   void Init() ;
  
  //! Set the value of "TheModel"
  Standard_EXPORT   void SetModel (const Handle(IGESData_IGESModel)& model) ;
  
  //! Returns the value of "TheModel"
  Standard_EXPORT   Handle(IGESData_IGESModel) GetModel()  const;
  
  //! Returns the value of the UnitFlag of the header of the model
  //! in meters.
  Standard_EXPORT   Standard_Real GetUnit()  const;
  
  //! Set the value of "TheMap"
  Standard_EXPORT   void SetTransferProcess (const Handle(Transfer_FinderProcess)& TP) ;
  
  //! Returns the value of "TheMap"
  Standard_EXPORT   Handle(Transfer_FinderProcess) GetTransferProcess()  const;
  
  //! Returns the result of the transfert of any Shape
  //! If  the transfer has  failed, this member return a NullEntity.
  Standard_EXPORT virtual   Handle(IGESData_IGESEntity) TransferShape (const TopoDS_Shape& start) ;
  
  //! Records a new Fail message
  Standard_EXPORT   void AddFail (const TopoDS_Shape& start, const Standard_CString amess) ;
  
  //! Records a new Warning message
  Standard_EXPORT   void AddWarning (const TopoDS_Shape& start, const Standard_CString amess) ;
  
  //! Records a new Fail message
  Standard_EXPORT   void AddFail (const Handle(Standard_Transient)& start, const Standard_CString amess) ;
  
  //! Records a new Warning message
  Standard_EXPORT   void AddWarning (const Handle(Standard_Transient)& start, const Standard_CString amess) ;
  
  //! Returns True if start was already treated and has a result in "TheMap"
  //! else returns False.
  Standard_EXPORT   Standard_Boolean HasShapeResult (const TopoDS_Shape& start)  const;
  
  //! Returns the result of the transfer of the Shape "start" contained
  //! in "TheMap" . (if HasShapeResult is True).
  Standard_EXPORT   Handle(Standard_Transient) GetShapeResult (const TopoDS_Shape& start)  const;
  
  //! set in "TheMap" the result of the transfer of the Shape "start".
  Standard_EXPORT   void SetShapeResult (const TopoDS_Shape& start, const Handle(Standard_Transient)& result) ;
  
  //! Returns True if start was already treated and has a result in "TheMap"
  //! else returns False.
  Standard_EXPORT   Standard_Boolean HasShapeResult (const Handle(Standard_Transient)& start)  const;
  
  //! Returns the result of the transfer of the Transient "start" contained
  //! in "TheMap" . (if HasShapeResult is True).
  Standard_EXPORT   Handle(Standard_Transient) GetShapeResult (const Handle(Standard_Transient)& start)  const;
  
  //! set in "TheMap" the result of the transfer of the Transient "start".
  Standard_EXPORT   void SetShapeResult (const Handle(Standard_Transient)& start, const Handle(Standard_Transient)& result) ;
  
  //! Returns mode for conversion of surfaces
  //! (value of parameter write.convertsurface.mode)
  Standard_EXPORT   Standard_Boolean GetConvertSurfaceMode()  const;
  
  //! Returns mode for writing pcurves
  //! (value of parameter write.surfacecurve.mode)
  Standard_EXPORT   Standard_Boolean GetPCurveMode()  const;
  
  Standard_EXPORT virtual   void Delete() ;
Standard_EXPORT virtual ~BRepToIGES_BREntity() { Delete(); }




protected:





private:



  Handle(IGESData_IGESModel) TheModel;
  Standard_Real TheUnitFactor;
  Standard_Boolean myConvSurface;
  Standard_Boolean myPCurveMode;
  Handle(Transfer_FinderProcess) TheMap;


};







#endif // _BRepToIGES_BREntity_HeaderFile
