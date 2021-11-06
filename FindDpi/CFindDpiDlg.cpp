
// FindDpiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FindDpi.h"
#include "CFindDpiDlg.h"
#include "afxdialogex.h"
#include "CInsertSheetDlg.h"
//#include "MenuEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//头文件中用到类 File，但不需要访问 File 类的声明
//插入窗口类的前置声明，减少需要包含的头文件数量
class CInsertSheetDlg;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFindDpiDlg 对话框




CFindDpiDlg::CFindDpiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindDpiDlg::IDD, pParent)
	, m_iDefaultWidth(300)
	, m_iDefaultHeight(600)
	, m_iCurSelRowIndex(0)
	, m_iCurSelColIndex(0)
	, m_vTableDataSheet(m_stuBandMaterialData.stuTableData.vTableData)
{
	// 20211103
	// 可以在初始化列表的时候把类中定义的引用初始化

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_LoadBandMaterialData();

	//如果表格为空，则初始化表格为一行一列
	if(0 == m_vTableDataSheet.size())
	{
		std::vector<SSAVEDATABASE*> InsertOneRow;
		for(int num = 0;num < 1; ++ num)
		{
			InsertOneRow.push_back(NULL);
		}
		m_vTableDataSheet.push_back(InsertOneRow);
	}
}

CFindDpiDlg::~CFindDpiDlg()
{
	m_stuBandMaterialData.Clear();
}

void CFindDpiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_INSTER_WIDTH, m_btnInsertWidth);
	DDX_Control(pDX, IDC_BUTTON_INSERT_HEIGHT, m_btnInsertHeight);
	DDX_Control(pDX, IDC_COMBO_DATA, m_cbSheetCell);
	DDX_Control(pDX, IDC_COMBO_DEFAULT_ID, m_cbDefaultID);
	DDX_Control(pDX, IDC_LIST_INSERT_DATA_FORM, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CFindDpiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFindDpiDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA, &CFindDpiDlg::OnCbnSelchangeComboData)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INSERT_DATA_FORM, &CFindDpiDlg::OnDblclkListInsertDataForm)
	ON_BN_CLICKED(IDC_BUTTON_INSTER_WIDTH, &CFindDpiDlg::OnBnClickedButtonInsterWidth)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_HEIGHT, &CFindDpiDlg::OnBnClickedButtonInsertHeight)
	ON_BN_CLICKED(IDC_BUTTON1, &CFindDpiDlg::OnBnClickedButton1)
	ON_CBN_KILLFOCUS(IDC_COMBO_DATA, &CFindDpiDlg::OnCbnKillfocusComboData)
END_MESSAGE_MAP()


// CFindDpiDlg 消息处理程序

BOOL CFindDpiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	_InitCtrl();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFindDpiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFindDpiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFindDpiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFindDpiDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	int iSaveValueTemp;
	iSaveValueTemp = GetDlgItemInt(IDC_EDIT_DEFAULT_WIDTH);
	m_stuBandMaterialData.iXDpi = iSaveValueTemp;
	iSaveValueTemp = GetDlgItemInt(IDC_EDIT_DEFAULT_HEIGHT);
	m_stuBandMaterialData.iYDpi = iSaveValueTemp;

	int iCurSelIndex = m_cbDefaultID.GetCurSel();
	m_stuBandMaterialData.stuDefaultMaterial.iID = m_cbDefaultID.GetItemData(iCurSelIndex);
	m_cbDefaultID.GetLBText(iCurSelIndex, m_stuBandMaterialData.stuDefaultMaterial.sDisplayText);

	_SaveBandMaterialData();
}


// 根据打印精度和打印尺寸查找绑定的材料ID，iXdpi:横向精度  iYdpi:纵向精度  iWidth:打印宽度  iHeight:打印高度  Add By Huangfei  On 2021/11/1 11:06:43
int CFindDpiDlg::GetBandMaterialID(int iXdpi, int iYdpi, int iWidth, int iHeight)
{
	if (iXdpi != m_stuBandMaterialData.iXDpi || iYdpi != m_stuBandMaterialData.iYDpi)
	{
		return m_stuBandMaterialData.stuDefaultMaterial.iID;
	}

	int iRowIndex = -1, iColIndex = -1;
	iRowIndex = _GetRowIndex(iHeight);
	iColIndex = _GetColIndex(iWidth);

	if (-1 == iRowIndex || -1 == iColIndex)
	{
		return m_stuBandMaterialData.stuDefaultMaterial.iID;
	}

	const SMATERIALDATA* pMaterialData = (SMATERIALDATA*)m_stuBandMaterialData.stuTableData.vTableData[iRowIndex][iColIndex];
	if (NULL == pMaterialData)
	{
		m_stuBandMaterialData.stuDefaultMaterial.iID;
	}
	return pMaterialData->iID;
}


