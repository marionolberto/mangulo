#if !defined(AFX_PRO_P370_LINEDOC_H__7671C9AD_EEED_4DE0_A8F7_A89130CC619B__INCLUDED_)
#define AFX_PRO_P370_LINEDOC_H__7671C9AD_EEED_4DE0_A8F7_A89130CC619B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pro_P370_LineDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPro_P370_LineDoc document

class CPro_P370_LineDoc : public CDocument
{
protected:
	CPro_P370_LineDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPro_P370_LineDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPro_P370_LineDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPro_P370_LineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPro_P370_LineDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int Np;
	double *m_hi;
	double *m_di;
	double *m_lati;
	double *m_loni;

	double Dmin,Dmax;

	double *Er;
	int m_ReadyDoc;
	double Linelat_ST,Linelon_ST,LinePtGt_ST,LineHagl_ST,LineAZ_ST,LineEL_ST,LineHasl_ST,LinefMHz_ST;
	CString LineName_ST,LineANT_ST;

	_int16 *bufArea;
	_int16 *bufAreaP370;
	int bufAreaW,bufAreaH;
	int Width,Height,m_ZoomFactor;
	double Linelat0,Linelon0,m_Resolution_x;
	CString TileInfo;
	double Linetime,Linelocation,Linek,LineRxH;
	BOOL LineClangle,Linelandsea;
	int Linesyst,Lineenvironment;
	_int16 m_NoData;

	double Distance_km(double lat1,double lon1,double lat2,double lon2);
	void GetData(); 
	void ReadAntennaData(CString antfile,int *f0,double *rH,double *rV);
	double Azimuth_Deg(double lat1,double lon1,double lat2,double lon2);
	int Round(double x);
	double Interp2(int *D0,double *E0,double d,int num);

	void reckon(double lat_deg,double lon_deg,double rng_km,double az_deg,double *latC_deg,double *lonC_deg) ;
	void LatLon2Point(double lat_deg,double lon_deg,CPoint *PointSt) ;
	double Points2HgAvr(CPoint point1 , CPoint point2); 
	double Point2Hg(CPoint point1) ;

	int WarmColdSea(double RLON_deg,double RLAT_deg) ;
	int LandWarmColdSea(double RLON_deg,double RLAT_deg) ;
	double D_Sea_kmIDWM(double lat1_deg,double lon1_deg,double lat2_deg,double lon2_deg,double *D_SeaW_km) ;
	void WarmColdCross(double RLON_deg1,double RLAT_deg1,double RLON_deg2,double RLAT_deg2,
							  double *RLON_degcross,double *RLAT_degcross); 

	CString _Z(CString s){return (((CSMS4DCApp *)AfxGetApp())->GetTranslatedString(_T(s)));};
int TileX;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRO_P370_LINEDOC_H__7671C9AD_EEED_4DE0_A8F7_A89130CC619B__INCLUDED_)
