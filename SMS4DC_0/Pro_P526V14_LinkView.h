#if !defined(AFX_PRO_P526V14_LINKVIEW_H__31A567F7_B275_4473_A319_684E5A0F81BF__INCLUDED_)
#define AFX_PRO_P526V14_LINKVIEW_H__31A567F7_B275_4473_A319_684E5A0F81BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pro_P526V14_LinkView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPro_P526V14_LinkView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Pro_P526V14_LinkDoc.h"

class CPro_P526V14_LinkView : public CFormView
{
protected:
	CPro_P526V14_LinkView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPro_P526V14_LinkView)

// Form Data
public:
	//{{AFX_DATA(CPro_P526V14_LinkView)
	enum { IDD = IDD_P526V14_FORM };
	double	m_Frequency;
	CString	m_Distance;
	CString	m_FsLoss;
	double	m_Gr;
	double	m_Gt;
	double	m_Hr;
	double	m_Ht;
	double	m_Lcr;
	double	m_Lct;
	CString	m_Lml;
	CString	m_Loss1;
	CString	m_Loss2;
	CString	m_Pb;
	CString	m_Pbw;
	CString	m_Pi;
	CString	m_Pid;
	CString	m_Po;
	CString	m_Pow;
	CString	m_Pr;
	CString	m_Prd;
	double	m_R;
	CString	m_RxName;
	CString	m_S;
	CString	m_Sw;
	CString	m_tit;
	CString	m_TxName;
	CString	m_VSWR1;
	CString	m_VSWR2;
	CString	m_VSWR2w;
	CString	m_kfactor;
	CString	m_EDIT_Lba;
	CString	m_EDIT_Lbs;
	CString	m_EDIT_Lsph;
	CString	m_thr;
	CString	m_thr2;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPro_P526V14_LinkView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPro_P526V14_LinkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPro_P526V14_LinkView)
	afx_msg void OnBUTTONLink();
	afx_msg void OnRadioRL();
	afx_msg void OnRadioLR();
	afx_msg void OnBUTTONSaveProfile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeEDITSw();
	afx_msg void OnChangeEDITVSWR2w();
	afx_msg void OnBUTTONSaveImage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnFilePrintPreview() ;

public:
	double *m_hi;
	double *m_di;
	double Hmin;
	int Np;
	void TextDraw(CDC* pDC,int x, int y,CString str,
							COLORREF FontColor=RGB(0,0,0),
							int Escapement=0,int nHeight=12,int nWidth=0,
							int FontWeight=FW_NORMAL,CString FontName="Arial") ;
	void LineDraw(CDC* pDC,int x1, int y1,int x2, int y2,
							int nWidth=1,COLORREF crColor=RGB(0,0,0), int nStyle=PS_SOLID) ;
	bool m_bLPressed;
	CPoint m_stPoint;
	int m_AxesT,m_AxesH,m_AxesW,m_AxesL;

	void Plot(CDC* pDC,double *xi, double *yi,int Np,
		double xiMIN,double xiMAX,double yiMIN,double yiMAX,
		int nWidth=1,COLORREF crColor=RGB(0,0,0), int nStyle=PS_SOLID,
		int nMode=0,COLORREF flColor=RGB(255,255,255)) ;

	double m_ytickMIN  ,   m_ytickSTEP  ,  m_ytickMAX;

	void DEG2DMS(CString type,double x,int *xD,int *xM,double *xS,char *xU);
	void YtickL(CDC* pDC,double yiMIN,double yiMAX);
	void Xtick(CDC* pDC,double xiMIN,double xiMAX);
	double MinValue(double *xi,int N); 
	double MaxValue(double *xi,int N); 
	double InterpXY(double *D0,double *E0,double d,int num);


	void DrawAntSymbol(CDC* pDC,double xi, double yi,
		double xiMIN,double xiMAX,double yiMIN,double yiMAX,
		COLORREF crColor=RGB(0,0,0), int nMode=1) ;
	int		m_Fresnel;
	void TxRxObj(CDC* pDC) ;
	void DrawRect(CDC* pDC,int x1,int y1,int x2,int y2,COLORREF flColor); 
	void DrawRoundRect(CDC* pDC,int x1,int y1,int x2,int y2,COLORREF flColor); 

	void InitForm(CPro_P526V14_LinkDoc* pDoc);
	BOOL m_InitForm;
	double m_TotalLoss;
private:
	CRect m_rcPrintRect;
	CString _Z(CString s){return (((CSMS4DCApp *)AfxGetApp())->GetTranslatedString(_T(s)));};

	CFont m_font;
	int m_AxesW1, m_AxesH1,	m_AxesT1, m_AxesL1;
	double m_Xmin, m_Xmax, m_Ymin, m_Ymax;
	double m_d0;
	bool m_bRPressed;

	int DP_LP(CDC* pDC, int nH_DP);	//97/10/12
	void initDP_LP(CDC * pDC);
	int m_fontH2, m_fontH6, m_fontH12, m_fontH14, m_fontH20, 
		m_fontH24, m_fontH28, m_fontH30, m_fontH40, m_fontH44, m_fontH100, m_fontH150;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRO_P526V14_LINKVIEW_H__31A567F7_B275_4473_A319_684E5A0F81BF__INCLUDED_)
