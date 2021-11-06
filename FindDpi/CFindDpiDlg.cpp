
// FindDpiDlg.cpp : ʵ���ļ�
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

//ͷ�ļ����õ��� File��������Ҫ���� File �������
//���봰�����ǰ��������������Ҫ������ͷ�ļ�����
class CInsertSheetDlg;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFindDpiDlg �Ի���




CFindDpiDlg::CFindDpiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindDpiDlg::IDD, pParent)
	, m_iDefaultWidth(300)
	, m_iDefaultHeight(600)
	, m_iCurSelRowIndex(0)
	, m_iCurSelColIndex(0)
	, m_vTableDataSheet(m_stuBandMaterialData.stuTableData.vTableData)
{
	// 20211103
	// �����ڳ�ʼ���б��ʱ������ж�������ó�ʼ��

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_LoadBandMaterialData();

	//������Ϊ�գ����ʼ�����Ϊһ��һ��
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


// CFindDpiDlg ��Ϣ�������

BOOL CFindDpiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	_InitCtrl();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFindDpiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFindDpiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFindDpiDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


// ���ݴ�ӡ���Ⱥʹ�ӡ�ߴ���Ұ󶨵Ĳ���ID��iXdpi:���򾫶�  iYdpi:���򾫶�  iWidth:��ӡ���  iHeight:��ӡ�߶�  Add By Huangfei  On 2021/11/1 11:06:43
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


// �����ά������������ݵ���ǰĿ¼�ļ��С�Data���µ������ļ���
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
				sprintf_s(szLog, 256, "����󶨲�������ʱ��ɾ�������ļ�:%s ʧ�ܣ�", szFilename);
				return;
			}
		}
	}

	CString sValue;
	sValue.Format(TEXT("%d"), iRows);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("ROWS"), sValue, szFilename);
	sValue.Format(TEXT("%d"), iCols);
	::WritePrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("COLS"), sValue, szFilename);

	//дĬ�Ͼ��ȵ������ļ�
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