// 保存二维数组里面的数据到当前目录文件夹“Data”下的配置文件里
void CFindDpiDlg::_SaveBandMaterialData(void)
{
	CString	szFilename;
	szFilename.Format(_T("%sData\\BandMaterialData.ini"), _GetCurFolder());
	int iRows = m_stuBandMaterialData.stuTableData.vTableData.size();
	int iCols = -1;
	if (iRows > 0)
	{
		iCols = m_stuBandMaterialData.stuTableData.vTableData[0].size();
	}

	if (iRows <= 0 || iCols <= 0)
	{
		return;
	}

	if(PathFileExists(szFilename) == TRUE)
	{
		BOOL bRet = DeleteFile(szFilename);
		if (!bRet)
		{
			bRet = DeleteFile(szFilename);
			if (!bRet)
			{
				char szLog[256] = {0};
				sprintf_s(szLog, 256, "保存绑定材料数据时，删除配置文件:%s 失败！", szFilename);
				return;
			}
		}
	}

	CString sValue;
	sValue.Format(TEXT("%d"), iRows);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("ROWS"), sValue, szFilename);
	sValue.Format(TEXT("%d"), iCols);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("COLS"), sValue, szFilename);

	//写默认精度到配置文件
	sValue.Format(TEXT("%d"),m_stuBandMaterialData.iXDpi);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("XDPI"), sValue, szFilename);
	sValue.Format(TEXT("%d"),m_stuBandMaterialData.iYDpi);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("YDPI"), sValue, szFilename);
	sValue.Format(TEXT("%d"), m_stuBandMaterialData.stuDefaultMaterial.iID);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_ID"), sValue, szFilename);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_NAME"), m_stuBandMaterialData.stuDefaultMaterial.sDisplayText, szFilename);

	for (int i = 0; i < iRows; ++i)
	{
		for (int j = 0; j < iCols; ++j)
		{
			const SSAVEDATABASE* pSaveDataBase = m_stuBandMaterialData.stuTableData.vTableData[i][j];
			if (NULL == pSaveDataBase)
			{
				continue;
			}

			CString sKey;
			sKey.Format(TEXT("ROW_%d_COL_%d_DISPLAY"), i, j);
			::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), sKey, pSaveDataBase->sDisplayText, szFilename);
			if (0 == i || 0 == j)
			{
				const SROWCOLHEAD* pRowColHead = (SROWCOLHEAD*)pSaveDataBase;
				if (NULL == pRowColHead)
				{
					continue;
				}

				sKey.Format(TEXT("ROW_%d_COL_%d_MIN"), i, j);
				sValue.Format(TEXT("%d"), pRowColHead->iMin);
				::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), sKey, sValue, szFilename);

				sKey.Format(TEXT("ROW_%d_COL_%d_MAX"), i, j);
				sValue.Format(TEXT("%d"), pRowColHead->iMax);
				::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), sKey, sValue, szFilename);
			}
			else
			{
				const SMATERIALDATA* pMaterialData = (SMATERIALDATA*)pSaveDataBase;
				if (NULL == pMaterialData)
				{
					continue;
				}

				sKey.Format(TEXT("ROW_%d_COL_%d"), i, j);
				sValue.Format(TEXT("%d"), pMaterialData->iID);
				int iValue = ::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), sKey, sValue, szFilename);
			}
		}
	}
}


