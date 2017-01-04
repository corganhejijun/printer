#include "stdafx.h"
#include "types.h"
#include <afxtempl.h>

#include "math.h"
#define MESSAGE_ENT_INFO 0
void AppFileClose();
static int getModularChars(BYTE*pbuf,int *pmc);
static void dwgReadSize(CFile&dwgFile,ESpace*pEntSpace,int size);
static void ReaddwgEnt(CFile&dwgFile,CArray<EntRaw,EntRaw>*pEar,
					   ESpace*pEntSpace,CArray<float,float>*pZar,CArray<int,int>*pIar);
void ReadArrange(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int,int>*pIar);
void ReadEntAdd(ESpace *pEntSpace,CArray<float,float>*pZar,CArray<int ,int>*pIar,
				CArray<EntRaw,EntRaw>*pear,EntRaw*pe,float Lz);

BOOL ReadDwgFile(LPSTR szFile,ESpace *pEntSpace){
	CFile dwgFile;
	char vr15[]="AC1015",version[6];
	long epos,esize;DWORD apos=0;int i,ri;short si;
	BYTE *pbuf,*pos;BYTE bi;
	CArray<float,float>Zar;
	CArray<int,int>Iar;
	CArray<EntRaw,EntRaw>ear;
	if(!dwgFile.Open(szFile,CFile::modeRead))return FALSE;	
	dwgFile.Read(version,6);
	for(i=0;i<6;i++)
		if(vr15[i]!=version[i]){
			AfxMessageBox("不是AutoCAD2000 dwg文件");
			return FALSE;
		}
	AppFileClose();
	////// HEAD VARIBLES
	dwgFile.Seek(0x19,CFile::begin);
	dwgFile.Read(&bi,1);ASSERT(bi==0);
	dwgFile.Read(&epos,4);dwgFile.Read(&esize,4);
	dwgFile.Seek(epos,CFile::begin);
	dwgReadSize(dwgFile,pEntSpace,esize);
	////// OBJECT MAP
	dwgFile.Seek(0x2b,CFile::begin);
	dwgFile.Read(&bi,1);ASSERT(bi==2);
	dwgFile.Read(&epos,4);dwgFile.Read(&esize,4);
	dwgFile.Seek(epos,CFile::begin);
	pos=pbuf=new BYTE[esize];dwgFile.Read(pbuf,esize);
//	dwgFile.Seek(0L,CFile::begin);
	while(pos<pbuf+esize){
		BYTE*psec=pos;
//		si=*((short*)pos);pos+=2;
		si=(((int)pos[0])<<8)+pos[1];pos+=2;
		if(si==2)break;
		apos=0;dwgFile.Seek(0L,CFile::begin);
		while(pos<psec+si){
			pos+=getModularChars(pos,&ri);
//			ASSERT(ri>0);
			pos+=getModularChars(pos,&ri);
			dwgFile.Seek(ri,CFile::current);
			apos+=ri;
			ASSERT(apos==dwgFile.GetPosition());
			ReaddwgEnt(dwgFile,&ear,pEntSpace,&Zar,&Iar);
			dwgFile.Seek(apos,CFile::begin);
		}
		pos+=2;
	}
	delete []pbuf;
	dwgFile.Close();
	if(pEntSpace->EntNum>0){
		Iar.Add(pEntSpace->EntNum-1);
		pEntSpace->LayerNum++;
		pEntSpace->pEntRaw=(EntRaw*)malloc(pEntSpace->EntNum*sizeof(EntRaw));
		memcpy(pEntSpace->pEntRaw,ear.GetData(),pEntSpace->EntNum*sizeof(EntRaw));
		ear.RemoveAll();
		ReadArrange(pEntSpace,&Zar,&Iar);
	}else{
		ear.RemoveAll();
		Iar.RemoveAll();
		Zar.RemoveAll();
	}
return TRUE;
}
static int getModularChars(BYTE*pbuf,int *pmc){
	int i,size=0,sgn=1;*pmc=0;
	while(pbuf[size]&0x80)size++;
	if(pbuf[size]&0x40){pbuf[size]&=0x3f;sgn=-1;}
	for(i=0;i<=size;i++)*pmc+=((int)(0x7f&pbuf[i]))<<(i*7);
	if(sgn==-1)*pmc=-(*pmc);
	return size+1;
}
static BYTE getByte(BYTE*pbuf,int bpos){
	return ((pbuf[0]<<bpos)&0xff)+(pbuf[1]>>(8-bpos));
}
static void getNByte(BYTE*pbuf,int bpos,BYTE*p,short n){
	for(int i=0;i<n;i++)p[i]=getByte(pbuf+i,bpos);
}
static short getBitShort(BYTE*&pbuf,int& bpos,BOOL dataread=TRUE){
	BYTE w;
	ASSERT(bpos<8&&bpos>=0);
	if(bpos<7){
		w=(pbuf[0]>>(6-bpos))&0x03;
	}else{
		w=((pbuf[0]<<1)&0x02)+(pbuf[1]>>7);
	}
	bpos+=2;
	if(bpos>7){bpos-=8;pbuf++;}
	if(dataread){
		if(w==0){
			short s;
			getNByte(pbuf,bpos,(BYTE*)(&s),2);
			pbuf+=2;
			return s;
		}else if(w==1){
			w=getByte(pbuf,bpos);
			pbuf++;
			return w;
		}else if(w==2){
			return 0;
		}else {
			return 256;
		}
	}else{
		if(w==0)pbuf+=2;
		else if(w==1)pbuf++;
		return 0;
	}
}
static long getBitLong(BYTE*&pbuf,int& bpos,BOOL dataread=TRUE){
	BYTE w;
	ASSERT(bpos<8&&bpos>=0);
	if(bpos<7){
		w=(pbuf[0]>>(6-bpos))&0x03;
	}else{
		w=((pbuf[0]<<1)&0x02)+(pbuf[1]>>7);
	}
	bpos+=2;
	if(bpos>7){bpos-=8;pbuf++;}
	if(dataread){
		if(w==0){
			long l;
			getNByte(pbuf,bpos,(BYTE*)(&l),4);
			pbuf+=4;
			return l;
		}else if(w==1){
			w=getByte(pbuf,bpos);
			pbuf++;
			return w;
		}else 
			return 0;
	}else{
		if(w==0)pbuf+=4;
		else if(w==1)pbuf++;
		return 0;
	}
	
}

