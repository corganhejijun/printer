// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _QANewModTopOpe_Limitation_HeaderFile
#define _QANewModTopOpe_Limitation_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TopoDS_Shape.hxx>
#include <QANewModTopOpe_CutPtr.hxx>
#include <QANewModTopOpe_CommonPtr.hxx>
#include <Standard_Boolean.hxx>
#include <QANewModTopOpe_ModeOfLimitation.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
class TopoDS_Shape;
class TopTools_ListOfShape;


//! provides  cutting  shape  by  face  or  shell;
class QANewModTopOpe_Limitation  : public BRepBuilderAPI_MakeShape
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! initializes and  fills data structure for  cutting and
  //! makes  cutting according to orientation theCutTool and
  //! theMode.
  //! if theCutTool is not face or shell does nothing.
  Standard_EXPORT QANewModTopOpe_Limitation(const TopoDS_Shape& theObjectToCut, const TopoDS_Shape& theCutTool, const QANewModTopOpe_ModeOfLimitation theMode = QANewModTopOpe_Forward);
  
  //! makes cutting  according to  orientation theCutTool
  //! and  current value   of  myMode.  Does nothing  if
  //! result already  exists.
  Standard_EXPORT   void Cut() ;
  
  Standard_EXPORT   void SetMode (const QANewModTopOpe_ModeOfLimitation theMode) ;
  
  Standard_EXPORT   QANewModTopOpe_ModeOfLimitation GetMode()  const;
  
  //! Returns the first shape.
  Standard_EXPORT  const  TopoDS_Shape& Shape1()  const;
  
  //! Returns the second shape.
  Standard_EXPORT  const  TopoDS_Shape& Shape2()  const;
  
  //! Returns the list  of shapes modified from the shape
  //! <S>.
  Standard_EXPORT virtual  const  TopTools_ListOfShape& Modified (const TopoDS_Shape& S) ;
  
  //! Returns the list  of shapes generated from the shape <S>.
  //! For use in BRepNaming.
  Standard_EXPORT virtual  const  TopTools_ListOfShape& Generated (const TopoDS_Shape& S) ;
  
  //! Returns true if there is at least one modified shape.
  //! For use in BRepNaming.
  Standard_EXPORT virtual   Standard_Boolean HasModified()  const;
  
  //! Returns true if there is at least one generated shape.
  //! For use in BRepNaming.
  Standard_EXPORT virtual   Standard_Boolean HasGenerated()  const;
  
  //! Returns true if there is at least one deleted shape.
  //! For use in BRepNaming.
  Standard_EXPORT virtual   Standard_Boolean HasDeleted()  const;
  
  Standard_EXPORT virtual   Standard_Boolean IsDeleted (const TopoDS_Shape& S) ;
  
  Standard_EXPORT virtual   void Delete() ;
Standard_EXPORT  ~QANewModTopOpe_Limitation()  {Delete();}




protected:





private:



  TopoDS_Shape myResultFwd;
  TopoDS_Shape myResultRvs;
  TopoDS_Shape myObjectToCut;
  TopoDS_Shape myCutTool;
  QANewModTopOpe_CutPtr myCut;
  QANewModTopOpe_CommonPtr myCommon;
  Standard_Boolean myFwdIsDone;
  Standard_Boolean myRevIsDone;
  QANewModTopOpe_ModeOfLimitation myMode;


};







#endif // _QANewModTopOpe_Limitation_HeaderFile
