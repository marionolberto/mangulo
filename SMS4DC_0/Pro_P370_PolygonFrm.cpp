// Pro_P370_PolygonFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SMS4DC.h"
#include "Pro_P370_PolygonFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPro_P370_PolygonFrm

IMPLEMENT_DYNCREATE(CPro_P370_PolygonFrm, CMDIChildWnd)

CPro_P370_PolygonFrm::CPro_P370_PolygonFrm()
{
}

CPro_P370_PolygonFrm::~CPro_P370_PolygonFrm()
{
}


BEGIN_MESSAGE_MAP(CPro_P370_PolygonFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPro_P370_PolygonFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPro_P370_PolygonFrm message handlers

void CPro_P370_PolygonFrm::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::MoveWindow(0, 0, 870,654);
	nCmdShow = SW_SHOW   ;
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CPro_P370_PolygonFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style&=~(LONG)FWS_ADDTOTITLE;    // no title
	
	return CMDIChildWnd::PreCreateWindow(cs);
}