static double getBitDouble(BYTE*&pbuf,int&bpos,BOOL dataread=TRUE){
	BYTE w;
	ASSERT(bpos<8&&bpos>=0);
	if(bpos<7){
		w=(pbuf[0]>>(6-bpos))&0x03;
	}else{
		w=((pbuf[0]<<1)&0x02)+(pbuf[1]>>7);
	}
	bpos+=2;
	if(bpos>7){bpos-=8;pbuf++;}
	if(dataread){
		if(w==0){
			double d;
			getNByte(pbuf,bpos,(BYTE*)(&d),8);
			pbuf+=8;
			return d;
		}else if(w==1){
			return 1.0;
		}else
			return 0.0;
	}else{
		if(w==0)pbuf+=8;
		return 0;
	}
}
static double getBitDoubleWithDefault(BYTE*&pbuf,int&bpos,double df){
	BYTE w;
	ASSERT(bpos<8&&bpos>=0);
	if(bpos<7){
		w=(pbuf[0]>>(6-bpos))&0x03;
	}else{
		w=((pbuf[0]<<1)&0x02)+(pbuf[1]>>7);
	}
	bpos+=2;
	if(bpos>7){bpos-=8;pbuf++;}
	if(w==0){
		return df;
	}else if(w==1){
		__int64 dl=(*((__int64*)(&df)));
		getNByte(pbuf,bpos,(BYTE*)(&dl),4);
		pbuf+=4;
		return *((double*)(&dl));
	}else if(w==2){
		__int64 dl=(*((__int64*)(&df)));
		getNByte(pbuf,bpos,((BYTE*)(&dl))+4,2);
		getNByte(pbuf,bpos,(BYTE*)(&dl),4);
		pbuf+=6;
		return *((double*)(&dl));		
	}else {
		double d;
		getNByte(pbuf,bpos,(BYTE*)(&d),8);
		pbuf+=8;
		return d;
	}
}

