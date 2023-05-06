// Pro_HATA_AreaDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SMS4DC.h"
#include "Pro_HATA_AreaDoc.h"
#include <math.h>
#include "HATA_Functions.h"
#include "UTM.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPro_HATA_AreaDoc

IMPLEMENT_DYNCREATE(CPro_HATA_AreaDoc, CDocument)

CPro_HATA_AreaDoc::CPro_HATA_AreaDoc()
{
	colormaptype="Jet";
	m_tile.ColorMap(colormaptype);
	for (int i=0;i<256;i++)
		RGBt[i]=m_tile.RGBt[i];

	m_NoData=(_int16)(m_tile.m_NoData);

	bufMin=32767;	bufMax=-32768;
	ErMin=32767.0;	ErMax=-32768.0;

	m_ZoomFactor=1;
	m_ReadyDoc=0;
	m_level = 255;
	m_bThreshold = false;
	m_Threshold = 60.0;

	m_HATAa1 = 69.55;
	m_HATAa2 = 26.16;
	m_HATAa3 = -13.82;
	m_HATAb1 = 44.9;
	m_HATAb2 = -6.55;
}

BOOL CPro_HATA_AreaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPro_HATA_AreaDoc::~CPro_HATA_AreaDoc()
{
	delete [] bufAreaHATA;
	delete [] bufArea;
	delete [] Er;
}


BEGIN_MESSAGE_MAP(CPro_HATA_AreaDoc, CDocument)
	//{{AFX_MSG_MAP(CPro_HATA_AreaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPro_HATA_AreaDoc diagnostics

#ifdef _DEBUG
void CPro_HATA_AreaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPro_HATA_AreaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPro_HATA_AreaDoc serialization

void CPro_HATA_AreaDoc::Serialize(CArchive& ar)
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
// CPro_HATA_AreaDoc commands

void CPro_HATA_AreaDoc::GetData()
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

HATAlon_ST = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(HATAlon_ST,TileX);
HATAlon0   = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(HATAlon0,  TileX);
HATAlonmax = ((CSMS4DCApp *)AfxGetApp())->ChangeLon(HATAlonmax,TileX);	//97/12/21

	Width  = m_ZoomFactor * bufAreaW;
	Height = m_ZoomFactor * bufAreaH;
	bufArea = new _int16[Width*Height];

	for(int i=0;i<Width;i++)
		for(int j=0;j<Height;j++)
		{
			bufArea[i+Width*j]=bufAreaHATA[(i/m_ZoomFactor)+(Width/m_ZoomFactor)*(j/m_ZoomFactor)];
			bufMin=min(bufMin,bufArea[i+Width*j]);
			bufMax=max(bufMax,bufArea[i+Width*j]);
		}

	Er=new double[Width*Height];

	double lat_j,lon_i;

	double pi=4.0*atan(1.0)  ,  r=6371.0 , Ghv;

	CPoint m_Point_ST,m_Point_STcr3,m_Point_STcr15,pointi;
	double lat3km,lon3km,lat15km,lon15km,lat015km,lon015km;

	double az,tetamax15=0.0,ttca=0.0,tetai;
	double dist,ds,  h1,hi;
	double Eb;

	LatLon2Point(HATAlat_ST,HATAlon_ST, &m_Point_ST) ;
	double Hg=Point2Hg(m_Point_ST);

/*
	double StepRes=1.0/m_ZoomFactor;
	CString globeTileInfo = ((CSMS4DCApp *)AfxGetApp())->m_AppPath + _T("Texts\\globe.txt");
	if (TileInfo != globeTileInfo)
		StepRes=(m_Resolution_x/1000.0)/m_ZoomFactor;
//		StepRes=(0.5/1000.0)/m_ZoomFactor;
*/
//	double StepRes = 0.5*Distance_km(HATAlat0, HATAlon0, HATAlat0+m_Resolution_x, HATAlon0+m_Resolution_x)/m_ZoomFactor;

