typedef struct _APP_PARAM{
	long 	bCncOrder;
	long	bAutoSaveParam;
	long	bAutoRunWMtool;
	long	bReadContinueOn3dEntity;
	long	bCheckSelfXOnLoopBuild;
	long	bCheckSelfXAfterLoopBuild;
	long	bCheckSelfXAfterOffset;
	long	bCheckCoXAfterOffset;
	long	bCornerLinkbyArc;
	long	bCenterAjustEveryTime;
	long    bCncLineCross;
	float	fOffsetDist;
	float	fCncLineDist;
	float	fCncFirstAngle;
	float	fCncAngleDelta;
	float	fLayerDist;
	long	lDist_O1_O;
	float   fEstep;
	float   fScale;
	float   fScalez;
	float   fCncLenMin;
	long	ifreebackinterval;
}_APP_PARAM;
/*
int  CardTest(void);
void CardPosTest(HWND hWndParent,BOOL Modal);
void FillCardPropertyPage(LPPROPSHEETPAGE ppage,LPARAM lParam);
int  InitCards();
void RP_SetZero();
int  RP_CardState(long *pos);
int  RP_MoveTo(long dstpos[3]);
int  RP_LineXY(double srcpos[3],double dstpos[3],BOOL gzon);
int  RP_ArcXY(double srcpos[3],double dstpos[3],double cntpos[3],short flag);
int  RP_MoveZ(long dstz);
int  RP_ActPos(long* pos);
int  RP_StartScraper(void);
void RP_LightSwitch(BOOL on);
int  RP_GetScraperFreeBack(void);*/
BOOL ZeroAjust();
BOOL InitRpcDll();

typedef struct __DRIVER{
	int		iscurrent;
	char	name[16];
	char	info[256];
	char	file[MAX_PATH];
}__DRIVER;
#ifndef DATA_IN_THISFILE
	
	extern _APP_PARAM	AppParam;
	extern BOOL zero_ok;
	extern BOOL rp_dllok;
	extern int  (*CardTest)(HWND);
	extern void	(*FillCardPropertyPage)(LPPROPSHEETPAGE,LPARAM);
	extern void (*CardPosTest)(HWND,BOOL);
	extern int  (*RP_InitCards)();
	extern int  (*RP_ArcXY)(double srcpos[3],double dstpos[3],double cntpos[3],short flag);
	extern int  (*RP_CardState)(long *pos);
	extern void (*RP_LightSwitch)(BOOL on);
	extern int  (*RP_LineXY)(double srcpos[3],double dstpos[3],BOOL gzon);
	extern int  (*RP_MoveTo)(long dstpos[3]);
	extern void (*RP_SetZero)();
	extern void (*RP_StopMove)();
	extern void (*RP_ZeroAbsolutePos)(long*pos);
	extern void (*RP_Debug)();
	extern int  (*RP_BeginWork)();
	extern int  (*RP_EndWork)();
	extern int  (*RP_BeginLayer)();
	extern int  (*RP_EndLayer)();
	extern int  (*RP_NextLayer)(long dstz);
#else
	BOOL zero_ok=FALSE;
	BOOL rp_dllok=FALSE;
	void (*CardPosTest)(HWND,BOOL);
	int  (*CardTest)(HWND);	
	void (* FillCardPropertyPage)(LPPROPSHEETPAGE,LPARAM);
	int  (*RP_ArcXY)(double srcpos[3],double dstpos[3],double cntpos[3],short flag);
	int  (*RP_CardState)(long *pos);
	int  (*RP_InitCards)();
	void (*RP_LightSwitch)(BOOL on);
	int  (*RP_LineXY)(double srcpos[3],double dstpos[3],BOOL gzon);
	int  (*RP_MoveTo)(long dstpos[3]);
	void (*RP_SetZero)();
	void (*RP_StopMove)();
	void (*RP_ZeroAbsolutePos)(long*pos);
	void (*RP_Debug)();
	int  (*RP_BeginWork)();
	int  (*RP_EndWork)();
	int  (*RP_BeginLayer)();
	int  (*RP_EndLayer)();
	int  (*RP_NextLayer)(long dstz);
#endif
