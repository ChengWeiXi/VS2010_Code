// InsertSheetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FindDpi.h"
#include "CInsertSheetDlg.h"
#include "afxdialogex.h"


// CInsertSheetDlg �Ի���

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


// CInsertSheetDlg ��Ϣ�������


void CInsertSheetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();

	m_iRangeMax = GetDlgItemInt(IDC_EDIT_RANGE_MAX);
	m_iRangeMin = GetDlgItemInt(IDC_EDIT_RANGE_MIN);

	if(m_iRangeMin >= m_iRangeMax)
	{
		CString WaringStr;
		WaringStr.Format(_T("���ֵӦ�ñ���Сֵ��"));
		AfxMessageBox(WaringStr,MB_OK);
		return ;
	}
}


BOOL CInsertSheetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_STATIC, m_szPageText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