// �ӵ�ǰ·���µ��ļ�������ġ�Data���ļ��е������ļ��ж�ȡ���ݵ���ά������
void CFindDpiDlg::_LoadBandMaterialData(void)
{
	CString	szFilename;
	szFilename.Format(_T("%sData\\BandMaterialData.ini"), _GetCurFolder());
	int iRows = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("ROWS"), 0, szFilename);
	int iCols = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("COLS"), 0, szFilename);
	int iXdpi = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("XDPI"), 0, szFilename);
	int iYdpi = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("YDPI"), 0, szFilename);

	//20211103 ����ID,������
	int iID = ::GetPrivateProfileInt(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_ID"), 0, szFilename);
	TCHAR szValue[256] = {0};
	::GetPrivateProfileString(TEXT("BAND_MATERIAL_DATA"), TEXT("DEFAULT_MATERIAL_NAME"), TEXT(""), szValue, 256, szFilename);

	m_stuBandMaterialData.Init(iRows, iCols);

	m_stuBandMaterialData.iXDpi = iXdpi;//һ��Ҫ�ȳ�ʼ������ٸ�ֵ
	m_stuBandMaterialData.iYDpi = iYdpi;
	m_stuBandMaterialData.stuDefaultMaterial.iID = iID;

	CString sDefaultName = TEXT("");
	int iDefaultID = 0;

	//��ȡȱʡ�Ĳ��ϣ�ID������
	iDefaultID = iID;
	sDefaultName = _T("DEFAULTMATERTAL");

	if (1)//������Ҳ���
	{
		//�Ҳ���������ȱʡ����
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

	//20211103 �ǵò���һ�²�����һ�仰
	//������Ϻ󣬿��ܲ���ģ���еĲ����б仯������Ҫ����һ��
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


// ˢ�±������
void CFindDpiDlg::_RefreshSheet()
{
	//��������ڰѶ�ά��������ͬ�������

	while(m_ListCtrl.DeleteColumn (0))
	{
		//�����ɾ��������
	};

	//ɾ��������
	m_ListCtrl.DeleteAllItems();

	int iRow = m_vTableDataSheet.size();
	int iCol = m_vTableDataSheet[0].size();

	//Ҫ�ȰѸ�ʽ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CFindDpiDlg::OnDblclkListInsertDataForm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

 	//���ε�һ�к͵�һ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int MinValueFromDlg;
	int MaxValueFromDlg;

	CInsertSheetDlg DataRangeDlg;           // ����Ի�����CDataRange��ʵ��
	DataRangeDlg.SetData(0,0,_T("<=W<"));

	//��Ҫ�ų���!
	if(DataRangeDlg.DoModal() != IDOK)
	{
		return;
	} 

	DataRangeDlg.GetData(MinValueFromDlg,MaxValueFromDlg);

	//�ж��ܷ�����ķ�Χ
	INT iColNum = m_vTableDataSheet[0].size();
	INT iPositionCol = -1;
	for(int t = 0; t < iColNum ; ++t)
	{
		SRowColHead* ptrTemp = (SRowColHead*) m_vTableDataSheet[0][t]; //ȡ����ͷԪ�أ�
		if(ptrTemp == NULL)
			continue;
		bool bContFlag = false;
		bContFlag = ptrTemp->Contain(MinValueFromDlg,MaxValueFromDlg);//���ֵ����Сֵ�������ڷ�Χ��

		/*****************************
		******** 20211102 ************
		**** Contain()�����ټ������ж�
		******************************/

 		if(bContFlag == true)
		{
			AfxMessageBox(_T("�����з�ΧӦ�ñ������з�Χ��"),MB_OK);

			return ;
		}
	}

	int iDefaultMaterialIndex = m_cbDefaultID.GetCurSel();
	int iDefaultID = m_cbDefaultID.GetItemData(iDefaultMaterialIndex);
	CString sDefaultName = TEXT("");
	m_cbDefaultID.GetLBText(iDefaultMaterialIndex, sDefaultName);

	//������
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


// �Զ���������С
bool CFindDpiDlg::_AdjustColumnWidth()
{
	if(m_vTableDataSheet.size() <= 0)
		return false;

	int ColumnNumTemp = m_vTableDataSheet[0].size();
	for (int i = 0; i < ColumnNumTemp; i++)
	{
		m_ListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth =m_ListCtrl.GetColumnWidth(i);		// ��ͷ�Ŀ��
		m_ListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);	// ���ݵĿ��
		int nHeaderWidth = m_ListCtrl.GetColumnWidth(i);
		m_ListCtrl.SetColumnWidth(i, nColumnWidth>nHeaderWidth?nColumnWidth:nHeaderWidth);  // ȡ�ϴ�ֵ
	}

	return false;
}


void CFindDpiDlg::OnBnClickedButtonInsertHeight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int MinValueFromDlg;
	int MaxValueFromDlg;
	CString InsertIntoListCtrlRow;

	CInsertSheetDlg DataRangeDlg;           // ����Ի�����CDataRange��ʵ��
	DataRangeDlg.SetData(0,0,_T("<=W<"));

	if(DataRangeDlg.DoModal() != IDOK)
	{
		return;
	} 

	DataRangeDlg.GetData(MinValueFromDlg,MaxValueFromDlg);


	//�ж��ܷ������еķ�Χ
	INT iRowNum = m_vTableDataSheet.size();
	INT iPositionRow = -1;
	for(int t = 0; t < iRowNum ; ++t)
	{
		SRowColHead* ptrTemp = (SRowColHead*) m_vTableDataSheet[t][0]; //ȡ����ͷԪ�أ�
		if(ptrTemp == NULL)
			continue;
		bool bContFlag = false;
		bContFlag = ptrTemp->Contain(MinValueFromDlg,MaxValueFromDlg);//���ֵ����Сֵ�������ڷ�Χ��
		if(bContFlag == true)
		{
			AfxMessageBox(_T("�����з�ΧӦ�ñ������з�Χ��"),MB_OK);

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

	//������
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


// ��ʼ���ؼ�
void CFindDpiDlg::_InitCtrl(void)
{
	if (0 == m_vTableDataSheet.size())
	{
		return;
	}

	m_iDefaultWidth = m_stuBandMaterialData.iXDpi;
	m_iDefaultHeight = m_stuBandMaterialData.iYDpi;

	//��ʼ�����Ϊһ������
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

	//��ʼ��ʱ��Ĭ��ֵ���õ��ؼ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
