#if !defined(AFX_EDITWND_H__859296E0_A127_424D_A9EC_B74276338F84__INCLUDED_)
#define AFX_EDITWND_H__859296E0_A127_424D_A9EC_B74276338F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditWnd window

class CEditWnd : public CEdit
{
// Construction
public:
	CEditWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditWnd)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWND_H__859296E0_A127_424D_A9EC_B74276338F84__INCLUDED_)
