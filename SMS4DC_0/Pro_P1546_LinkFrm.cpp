// Pro_P1546_LinkFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SMS4DC.h"
#include "Pro_P1546_LinkFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_LinkFrm

IMPLEMENT_DYNCREATE(CPro_P1546_LinkFrm, CMDIChildWnd)

CPro_P1546_LinkFrm::CPro_P1546_LinkFrm()
{
}

CPro_P1546_LinkFrm::~CPro_P1546_LinkFrm()
{
}


BEGIN_MESSAGE_MAP(CPro_P1546_LinkFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPro_P1546_LinkFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPro_P1546_LinkFrm message handlers

void CPro_P1546_LinkFrm::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::MoveWindow(0, 0, 960,654);
	nCmdShow = SW_SHOW   ;
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CPro_P1546_LinkFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
   if( !CMDIChildWnd::PreCreateWindow(cs) )
        return FALSE;

	cs.style &= ~(WS_THICKFRAME);
	cs.style &= ~(WS_MAXIMIZEBOX);
	cs.style |= WS_BORDER;

	cs.style&=~(LONG)FWS_ADDTOTITLE;    // no title

    return TRUE;	
//	return CMDIChildWnd::PreCreateWindow(cs);
}
