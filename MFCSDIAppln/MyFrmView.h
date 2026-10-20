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
#pragma once


class CMyFrmView : public CView
{
	DECLARE_DYNCREATE(CMyFrmView)

protected:
	CMyFrmView();
	virtual ~CMyFrmView();

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	enum
	{
		IDC_EXAM_TYPE_COMBO = 5001,
		IDC_WEIGHT_EDIT,
		IDC_DOSE_EDIT,
		IDC_CAPACITY_EDIT,
		IDC_CALCULATE_BUTTON,
		IDC_RESET_BUTTON,
		IDC_RESULT_EDIT
	};

	double m_patientWeightKg;
	double m_targetDosePerKg;
	int m_examTypeIndex;
	int m_scannerCapacityPerHour;
	bool m_controlsCreated;

	CStatic m_examTypeLabel;
	CComboBox m_examTypeCombo;
	CStatic m_weightLabel;
	CEdit m_weightEdit;
	CStatic m_doseLabel;
	CEdit m_doseEdit;
	CStatic m_capacityLabel;
	CEdit m_capacityEdit;
	CButton m_calculateButton;
	CButton m_resetButton;
	CEdit m_resultEdit;

	CString GetExamName() const;
	void EnsureControlsCreated();
	void LayoutControls();
	void SyncModelToControls();
	void SyncControlsToModel();
	void RecalculatePlan();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedReset();
	afx_msg void OnCbnSelchangeExamType();
};

