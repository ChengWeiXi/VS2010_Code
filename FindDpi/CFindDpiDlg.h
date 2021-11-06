
// FindDpiDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"

/************************************************************************************/
typedef struct SSaveDataBase
{
	CString		sDisplayText;

	void Clear()
	{
		sDisplayText = TEXT("");
	}
}SSAVEDATABASE;

typedef struct SRowColHead : public SSAVEDATABASE
{
	int			iMin;
	int			iMax;

	const bool Contain(int iMinValue, int iMaxValue) const
	{
		if (Contain(iMinValue) || Contain(iMaxValue)|| (iMinValue < iMin && iMaxValue > iMax))
		{
			return true;
		}

		return false;
	}

	const bool Contain(int iValue) const
	{
		if (iValue >= iMin && iValue < iMax)
		{
			return true;
		}

		return false;
	}

	void Clear()
	{
		iMin			= 0;
		iMax			= 0;
	}
}SROWCOLHEAD;

typedef struct SMaterialData : public SSAVEDATABASE
{
	int			iID;
	void Clear()
	{
		iID				= 0;
	}
}SMATERIALDATA;


typedef struct STableData
{
	std::vector<std::vector<SSAVEDATABASE*> >	vTableData;
	void Clear()
	{
		for (UINT i = 0; i < vTableData.size(); ++i)
		{
			for (UINT j = 0; j < vTableData[i].size(); ++j)
			{
				if (vTableData[i][j])
				{
					delete vTableData[i][j];
				}
			}

			vTableData[i].clear();
		}

		vTableData.clear();
	}
}STABLEDATA, *LPSTABLEDATA;


typedef struct SBandMaterialData
{
	STABLEDATA			stuTableData;
	SMATERIALDATA		stuDefaultMaterial;
	int					iXDpi;
	int					iYDpi;

	void Init(int iRows, int iCols)
	{
		Clear();

		std::vector<SSAVEDATABASE*> oneRow;
		for (int i = 0; i < iCols; ++i)
		{
			oneRow.push_back(NULL);
		}

		for (int j = 0; j < iRows; ++j)
		{
			stuTableData.vTableData.push_back(oneRow);
		}
	}

	void Clear()
	{
		iXDpi			= 0;
		iYDpi			= 0;
		stuDefaultMaterial.Clear();
		stuTableData.Clear();
	}
}SBANDMATERIALDATA;

/*************************************************************************************/

typedef struct SMateriLinkID
{
	enum _EMUN_ID
	{
		A1 = 0,
		B1,
		C1,
		D1
	};

	CString csMateriName;

}SMATERILINKID;

// CFindDpiDlg �Ի���
class CFindDpiDlg : public CDialogEx
{
// ����
public:
	// �Ի�������
	enum { IDD = IDD_FINDDPI_DIALOG };

	CFindDpiDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CFindDpiDlg();

	// ���ݴ�ӡ���Ⱥʹ�ӡ�ߴ���Ұ󶨵Ĳ���ID��iXdpi:���򾫶�  iYdpi:���򾫶�  iWidth:��ӡ���  iHeight:��ӡ�߶�  Add By Huangfei  On 2021/11/1 11:06:43
	int GetBandMaterialID(int iXdpi, int iYdpi, int iWidth, int iHeight);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	afx_msg void OnCbnSelchangeComboData();
	afx_msg void OnDblclkListInsertDataForm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonInsterWidth();
	afx_msg void OnBnClickedButtonInsertHeight();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnKillfocusComboData();
	DECLARE_MESSAGE_MAP()

private:
	// �����ά������������ݵ���ǰĿ¼�ļ��С�Data���µ������ļ���
	void _SaveBandMaterialData(void);
	// �ӵ�ǰ·���µ��ļ�������ġ�Data���ļ��е������ļ��ж�ȡ���ݵ���ά������
	void _LoadBandMaterialData(void);
	// ��ȡ��ǰ·��
	inline	CString _GetCurFolder(void);
	// ˢ�±������
	void _RefreshSheet();
	// �Զ���������С
	bool _AdjustColumnWidth();
	// ��ʼ���ؼ�
	void _InitCtrl(void);

	void _CalCurSelRowColIndex(int& iRowIndex, int& iColIndex);

	int _GetcmbGridSel(const CString& szText);

	int _GetColIndex(int iValue);
	int _GetRowIndex(int iValue);


	SBANDMATERIALDATA m_stuBandMaterialData;
	std::vector<std::vector<SSAVEDATABASE*> >&	m_vTableDataSheet;

	// �ڱ���в����ȣ�����һ�У�
	CButton m_btnInsertWidth;
	// �ڱ���в���߶ȣ�����һ�У�
	CButton m_btnInsertHeight;
	// ���Ԫ���combox�ؼ�
	CComboBox m_cbSheetCell;
	// Ĭ��ID��Comboxk�ؼ�
	CComboBox m_cbDefaultID;

	CListCtrl m_ListCtrl;

	// Ĭ�ϵĿ��
	int m_iDefaultWidth;
	// Ĭ�ϵĸ߶�
	int m_iDefaultHeight;
	// ��ȡ���ı���ĳ���к�
	int m_iCurSelRowIndex;
	// ��ȡ���ı���ĳ���к�
	int m_iCurSelColIndex;
};

CString CFindDpiDlg ::_GetCurFolder(void)
{
	wchar_t	szExePath[MAX_PATH+1] = {0};
	CString strCurPath;
	GetModuleFileName(NULL,szExePath,_MAX_PATH);
	strCurPath = szExePath;
	strCurPath = strCurPath.Mid(0, strCurPath.ReverseFind(_T('\\'))+1);
	return strCurPath;
}
