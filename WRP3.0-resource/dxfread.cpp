#include "stdafx.h"
#include "types.h"
#include "math.h"
#include <afxtempl.h>

/////static 
static CArray<float,float>Zar;
static CArray<int,int>Iar;
///////////////Local Functions///////////////////
void AppFileClose();
static void ReadSize(CStdioFile& dxfFile,CString& Code,CString& Value,ESpace *pEntSpace);
static void ReadLine(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>&era,CString&dxfValue,ESpace *pEntSpace);
static void ReadArc(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>&era,CString&dxfValue,ESpace *pEntSpace);
static void ReadCircle(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>& era,CString&dxfValue,ESpace *pEntSpace);
void ReadArrange(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int,int>*pIar);
void ReadEntAdd(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int ,int>*pIar,
				CArray<EntRaw,EntRaw>*pear,EntRaw*pe,float Lz);
/////��ȡDXF�ļ�����Ϣ
BOOL ReadDxfFile(LPSTR szFile,ESpace *pEntSpace){
	////��ʼ���ڴ���ļ� 
	CArray<EntRaw,EntRaw> era;
	CStdioFile dxfFile;
	if(!dxfFile.Open(szFile,CFile::modeRead))return FALSE;	
	AppFileClose();
	//��ʼ��ȡ��Ϣ
	CString dxfCode="",dxfValue="";//��������ʼ�����뼰���Ӧֵ
	BOOL bFinish=FALSE;//������ɱ��
/*	while(dxfFile.ReadString(dxfCode)&&dxfFile.ReadString(dxfValue))
	{
		if((dxfCode=="  9")&&(dxfValue=="$INSUNITS"))
		{
			dxfFile.ReadString(dxfCode);
			dxfFile.ReadString(dxfValue);
			if(dxfCode==" 70")//������ȷ
			{
				if(dxfValue=="    13")//��λΪ΢��
					UnitFactor=1;
				else
					UnitFactor=1000;//��λΪ����
			}
			else//���벻��ȷ
			{
				AfxMessageBox("DXF�ļ���������");
				return FALSE;
			}
		}
	}*/
	dxfFile.SeekToBegin();
	//��ȡdxf�ļ����ݣ���д���ڴ��ļ�
	while(dxfFile.ReadString(dxfCode)&&
		dxfFile.ReadString(dxfValue))
	{
		
		//��ȡ�汾��Ϣ
//		if(!(dxfCode.Compare("  9")||dxfValue.Compare("$ACADVER"))) 
//			return NULL;
		//��ȡͼ����С
		if((dxfCode=="  9")&&(dxfValue=="$EXTMIN")||
			(dxfValue=="$EXTMAX"))
			ReadSize(dxfFile,dxfCode,dxfValue,pEntSpace);
		//��ȡʵ����Ϣ
		if((dxfCode=="  2")&&(dxfValue=="ENTITIES")) {
			while(TRUE){
				if(dxfValue=="ARC") ReadArc(dxfFile,era,dxfValue,pEntSpace);
				else if(dxfValue=="CIRCLE") ReadCircle(dxfFile,era,dxfValue,pEntSpace);
				else if(dxfValue=="LINE") ReadLine(dxfFile,era,dxfValue,pEntSpace);
				else if(dxfValue=="ENDSEC") break;
				else{
					dxfFile.ReadString(dxfCode);
					dxfFile.ReadString(dxfValue);}
			}
			break;
		}

	}
	dxfFile.Close();
	if(pEntSpace->EntNum>0){
		Iar.Add(pEntSpace->EntNum-1);
		pEntSpace->LayerNum++;
		pEntSpace->pEntRaw=new EntRaw[pEntSpace->EntNum];
		memcpy(pEntSpace->pEntRaw,era.GetData(),pEntSpace->EntNum*sizeof(EntRaw));
		era.RemoveAll();
		ReadArrange(pEntSpace,&Zar,&Iar);
	}else{
		era.RemoveAll();
		Iar.RemoveAll();
		Zar.RemoveAll();
	}
	return TRUE;
}
//////////��ȡͼ������Ϣ
void ReadSize(CStdioFile& dxfFile,CString& Code,CString& Value,ESpace *pEntSpace)
{
	int i;
	CString dxfCode=Code,dxfValue=Value;
	if((dxfCode=="  9")&&(dxfValue=="$EXTMIN"))
	{		
		for(i=0;i<4;i++){
			dxfFile.ReadString(dxfCode);
			dxfFile.ReadString(dxfValue);
			if (dxfCode==" 10") 
				pEntSpace->xmin=(float)atof(dxfValue);
			if (dxfCode==" 20") 
				pEntSpace->ymin=(float)atof(dxfValue);
			}
	}
	if((dxfCode="  9")&&(dxfValue=="$EXTMAX"))
	{
			for(i=0;i<3;i++){
				dxfFile.ReadString(dxfCode);
				dxfFile.ReadString(dxfValue);
				if (dxfCode==" 10") 
					pEntSpace->xmax=(float)atof(dxfValue);
				if (dxfCode==" 20") 
					pEntSpace->ymax=(float)atof(dxfValue);
			}
	}
	//////Unitfacto control
//	if(UnitFactor=1000&&(Xmax-Xmin>1000||Ymax-Ymin>1000)){
//		UnitFactor=1;Ymax/=1000;Ymin/=1000;Xmax/=1000;Xmin/=1000;}
	////////////////////
}
/////////��ȡֱ�ߵ���Ϣ
static void ReadLine(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>& era,CString&dxfValue,ESpace *pEntSpace){
	float Lh;//���
	int code;EntRaw et;
	CString dxfCode="";
	et.bulge=0;
	while(!(dxfCode=="  0"))
	{
		dxfFile.ReadString(dxfCode);
		dxfFile.ReadString(dxfValue);
		code=(int)atof(dxfCode);
		switch(code)
		{
		case 10:et.xstart =(float)atof(dxfValue);break;
		case 20:et.ystart =(float)atof(dxfValue);break;
		case 30:Lh=(float)atof(dxfValue);break;
		case 11:et.xend =(float)atof(dxfValue);break;
		case 21:et.yend =(float)atof(dxfValue);break;
		}
	}
	ReadEntAdd(pEntSpace,&Zar,&Iar,&era,&et,Lh);
}
////////��ȡԲ������Ϣ
static void ReadArc(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>& era,CString&dxfValue,ESpace *pEntSpace){
	float Lh = 0;
	float nom = 0;
	float sa = 0;
	float ea = 0;//����ߣ��뾶����ֹ��
	int code;EntRaw et;
	CString dxfCode="";
	//��ȡԲ������
	while(!(dxfCode=="  0"))
	{
		dxfFile.ReadString(dxfCode);
		dxfFile.ReadString(dxfValue);
		code=(int)atof(dxfCode);
		switch(code)
		{
		case 10:et.xc=(float)atof(dxfValue);break;
		case 20:et.yc=(float)atof(dxfValue);break;
		case 30:Lh=(float)atof(dxfValue);break;
		case 40:et.r=(float)atof(dxfValue);break;
		case 50:sa=(float)atof(dxfValue);break;
		case 51:ea=(float)atof(dxfValue);break;
		case 230:nom=float(atof(dxfValue));break;
		}
	}
	if(nom==-1.0){
		et.xc=-et.xc;Lh=-Lh;nom=sa;
		sa=-ea-180;ea=-nom+180;
		if(ea-sa>360)sa+=360;
	}
	//����Բ��ȡֵ��Χ��sa��ea����-2*Pi��2*Pi��Χ��
	if(ea<=sa) ea=ea+360;
	if(ea>360) {ea-=360;sa-=360;}
	if(ea<0 && sa<0) {ea+=360;sa+=360;}
	sa=sa*PI/180;ea=ea*PI/180;//�ǶȻ��Ȼ���
	//����Բ����ռ��Χ
	et.angle=ea-sa;
	et.sa=sa;
	ReadEntAdd(pEntSpace,&Zar,&Iar,&era,&et,Lh);
}
////////��ȡԲ����Ϣ
static void ReadCircle(CStdioFile& dxfFile,CArray<EntRaw,EntRaw>& era,CString&dxfValue,ESpace *pEntSpace){
	float Lh = 0;
	float nom = 0;//���
	int code;EntRaw et;
	CString dxfCode="";
	while(!(dxfCode=="  0"))
	{
		dxfFile.ReadString(dxfCode);
		dxfFile.ReadString(dxfValue);
		code=(int)atof(dxfCode);
		switch(code)
		{
		case 10:et.xc =(float)atof(dxfValue);break;
		case 20:et.yc =(float)atof(dxfValue);break;
		case 30:Lh=(float)atof(dxfValue);break;
		case 40:et.r=(float)atof(dxfValue);break;
		case 230:nom=(float)atof(dxfValue);break;
		}
	}
//	ASSERT(et.r>0&&et.r!=5.0f);
	if(nom==-1.0){et.xc=-et.xc;Lh=-Lh;}
//	et.angle=PI;
	et.angle=TWOPI;
	et.sa=0;	ReadEntAdd(pEntSpace,&Zar,&Iar,&era,&et,Lh);
//	et.sa=PI;	ReadEntAdd(pEntSpace,&Zar,&Iar,&era,&et,Lh);
}
static int LayerCompare(const void*l1,const void*l2){
	float dz=((ELayer*)l1)->z-((ELayer*)l2)->z;
	if(ZERO(dz))return 0;
	if(dz<0)return -1;
	return 1;
}
void ReadArrange(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int,int>*pIar){
	int i;float XO,YO;
	EntRaw*m_pEnts=pEntSpace->pEntRaw;
	XO=.5f*(pEntSpace->xmax+pEntSpace->xmin);
	YO=.5f*(pEntSpace->ymax+pEntSpace->ymin);
	///////����ƫ��
	for(i=0;i<pEntSpace->EntNum;i++){
			m_pEnts[i].xstart=m_pEnts[i].xstart-XO;
			m_pEnts[i].ystart=m_pEnts[i].ystart-YO;
		if(m_pEnts[i].bulge==0){
			m_pEnts[i].xend=m_pEnts[i].xend-XO;
			m_pEnts[i].yend=m_pEnts[i].yend-YO;
		}
		else{
			m_pEnts[i].yend=m_pEnts[i].yend;			
		}
	}
	pEntSpace->xmin-=XO;	pEntSpace->xmax-=XO;
	pEntSpace->ymin-=YO;	pEntSpace->ymax-=YO;
	//////����
	pEntSpace->pLayer=new ELayer[pEntSpace->LayerNum];
	ELayer*pLayer=pEntSpace->pLayer;
	float z;
	pLayer[0].pEntRaw=pEntSpace->pEntRaw;
	pLayer[0].iEntNum=pIar->GetAt(0)+1;
	pLayer[0].z=pZar->GetAt(0);
	for(i=1;i<pEntSpace->LayerNum;i++){
		pLayer[i].pEntRaw=pEntSpace->pEntRaw+pIar->GetAt(i-1)+1;
		pLayer[i].iEntNum=pIar->GetAt(i)-pIar->GetAt(i-1);
		pLayer[i].z=pZar->GetAt(i);
	}
	qsort(pLayer,pEntSpace->LayerNum,sizeof(ELayer),LayerCompare);
	pEntSpace->pz=new float[pEntSpace->LayerNum];z=pLayer[0].z;
	for(i=0;i<pEntSpace->LayerNum;i++)pEntSpace->pz[i]=pLayer[i].z-z;
	//////////
	pEntSpace->zmin=0;
	pEntSpace->zmax=pEntSpace->pz[pEntSpace->LayerNum-1];
	pIar->RemoveAll();
	pZar->RemoveAll();
}

void ReadEntAdd(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int ,int>*pIar,
				CArray<EntRaw,EntRaw>*pear,EntRaw*pe,float Lz){
	if(pEntSpace->EntNum==0){
		pZar->Add(Lz);
		pear->Add(*pe);
	}else{
		if(EQU(pZar->GetAt(pEntSpace->LayerNum),Lz)){
			pear->Add(*pe);		
		}else{
			int i=pEntSpace->LayerNum-1;
			for(;i>=0;i--){if(EQU(pZar->GetAt(i),Lz)) break;}
			if(i>=0){
				int inspos=pIar->GetAt(i)+1;
				pear->InsertAt(inspos,*pe);
				for(;i<pEntSpace->LayerNum;i++)(*pIar)[i]++;
			}else{
				pear->Add(*pe);		
				pIar->Add(pEntSpace->EntNum-1);
				pZar->Add(Lz);
				pEntSpace->LayerNum++;
			}
		}
	}
	pEntSpace->EntNum++;
}