// 从当前路径下的文件夹下面的“Data”文件夹的配置文件中读取数据到二维数组里
void CFindDpiDlg::_LoadBandMaterialData(void)
{
	CString	szFilename;
	szFilename.Format(_T("%sData\\BandMaterialData.ini"), _GetCurFolder());
	int iRows = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("ROWS"), 0, szFilename);
	int iCols = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("COLS"), 0, szFilename);
	int iXdpi = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("XDPI"), 0, szFilename);
	int iYdpi = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("YDPI"), 0, szFilename);

	//20211103 新增ID,材料名
	int iID = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_ID"), 0, szFilename);
	TCHAR szValue[256] = {0};
	::GetPrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_NAME"), TEXT(""), szValue, 256, szFilename);

	m_stuBandMaterialData.Init(iRows, iCols);

	m_stuBandMaterialData.iXDpi = iXdpi;//一定要先初始化完后再赋值
	m_stuBandMaterialData.iYDpi = iYdpi;
	m_stuBandMaterialData.stuDefaultMaterial.iID = iID;

	CString sDefaultName = TEXT("");
	int iDefaultID = 0;

	//获取缺省的材料，ID和名字
	iDefaultID = iID;
	sDefaultName = _T("DEFAULTMATERTAL");

	if (1)//这里查找材料
	{
		//找不到，采用缺省材料
		m_stuBandMaterialData.stuDefaultMaterial.iID = iDefaultID;
		m_stuBandMaterialData.stuDefaultMaterial.sDisplayText = sDefaultName;
	}
	else
	{
		m_stuBandMaterialData.stuDefaultMaterial.sDisplayText = _T("FINDED");
	}

	for (int i = 0; i < iRows; ++i)
	{
		for (int j = 0; j < iCols; ++j)
		{
			CString sKey;
			sKey.Format(TEXT("ROW_%d_COL_%d_DISPLAY"), i, j);
			TCHAR szValue[256] = {0};
			::GetPrivateProfileString(TEXT("BAND_MATERIAL_DATA"), sKey, TEXT(""), szValue, 256, szFilename);
			if (0 == i || 0 == j)
			{
				sKey.Format(TEXT("ROW_%d_COL_%d_MIN"), i, j);
				int iMin = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), sKey, 0, szFilename);
				sKey.Format(TEXT("ROW_%d_COL_%d_MAX"), i, j);
				int iMax = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), sKey, 0, szFilename);
				SROWCOLHEAD* pRowCol = new SROWCOLHEAD;
				pRowCol->sDisplayText = szValue;
				pRowCol->iMin = iMin;
				pRowCol->iMax = iMax;
				m_stuBandMaterialData.stuTableData.vTableData[i][j] = (SSAVEDATABASE*)pRowCol;
			}
			else
			{
				sKey.Format(TEXT("ROW_%d_COL_%d"), i, j);
				int iValue = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), sKey, 0, szFilename);
				SMATERIALDATA* pMaterialData = new SMATERIALDATA;
				pMaterialData->sDisplayText = szValue;
				pMaterialData->iID = iValue;
				m_stuBandMaterialData.stuTableData.vTableData[i][j] = (SSAVEDATABASE*)pMaterialData;
			}
		}
	}

	//20211103 记得测试一下不加这一句话
	//加载完毕后，可能参数模块中的材料有变化所以需要保存一下
	_SaveBandMaterialData();
}


int CFindDpiDlg::_GetColIndex(int iValue)
{
	int iRet = -1;
	if (m_stuBandMaterialData.stuTableData.vTableData.size() <= 0)
	{
		return iRet;
	}

	const std::vector<SSAVEDATABASE*>& vFirstRowData = m_stuBandMaterialData.stuTableData.vTableData[0];
	if (vFirstRowData.size() <= 1)
	{
		return iRet;
	}

	for (UINT i = 0; i < vFirstRowData.size(); ++i)
	{
		const SROWCOLHEAD* pRowHead = (SROWCOLHEAD*)vFirstRowData[i];
		if (NULL != pRowHead && pRowHead->Contain(iValue))
		{
			iRet = i;
			break;
		}
	}

	return iRet;
}


int CFindDpiDlg::_GetRowIndex(int iValue)
{
	int iRet = -1;
	for (UINT i = 0; i < m_stuBandMaterialData.stuTableData.vTableData.size(); ++i)
	{
		const std::vector<SSAVEDATABASE*>& oneRowData = m_stuBandMaterialData.stuTableData.vTableData[i];
		if (oneRowData.size() <= 1)
		{
			break;
		}

		const SROWCOLHEAD* pRowHead = (SROWCOLHEAD*)oneRowData[0];
		if (NULL != pRowHead && pRowHead->Contain(iValue))
		{
			iRet = i;
			break;
		}
	}

	return iRet;
}


