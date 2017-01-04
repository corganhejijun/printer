#include "stdafx.h"
#include "types.h"
#include "math.h"
#include <afxtempl.h>



BOOL ReadStlFile(LPSTR szFile,ESpace *pEntSpace){
	CFile stlFile;
	if(!stlFile.Open(szFile,CFile::modeRead))return FALSE;	
	return TRUE;

}