double disttest1 = Distance_km(HATAlat_ST,HATAlon_ST, HATAlat_ST+0, HATAlon_ST+m_Resolution_x); 
double disttest2 = Distance_km(HATAlat_ST,HATAlon_ST, HATAlat_ST+m_Resolution_x, HATAlon_ST+0); 
double StepRes = min(disttest1, disttest2)/m_ZoomFactor;

	double mds;
	CHATA_Functions CHATA;

	double Hr = HATARxH ;
	double re = HATAk*r*1000.0;
	double AZ0 = HATAAZ_ST*pi/180.0 ,	EL0 = HATAEL_ST*pi/180.0  ,  Ht = HATAHagl_ST;
	double Hgi;
	double elevp,tp,fp,f,t;

	CString antfile;
	antfile.Format("%sAntenna\\ant_%s.ant",((CSMS4DCApp *)AfxGetApp())->m_AppPath ,HATAANT_ST);
	
	int f0[360];	double rH[360]  ,  rV[360];
	ReadAntennaData(antfile,f0,rH,rV) ;

	CProgressBar bar(_T("Progress"), 40, Width*Height,FALSE,0);
	int progress_num=0;
	CString progress_str;
	bar.SetText("  ");
	bar.SetBarColour(RGB(127,0,0));
	//bar.SetBkColour(RGB(255,0,0));

	for( i=0;i<Width;i++)
		for(int j=0;j<Height;j++)
		{
			lat_j = HATAlat0+(j/m_ZoomFactor)*m_Resolution_x;
			lon_i = HATAlon0+(i/m_ZoomFactor)*m_Resolution_x;
			CString globeTileInfo = ((CSMS4DCApp *)AfxGetApp())->m_AppPath + _T("Texts\\globe.txt");
			if (TileInfo != globeTileInfo)
			{
				CUtm m_utm;
				m_utm.y = lat_j;
				m_utm.x = lon_i;
				m_utm.UTM2philambda();
				lat_j=m_utm.phi;
				lon_i=m_utm.lambda;
			}

			dist=Distance_km(HATAlat_ST,HATAlon_ST,lat_j,lon_i);
	//		dist = max(dist,0.00001);			//98/01/20
			dist = max(dist,StepRes/2.0);		//98/01/20

			az=Azimuth_Deg(HATAlat_ST,HATAlon_ST,lat_j,lon_i);

			/////////////////////////////// h1 (Heff) ///////////////////////////////////////////////
			if (dist>=15.0)
			{
				reckon( HATAlat_ST,HATAlon_ST,  3.0 , az , &lat3km  , &lon3km) ;
				reckon( HATAlat_ST,HATAlon_ST, 15.0 , az , &lat15km , &lon15km) ;

				LatLon2Point(lat3km  , lon3km , &m_Point_STcr3) ;
				LatLon2Point(lat15km , lon15km, &m_Point_STcr15) ;

				h1=Hg+HATAHagl_ST - Points2HgAvr(m_Point_STcr3 , m_Point_STcr15) ;
			}
			else
			{
				reckon( HATAlat_ST,HATAlon_ST, 0.2*dist , az , &lat3km  , &lon3km) ;
				reckon( HATAlat_ST,HATAlon_ST,     dist , az , &lat15km , &lon15km) ;

				LatLon2Point(lat3km  , lon3km , &m_Point_STcr3) ;
				LatLon2Point(lat15km , lon15km, &m_Point_STcr15) ;

				h1=Hg+HATAHagl_ST - Points2HgAvr(m_Point_STcr3 , m_Point_STcr15) ;
			}
			///////////////////////////////// tetamax15 /////////////////////////////////////////////
			if (h1<0.0)
			{
				tetai=-pi/2.0;
				mds=m_ZoomFactor*min(dist,15.0);
				ds=0.0;
				while( ds<= mds )  //ds=0->15
				{
					ds=ds+StepRes;
					reckon( HATAlat_ST,HATAlon_ST, ds , az , &lat015km , &lon015km) ;
					LatLon2Point(lat015km , lon015km, &pointi) ;
					hi=Point2Hg(pointi);
					tetai=max(tetai   ,(((hi-Hg-HATAHagl_ST)/(ds*1000))-(ds/(2.0*HATAk*r))));
				}
				tetamax15=-(180.0/pi)*tetai;
			}
			Eb = CHATA.E_HATA(m_HATAa1,m_HATAa2,m_HATAa3,m_HATAb1,m_HATAb2,
						HATAfMHz_ST,h1,dist,HATARxH,HATAenvironment,tetamax15);
			//Eb=min(Eb,Ebfs);

           //0000000000000000000000000000000000000000000000000000000000000000
            //                     Antenna    Effect
            //0000000000000000000000000000000000000000000000000000000000000000
			LatLon2Point(lat_j,lon_i, &pointi) ;
			Hgi=Point2Hg(pointi);
									//with Terrain
			elevp = atan(((re+Hr+Hgi)*cos(1000.0*dist/re)-(re+Ht+Hg))/((re+Hr+Hgi)*sin(1000.0*dist/re)));
			dist = sqrt( pow(re+Ht+Hg,2.0) + pow(re+Hr+Hgi,2.0) - 2.0*(re+Ht+Hg)*(re+Hr+Hgi)*cos(1000.0*dist/re) );
									//without Terrain
			//elevp = atan(((re+Hr)*cos((dist*1000.0)/re)-(re+Ht+Hg))/((re+Hr)*sin((dist*1000.0)/re)));
			//dist = sqrt( pow(re+Ht+Hg,2.0) + pow(re+Hr,2.0) -2.0*(re+Ht+Hg)*(re+Hr)*cos(1000.0*dist/re) );

			dist=dist/1000.0;	fp=az*pi/180.0;		tp = (pi/2.0) - elevp;

			f = (180.0/pi)*atan2(sin(tp)*sin(fp-AZ0),(cos(EL0)*sin(tp)*cos(fp-AZ0)+cos(tp)*sin(EL0)));
			t = (180.0/pi)*acos(cos(tp)*cos(EL0)-sin(EL0)*sin(tp)*cos(fp-AZ0));

			if (f<0.0)		f=f+359.4;

			Ghv = Interp2(f0,rH,f,360) * Interp2(f0,rV,t,360);
			//Ghv = rH[Round(f)] * rV[Round(t)];
            //0000000000000000000000000000000000000000000000000000000000000000

			Eb=Eb+10.0*log10(HATAPtGt_ST*Ghv/1650.0);
			//Ebfs=Ebfs+10.0*log10(PtGt*Ghv/1650.0);

			Er[i+Width*j]=Eb;
			ErMax = max(ErMax , Eb );
			ErMin = min(ErMin , Eb );
			////////////////////progress bar////////////////////
			if(progress_num%100==0)
			{
				progress_str.Format("%02d%%"+_Z(" complete"), progress_num*100/(Width*Height));
				bar.SetText(progress_str);
				bar.SetPos(progress_num);
				//bar.StepIt();
				PeekAndPump();
			}
			progress_num++;
			////////////////////progress bar////////////////////
		}
	m_MinValue = (double)Round(ErMin)+1.0;	//98/01/19
	m_MaxValue = (double)Round(ErMax)-1.0;
