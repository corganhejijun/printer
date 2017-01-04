///////ENT FUNCTIONS
float EntDirChange(EntFull*pin,EntFull*pout,float d);//角度变化
int EntHaveBottom(EntFull*pEnt,float Ymin,BOOL flip);///找到实体的底点类型
int EntPtOnArc(EntFull*parc,float& ag);
int EntPtOnLine(EntFull *pEnt,float x,float y);
void EntCopy(EntFull*dest,EntFull*src);
void ParamFix(EntFull*pEnt,EntFull*pEntPre);
void ParamMakeDir(EntFull*pEnt);
void EntGetFull(EntFull*pEntfull,EntRaw*pEnt);
void ParamMakeLine(EntFull*pEnt);////由直线得到直线方程的参数
void CenterAngle(float& ag,float center=0);/////把ag缩小到center的上下pi范围内
void FlipEnt(EntFull*pEnt);////把实体反向
void ReOrganizeEnts(EntFull*pEnt);/////重新组织实体，反Flip标志为TRUE的实体反向
void ParamMakeExt(EntFull*pEnt);////获得实体的边界
EntFull*EntNewLine(float xs,float ys,float xe,float ye);////生成一条新的直线　
EntFull*EntNewArc(float xc,float yc,float sa,float ag,float r);//// 生成一条新的圆弧
EntFull*EntNewArc(float xc,float yc,float xstart,float ystart,float xend,
				  float yend,float gstart,float gend,float r);
/////////////////////端点的处理<<<
void FitAngle(float& ag,EntFull*pArc,BOOL start,float da);////将ag适合到一个圆弧上
void EntSetEnd(EntFull*pEnt,float x,float y);//// 设置直线的终点
void EntSetStart(EntFull*pEnt,float x,float y);////设置直线的起点
////>>>>>>/////端点的处理
///////实体相交处理
int XLineCircle(EntFull*pArc,double a,double b,double c,///直线和圆弧相交返回交点数
					  float& x1,float& y1,float& x2,float&y2);
int XLineLine(double a1,double b1,double c1,EntFull*pEnt,float&x,float&y);
int XLineLine(EntFull*pEnt1,EntFull*pEnt2,float& x,float& y);
int XLineArc(double a,double b,double c,EntFull*pArc,float& x1,float& y1,
				float&ag1,float& x2,float& y2,float&ag2,BOOL arcisnext);
int XLineArc(EntFull*pEnt,EntFull*pArc,float& x,float& y,float& ag);
int XArcLine(EntFull*pArc,EntFull*pEnt,float& x,float& y,float& ag);
int XArcArc(EntFull*pArc1,EntFull*pArc2,float& x,float&y,float& ag1,float& ag2);
int XEntEnt(EntFull*pEnt1,EntFull*pEnt2,float&x,float&y,float&ag1,float&ag2,BOOL extcheck);
EntFull* EntBreakStart(EntFull*pEnt,float x,float y,float ag,int p);
EntFull* EntBreakEnd(EntFull*pEnt,float x,float y,float ag,int p);
