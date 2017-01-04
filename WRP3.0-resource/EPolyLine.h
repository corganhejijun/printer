// EPolyLine.h: interface for the EPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPOLYLINE_H__1DD2E850_26B8_44E2_BD21_FEA1BEE9BF78__INCLUDED_)
#define AFX_EPOLYLINE_H__1DD2E850_26B8_44E2_BD21_FEA1BEE9BF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "types.h"
class EPolyLine  
{
public:
	ELoop*EGetLoop();
	void Reverse();
	int Append(EPolyLine *pEpl);
	EPolyLine(EntFull*pEnt);
	virtual ~EPolyLine();
	float xstart,ystart;
	float xend,yend;
	EntFull *pLoop;
	EntFull *pRear;
};

#endif // !defined(AFX_EPOLYLINE_H__1DD2E850_26B8_44E2_BD21_FEA1BEE9BF78__INCLUDED_)
