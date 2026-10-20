/****************************** Module Header ******************************\
* Module Name:  MyFrmView.h
* Project:      MFCSDIAppln
* Copyright (c) Microsoft Corporation.
*
* This is a planning view.
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
#include "MFCSDIAppln.h"
#include "MyFrmView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMyFrmView, CView)

CMyFrmView::CMyFrmView()
	: m_patientWeightKg(75.0)
	, m_targetDosePerKg(1.2)
	, m_examTypeIndex(0)
	, m_scannerCapacityPerHour(3)
	, m_controlsCreated(false)
{
}

CMyFrmView::~CMyFrmView()
{
}

BEGIN_MESSAGE_MAP(CMyFrmView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CALCULATE_BUTTON, &CMyFrmView::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CMyFrmView::OnBnClickedReset)
	ON_CBN_SELCHANGE(IDC_EXAM_TYPE_COMBO, &CMyFrmView::OnCbnSelchangeExamType)
END_MESSAGE_MAP()

BOOL CMyFrmView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CMyFrmView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	EnsureControlsCreated();
	LayoutControls();
	SyncModelToControls();
	RecalculatePlan();
}

CString CMyFrmView::GetExamName() const
{
	switch (m_examTypeIndex)
	{
	case 0:
		return _T("General XRay");
	case 1:
		return _T("Cardiac CT");
	case 2:
		return _T("Chest MRI");
	default:
		return _T("General XRay");
	}
}

void CMyFrmView::EnsureControlsCreated()
{
	if (m_controlsCreated || !::IsWindow(GetSafeHwnd()))
	{
		return;
	}

	m_examTypeLabel.Create(_T("Exam Type"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_examTypeCombo.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_EXAM_TYPE_COMBO);
	m_examTypeCombo.AddString(_T("General XRay"));
	m_examTypeCombo.AddString(_T("Cardiac CT"));
	m_examTypeCombo.AddString(_T("Chest MRI"));

	m_weightLabel.Create(_T("Patient Weight (kg)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_weightEdit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_WEIGHT_EDIT);

	m_doseLabel.Create(_T("Target Dose (mL/kg)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_doseEdit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_DOSE_EDIT);

	m_capacityLabel.Create(_T("Scanner Capacity (patients/hr)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_capacityEdit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_CAPACITY_EDIT);

	m_calculateButton.Create(_T("Calculate"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_CALCULATE_BUTTON);
	m_resetButton.Create(_T("Reset"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_RESET_BUTTON);

	m_resultEdit.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
		CRect(0, 0, 0, 0),
		this,
		IDC_RESULT_EDIT);

	m_controlsCreated = true;
}

void CMyFrmView::LayoutControls()
{
	if (!m_controlsCreated)
	{
		return;
	}

	CRect rc;
	GetClientRect(&rc);

	const int margin = 24;
	const int labelWidth = 180;
	const int inputWidth = 180;
	const int rowHeight = 22;
	const int rowGap = 10;
	const int yStart = 86;

	int y = yStart;
	m_examTypeLabel.MoveWindow(margin, y + 4, labelWidth, rowHeight);
	m_examTypeCombo.MoveWindow(margin + labelWidth + 10, y, inputWidth, 300);
	y += rowHeight + rowGap;

	m_weightLabel.MoveWindow(margin, y + 4, labelWidth, rowHeight);
	m_weightEdit.MoveWindow(margin + labelWidth + 10, y, inputWidth, rowHeight);
	y += rowHeight + rowGap;

	m_doseLabel.MoveWindow(margin, y + 4, labelWidth, rowHeight);
	m_doseEdit.MoveWindow(margin + labelWidth + 10, y, inputWidth, rowHeight);
	y += rowHeight + rowGap;

	m_capacityLabel.MoveWindow(margin, y + 4, labelWidth, rowHeight);
	m_capacityEdit.MoveWindow(margin + labelWidth + 10, y, inputWidth, rowHeight);
	y += rowHeight + rowGap + 6;

	m_calculateButton.MoveWindow(margin, y, 100, 26);
	m_resetButton.MoveWindow(margin + 110, y, 100, 26);
	y += 36;

	m_resultEdit.MoveWindow(margin, y, max(200, rc.Width() - 2 * margin), max(120, rc.Height() - y - margin));
}

void CMyFrmView::SyncModelToControls()
{
	if (!m_controlsCreated)
	{
		return;
	}

	CString text;
	text.Format(_T("%.1f"), m_patientWeightKg);
	m_weightEdit.SetWindowTextW(text);

	text.Format(_T("%.2f"), m_targetDosePerKg);
	m_doseEdit.SetWindowTextW(text);

	text.Format(_T("%d"), m_scannerCapacityPerHour);
	m_capacityEdit.SetWindowTextW(text);

	m_examTypeCombo.SetCurSel(m_examTypeIndex);
}

void CMyFrmView::SyncControlsToModel()
{
	if (!m_controlsCreated)
	{
		return;
	}

	CString text;

	m_weightEdit.GetWindowTextW(text);
	m_patientWeightKg = _tstof(text);
	if (m_patientWeightKg < 10.0) m_patientWeightKg = 10.0;
	if (m_patientWeightKg > 300.0) m_patientWeightKg = 300.0;

	m_doseEdit.GetWindowTextW(text);
	m_targetDosePerKg = _tstof(text);
	if (m_targetDosePerKg < 0.1) m_targetDosePerKg = 0.1;
	if (m_targetDosePerKg > 5.0) m_targetDosePerKg = 5.0;

	m_capacityEdit.GetWindowTextW(text);
	m_scannerCapacityPerHour = _ttoi(text);
	if (m_scannerCapacityPerHour < 1) m_scannerCapacityPerHour = 1;
	if (m_scannerCapacityPerHour > 12) m_scannerCapacityPerHour = 12;

	const int sel = m_examTypeCombo.GetCurSel();
	if (sel >= 0)
	{
		m_examTypeIndex = sel;
	}

	SyncModelToControls();
}

void CMyFrmView::RecalculatePlan()
{
	SyncControlsToModel();

	const double contrastVolumeMl = m_patientWeightKg * m_targetDosePerKg;
	const int slotMinutes = static_cast<int>(60.0 / m_scannerCapacityPerHour + 0.5);
	const double prepFactor = (m_examTypeIndex == 1) ? 1.15 : (m_examTypeIndex == 2 ? 1.10 : 1.0);
	const int prepMinutes = static_cast<int>(slotMinutes * prepFactor + 0.5);

	CString result;
	result.Format(
		_T("Protocol Plan\r\n")
		_T("------------\r\n")
		_T("Exam Type: %s\r\n")
		_T("Patient Weight: %.1f kg\r\n")
		_T("Target Dose: %.2f mL/kg\r\n")
		_T("Scanner Capacity: %d patients/hr\r\n\r\n")
		_T("Calculated Contrast Volume: %.1f mL\r\n")
		_T("Estimated Slot Duration: %d minutes\r\n")
		_T("Suggested Prep Buffer: %d minutes\r\n\r\n")
		_T("Operational Note: Use this estimate for scheduling triage and staffing alignment."),
		GetExamName().GetString(),
		m_patientWeightKg,
		m_targetDosePerKg,
		m_scannerCapacityPerHour,
		contrastVolumeMl,
		slotMinutes,
		prepMinutes);

	m_resultEdit.SetWindowTextW(result);
	Invalidate(FALSE);
}

void CMyFrmView::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(246, 249, 252));
	pDC->SetBkMode(TRANSPARENT);

	CFont titleFont;
	titleFont.CreatePointFont(170, _T("Segoe UI Semibold"));
	CFont subtitleFont;
	subtitleFont.CreatePointFont(95, _T("Segoe UI"));

	CFont* old = pDC->SelectObject(&titleFont);
	pDC->SetTextColor(RGB(33, 49, 70));
	pDC->TextOutW(24, 18, _T("Protocol Planning Workbench"));

	pDC->SelectObject(&subtitleFont);
	pDC->SetTextColor(RGB(70, 87, 109));
	pDC->TextOutW(24, 50, _T("Use fields below, then click Calculate. This view remains CView-based for stability."));

	pDC->SelectObject(old);
}

void CMyFrmView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CView::OnLButtonDown(nFlags, point);
}

BOOL CMyFrmView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(pt);
	SetFocus();
	m_patientWeightKg += (zDelta > 0 ? 1.0 : -1.0);
	if (m_patientWeightKg < 10.0) m_patientWeightKg = 10.0;
	if (m_patientWeightKg > 300.0) m_patientWeightKg = 300.0;
	SyncModelToControls();
	RecalculatePlan();
	return TRUE;
}

void CMyFrmView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		m_patientWeightKg += 1.0;
		break;
	case VK_DOWN:
		m_patientWeightKg -= 1.0;
		break;
	case VK_LEFT:
		m_targetDosePerKg -= 0.05;
		break;
	case VK_RIGHT:
		m_targetDosePerKg += 0.05;
		break;
	case 'E':
		m_examTypeIndex = (m_examTypeIndex + 1) % 3;
		break;
	case VK_ADD:
	case '+':
		if (m_scannerCapacityPerHour < 12) m_scannerCapacityPerHour++;
		break;
	case VK_SUBTRACT:
	case '-':
		if (m_scannerCapacityPerHour > 1) m_scannerCapacityPerHour--;
		break;
	default:
		break;
	}

	if (m_patientWeightKg < 10.0) m_patientWeightKg = 10.0;
	if (m_patientWeightKg > 300.0) m_patientWeightKg = 300.0;
	if (m_targetDosePerKg < 0.1) m_targetDosePerKg = 0.1;
	if (m_targetDosePerKg > 5.0) m_targetDosePerKg = 5.0;

	SyncModelToControls();
	RecalculatePlan();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyFrmView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	UNREFERENCED_PARAMETER(cx);
	UNREFERENCED_PARAMETER(cy);
	LayoutControls();
}

void CMyFrmView::OnBnClickedCalculate()
{
	RecalculatePlan();
}

void CMyFrmView::OnBnClickedReset()
{
	m_patientWeightKg = 75.0;
	m_targetDosePerKg = 1.2;
	m_examTypeIndex = 0;
	m_scannerCapacityPerHour = 3;
	SyncModelToControls();
	RecalculatePlan();
}

void CMyFrmView::OnCbnSelchangeExamType()
{
	RecalculatePlan();
}

#ifdef _DEBUG
void CMyFrmView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFrmView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif
