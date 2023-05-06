// Pro_P1546_PolygonDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SMS4DC.h"
#include "Pro_P1546_PolygonDoc.h"
#include <math.h>
#include "UTM.h"
//#include "P1546_Functions.h"

extern "C" {
#include "C:\\Br_soft\\IDWM\\include\\idwm32.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_PolygonDoc

IMPLEMENT_DYNCREATE(CPro_P1546_PolygonDoc, CDocument)

CPro_P1546_PolygonDoc::CPro_P1546_PolygonDoc()
{
	m_ReadyDoc=0;
	m_ZoomFactor=16;

	long UNR = 72 ;	WDBUNIT(&UNR) ;
	
	pi = 4.0*atan(1.0);
	D2R = pi/180.0;
	R2D = 180.0/pi;
}

BOOL CPro_P1546_PolygonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPro_P1546_PolygonDoc::~CPro_P1546_PolygonDoc()
{
	delete [] m_di;
	delete [] m_hi;
	delete [] m_lati;
	delete [] m_loni;
	delete [] Npt;
	delete [] Er;
	delete [] bufAreaP1546;
	delete [] bufArea;
}


BEGIN_MESSAGE_MAP(CPro_P1546_PolygonDoc, CDocument)
	//{{AFX_MSG_MAP(CPro_P1546_PolygonDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_PolygonDoc diagnostics

#ifdef _DEBUG
void CPro_P1546_PolygonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPro_P1546_PolygonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_PolygonDoc serialization

void CPro_P1546_PolygonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_PolygonDoc commands

__declspec(dllimport) double FieldP1546V5(double f_MHz,double t,int p23path,double h1,double dT_km,double dsea_km,
										  double tetamax15_deg, double tetamax16_deg ,double h2, int ENV,
										  int pRX, double ha, int pTX,
										  double Hgt, double Hgr, int srv, double L, BOOL P1546Clangle, BOOL P1546landsea);

void CPro_P1546_PolygonDoc::GetData()
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	Width  = m_ZoomFactor * bufAreaW;
	Height = m_ZoomFactor * bufAreaH;
	bufArea = new _int16[Width*Height];

	for(int i=0;i<Width;i++)
		for(int j=0;j<Height;j++)
			bufArea[i+Width*j]=bufAreaP1546[(i/m_ZoomFactor)+(Width/m_ZoomFactor)*(j/m_ZoomFactor)];

	Er=new double[Np];
	double dist,Ghv=1.0;

	CString antfile;
	antfile.Format("%sAntenna\\ant_%s.ant",((CSMS4DCApp *)AfxGetApp())->m_AppPath ,LineANT_ST);
	
	int f0[360];	double rH[360]  ,  rV[360];
	ReadAntennaData(antfile,f0,rH,rV) ;

	double elevp,tp,fp,az,f,t, rearth=6371.0;

	double re = Linek*rearth*1000.0;
	double AZ0 = LineAZ_ST*D2R ,	EL0 = LineEL_ST*D2R;
	double Hgi, Hg = LineHasl_ST,  Ht = LineHagl_ST,  Hr = LineRxH;

	CPoint m_Point_STcr3,m_Point_STcr15;
	double lat3km,lon3km,lat15km,lon15km,h1,tetai,mds,ds,lat015km , lon015km,hi,tetamax15;
	double dz,mdz,azI,tetamax16,hii,ttca;

//	CPoint pointi;
//	double Dland,ELand,ESea,EfsLand,EfsSea,Eb,Ebfs;
//	double corr_ca=0.0;
//	double delta,deltafs,E_sea_total,Efs_sea_total,Emix,Efsmix;
//	double E_Land_total,Efs_Land_total,deltaE,deltaEfs,alpha,beta,XX;
//	CP1546_Functions CP1546;

	CString globeTileInfo = ((CSMS4DCApp *)AfxGetApp())->m_AppPath + _T("Texts\\globe.txt");
//	double StepRes=1.0/m_ZoomFactor;
//	if (TileInfo != globeTileInfo)
//		StepRes=8.0*(m_Resolution_x/1000.0)/m_ZoomFactor;

double disttest1 = Distance_km(Linelat_ST, Linelon_ST, Linelat_ST+0, Linelon_ST+m_Resolution_x); 
double disttest2 = Distance_km(Linelat_ST, Linelon_ST, Linelat_ST+m_Resolution_x, Linelon_ST+0); 
double StepRes = min(disttest1, disttest2)/m_ZoomFactor;

/*
	int	U = Lineenvironment;
	double R;
	int p;
	if		(U==0){ R=10;p=1;}
	else if (U==1){	R=10;p=1;}
	else if (U==2){	R=10;p=1;}
	else if (U==3){	R=20;p=1;}
	else if (U==4){	R=30;p=1;}
	else if (U==5){	R=10;p=2;}
	else if (U==6){	R=10;p=3;}
	else		  {	R=10;p=1;}
*/

Linelon_ST = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(Linelon_ST,TileX);
Linelon0   = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(Linelon0,TileX);

	double DseaW,DseaC,DSea;
//int k;
//double lati,loni,dx,di,step,EsigmaL,sigma;

/////////////////////Load IDWM map////////////////
	LoadIDWMmap();
//////////////////////////////////////////////////
	
	int p = LandWarmColdSea(Linelon_ST, Linelat_ST);
	int pRX = p, p23path = p;

	double dist1000_re,sintp, costp, sinfp_AZ0, cosfp_AZ0, sinEL0 = sin(EL0), cosEL0 = cos(EL0);

	for ( i=0;i<Np;i++)
	{
		dist = Distance_km(Linelat_ST,Linelon_ST,m_lati[i],m_loni[i]);	dist = max(dist , 0.0005);
		az = Azimuth_Deg(Linelat_ST,Linelon_ST,m_lati[i],m_loni[i]);				
		Hgi = m_hi[i];

		/////////////////////////////// h1 (Heff) ///////////////////////////////////////////////
		if (dist>=15.0)
		{
			reckon( Linelat_ST,Linelon_ST,  3.0 , az , &lat3km  , &lon3km) ;
			reckon( Linelat_ST,Linelon_ST, 15.0 , az , &lat15km , &lon15km) ;
		}
		else
		{
			reckon( Linelat_ST,Linelon_ST, 0.2*dist , az , &lat3km  , &lon3km) ;
			reckon( Linelat_ST,Linelon_ST,     dist , az , &lat15km , &lon15km) ;
		}
		LatLon2Point(lat3km  , lon3km , &m_Point_STcr3) ;
		LatLon2Point(lat15km , lon15km, &m_Point_STcr15) ;
		h1=Hg+LineHagl_ST - Points2HgAvr(m_Point_STcr3 , m_Point_STcr15) ;

		///////////////////////////////// tetamax15 /////////////////////////////////////////////
//		if (h1<0.0)
		{
			tetai=-pi/2.0;
			mds=min(dist,15.0);
			ds=0.0;
			while( ds<= mds )  //ds=0->15
			{
				ds=ds+StepRes;
				reckon( Linelat_ST,Linelon_ST, ds , az , &lat015km , &lon015km) ;
				hi = LatLon2Hg(lat015km, lon015km);
				tetai=max(tetai   ,(((hi-Hg-LineHagl_ST)/(ds*1000))-(ds/(2.0*Linek*rearth))));
			}
			tetamax15 = R2D*tetai;
		}
		/////////////////////////////////Rx ttca /////////////////////////////////////////////
//  	if (LineClangle==1)
		{
			azI=Azimuth_Deg(m_lati[i],m_loni[i],Linelat_ST,Linelon_ST);
			mdz=min(dist,16.0);
			tetamax16=-pi/2.0;
			dz=0.0;
			while( dz<= mdz )  //dz=0->16
			{
				dz=dz+StepRes;
				reckon(m_lati[i],m_loni[i], dz , azI , &lat015km , &lon015km) ;
				hii = LatLon2Hg(lat015km, lon015km) ;
//				tetamax16=max(tetamax16   ,(((hii-10.0-Hgi)/(dz*1000))-(dz/(2.0*Linek*rearth))));
				tetamax16 = max(tetamax16   ,(((hii-10.0-Hgi)/(dz*1000.0))-(dz/(2.0*rearth))));  //P.1546-5  section 11
			}
			ttca = tetamax16*R2D;
//			corr_ca = TCAcorr1546(ttca,LinefMHz_ST);
		}
		//////////////////////////////////////////////////////////////////////////////
		if (Linelandsea==1)
		{
			DSea = LandWarmColdSea_D_Sea_kmIDWM(Linelat_ST,Linelon_ST, m_lati[i],m_loni[i], &DseaW, &pRX); 
			DseaC = DSea - DseaW;
											p23path = 1;
			if      (DseaW>0)				p23path = 3;
			else if((DseaW<=0)&&(DseaC>0))	p23path = 2;
		}
		double Eb = FieldP1546V5(LinefMHz_ST,Linetime, p23path, h1,dist, DSea,
								  tetamax15, ttca ,Hr, Lineenvironment,
								  pRX, Ht, p,  Hg, Hgi, 
								  Linesyst, Linelocation, LineClangle, Linelandsea);
/*	//old version
		if (Linelandsea==1)
		{
	//		step=StepRes;
	//		int n=1+(int)(dist/step);
	//		dx=0.0;k=0;
	//		for (int j2=0;j2<n;j2++)
	//		{
	//			di=dx;
	//			reckon(Linelat_ST,Linelon_ST,di,az,&lati,&loni);
	//			LatLon2Point(lati,loni,&pointi);
	//			hi=Point2Hg(pointi);
	//			if(hi<= m_NoData)	k=k+1;	//Sea
	//			dx=dx+step;
	//		}
	//		Dland=dist*( (double)(n-k) )/( (double)n );

			DSea = D_Sea_kmIDWM(Linelat_ST,Linelon_ST,m_lati[i],m_loni[i], &DseaW);
			Dland = dist - DSea;
			DseaC = DSea - DseaW;

			if (Dland==dist)		Eb = CP1546.P1546_8(LinefMHz_ST,Linetime,1,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Ebfs);
			else if (DseaC==dist)	Eb = CP1546.P1546_8(LinefMHz_ST,Linetime,2,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Ebfs);
			else if (DseaW==dist)	Eb = CP1546.P1546_8(LinefMHz_ST,Linetime,3,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Ebfs);
			else
			{
				if (Dland<1.0)
				{
					ELand = CP1546.P1546_8(LinefMHz_ST,Linetime,1,h1,1.0,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&EfsLand);
					ESea = CP1546.P1546_8(LinefMHz_ST,Linetime,3,h1,1.0,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&EfsSea);
					delta = Dland * (ELand - ESea);
					deltafs = Dland * (EfsLand - EfsSea);
				}
				else if (Dland>=1.0)
				{
					ELand = CP1546.P1546_8(LinefMHz_ST,Linetime,1,h1,Dland,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&EfsLand);
					ESea = CP1546.P1546_8(LinefMHz_ST,Linetime,3,h1,Dland,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&EfsSea);
					delta = ELand - ESea;
					deltafs = EfsLand - EfsSea;
				}

				E_sea_total = CP1546.P1546_8(LinefMHz_ST,Linetime,3,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Efs_sea_total);
				Emix = E_sea_total + delta;
				Efsmix = Efs_sea_total + deltafs;

				E_Land_total = CP1546.P1546_8(LinefMHz_ST,Linetime,1,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Efs_Land_total);
				deltaE = Emix - E_Land_total;
				deltaEfs = Efsmix - Efs_Land_total;

				alpha = 0.3;      beta=0.0001;
				XX = alpha + (1.0 - alpha) * exp(-(beta *    pow( Dland , (2.42-0.0003527*h1) )    ));
				Eb = E_Land_total + deltaE * XX;
				Ebfs = Efs_Land_total + deltaEfs * XX;
			}

			//	if(m_hi[i]<= m_NoData)	EsigmaL = 0.0;	//Sea  p=3
			//	else					EsigmaL = zsigmaLcrr(1,Linesyst,LinefMHz_ST,Linelocation,&sigma);   //Land   p=1
		}
		else
		{
			Eb = CP1546.P1546_8(LinefMHz_ST,Linetime,p,h1,dist,LineRxH,Lineenvironment,tetamax15,Linesyst,Linelocation,&Ebfs);
		//	EsigmaL = zsigmaLcrr(p,Linesyst,LinefMHz_ST,Linelocation,&sigma);
		}

		Eb = Eb + corr_ca;
	//	Eb=Eb+EsigmaL;
		Eb=min(Eb,Ebfs);
*/
		//0000000000000000000000000000000000000000000000000000000000000000
		//                     Antenna    Effect
		//0000000000000000000000000000000000000000000000000000000000000000
		dist1000_re = 1000.0*dist/re;
								
		elevp = atan(((re+Hr+Hgi)*cos(dist1000_re)-(re+Ht+Hg))/((re+Hr+Hgi)*sin(dist1000_re)));  //with Terrain
	//	elevp = atan(((re+Hr    )*cos(dist1000_re)-(re+Ht+Hg))/((re+Hr    )*sin(dist1000_re)));  //without Terrain
	
		fp=az*D2R;		tp = (pi/2.0) - elevp;
		sinfp_AZ0 = sin(fp-AZ0);	cosfp_AZ0 = cos(fp-AZ0);	sintp = sin(tp);	costp = cos(tp);

		f = R2D*atan2(sintp*sinfp_AZ0 , cosEL0*sintp*cosfp_AZ0 + costp*sinEL0);
		t = R2D*acos(costp*cosEL0 - sinEL0*sintp*cosfp_AZ0);
		if (f<0.0)			f=f+359.4;

		Ghv = Interp2(f0,rH,f,360) * Interp2(f0,rV,t,360);
		//0000000000000000000000000000000000000000000000000000000000000000
		Eb = Eb + 10.0*log10(LinePtGt_ST*Ghv/1650.0);
		Er[i] = Eb;
	}
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

double CPro_P1546_PolygonDoc::Distance_km(double lat1,double lon1,double lat2,double lon2) 
{
	float	lat10 = (float)(lat1*pi/180.0),
			lon10 = (float)(lon1*pi/180.0),
			lat20 = (float)(lat2*pi/180.0),
			lon20 = (float)(lon2*pi/180.0),	rng;
	GEODSTR( &lon10, &lat10, &lon20, &lat20, &rng);
	return (rng);

/*
	double temp1,temp2,temp3,rng;
	double pi=4.0*atan(1.0);
	double R = 6371.0;

	lat1=lat1*pi/180.0;
	lat2=lat2*pi/180.0;
	lon1=lon1*pi/180.0;
	lon2=lon2*pi/180.0;

	temp1 = sin(lat1)*sin(lat2);
	temp2 = cos(lat1)*cos(lat2)*cos(lon2-lon1);
	temp3 = temp1+temp2;
	temp3=min(1.0,max(-1.0,temp3));

	rng = acos(temp3);
	rng=R*rng;    //km
	return (rng);
*/
}

void CPro_P1546_PolygonDoc::ReadAntennaData(CString antfile,int *f0,double *rH,double *rV) 
{
	FILE *fid;
	if( (fid  = fopen(antfile,"rt")) != NULL )
	{
		char Name [20],
			 Gain_dBi [20],
			 BeamWidth_H [20],
			 BeamWidth_V [20],
			 Frq_Lo [20],
			 Frq_Hi [20],
			 Frq_unit [20],
			 Polarization [20],
			 dum[20];

		char Ang[20],PatH[20],PatV[20];

		fscanf( fid, "%s %s", dum,Name);
		fscanf( fid, "%s %s", dum,Gain_dBi);
		fscanf( fid, "%s %s", dum,BeamWidth_H);
		fscanf( fid, "%s %s", dum,BeamWidth_V);
		fscanf( fid, "%s %s", dum,Frq_Lo);
		fscanf( fid, "%s %s", dum,Frq_Hi);
		fscanf( fid, "%s %s", dum,Frq_unit);
		fscanf( fid, "%s %s", dum,Polarization);
		fscanf( fid, "%s", dum);

		double pi=4.0*atan(1.0);
		double rH1,rV1;
		for (int i=0;i<360;i++)
		{
			fscanf( fid, "%s %s %s", Ang,PatH,PatV);
			f0[i] = atoi(Ang);
			rH1 = atof(PatH);		rV1 = atof(PatV);
			rH1 = pow( 10.0 , (-rH1/10.0) );
			rV1 = pow( 10.0 , (-rV1/10.0) );
			rH[i]=rH1;		rV[i]=rV1;
		}
		fclose(fid);
	}
	else
	{
		CString str;
		str.Format(_Z("The Antenna name  [%s]  was not found!  \r\rThe Isotropic Antenna will be select."),antfile);
		MessageBox(NULL,str, _Z("Warning!!!"), MB_ICONWARNING | MB_OK);
		for (int i=0;i<360;i++)
		{
			f0[i] = i;		rH[i]=1.0;		rV[i]=1.0;
		}
	}
}

double CPro_P1546_PolygonDoc::Azimuth_Deg(double lat1,double lon1,double lat2,double lon2) 
{
	float	lat10 = (float)(lat1*pi/180.0),
			lon10 = (float)(lon1*pi/180.0),
			lat20 = (float)(lat2*pi/180.0),
			lon20 = (float)(lon2*pi/180.0),	RDIST, RAZIM;
	GEOPPDA( &lon10 ,&lat10, &lon20,&lat20, &RDIST, &RAZIM);
	return RAZIM;

/*
	double pi=4.0*atan(1.0);
	double epsilon =  0.000000017453;

	lat1=lat1*pi/180.0;
	lon1=lon1*pi/180.0;
	lat2=lat2*pi/180.0;
	lon2=lon2*pi/180.0;
	double az=0.0;

	if (lat1 >= pi/2.0-epsilon)
		az=pi;
	else if (lat1 <= epsilon-pi/2.0)
		az=0.0;
	else if (lat2 >= pi/2-epsilon)
		az=0.0;
	else if (lat2 <= epsilon-pi/2.0)
		az=pi;
	else
		{
			double fact1 = cos(lat2) * sin(lon2-lon1);
			double fact2 = cos(lat1) * sin(lat2);
			double fact3 = sin(lat1) * cos(lat2) * cos(lon2-lon1);
			az = atan2(fact1,fact2-fact3);

			//az=round(az,-8);
			int az8=(int)(az*100000000.0);
			az=((double)az8)/100000000.0;
			
			if (az < 0.0)
				az=2*pi+az;
		}
	az = az*180.0/pi;
	return az;
*/
}

double CPro_P1546_PolygonDoc::Interp2(int *D0,double *E0,double d,int num) 
{
	double d1,d2,E1,E2,Eb;
	int m;

	if (d<=D0[0])
		m=0;
	else if (d>=D0[num-1])
		m=num-2;
	else
	{
		int n=0;
		while(D0[n]<=d)
			n++;

		m=n-1;
	}
	d1=D0[m];d2=D0[m+1];
	E1=E0[m];E2=E0[m+1];
	Eb = E1+(E2-E1)*(d-d1)/(d2-d1);
	return Eb;
}

void CPro_P1546_PolygonDoc::reckon(double lat_deg,double lon_deg,double rng_km,double az_deg,double *latC_deg,double *lonC_deg) 
{
	float	lat_rad = (float)(lat_deg*pi/180.0),
			lon_rad = (float)(lon_deg*pi/180.0),
			RLON, RLAT,	 rng_km1 = (float)rng_km, az_deg1 = (float)az_deg;
	GEOPDAP( &lon_rad, &lat_rad, &rng_km1, &az_deg1, &RLON, &RLAT);
	*latC_deg = RLAT*180.0/pi;
	*lonC_deg = RLON*180.0/pi;
/*
	double pi=4.0*atan(1.0);
	double R=6371.0;

	double rng_rad=rng_km/R;
	double az_rad=az_deg*pi/180.0;
	double lat_rad=lat_deg*pi/180.0;
	double lon_rad=lon_deg*pi/180.0;

	double temp1  = sin(lat_rad)*cos(rng_rad);          
	double temp2  = cos(lat_rad)*sin(rng_rad)*cos(az_rad);
	double newlat = asin(temp1+temp2);

	temp1  = sin(rng_rad)*sin(az_rad);            
	temp2  = cos(lat_rad)*cos(rng_rad);
	double temp3  = sin(lat_rad)*sin(rng_rad)*cos(az_rad);
	double newlon = lon_rad + atan2(temp1,temp2-temp3);

	*latC_deg=newlat*180.0/pi;
	*lonC_deg=newlon*180.0/pi;
*/
}


void CPro_P1546_PolygonDoc::LatLon2Point(double lat_deg,double lon_deg,CPoint *PointSt) 
{
lon_deg = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(lon_deg,TileX);

	CString globeTileInfo = ((CSMS4DCApp *)AfxGetApp())->m_AppPath + _T("Texts\\globe.txt");
	if (TileInfo != globeTileInfo)
	{
		CUtm m_utm;
		m_utm.phi=lat_deg;
		m_utm.lambda=lon_deg;
		m_utm.philambda2UTM();
		lat_deg=m_utm.y;
		lon_deg=m_utm.x;
	}
	double ix = (double)(m_ZoomFactor)*((lon_deg - Linelon0)/(m_Resolution_x));
	double jy = (double)(m_ZoomFactor)*((lat_deg - Linelat0)/(m_Resolution_x));

	(*PointSt).x = (long)ix;
	(*PointSt).y=(long)(Height-1-jy);
}
double CPro_P1546_PolygonDoc::Point2Hg(CPoint point1) 
{
	int i1=point1.x   ,   j1=(Height-1)-point1.y;
	_int16 Hg=bufArea[i1+Width*j1];
	return (double)Hg;
}
double CPro_P1546_PolygonDoc::LatLon2Hg0(double lat_deg,double lon_deg) 
{
	CPoint pointi;
	LatLon2Point(lat_deg, lon_deg, &pointi) ;	
	double hi = Point2Hg(pointi);
	return hi;
}
double CPro_P1546_PolygonDoc::LatLon2Hg(double lat_deg,double lon_deg)
{
	lon_deg = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(lon_deg,TileX);

	CString globeTileInfo = ((CSMS4DCApp *)AfxGetApp())->m_AppPath + _T("Texts\\globe.txt");
	if (TileInfo != globeTileInfo)
	{
		CUtm m_utm;
		m_utm.phi=lat_deg;
		m_utm.lambda=lon_deg;
		m_utm.philambda2UTM();
		lat_deg=m_utm.y;
		lon_deg=m_utm.x;
	}
	double ix = (double)(m_ZoomFactor)*((lon_deg - Linelon0)/(m_Resolution_x));
	double jy = (double)(m_ZoomFactor)*((lat_deg - Linelat0)/(m_Resolution_x));

	double Hg = RoundBUF_Hg(ix, jy);
	return Hg;
}
double CPro_P1546_PolygonDoc::RoundBUF_Hg(double xps,double yps) 
{
	int i1 = int(xps);				int j1 = int(yps);
	i1 = max(min(i1,Width-1),0);	j1 = max(min(j1,Height-1),0);
	int i2 = i1+1;					int j2 = j1+1;
	double h11, h12, h21, h22, hx1, hx2, hy;

	if     ((i1 == (Width-1))&&(j1 == (Height-1)))		hy = (double)bufArea[i1+Width*j1];
	else if((i1 == (Width-1))&&(j1  < (Height-1)))
	{
		h11 = (double)bufArea[i1+Width*j1]; h12 = (double)bufArea[i1+Width*j2];	hy  = h11 +(yps-j1)*(h12-h11)/(j2-j1);
	}
	else if((i1 < (Width-1))&&(j1 == (Height-1)))
	{
		h11 = (double)bufArea[i1+Width*j1]; h21 = (double)bufArea[i2+Width*j1];	hy  = h11 +(xps-i1)*(h21-h11)/(i2-i1);
	}
	else
	{
		h11 = (double)bufArea[i1+Width*j1]; h12 = (double)bufArea[i1+Width*j2]; h21 = (double)bufArea[i2+Width*j1]; h22 = (double)bufArea[i2+Width*j2];

		hx1 = h11 +(xps-i1)*(h21-h11)/(i2-i1); hx2 = h12 +(xps-i1)*(h22-h12)/(i2-i1);
		hy  = hx1 +(yps-j1)*(hx2-hx1)/(j2-j1);
	}
	return hy;
}
double CPro_P1546_PolygonDoc::Points2HgAvr(CPoint point1 , CPoint point2) 
{
	double HgAvr = 0.0;
	if (point1 != point2)
	{
		long x1 = point1.x,		y1 = point1.y;
		long x2 = point2.x,		y2 = point2.y;
		long xabs = abs(x1-x2),	yabs = abs(y1-y2);

		int Np = ((xabs>=yabs) ? xabs+1 : yabs+1);
		int *X;		X = new int[Np];
		int *Y;		Y = new int[Np];
		if (xabs>=yabs)
		{
			for (int i=0; i<Np; i++)
			{
				X[i]=((x1<x2) ? x1+i : x1-i);
				Y[i]=Round((double)y1+(double)((X[i]-x1)*(y2-y1))/((double)(x2-x1)));
			}
		}
		else
		{
			for (int i=0; i<Np; i++)
			{
				Y[i]=((y1<y2) ? y1+i :y1-i);
				X[i]=Round((double)x1+(double)((Y[i]-y1)*(x2-x1))/((double)(y2-y1)));
			}
		}

		CPoint point0;
		double HgSum = 0.0;
		for (int i=0; i<Np; i++)
		{	point0.x = X[i];	point0.y = Y[i];
			HgSum = HgSum + Point2Hg(point0);
		}
		HgAvr = HgSum/((double)Np);
		delete [] X; 
		delete [] Y; 
	}
	else
		HgAvr = Point2Hg(point1);

	return HgAvr;
}

double CPro_P1546_PolygonDoc::TCAcorr1546(double Tdeg,double f_MHz)
{
	Tdeg=min(max(Tdeg , -0.8) ,40.0);

	double v = 0.065 * Tdeg * sqrt(f_MHz);
	double v01 = v - 0.1;
	double Jv = 6.9 + 20.0 * log10( sqrt(1+v01*v01) + v01 );
	double vp = 0.036 * sqrt(f_MHz);
	double vp01 = vp - 0.1;
	double Jvp = 6.9 + 20.0 *log10( sqrt(1+vp01*vp01) + vp01 );
	double Corr = Jvp - Jv;

	return Corr;
}

int CPro_P1546_PolygonDoc::Round(double x) 
{
	int x1 = (int) x;
	int x2 = x1+1;
	return ( (x > ((double)x1)+0.45) ? x2 : x1);
}


int CPro_P1546_PolygonDoc::WarmColdSea(double RLON_deg,double RLAT_deg) 
{
	int nFlag = 0;  //"Cold Sea"

	if ((((RLON_deg>=2.0 && RLON_deg<=42.0)&&(RLAT_deg>=30.0 && RLAT_deg<=48.0))||
		((RLON_deg>=-5.6 && RLON_deg<=2.0)&&(RLAT_deg>=30.0 && RLAT_deg<=42.0)))||(RLAT_deg<=30.0))
		nFlag = 1;  // "Warm Sea"

	return nFlag;
}

int CPro_P1546_PolygonDoc::LandWarmColdSea(double RLON_deg,double RLAT_deg) 
{
	int WCSea = WarmColdSea(RLON_deg, RLAT_deg);

	float RLON = (float)(RLON_deg*pi/180.0);
	float RLAT = (float)(RLAT_deg*pi/180.0);

	long int N=4, IER;	GEOGCMS ( &N, &IER );

	float RLON1 = (float)(RLON-pi/90.0), RLAT1 = (float)(RLAT-pi/90.0),
		  RLON2 = (float)(RLON+pi/90.0), RLAT2 = (float)(RLAT+pi/90.0);

	long IVAL = 2 , IRANGE = 0,  NRPNT = 2;
	float  ARRAY[2][2]={RLON1, RLAT1, RLON2, RLAT2},  WND[2][2],  GC;
	GEOWND( (float *)ARRAY, &NRPNT, (float *)WND);
	GEOGCM( &IVAL, (float *)WND, &IRANGE);
	GEOGCP( &RLON, &RLAT, &GC);

	int p=1;
	if		((GC==1)&&(WCSea==0)) p=2;	//LandSea = "Coastal Zone : Cold Sea";
	else if ((GC==1)&&(WCSea==1)) p=3;	//LandSea = "Coastal Zone : Warm Sea";
	else if (GC==3)				  p=1;	//LandSea = "Coastal Zone : Coastal Land";
	else if (GC==4)			 	  p=1;	//LandSea = "Coastal Zone : Land";

	return p;
}



double CPro_P1546_PolygonDoc::D_Sea_kmIDWM(double lat1_deg,double lon1_deg,double lat2_deg,double lon2_deg,
									   double *D_SeaW_km)  
{
	float RLON1 = (float)(lon1_deg*pi/180.f), RLAT1 = (float)(lat1_deg*pi/180.f),
		  RLON2 = (float)(lon2_deg*pi/180.f), RLAT2 = (float)(lat2_deg*pi/180.f); 

	float RDIST, RAZIM;
	GEOPPDA( &RLON1, &RLAT1, &RLON2, &RLAT2, &RDIST, &RAZIM);

	long int N=4, IER;	GEOGCMS ( &N, &IER );

	long IVAL = 2 , IRANGE = 0,  NRPNT = 2;
	float  ARRAY[2][2]={RLON1, RLAT1, RLON2, RLAT2},  WND[2][2];
	GEOWND( (float *)ARRAY, &NRPNT, (float *)WND);
	GEOGCM( &IVAL, (float *)WND, &IRANGE);

	float RIDIST[200], CROSVEK[200][2], CNDVEK[200];
	long  MAXCROS=200, CROSS, IREST;
	GEOGCC3( &RLON1, &RLAT1, &RDIST, &RAZIM, &MAXCROS, (float *)CROSVEK, RIDIST, 
                     (float *)CNDVEK, &CROSS, &IREST);
	int i;
	float PointVEK[200][2];
	PointVEK[0][0] = RLON1;
	PointVEK[0][1] = RLAT1;
	for (i=1;i<=CROSS;i++)
	{
		PointVEK[i][0] = CROSVEK[i-1][0];
		PointVEK[i][1] = CROSVEK[i-1][1];
	}
	PointVEK[i][0] = RLON2;
	PointVEK[i][1] = RLAT2;

	double latCross,lonCross, Dsea = 0, DseaWarm = 0, DseaCold = 0;
	float dst2p, latCross1,lonCross1, loni0,lati0,loni1,lati1;
	int flag1, flag2, LandSea = 1;

//		if(CNDVEK[i]==1) //"S";  //"Sea";
//		if(CNDVEK[i]==3) //"CL"; //"Coastal Land";
//		if(CNDVEK[i]==4) //"L";   //"Inland Land";

	for(i=0;i<CROSS;i++)
	{
		if(CNDVEK[i]==1)  //"Sea";
		{
			loni0 = PointVEK[i][0];
			lati0 = PointVEK[i][1];
			loni1 = PointVEK[i+1][0];
			lati1 = PointVEK[i+1][1];

			GEODSTR( &loni0, &lati0, &loni1, &lati1, &dst2p);
			Dsea = Dsea + dst2p;

			flag1 = WarmColdSea(loni0*180.0/pi, lati0*180.0/pi);
			flag2 = WarmColdSea(loni1*180.0/pi, lati1*180.0/pi);
			if (flag1!=flag2)
			{
				WarmColdCross(loni0*180.0/pi, lati0*180.0/pi, loni1*180.0/pi, lati1*180.0/pi,
																		&lonCross, &latCross);
				lonCross1 = lonCross*pi/180.0;
				latCross1 = latCross*pi/180.0;
				if(flag1)
					GEODSTR( &loni0, &lati0, &lonCross1,&latCross1, &dst2p);
				else
					GEODSTR( &loni1, &lati1, &lonCross1,&latCross1, &dst2p);
				DseaWarm = DseaWarm + dst2p;
			}
			else
			{
				if(flag1)
					DseaWarm = Dsea;
				else
					DseaWarm = 0.0;
			}
		}
		*D_SeaW_km = DseaWarm;
	}
	return Dsea;
}



void CPro_P1546_PolygonDoc::WarmColdCross(double RLON_deg1,double RLAT_deg1,double RLON_deg2,double RLAT_deg2,
						  double *RLON_degcross,double *RLAT_degcross) 
{
	double lon1,lon2,lat1,lat2,lon1p,lon2p,lat1p,lat2p;
	lon1=min(RLON_deg1,RLON_deg2);
	lon2=max(RLON_deg1,RLON_deg2);
	lat1=min(RLAT_deg1,RLAT_deg2);
	lat2=max(RLAT_deg1,RLAT_deg2);
	lon1p = lon1;
	lon2p = lon2;
	lat1p = lat1;
	lat2p = lat2;
	if ((lat1<30.0)&&(lat2>30.0))
	{
		lon1p = max((lon1-1.0),-180.0);
		lon2p = min((lon2+1.0), 180.0);
		lat1p = 30.0;
		lat2p = 30.0;
	}
	else if ((lon1<-5.6)&&(lon2>-5.6))
	{
		lon1p = -5.6;
		lon2p = -5.6;
		lat1p = max((lat1-1.0),-90.0);
		lat2p = min((lat2+1.0), 90.0);
	}
	double pi = 4.0*atan(1.0);
	float RLON1 = (float)(RLON_deg1*pi/180.0),
		  RLAT1 = (float)(RLAT_deg1*pi/180.0),
		  RLON2 = (float)(RLON_deg2*pi/180.0),
		  RLAT2 = (float)(RLAT_deg2*pi/180.0);

	float RLON10 = (float)(lon1p*pi/180.0),
		  RLAT10 = (float)(lat1p*pi/180.0),
		  RLON20 = (float)(lon2p*pi/180.0),
		  RLAT20 = (float)(lat2p*pi/180.0);

	float CRDLN1[2][2] = {RLON1, RLAT1, RLON2, RLAT2};
	float CRDLN2[2][2] = {RLON10, RLAT10, RLON20, RLAT20};
	float CROSVEK[1][2];

	long NRPNT1 = 2, NRPNT2 = 2, C180=0, IEND=0, MAXCROS=100, CROSS, IREST;
	GEOC2L( (float *)CRDLN1, &NRPNT1, (float *)CRDLN2, &NRPNT2,
						&C180, &IEND, (float *)CROSVEK,  &MAXCROS, &CROSS, &IREST);
	if(CROSS>0)
	{
		*RLON_degcross = CROSVEK[0][0]*180.0/pi;
		*RLAT_degcross = CROSVEK[0][1]*180.0/pi;
	}
}


double CPro_P1546_PolygonDoc::LandWarmColdSea_D_Sea_kmIDWM(double lat1_deg,double lon1_deg,double lat2_deg,double lon2_deg,
									   double *D_SeaW_km, int *p2) 
{
	float RLON1 = (float)(lon1_deg*D2R), RLAT1 = (float)(lat1_deg*D2R),
		  RLON2 = (float)(lon2_deg*D2R), RLAT2 = (float)(lat2_deg*D2R); 

	float RDIST, RAZIM, GC;
	GEOPPDA( &RLON1, &RLAT1, &RLON2, &RLAT2, &RDIST, &RAZIM);

//	long int N=4, IER;	GEOGCMS ( &N, &IER );

//	long IVAL = 2 , IRANGE = 0,  NRPNT = 2;
//	float  ARRAY[2][2]={RLON1, RLAT1, RLON2, RLAT2},  WND[2][2], GC;
//	GEOWND( (float *)ARRAY, &NRPNT, (float *)WND);
//	GEOGCM( &IVAL, (float *)WND, &IRANGE);

////////////////////
	int WCSea = WarmColdSea(lon2_deg, lat2_deg);
	GEOGCP( &RLON2, &RLAT2, &GC);
	*p2 = 1;
	if		((GC==1)&&(WCSea==0)) *p2=2;	//LandSea = "Coastal Zone : Cold Sea";
	else if ((GC==1)&&(WCSea==1)) *p2=3;	//LandSea = "Coastal Zone : Warm Sea";
//	else if (GC==3)				  *p2=1;	//LandSea = "Coastal Zone : Coastal Land";
//	else if (GC==4)			 	  *p2=1;	//LandSea = "Coastal Zone : Land";
/////////////////////

	float RIDIST[200], CROSVEK[200][2], CNDVEK[200];
	long  MAXCROS=200, CROSS, IREST;
	GEOGCC3( &RLON1, &RLAT1, &RDIST, &RAZIM, &MAXCROS, (float *)CROSVEK, RIDIST, 
                     (float *)CNDVEK, &CROSS, &IREST);
	int i;
	float PointVEK[200][2];
	PointVEK[0][0] = RLON1;
	PointVEK[0][1] = RLAT1;
	for (i=1;i<=CROSS;i++)
	{
		PointVEK[i][0] = CROSVEK[i-1][0];
		PointVEK[i][1] = CROSVEK[i-1][1];
	}
	PointVEK[i][0] = RLON2;
	PointVEK[i][1] = RLAT2;

	double latCross,lonCross, Dsea = 0, DseaWarm = 0, DseaCold = 0;
	float dst2p, latCross1,lonCross1, loni0,lati0,loni1,lati1;
	int flag1, flag2, LandSea = 1;

//	if(CNDVEK[i]==1) //"S";  //"Sea";
//	if(CNDVEK[i]==3) //"CL"; //"Coastal Land";
//	if(CNDVEK[i]==4) //"L";  //"Inland Land";

	for(i=0;i<CROSS;i++)
	{
		if(CNDVEK[i]==1)  //"Sea";
		{
			loni0 = PointVEK[i][0];
			lati0 = PointVEK[i][1];
			loni1 = PointVEK[i+1][0];
			lati1 = PointVEK[i+1][1];

			GEODSTR( &loni0, &lati0, &loni1, &lati1, &dst2p);
			Dsea = Dsea + dst2p;

			flag1 = WarmColdSea(loni0*R2D, lati0*R2D);
			flag2 = WarmColdSea(loni1*R2D, lati1*R2D);
			if (flag1!=flag2)
			{
				WarmColdCross(loni0*R2D, lati0*R2D, loni1*R2D, lati1*R2D, &lonCross, &latCross);
				lonCross1 = lonCross*D2R;
				latCross1 = latCross*D2R;
				if(flag1)		GEODSTR( &loni0, &lati0, &lonCross1,&latCross1, &dst2p);
				else			GEODSTR( &loni1, &lati1, &lonCross1,&latCross1, &dst2p);
				DseaWarm = DseaWarm + dst2p;
			}
			else
			{
				if(flag1)	DseaWarm = Dsea;
				else		DseaWarm = 0.0;
			}
		}
		*D_SeaW_km = DseaWarm;
	}
	return Dsea;
}

void CPro_P1546_PolygonDoc::LoadIDWMmap() 
{
	double minLat = Linelat0,	 maxLat = Linelat0+((Height-1)/m_ZoomFactor)*m_Resolution_x;
	double minLon = Linelon0,	 maxLon = Linelon0+((Width- 1)/m_ZoomFactor)*m_Resolution_x;
	float RLON1 = (float)(minLon*D2R-pi/90.0), RLAT1 = (float)(minLat*D2R-pi/90.0),	  RLON2 = (float)(maxLon*D2R+pi/90.0), RLAT2 = (float)(maxLat*D2R+pi/90.0); 

	long N = 4, IER;	GEOGCMS (&N, &IER );

	long IVAL = 2 , IRANGE = 0,  NRPNT = 2;
	float  ARRAY[2][2]={RLON1, RLAT1, RLON2, RLAT2},  WND[2][2];
	GEOWND( (float *)ARRAY, &NRPNT, (float *)WND);
	GEOGCM( &IVAL, (float *)WND, &IRANGE);
}

