void ReleaseArray(CPtrArray&ar);
ELoop* BuildLayerLoopOriginal(ELayer*pLayer);
void LoopSfxCheck(ELoop*&ploop,BOOL exterior=TRUE);
ELoop* BuildLayerLoopOffset(ELoop*&ploop,float d ,BOOL exterior);
void BuildLoopTreeFromBTree(ELoop*pLoopBtree);
////////////////////NC
BOOL NcToEntFull(NcEnt*p,float& Px,float& Py,EntFull&e);
void EntCopy(EntFull*dest,EntFull*src);
ELoop* LoopTreeCopy(ELoop*pLoopTree,float scale=1.0f);
ELoop*LoopTreeCopyNoEnt(ELoop*pLoopTree);
void LoopTreeGetExt(ELoop*ploop);
void LoopArrangeDirection(ELoop*ploop,BOOL exterior=TRUE);
void BuildLayerNC(ELayer*pLayer,double angle,float d,
				  float lenmin,float r,int cncOrder=0x06,int linecross=TRUE);
void LayerGetInfo(ELayer*pLayer,ELayerInfo*pELI);
BOOL ReadDxfFile(LPSTR szFile,ESpace *pEntSpace);
BOOL ReadDwgFile(LPSTR szFile,ESpace *pEntSpace);

void NcEntsToPul(ESpace*pEntSpace,CString&puls);

///
