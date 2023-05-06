#if !defined(AFX_PRO_P370_NETFVIEW_H__D9A3D10C_BDD5_44EC_833B_6CD9C961C9B4__INCLUDED_)
#define AFX_PRO_P370_NETFVIEW_H__D9A3D10C_BDD5_44EC_833B_6CD9C961C9B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pro_P370_NetFView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPro_P370_NetFView view

class CPro_P370_NetFView : public CView
{
protected:
	CPro_P370_NetFView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPro_P370_NetFView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPro_P370_NetFView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPro_P370_NetFView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPro_P370_NetFView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolsNormalize();
	afx_msg void OnShowLegend();
	afx_msg void OnCopper();
	afx_msg void OnUpdateCopper(CCmdUI* pCmdUI);
	afx_msg void OnGlobe();
	afx_msg void OnUpdateGlobe(CCmdUI* pCmdUI);
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnJet();
	afx_msg void OnUpdateJet(CCmdUI* pCmdUI);
	afx_msg void OnPink();
	afx_msg void OnUpdatePink(CCmdUI* pCmdUI);
	afx_msg void OnTopo0();
	afx_msg void OnUpdateTopo0(CCmdUI* pCmdUI);
	afx_msg void OnTopo1();
	afx_msg void OnUpdateTopo1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowLegend(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolsContour();
	afx_msg void OnToolsCoveragearea();
	afx_msg void OnUpdateToolsCoveragearea(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnToolsGoogle();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnFilePrintPreview();

private:
	int old_cx,old_cy;
	int new_cx,new_cy;
	void DEG2DMS(CString type,double x,int *xD,int *xM,double *xS,char *xU) ;
	double Distance_km(double lat1,double lon1,double lat2,double lon2) ;
	BOOL m_LegendFlag;
	int Contour(double y,double MinV,double MaxV,int nL);
	int m_CoveredPix;
	void LegendLOC(CDC* pDC) ;
	void AreaShow(CDC* pDC) ;
	int m_x1L,m_y1L,m_x0L,m_y0L;
	BOOL m_bLPressed;
	BOOL m_bLSelect;
	CRect m_rcPrintRect;
	void CaptureDC();
	CString _Z(CString s){return (((CSMS4DCApp *)AfxGetApp())->GetTranslatedString(_T(s)));};

	void OnDraw4(CDC* pDC);		//98/01/25
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRO_P370_NETFVIEW_H__D9A3D10C_BDD5_44EC_833B_6CD9C961C9B4__INCLUDED_)