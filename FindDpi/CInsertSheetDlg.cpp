// InsertSheetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FindDpi.h"
#include "CInsertSheetDlg.h"
#include "afxdialogex.h"


// CInsertSheetDlg 对话框

IMPLEMENT_DYNAMIC(CInsertSheetDlg, CDialogEx)

CInsertSheetDlg::CInsertSheetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInsertSheetDlg::IDD, pParent)
{
	m_iRangeMin = 0;
	m_iRangeMax = 0;
}

CInsertSheetDlg::~CInsertSheetDlg()
{
}

void CInsertSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RANGE_MIN, m_iRangeMin);
	DDX_Text(pDX, IDC_EDIT_RANGE_MAX, m_iRangeMax);
}


BEGIN_MESSAGE_MAP(CInsertSheetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInsertSheetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInsertSheetDlg 消息处理程序


void CInsertSheetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	m_iRangeMax = GetDlgItemInt(IDC_EDIT_RANGE_MAX);
	m_iRangeMin = GetDlgItemInt(IDC_EDIT_RANGE_MIN);

	if(m_iRangeMin >= m_iRangeMax)
	{
		CString WaringStr;
		WaringStr.Format(_T("最大值应该比最小值大"));
		AfxMessageBox(WaringStr,MB_OK);
		return ;
	}
}


BOOL CInsertSheetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_STATIC, m_szPageText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInsertSheetDlg::SetData(int iMinValue, int iMaxValue,CString sText)
{
	m_iRangeMin = iMinValue;
	m_iRangeMax = iMaxValue;
	m_szPageText = sText;
}


void CInsertSheetDlg::GetData(int& iMinValue, int& iMaxValue)
{
	iMinValue = m_iRangeMin;
	iMaxValue = m_iRangeMax;
}
