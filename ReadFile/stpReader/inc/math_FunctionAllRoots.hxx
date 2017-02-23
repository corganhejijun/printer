// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _math_FunctionAllRoots_HeaderFile
#define _math_FunctionAllRoots_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Boolean.hxx>
#include <TColStd_SequenceOfReal.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <Standard_Real.hxx>
#include <Standard_Integer.hxx>
#include <Standard_OStream.hxx>
class Standard_OutOfRange;
class StdFail_NotDone;
class Standard_NumericError;
class math_FunctionWithDerivative;
class math_FunctionSample;


//! This algorithm uses a sample of the function to find
//! all intervals on which the function is null, and afterwards
//! uses the FunctionRoots algorithm to find the points
//! where the function is null outside the "null intervals".
//! Knowledge of the derivative is required.
class math_FunctionAllRoots 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! The algorithm uses the sample to find intervals on which
  //! the function is null. An interval is found if, for at least
  //! two consecutive points of the sample, Ui and Ui+1, we get
  //! |F(Ui)|<=EpsNul and |F(Ui+1)|<=EpsNul. The real bounds of
  //! an interval are computed with the FunctionRoots.
  //! algorithm.
  //! Between two intervals, the roots of the function F are
  //! calculated using the FunctionRoots algorithm.
  Standard_EXPORT math_FunctionAllRoots(math_FunctionWithDerivative& F, const math_FunctionSample& S, const Standard_Real EpsX, const Standard_Real EpsF, const Standard_Real EpsNul);
  
  //! Returns True if the computation has been done successfully.
      Standard_Boolean IsDone()  const;
  
  //! Returns the number of intervals on which the function
  //! is Null.
  //! An exception is raised if IsDone returns False.
      Standard_Integer NbIntervals()  const;
  
  //! Returns the interval of parameter of range Index.
  //! An exception is raised if IsDone returns False;
  //! An exception is raised if Index<=0 or Index >Nbintervals.
      void GetInterval (const Standard_Integer Index, Standard_Real& A, Standard_Real& B)  const;
  
  //! returns the State Number associated to the interval Index.
  //! An exception is raised if IsDone returns False;
  //! An exception is raised if Index<=0 or Index >Nbintervals.
      void GetIntervalState (const Standard_Integer Index, Standard_Integer& IFirst, Standard_Integer& ILast)  const;
  
  //! returns the number of points where the function is Null.
  //! An exception is raised if IsDone returns False.
      Standard_Integer NbPoints()  const;
  
  //! Returns the parameter of the point of range Index.
  //! An exception is raised if IsDone returns False;
  //! An exception is raised if Index<=0 or Index >NbPoints.
      Standard_Real GetPoint (const Standard_Integer Index)  const;
  
  //! returns the State Number associated to the point Index.
  //! An exception is raised if IsDone returns False;
  //! An exception is raised if Index<=0 or Index >Nbintervals.
      Standard_Integer GetPointState (const Standard_Integer Index)  const;
  
  //! Prints on the stream o information on the current state
  //! of the object.
  Standard_EXPORT   void Dump (Standard_OStream& o)  const;




protected:





private:



  Standard_Boolean done;
  TColStd_SequenceOfReal pdeb;
  TColStd_SequenceOfReal pfin;
  TColStd_SequenceOfReal piso;
  TColStd_SequenceOfInteger ideb;
  TColStd_SequenceOfInteger ifin;
  TColStd_SequenceOfInteger iiso;


};


#include <math_FunctionAllRoots.lxx>





#endif // _math_FunctionAllRoots_HeaderFile