
// FindDpiDlg.h : 头文件
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

// CFindDpiDlg 对话框
class CFindDpiDlg : public CDialogEx
{
// 构造
public:
	// 对话框数据
	enum { IDD = IDD_FINDDPI_DIALOG };

	CFindDpiDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CFindDpiDlg();

	// 根据打印精度和打印尺寸查找绑定的材料ID，iXdpi:横向精度  iYdpi:纵向精度  iWidth:打印宽度  iHeight:打印高度  Add By Huangfei  On 2021/11/1 11:06:43
	int GetBandMaterialID(int iXdpi, int iYdpi, int iWidth, int iHeight);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
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
	// 保存二维数组里面的数据到当前目录文件夹“Data”下的配置文件里
	void _SaveBandMaterialData(void);
	// 从当前路径下的文件夹下面的“Data”文件夹的配置文件中读取数据到二维数组里
	void _LoadBandMaterialData(void);
	// 获取当前路径
	inline	CString _GetCurFolder(void);
	// 刷新表格数据
	void _RefreshSheet();
	// 自动调整表格大小
	bool _AdjustColumnWidth();
	// 初始化控件
	void _InitCtrl(void);

	void _CalCurSelRowColIndex(int& iRowIndex, int& iColIndex);

	int _GetcmbGridSel(const CString& szText);

	int _GetColIndex(int iValue);
	int _GetRowIndex(int iValue);


	SBANDMATERIALDATA m_stuBandMaterialData;
	std::vector<std::vector<SSAVEDATABASE*> >&	m_vTableDataSheet;

	// 在表格中插入宽度（插入一列）
	CButton m_btnInsertWidth;
	// 在表格中插入高度（插入一行）
	CButton m_btnInsertHeight;
	// 表格单元格的combox控件
	CComboBox m_cbSheetCell;
	// 默认ID的Comboxk控件
	CComboBox m_cbDefaultID;

	CListCtrl m_ListCtrl;

	// 默认的宽度
	int m_iDefaultWidth;
	// 默认的高度
	int m_iDefaultHeight;
	// 获取到的表格的某行行号
	int m_iCurSelRowIndex;
	// 获取到的表格的某列列号
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