// 刷新表格数据
void CFindDpiDlg::_RefreshSheet()
{
	//先清除，在把二维数组数据同步到表格

	while(m_ListCtrl.DeleteColumn (0))
	{
		//在这里，删除所有行
	};

	//删除所有列
	m_ListCtrl.DeleteAllItems();

	int iRow = m_vTableDataSheet.size();
	int iCol = m_vTableDataSheet[0].size();

	//要先把格式创造出来
	for(int ii = 0; ii < iRow ;  ++ii)
	{
		m_ListCtrl.InsertItem(0,_T(" "));
	}
	for(int jj = 0; jj < iCol ;++jj)
	{
		m_ListCtrl.InsertColumn(jj, _T(" "), LVCFMT_LEFT,60);
	}

	for(int i =0 ; i < iRow ; ++i)
	{
		for(int j = 0; j < iCol ; ++j)
		{
			if(m_vTableDataSheet[i][j] == NULL)
			{
				continue;
				m_ListCtrl.SetItemText(i,j,_T(" "));
			}
			else
			{
				m_ListCtrl.SetItemText(i,j,m_vTableDataSheet[i][j]->sDisplayText);
			}
		}
	}
	m_ListCtrl.SetItemText(0,0,_T("H/W"));

	_AdjustColumnWidth();
}


void CFindDpiDlg::OnCbnSelchangeComboData()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CFindDpiDlg::OnDblclkListInsertDataForm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

 	//屏蔽第一行和第一列
	_CalCurSelRowColIndex(m_iCurSelRowIndex, m_iCurSelColIndex);
	if (m_iCurSelRowIndex > 0 && m_iCurSelColIndex > 0)
	{
		CRect	rcItem, rcList, rect;
		CString szText;

		m_ListCtrl.GetWindowRect(rcList);
		ScreenToClient(rcList);

		m_ListCtrl.GetSubItemRect(m_iCurSelRowIndex, m_iCurSelColIndex, LVIR_BOUNDS, rcItem);
		rect.left	= rcList.left + rcItem.left + 2;
		rect.top	= rcList.top + rcItem.top;
		rect.right	= rect.left + rcItem.Width() - 2;
		rect.bottom = rect.top + rcItem.Height() + 50;	


		szText = m_ListCtrl.GetItemText(m_iCurSelRowIndex, 1);
		m_cbSheetCell.SetCurSel(_GetcmbGridSel(szText));

		m_cbSheetCell.MoveWindow(rect);
		m_cbSheetCell.BringWindowToTop();
		m_cbSheetCell.ShowWindow(SW_SHOW);
		m_cbSheetCell.SetFocus(); 
	}
	else
	{
		m_cbSheetCell.ShowWindow(SW_HIDE);
	}

}


void CFindDpiDlg::OnBnClickedButtonInsterWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	int MinValueFromDlg;
	int MaxValueFromDlg;

	CInsertSheetDlg DataRangeDlg;           // 构造对话框类CDataRange的实例
	DataRangeDlg.SetData(0,0,_T("<=W<"));

	//需要放出来!
	if(DataRangeDlg.DoModal() != IDOK)
	{
		return;
	} 

	DataRangeDlg.GetData(MinValueFromDlg,MaxValueFromDlg);

	//判断能否插入表的范围
	INT iColNum = m_vTableDataSheet[0].size();
	INT iPositionCol = -1;
	for(int t = 0; t < iColNum ; ++t)
	{
		SRowColHead* ptrTemp = (SRowColHead*) m_vTableDataSheet[0][t]; //取出表头元素，
		if(ptrTemp == NULL)
			continue;
		bool bContFlag = false;
		bContFlag = ptrTemp->Contain(MinValueFromDlg,MaxValueFromDlg);//最大值和最小值都不能在范围里

		/*****************************
		******** 20211102 ************
		**** Contain()函数再加入了判断
		******************************/

 		if(bContFlag == true)
		{
			AfxMessageBox(_T("插入行范围应该比现有行范围大！"),MB_OK);

			return ;
		}
	}

	int iDefaultMaterialIndex = m_cbDefaultID.GetCurSel();
	int iDefaultID = m_cbDefaultID.GetItemData(iDefaultMaterialIndex);
	CString sDefaultName = TEXT("");
	m_cbDefaultID.GetLBText(iDefaultMaterialIndex, sDefaultName);

	//插入列
	INT DataRow = m_vTableDataSheet.size();
	for(int i = 0; i < DataRow ; ++i)
	{
		if(0 == i)
		{
			SRowColHead* TempPtr = new SRowColHead;
			TempPtr->iMax = MaxValueFromDlg;
			TempPtr->iMin = MinValueFromDlg;
			CString str;
			str.Format(_T("%d<=W<%d"),TempPtr->iMin ,TempPtr->iMax);
			TempPtr->sDisplayText = str;
			m_vTableDataSheet[i].push_back((SSAVEDATABASE*)TempPtr);
		}
		else
		{
			SMATERIALDATA* pMaterialData = new SMATERIALDATA;
			pMaterialData->iID = iDefaultID;
			pMaterialData->sDisplayText = sDefaultName;
			m_vTableDataSheet[i].push_back((SSAVEDATABASE*)pMaterialData);
		}
	}

	_RefreshSheet();
}