static void ReaddwgEnt(CFile&dwgFile,CArray<EntRaw,EntRaw>*pEar,
					   ESpace*pEntSpace,CArray<float,float>*pZar,CArray<int,int>*pIar)
{
	int i=0,sizebit=0,sizex=0,l,ms=0,bpos=0;
	short etype,s;
	BYTE b,bit,*pbuf,*pos;
	do{
		VERIFY(dwgFile.Read(&s,2)==2);
		if(s&0x8000){ms+=(((int)(s&0x7fff))<<(15*i));i++;}
		else break;
	}while(1);
//	ASSERT((s&0x4000)==0);
	s&=0x7fff;
	ms+=(s<<(i*15));ASSERT(ms>2);
	pos=pbuf=new BYTE[ms];
	dwgFile.Read(pbuf,ms);
	etype=getBitShort(pos,bpos);/// read type
	if(etype==0x13/*LINE*/||etype==0x11/*arc*/||etype==0x12){
		getNByte(pos,bpos,(BYTE*)(&sizebit),4);pos+=4;//read size of object data
		b=getByte(pos,bpos);pos+=1;
		pos+=b&0x0f;
		sizex=getBitShort(pos,bpos);
		pos+=sizex;
		bit=(pos[0]>>(7-bpos))&0x01;bpos++;
		if(bpos>7){bpos-=8;pos++;}
		if(bit){
			getNByte(pos,bpos,(BYTE*)(&l),4);
			pos+=(4+l);
		}
		bpos+=2;if(bpos>7){bpos-=8;pos++;}
		getBitLong(pos,bpos,FALSE);
		bpos++;if(bpos>7){bpos-=8;pos++;}
		getBitShort(pos,bpos,FALSE);
		getBitDouble(pos,bpos,FALSE);
		bpos+=4;if(bpos>7){bpos-=8;pos++;}
		getBitShort(pos,bpos,FALSE);
		pos++;
		EntRaw e;double d;
		//////read ent data
		if(etype==0x13){
			bit=(pos[0]>>(7-bpos))&0x01;
			bpos++;if(bpos>7){bpos-=8;pos++;}
			e.bulge=0;float zs,ze;
			getNByte(pos,bpos,(BYTE*)(&d),8);pos+=8;e.xstart=float(d);
			e.xend=(float)getBitDoubleWithDefault(pos,bpos,d);
			getNByte(pos,bpos,(BYTE*)(&d),8);pos+=8;e.ystart=float(d);
			e.yend=(float)getBitDoubleWithDefault(pos,bpos,d);
			if(bit){zs=ze=0;}
			else {
				getNByte(pos,bpos,(BYTE*)(&d),8);pos+=8;zs=float(d);
				ze=(float)getBitDoubleWithDefault(pos,bpos,d);
				if(!EQU(ze,zs)){delete []pbuf;return;}
			}
			ReadEntAdd(pEntSpace,pZar,pIar,pEar,&e,zs);
#if MESSAGE_ENT_INFO
			CString str;
			str.Format("LINE:\nxstart:%f\nxend:%f\nystart:%f\nyend:%f\nzstart:%f\nzend:%f\n",
				e.xstart,e.xend,e.ystart,e.yend,zs,ze);
			AfxMessageBox(str);
#endif
		}else if(etype==0x12){/*CIRCLE*/
			float lz;
			e.xc=(float)getBitDouble(pos,bpos);
			e.yc=(float)getBitDouble(pos,bpos);
			lz=(float)getBitDouble(pos,bpos);
			e.r=(float)getBitDouble(pos,bpos);
			bit=(pos[0]>>(7-bpos))&0x01;bpos++;if(bpos>7){bpos-=8;pos++;}
			if(bit==0)getBitDouble(pos,bpos,FALSE);////THICHNESS
			bit=(pos[0]>>(7-bpos))&0x01;bpos++;if(bpos>7){bpos-=8;pos++;}
			if(bit==0){
				float exx,exy,exz;
				exx=(float)getBitDouble(pos,bpos);if(!ZERO(exx)){delete []pbuf;return;}
				exy=(float)getBitDouble(pos,bpos);if(!ZERO(exx)){delete []pbuf;return;}
				exz=(float)getBitDouble(pos,bpos);
				if(exz<0){e.xc=-e.xc;lz=-lz;}
			}////EXTRUTION
			e.angle=TWOPI;
			e.sa=0;ReadEntAdd(pEntSpace,pZar,pIar,pEar,&e,lz);
#if MESSAGE_ENT_INFO
			CString str;
			str.Format("CIRCLE:\nxc:%f\nyc:%f\nzc:%f\nr:%f",
				e.xc,e.yc,lz,e.r);
			AfxMessageBox(str);	
#endif
		}else if(etype==0x11){/* ARC*/
			float lz,exz=1.0;
			e.xc=(float)getBitDouble(pos,bpos);
			e.yc=(float)getBitDouble(pos,bpos);
			lz=(float)getBitDouble(pos,bpos);
			e.r=(float)getBitDouble(pos,bpos);
			bit=(pos[0]>>(7-bpos))&0x01;bpos++;if(bpos>7){bpos-=8;pos++;}
			if(bit==0)getBitDouble(pos,bpos,FALSE);////THICHNESS
			bit=(pos[0]>>(7-bpos))&0x01;bpos++;if(bpos>7){bpos-=8;pos++;}
			if(bit==0){
				float exx,exy;
				exx=(float)getBitDouble(pos,bpos);if(!ZERO(exx)){delete []pbuf;return;}
				exy=(float)getBitDouble(pos,bpos);if(!ZERO(exx)){delete []pbuf;return;}
				exz=(float)getBitDouble(pos,bpos);
			}////EXTRUTION
			float gstart,gend;
			gstart=(float)getBitDouble(pos,bpos);
			gend=(float)getBitDouble(pos,bpos);
			if(exz<0){
				e.xc=-e.xc;lz=-lz;exz=gstart;
				gstart=-gend-PI;gend=-exz+PI;
				if(gend-gstart>TWOPI)gstart+=TWOPI;
			}
			if(gend<gstart)gend+=TWOPI;
			if(gend>TWOPI){gend-=TWOPI;gstart-=TWOPI;}
			if(gend<0&&gstart<0){gend+=TWOPI;gstart+=TWOPI;}
			e.sa=gstart;e.angle=gend-gstart;
			ReadEntAdd(pEntSpace,pZar,pIar,pEar,&e,lz);
#if MESSAGE_ENT_INFO
			CString str;
			str.Format("ARC:\nxc:%f\nyc:%f\nzc:%f\nr:%f\nstart angle:%f\nend angle:%f",
				e.xc,e.yc,lz,e.r,gstart,gend);
			AfxMessageBox(str);	
#endif
		}


	}
	delete []pbuf;
}

