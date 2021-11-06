#pragma once


// CInsertSheetDlg �Ի���

class CInsertSheetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInsertSheetDlg)

public:
	CInsertSheetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInsertSheetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INSERT_INTO_SHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


private:
	int m_iRangeMin;
	int m_iRangeMax;
	CString m_szPageText;
public:
	void SetData(int iMinValue, int iMaxValue,CString sText);
	void GetData(int& iMinValue, int& iMaxValue);
};