//	m_MinValue = ErMin;
//	m_MaxValue = ErMax;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}
//98/01/19
int CPro_HATA_AreaDoc::Round(double x) 
{
	int y;
	int x1 = (int)x;
	if(x>=0)	{int x2 = x1+1;		y = ( (x > ((double)x1)+0.45) ? x2 : x1);}
	else		{int x2 = x1-1;		y = ( (x > ((double)x2)+0.45) ? x1 : x2);}
	return y;
}
/*
int CPro_HATA_AreaDoc::Round(double x) 
{
	int x1 = (int) x;
	int x2 = x1+1;
	return ( (x > ((double)x1)+0.45) ? x2 : x1);
}
*/

double CPro_HATA_AreaDoc::Distance_km(double lat1,double lon1,double lat2,double lon2) 
{
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

}
double CPro_HATA_AreaDoc::Azimuth_Deg(double lat1,double lon1,double lat2,double lon2) 
{
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
}

void CPro_HATA_AreaDoc::LatLon2Point(double lat_deg,double lon_deg,CPoint *PointSt) 
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
	double ix = (double)(m_ZoomFactor)*((lon_deg - HATAlon0)/(m_Resolution_x));
	double jy = (double)(m_ZoomFactor)*((lat_deg - HATAlat0)/(m_Resolution_x));

	(*PointSt).x = (long)ix;
	(*PointSt).y=(long)(Height-1-jy);
}



double CPro_HATA_AreaDoc::Points2HgAvr(CPoint point1 , CPoint point2) 
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


double CPro_HATA_AreaDoc::Point2Hg(CPoint point1) 
{
	int i1=point1.x   ,   j1=(Height-1)-point1.y;
	_int16 Hg=bufArea[i1+Width*j1];
	return (double)Hg;
}

void CPro_HATA_AreaDoc::reckon(double lat_deg,double lon_deg,double rng_km,double az_deg,double *latC_deg,double *lonC_deg) 
{
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
}


double CPro_HATA_AreaDoc::Interp2(int *D0,double *E0,double d,int num) 
{
	double d1,d2,E1,E2,Eb;
	if (d<D0[0])
	{
		d1=D0[0];d2=D0[1];
		E1=E0[0];E2=E0[1];
	}
	else if (d>D0[num-1])
	{
		d1=D0[num-2];d2=D0[num-1];
		E1=E0[num-2];E2=E0[num-1];
	}
	else
	{
		/////////////////////////////////////////////
		int n1=0;
		while(d>=D0[n1])
		{
			n1++;
			if (n1==num)
				break;
		}
		if (n1==0)
			n1=1;
		d1=D0[n1-1];E1=E0[n1-1];
		///////////////////////////////////////////////
		int n2=num-1;
		while(d<=D0[n2])
			n2--;
		d2=D0[n2+1];E2=E0[n2+1];
		////////////////////////////////////////////////
	}
	Eb=((d1==d2) ? E1 : E1+(E2-E1)*(d-d1)/(d2-d1));

	return Eb;

}


void CPro_HATA_AreaDoc::ReadAntennaData(CString antfile,int *f0,double *rH,double *rV) 
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

BOOL CPro_HATA_AreaDoc::PeekAndPump()
{
	static MSG msg;

	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			return FALSE;
		}	
	}
	return TRUE;
}