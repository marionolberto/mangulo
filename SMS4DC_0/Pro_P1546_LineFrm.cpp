// Pro_P1546_LineFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SMS4DC.h"
#include "Pro_P1546_LineFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_LineFrm

IMPLEMENT_DYNCREATE(CPro_P1546_LineFrm, CMDIChildWnd)

CPro_P1546_LineFrm::CPro_P1546_LineFrm()
{
}

CPro_P1546_LineFrm::~CPro_P1546_LineFrm()
{
}


BEGIN_MESSAGE_MAP(CPro_P1546_LineFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPro_P1546_LineFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_LineFrm message handlers

void CPro_P1546_LineFrm::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::MoveWindow(0, 0, 870,654);
	nCmdShow = SW_SHOW   ;
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CPro_P1546_LineFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style&=~(LONG)FWS_ADDTOTITLE;    // no title
	
	return CMDIChildWnd::PreCreateWindow(cs);
}