// 自动调整表格大小
bool CFindDpiDlg::_AdjustColumnWidth()
{
	if(m_vTableDataSheet.size() <= 0)
		return false;

	int ColumnNumTemp = m_vTableDataSheet[0].size();
	for (int i = 0; i < ColumnNumTemp; i++)
	{
		m_ListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth =m_ListCtrl.GetColumnWidth(i);		// 表头的宽度
		m_ListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);	// 数据的宽度
		int nHeaderWidth = m_ListCtrl.GetColumnWidth(i);
		m_ListCtrl.SetColumnWidth(i, nColumnWidth>nHeaderWidth?nColumnWidth:nHeaderWidth);  // 取较大值
	}

	return false;
}


void CFindDpiDlg::OnBnClickedButtonInsertHeight()
{
	// TODO: 在此添加控件通知处理程序代码
	int MinValueFromDlg;
	int MaxValueFromDlg;
	CString InsertIntoListCtrlRow;

	CInsertSheetDlg DataRangeDlg;           // 构造对话框类CDataRange的实例
	DataRangeDlg.SetData(0,0,_T("<=W<"));

	if(DataRangeDlg.DoModal() != IDOK)
	{
		return;
	} 

	DataRangeDlg.GetData(MinValueFromDlg,MaxValueFromDlg);


	//判断能否插入表列的范围
	INT iRowNum = m_vTableDataSheet.size();
	INT iPositionRow = -1;
	for(int t = 0; t < iRowNum ; ++t)
	{
		SRowColHead* ptrTemp = (SRowColHead*) m_vTableDataSheet[t][0]; //取出表头元素，
		if(ptrTemp == NULL)
			continue;
		bool bContFlag = false;
		bContFlag = ptrTemp->Contain(MinValueFromDlg,MaxValueFromDlg);//最大值和最小值都不能在范围里
		if(bContFlag == true)
		{
			AfxMessageBox(_T("插入列范围应该比现有列范围大！"),MB_OK);

			return ;
		}
	}

	int iDefaultMaterialIndex = m_cbDefaultID.GetCurSel();
	int iDefaultID = m_cbDefaultID.GetItemData(iDefaultMaterialIndex);
	CString sDefaultName = TEXT("");
	m_cbDefaultID.GetLBText(iDefaultMaterialIndex, sDefaultName);

	SRowColHead* TempPtr = new SRowColHead;
	TempPtr->iMax = MaxValueFromDlg;
	TempPtr->iMin = MinValueFromDlg;
	CString str1;
	str1.Format(_T("%d<=W<%d"),TempPtr->iMin ,TempPtr->iMax);
	TempPtr->sDisplayText = str1;

	//插入行
	std::vector<SSAVEDATABASE*> InsertOneRow;
	for(UINT num = 0;num < m_vTableDataSheet[0].size(); ++num)
	{
		if (0 == num)
		{
			InsertOneRow.push_back(TempPtr);
			continue;
		}
		SMATERIALDATA* pMaterialData = new SMATERIALDATA;
		pMaterialData->iID = iDefaultID;
		pMaterialData->sDisplayText = sDefaultName;
		InsertOneRow.push_back(pMaterialData);
	}
	m_vTableDataSheet.push_back(InsertOneRow);

	_RefreshSheet();
}