void dwgReadSize(CFile&dwgFile,ESpace*pEntSpace,int size){
	int bpos=0,i;BYTE*pos,*pbuf,b;
	pos=pbuf=new BYTE[size];
	dwgFile.Read(pbuf,size);
	pos+=20;
	for(i=0;i<4;i++)getBitDouble(pos,bpos,FALSE);
	for(i=0;i<4;i++)pos+=getBitShort(pos,bpos);
	getBitLong(pos,bpos,FALSE);getBitLong(pos,bpos,FALSE);
	b=getByte(pos,bpos);pos+=(1+(b&0x0f));
	pos+=2;bpos+=4;if(bpos>7){bpos-=8;pos++;}
	for(i=0;i<27;i++)getBitShort(pos,bpos,FALSE);
	for(i=0;i<21;i++)getBitDouble(pos,bpos,FALSE);
	pos+=getBitShort(pos,bpos);
	for(i=0;i<8;i++)getBitLong(pos,bpos,FALSE);
	getBitShort(pos,bpos,FALSE);
	for(i=0;i<6;i++){b=getByte(pos,bpos);pos+=(1+(b&0x0f));}
	for(i=0;i<10;i++)getBitDouble(pos,bpos,FALSE);
	pos+=16;
	for(i=0;i<10;i++)getBitDouble(pos,bpos,FALSE);
	b=getByte(pos,bpos);pos+=(1+(b&0x0f));
	b=getByte(pos,bpos);pos+=(1+(b&0x0f));
	getBitShort(pos,bpos);
	b=getByte(pos,bpos);pos+=(1+(b&0x0f));
	for(i=0;i<21;i++)getBitDouble(pos,bpos,FALSE);
	pEntSpace->xmin=(float)getBitDouble(pos,bpos);
	pEntSpace->ymin=(float)getBitDouble(pos,bpos);
	pEntSpace->zmin=(float)getBitDouble(pos,bpos);
	pEntSpace->xmax=(float)getBitDouble(pos,bpos);
	pEntSpace->ymax=(float)getBitDouble(pos,bpos);
	pEntSpace->zmax=(float)getBitDouble(pos,bpos);
	delete pbuf;
}
