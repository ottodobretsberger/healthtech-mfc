/****************************** Module Header ******************************\
* Module Name:  MyEditView.cpp
* Project:      MFCSDIAppln
* Copyright (c) Microsoft Corporation.
*
* This is an EditView.
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
#include "MyEditView.h"


// CMyEditView

IMPLEMENT_DYNCREATE(CMyEditView, CEditView)

CMyEditView::CMyEditView()
	: m_isUpdatingText(false)
{

}

CMyEditView::~CMyEditView()
{
}

BEGIN_MESSAGE_MAP(CMyEditView, CEditView)
	ON_CONTROL_REFLECT(EN_CHANGE, &CMyEditView::OnEnChange)
END_MESSAGE_MAP()


// CMyEditView diagnostics

#ifdef _DEBUG
void CMyEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyEditView message handlers


void CMyEditView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	const CString reportTemplate =
		_T("Structured Imaging Report\r\n")
		_T("=========================\r\n")
		_T("Clinical Context:\r\n")
		_T("Technique:\r\n")
		_T("Findings:\r\n")
		_T("Impression:\r\n")
		_T("Recommended Follow-up:\r\n\r\n")
		_T("Completeness: 0%\r\n")
		_T("Status: Draft");

	GetEditCtrl().SetWindowTextW(reportTemplate);
	UpdateReportCompleteness();
}

void CMyEditView::UpdateReportCompleteness()
{
	CString text;
	GetEditCtrl().GetWindowTextW(text);

	int completedSections = 0;
	const CString tags[5] =
	{
		_T("Clinical Context:"),
		_T("Technique:"),
		_T("Findings:"),
		_T("Impression:"),
		_T("Recommended Follow-up:")
	};

	for (int i = 0; i < 5; ++i)
	{
		const int tagPos = text.Find(tags[i]);
		if (tagPos < 0)
		{
			continue;
		}

		const int colonPos = tagPos + tags[i].GetLength();
		int lineEnd = text.Find(_T("\n"), colonPos);
		if (lineEnd < 0)
		{
			lineEnd = text.GetLength();
		}

		CString sameLineValue = text.Mid(colonPos, lineEnd - colonPos);
		sameLineValue.Trim();
		if (!sameLineValue.IsEmpty())
		{
			completedSections++;
		}
	}

	const int percent = completedSections * 100 / 5;
	CString status = percent >= 80 ? _T("Ready for review") : _T("Draft");

	CString updatedText = text;
	const int completenessPos = updatedText.Find(_T("Completeness:"));
	if (completenessPos >= 0)
	{
		int completenessEnd = updatedText.Find(_T("\n"), completenessPos);
		if (completenessEnd < 0)
		{
			completenessEnd = updatedText.GetLength();
		}

		CString replacement;
		replacement.Format(_T("Completeness: %d%%"), percent);
		updatedText = updatedText.Left(completenessPos) + replacement + updatedText.Mid(completenessEnd);
	}

	const int statusPos = updatedText.Find(_T("Status:"));
	if (statusPos >= 0)
	{
		int statusEnd = updatedText.Find(_T("\n"), statusPos);
		if (statusEnd < 0)
		{
			statusEnd = updatedText.GetLength();
		}

		CString replacement;
		replacement.Format(_T("Status: %s"), status);
		updatedText = updatedText.Left(statusPos) + replacement + updatedText.Mid(statusEnd);
	}

	if (updatedText != text)
	{
		const int selStart = GetEditCtrl().LineIndex(GetEditCtrl().LineFromChar());
		m_isUpdatingText = true;
		GetEditCtrl().SetWindowTextW(updatedText);
		GetEditCtrl().SetSel(selStart, selStart);
		m_isUpdatingText = false;
	}
}

void CMyEditView::OnEnChange()
{
	if (m_isUpdatingText)
	{
		return;
	}

	UpdateReportCompleteness();
}