// 初始化控件
void CFindDpiDlg::_InitCtrl(void)
{
	if (0 == m_vTableDataSheet.size())
	{
		return;
	}

	m_iDefaultWidth = m_stuBandMaterialData.iXDpi;
	m_iDefaultHeight = m_stuBandMaterialData.iYDpi;

	//初始化表格为一行两列
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertItem(0,_T(" "));
	m_ListCtrl.InsertColumn(0,_T(" "),LVCFMT_LEFT,60);
	m_ListCtrl.InsertColumn(1,_T(" "),LVCFMT_LEFT,60);


	m_cbSheetCell.ResetContent();
	m_cbSheetCell.AddString(_T("A1"));
	m_cbSheetCell.AddString(_T("B1"));
	m_cbSheetCell.AddString(_T("C1"));
	m_cbSheetCell.AddString(_T("D1"));
	m_cbSheetCell.SetItemData(0,1);
	m_cbSheetCell.SetItemData(1,2);
	m_cbSheetCell.SetItemData(2,3);
	m_cbSheetCell.SetItemData(3,4);
	m_cbSheetCell.SetCurSel(0);

	m_cbDefaultID.ResetContent();
	m_cbDefaultID.AddString(_T("A1"));
	m_cbDefaultID.AddString(_T("B1"));
	m_cbDefaultID.AddString(_T("C1"));
	m_cbDefaultID.AddString(_T("D1"));
	m_cbDefaultID.SetItemData(0,1);
	m_cbDefaultID.SetItemData(1,2);
	m_cbDefaultID.SetItemData(2,3);
	m_cbDefaultID.SetItemData(3,4);
	int com_index = m_stuBandMaterialData.stuDefaultMaterial.iID % 4;//= GetPrivateProfileInt(_T("DefaultParamId"), _T("ID"),1, m_strInkNameCfg);
	m_cbDefaultID.SetCurSel(com_index);

	//初始化时把默认值设置到控件
	CString str_default_temp;
	str_default_temp.Format(_T("%d"),m_stuBandMaterialData.iXDpi);
	SetDlgItemText(IDC_EDIT_DEFAULT_WIDTH,str_default_temp);

	str_default_temp.Format(_T("%d"),m_stuBandMaterialData.iYDpi);
	SetDlgItemText(IDC_EDIT_DEFAULT_HEIGHT,str_default_temp);

	_RefreshSheet();
}


void CFindDpiDlg::_CalCurSelRowColIndex(int& iRowIndex, int& iColIndex)
{
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
	m_ListCtrl.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_BELOW;

	iRowIndex = iColIndex = -1;
	int nItem = m_ListCtrl.SubItemHitTest(&lvinfo);
	if(nItem != -1)
	{
		iRowIndex = lvinfo.iItem ;
		iColIndex = lvinfo.iSubItem;
	}
}


void CFindDpiDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	INT iDpiWidth , iDpiHeight;
	INT iWidthIn,iHeightIn;

	iDpiWidth = GetDlgItemInt(IDC_EDIT_WIDTH_DPI);
	iDpiHeight = GetDlgItemInt(IDC_EDIT_HEIGHT_DPI);

	iWidthIn = GetDlgItemInt(IDC_EDIT_IN_WIDTH);
	iHeightIn = GetDlgItemInt(IDC_EDIT_IN_HEIGHT);

	int id = GetBandMaterialID(iDpiWidth,iDpiHeight,iWidthIn,iHeightIn);
}


int CFindDpiDlg::_GetcmbGridSel(const CString& szText)
{
	CString szItem;
	for (INT i = 0; i < m_cbSheetCell.GetCount(); i++)
	{
		m_cbSheetCell.GetLBText(i, szItem);
		if (0 == szItem.CompareNoCase(szText))
		{
			return i;
		}
	}

	return -1;
}


void CFindDpiDlg::OnCbnKillfocusComboData()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cbSheetCell.ShowWindow(SW_HIDE);
	CString		szText;
	int			nCurSel = -1;
	int			nBindID = 0;

	nCurSel = m_cbSheetCell.GetCurSel();
	m_cbSheetCell.GetLBText(nCurSel, szText);
	int iID = m_cbSheetCell.GetItemData(nCurSel);

	SMaterialData* ptrTemp = new SMaterialData;
	ptrTemp->sDisplayText = szText;
	ptrTemp->iID = iID;
	m_vTableDataSheet[m_iCurSelRowIndex][m_iCurSelColIndex] = ptrTemp;

	m_ListCtrl.SetItemText(m_iCurSelRowIndex,m_iCurSelColIndex,szText);
}
