/****************************** Module Header ******************************\
* Module Name:  MFCSDIApplnView.cpp
* Project:      MFCSDIAppln
* Copyright (c) Microsoft Corporation.
*
* This is Main View.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCSDIAppln.h"
#endif

#include "MFCSDIApplnDoc.h"
#include "MFCSDIApplnView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSDIApplnView

IMPLEMENT_DYNCREATE(CMFCSDIApplnView, CView)

BEGIN_MESSAGE_MAP(CMFCSDIApplnView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCSDIApplnView construction/destruction

CMFCSDIApplnView::CMFCSDIApplnView()
{
	// TODO: add construction code here

}

CMFCSDIApplnView::~CMFCSDIApplnView()
{
}

BOOL CMFCSDIApplnView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCSDIApplnView drawing

void CMFCSDIApplnView::OnDraw(CDC* pDC)
{
	CMFCSDIApplnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	pDC->FillSolidRect(clientRect, RGB(245, 248, 252));

	CFont titleFont;
	titleFont.CreatePointFont(180, _T("Segoe UI Semibold"));
	CFont sectionFont;
	sectionFont.CreatePointFont(110, _T("Segoe UI"));

	CFont* oldFont = pDC->SelectObject(&titleFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(32, 45, 64));
	pDC->TextOutW(24, 18, _T("Imaging Operations Dashboard"));

	pDC->SelectObject(&sectionFont);
	pDC->SetTextColor(RGB(64, 84, 108));
	pDC->TextOutW(26, 56, _T("Shift overview for modality rooms"));

	const int panelTop = 95;
	const int panelHeight = 110;
	const int gap = 14;
	const int panelWidth = (clientRect.Width() - 24 * 2 - gap * 2);
	const int cardWidth = panelWidth / 3;

	struct DashboardCard
	{
		LPCTSTR title;
		LPCTSTR value;
		LPCTSTR detail;
		COLORREF accent;
	};

	DashboardCard cards[3] =
	{
		{ _T("Patients in Queue"), _T("14"), _T("Median wait: 11 min"), RGB(0, 120, 212) },
		{ _T("Avg Exam Duration"), _T("22 min"), _T("Within target threshold"), RGB(16, 124, 16) },
		{ _T("Critical Follow-ups"), _T("3"), _T("2 pending physician review"), RGB(196, 43, 28) }
	};

	for (int i = 0; i < 3; ++i)
	{
		CRect cardRect(
			24 + i * cardWidth + i * gap,
			panelTop,
			24 + (i + 1) * cardWidth + i * gap,
			panelTop + panelHeight);

		pDC->FillSolidRect(cardRect, RGB(255, 255, 255));
		pDC->Draw3dRect(cardRect, RGB(220, 226, 234), RGB(220, 226, 234));

		CRect accentRect = cardRect;
		accentRect.bottom = accentRect.top + 4;
		pDC->FillSolidRect(accentRect, cards[i].accent);

		pDC->SetTextColor(RGB(63, 63, 70));
		pDC->TextOutW(cardRect.left + 10, cardRect.top + 12, cards[i].title);

		CFont metricFont;
		metricFont.CreatePointFont(160, _T("Segoe UI Semibold"));
		CFont* previous = pDC->SelectObject(&metricFont);
		pDC->SetTextColor(RGB(32, 45, 64));
		pDC->TextOutW(cardRect.left + 10, cardRect.top + 36, cards[i].value);
		pDC->SelectObject(previous);

		pDC->SetTextColor(RGB(92, 102, 115));
		pDC->TextOutW(cardRect.left + 10, cardRect.top + 74, cards[i].detail);
	}

	CRect workflowRect(24, panelTop + panelHeight + 24, clientRect.right - 24, clientRect.bottom - 28);
	pDC->FillSolidRect(workflowRect, RGB(255, 255, 255));
	pDC->Draw3dRect(workflowRect, RGB(220, 226, 234), RGB(220, 226, 234));

	pDC->SetTextColor(RGB(40, 58, 82));
	pDC->TextOutW(workflowRect.left + 12, workflowRect.top + 10, _T("Today's Throughput Actions"));

	pDC->SetTextColor(RGB(74, 85, 101));
	pDC->TextOutW(workflowRect.left + 14, workflowRect.top + 40, _T("1. Prep room 2 for contrast-enhanced exam (next slot: 09:40)."));
	pDC->TextOutW(workflowRect.left + 14, workflowRect.top + 64, _T("2. Re-route low acuity cases to room 4 to balance queue."));
	pDC->TextOutW(workflowRect.left + 14, workflowRect.top + 88, _T("3. Confirm protocol update acknowledgment for all technologists."));
	pDC->TextOutW(workflowRect.left + 14, workflowRect.top + 112, _T("4. Escalate unresolved critical findings before shift handoff."));

	pDC->SelectObject(oldFont);
}


// CMFCSDIApplnView printing

BOOL CMFCSDIApplnView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCSDIApplnView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCSDIApplnView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCSDIApplnView diagnostics

#ifdef _DEBUG
void CMFCSDIApplnView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSDIApplnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSDIApplnDoc* CMFCSDIApplnView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSDIApplnDoc)));
	return (CMFCSDIApplnDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCSDIApplnView message handlers
