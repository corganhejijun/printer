///////ENT FUNCTIONS
float EntDirChange(EntFull*pin,EntFull*pout,float d);//�Ƕȱ仯
int EntHaveBottom(EntFull*pEnt,float Ymin,BOOL flip);///�ҵ�ʵ��ĵ׵�����
int EntPtOnArc(EntFull*parc,float& ag);
int EntPtOnLine(EntFull *pEnt,float x,float y);
void EntCopy(EntFull*dest,EntFull*src);
void ParamFix(EntFull*pEnt,EntFull*pEntPre);
void ParamMakeDir(EntFull*pEnt);
void EntGetFull(EntFull*pEntfull,EntRaw*pEnt);
void ParamMakeLine(EntFull*pEnt);////��ֱ�ߵõ�ֱ�߷��̵Ĳ���
void CenterAngle(float& ag,float center=0);/////��ag��С��center������pi��Χ��
void FlipEnt(EntFull*pEnt);////��ʵ�巴��
void ReOrganizeEnts(EntFull*pEnt);/////������֯ʵ�壬��Flip��־ΪTRUE��ʵ�巴��
void ParamMakeExt(EntFull*pEnt);////���ʵ��ı߽�
EntFull*EntNewLine(float xs,float ys,float xe,float ye);////����һ���µ�ֱ�ߡ�
EntFull*EntNewArc(float xc,float yc,float sa,float ag,float r);//// ����һ���µ�Բ��
EntFull*EntNewArc(float xc,float yc,float xstart,float ystart,float xend,
				  float yend,float gstart,float gend,float r);
/////////////////////�˵�Ĵ���<<<
void FitAngle(float& ag,EntFull*pArc,BOOL start,float da);////��ag�ʺϵ�һ��Բ����
void EntSetEnd(EntFull*pEnt,float x,float y);//// ����ֱ�ߵ��յ�
void EntSetStart(EntFull*pEnt,float x,float y);////����ֱ�ߵ����
////>>>>>>/////�˵�Ĵ���
///////ʵ���ཻ����
int XLineCircle(EntFull*pArc,double a,double b,double c,///ֱ�ߺ�Բ���ཻ���ؽ�����
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
