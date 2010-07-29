
// Excel2SQLiteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Excel2SQLite.h"
#include "Excel2SQLiteDlg.h"
#include "BaseLib/xI18N.h"
USING_NS_XEVOL3D
static std::ds_wstring  _ToWstring(const char* _val)
{
	wchar_t v[512] = {0};
	XEvol_Utf8ToUnicode(_val , v , 512);
	return v;
}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CExcel2SQLiteDlg 对话框




CExcel2SQLiteDlg::CExcel2SQLiteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcel2SQLiteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pExcelWriter = NULL;
}
CExcel2SQLiteDlg::~CExcel2SQLiteDlg()
{
	if(m_pExcelWriter)
	{
		m_pExcelWriter->close();
		m_pExcelWriter->ReleaseObject();
	}
}
void CExcel2SQLiteDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_EXCEL_SHEET, m_cbExcelSheets);
    DDX_Control(pDX, IDC_EXCEL_KEY, m_cbExcelKey);
    DDX_Control(pDX, IDC_EXCEL_FIELD_LIST, m_ListExcelFields);
    DDX_Control(pDX, IDC_SQLITE_TABLE, m_cbSqliteTable);
    DDX_Control(pDX, IDC_LIST_EXCEL_DATA, m_listExcelData);
    DDX_Control(pDX, IDC_TXT_SQLUPDATE, m_textUpdateSql);
    DDX_Control(pDX, IDC_TXT_SQL_INSERT, m_textInsertExcel);
}

BEGIN_MESSAGE_MAP(CExcel2SQLiteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPEN_SQLITE, &CExcel2SQLiteDlg::OnBnClickedBtnOpenSqlite)
	ON_BN_CLICKED(IDC_BTN_OPEN_EXCEL, &CExcel2SQLiteDlg::OnBnClickedBtnOpenExcel)
	ON_CBN_SELCHANGE(IDC_LIST_EXCEL_SHEET, &CExcel2SQLiteDlg::OnCbnSelchangeListExcelSheet)
	ON_CBN_SELCHANGE(IDC_EXCEL_KEY, &CExcel2SQLiteDlg::OnCbnSelchangeExcelKey)
	ON_CBN_SELCHANGE(IDC_SQLITE_TABLE, &CExcel2SQLiteDlg::OnCbnSelchangeSqliteTable)
	ON_BN_CLICKED(IDC_BTN_EXCUTE, &CExcel2SQLiteDlg::OnBnClickedBtnExcute)
    ON_BN_CLICKED(IDC_BTN_GEN_SQL, &CExcel2SQLiteDlg::OnBnClickedBtnGenSql)
END_MESSAGE_MAP()


// CExcel2SQLiteDlg 消息处理程序

BOOL CExcel2SQLiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExcel2SQLiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExcel2SQLiteDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExcel2SQLiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool get_excel_filename(wchar_t* file_name)
{
    OPENFILENAMEW ofn; 
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = GetActiveWindow();
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"电子表格office 2003(*.xls)\0*.xls\0电子表格office 2007(*.xlsx)\0*.xlsx\0订单文件(*.xml)\0*.xml\0所有文件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrDefExt = L"pod";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileNameW(&ofn) == FALSE)
    {
        return false;
    }
    return true;
}

bool get_sqlite_filename(wchar_t* file_name)
{
    OPENFILENAMEW ofn; 
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = GetActiveWindow();
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"SQLite数据库文件(*.db3)\0*.db3\0所有文件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrDefExt = L"pod";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileNameW(&ofn) == FALSE)
    {
        return false;
    }
    return true;
}


void CExcel2SQLiteDlg::OnBnClickedBtnOpenSqlite()
{
    wchar_t file_name[1024] = {0};
    if(false == get_sqlite_filename(file_name) )
        return ;
	theDataBase.Close();
    if(theDataBase.OpenDatabase(file_name) == false)
	{
		MessageBox(L"打开数据库失败");
		theDataBase.Close();
		return ;
	}

	m_cbSqliteTable.Clear();
	return UpdateSQliteTable();


    // TODO: Add your control notification handler code here
}

void CExcel2SQLiteDlg::OnBnClickedBtnOpenExcel()
{
	// TODO: Add your control notification handler code here
	wchar_t file_name[1024] = {0};
	if(false == get_excel_filename(file_name) )
		return ;

    m_cbExcelSheets.ResetContent();
	if(m_pExcelWriter) m_pExcelWriter->close();
	m_pExcelWriter = createExcelWriter();
	m_pExcelWriter->init(file_name,false);
    
	int nSheet = m_pExcelWriter->nSheet();
	for(int i = 0 ; i < nSheet ; i ++)
	{
		IExcelSheet* pSheet  = m_pExcelWriter->get_Sheet( (int)i + 1 );
		wchar_t _name[256];
		pSheet->getName(_name , 256);
		this->m_cbExcelSheets.InsertString(i , _name);
		pSheet->ReleaseObject();
	}
    
	SelectExcelSheet(0);

}

void   CExcel2SQLiteDlg::UpdateSQliteTable() 
{
	std::wstring strSQL = L"select * from sqlite_master where type=\'table\'";


	bool sqlRet = false;
	SqliteRecordset result_set;
	sqlRet = theDataBase.Query( strSQL.c_str() , result_set);
	if(sqlRet == false)
	{
		MessageBox(L"数据库里没有表");
		return ;
	}

	int nColum = 0;
	int nRow = 0 ;

	std::vector<std::ds_wstring> vFields;

	nColum = result_set.GetNumOfColumns() ; 
	for(int iColum = 0 ; iColum < nColum ; iColum ++)
	{
		SqliteRecordset::FieldInfo fInfo = result_set.GetFiledInfoByIndex(iColum);
		vFields.push_back( _ToWstring(fInfo.pFieldName).c_str() );
	}				 

	int iRow = 0;
	int iCol = 0;
	while(result_set.Next())
	{ 
		for(int iCol = 0 ; iCol < (int)vFields.size() ; iCol ++)
		{
			//char _FieldName[256] = {0};
			//WideCharToMultiByte( CP_UTF8, 0, vFields[iRow].c_str(),vFields[iRow].length(), _FieldName, 256, 0, 0 );
			const char* vRet = (const char*)result_set.GetText( vFields[iCol].c_str() );// (TCHAR*)_FieldName );
			if(vRet != NULL)
			{
				std::ds_wstring retString = _ToWstring(vRet);
				if     (vFields[iCol] == L"name"  ||   vFields[iCol] == L"Name" || vFields[iCol] == L"NAME") 
				{
					m_cbSqliteTable.InsertString(iRow , retString.c_str());
					iRow ++;
				}
			}
		}
	}
	m_cbSqliteTable.SetCurSel(0);
}

void  CExcel2SQLiteDlg::GenerateSQL()
{
     int nSelect = m_ListExcelFields.GetSelectedCount();
     POSITION pos = m_ListExcelFields.GetFirstSelectedItemPosition();

	 CString TableName ; m_cbSqliteTable.GetWindowText(TableName);
	 CString KeyField  ; m_cbExcelKey.GetWindowText(KeyField);

     TableName = L"{$Arg表名}";
	 m_strInsertSQL = std::wstring(L"INSERT INTO   ") + TableName.GetBuffer() + L" ";
	 m_strQuerySQL  = std::wstring(L"SELECT * FROM ") + TableName.GetBuffer() + L" ";
	 m_strUpdateSQL = std::wstring(L"UPDATE "       ) + TableName.GetBuffer() + L" SET "; 
	 int iField = 0;


     SqliteRecordset _SqlRet;
     theDataBase.Query(m_strQuerySQL.c_str() , _SqlRet);

	 CString _InsertTable = L" ( ";
	 CString _InsertValue = L" VALUES( ";
	 CString _updateTable = L" ";
	 while(1)
	 {
		 int iSel = m_ListExcelFields.GetNextSelectedItem(pos);
		 if(iSel < 0) 
			 break;
		 //CString _fieldName  = m_ListExcelFields.GetItemText(iSel , 0 );
		 wchar_t _fieldName[1024] = {0};
		 LVITEM lvi;
		 memset(&lvi, 0, sizeof(LVITEM));
		 lvi.iSubItem = 0;
		 lvi.cchTextMax = 1024;
		 lvi.pszText = _fieldName;
		 (int)::SendMessage(m_ListExcelFields.GetSafeHwnd(), LVM_GETITEMTEXT, (WPARAM)iSel, (LPARAM)&lvi);


         SqliteRecordset::FieldInfo  fieldType = _SqlRet.GetFiledInfoByName( _fieldName );//.GetBuffer() );
         bool isText = true;
         if(fieldType.pFieldDecl)
         {
             isText = (fieldType.pFieldDecl == std::string("text") || fieldType.kType == SqliteRecordset::kCTText );
         }
         if(iField != 0)
		 {
			 _InsertTable += L" , ";
			 _InsertValue +=  L" , ";
			 _updateTable += L" , ";
		 }
		 _InsertTable += L"\'" ;
		 _InsertTable += _fieldName;
		 _InsertTable += L"\' " ;

		
         if(isText) _InsertValue += L"\'" ;
		 _InsertValue += L"{$" ;
		 _InsertValue += _fieldName;
		 _InsertValue += L"}" ;
         if(isText) _InsertValue += L"\'" ;


		 _updateTable += _fieldName;
		 _updateTable += L"=";

         if(isText) _updateTable += L"\'" ;
		 _updateTable += L"{$" ;
		 _updateTable += _fieldName;
		 _updateTable += L"}" ;
         if(isText) _updateTable += L"\'" ;

		 iField ++;

	 }

	 _InsertTable += L" ) ";
	 _InsertValue += L" ) ";
	 m_strInsertSQL += _InsertTable.GetBuffer();
	 m_strInsertSQL += _InsertValue.GetBuffer(); 

	 m_strUpdateSQL += _updateTable;

	 CString _Where = CString(L" WHERE ") + KeyField.GetBuffer() + L" = \'{$" + KeyField.GetBuffer() + L"}\'";
	 m_strUpdateSQL += _Where.GetBuffer();
	 m_strQuerySQL  += _Where.GetBuffer();

	 m_textInsertExcel.SetWindowText( m_strInsertSQL.c_str() );
	 m_textUpdateSql.SetWindowText(m_strUpdateSQL.c_str() );
}

int GetColIndex(xXmlNode* pNode , std::wstring _fieldName)
{
	for(int i = 0 ; i < pNode->int_value(L"nCol") ; i ++)
	{
		if( pNode->findNode(i)->value(L"name") == _fieldName )
			return i;
	}
	return -1;
}
void CExcel2SQLiteDlg::SelectExcelSheet(int iSheet)
{
	IExcelSheet* pSheet  = m_pExcelWriter->get_Sheet( (int)iSheet + 1 );
	if(pSheet == NULL)
		return ;

    m_listExcelData.DeleteAllItems();
	m_ListExcelFields.DeleteAllItems();
	m_cbExcelKey.ResetContent();// .Clear();

	int nOldColumn = m_DataField.int_value(L"nCol");
	while(nOldColumn)
	{
	    BOOL Ret =	m_listExcelData.DeleteColumn(0);
		nOldColumn -- ;
	}

	m_DataField.clear();
	m_ExcelData.clear();
	int nCol = 0;
	int blankLine = 0;
	for(int i = 0 ; i < 1000 ; i ++)
	{
		wchar_t filedName[256];
		pSheet->get_Value(1 + i , 1 , filedName , 256);
		xXmlNode* pNode = m_DataField.insertNode(L"Field");
		pNode->setValue(L"name" , filedName);
		pNode->setValue(L"col"  , i + 1);
		if( wcslen_x(filedName) != 0 )
		{
			m_ListExcelFields.InsertItem(0 , filedName);
			m_cbExcelKey.InsertString(0 , filedName);
			nCol = i + 1;
			blankLine = 0;
		}
		else
		{
			blankLine ++;
			if(blankLine > 10)
				break;
		}
	}
	m_DataField.setValue(L"nCol" , nCol);
	m_cbExcelKey.SetCurSel(0);
	m_cbExcelSheets.SetCurSel(iSheet);
    
	m_listExcelData.DeleteAllItems();
	for(int i = 0 ; i < nCol ; i ++)
	{
		m_listExcelData.InsertColumn(i , m_DataField.findNode(i)->value(L"name") );
        m_listExcelData.SetColumnWidth(i , 80);
	}



	int iRow = 0;
	for(int y = 2 ; true ; y ++)
	{
		xXmlNode* pRecord = m_ExcelData.insertNode(L"Record");
		bool IsBlankLine = true;
		for(int x = 1 ; x < nCol + 1 ; x ++)
		{
			const wchar_t* filedName = m_DataField.findNode(x - 1)->value(L"name");
			wchar_t _value[2048] = {0};
			pSheet->get_Value(x , y , _value , 2048);
			if( wcslen_x(_value) != 0 )
			{				
				pRecord->setValue(filedName , _value );
				IsBlankLine = false ;
				if(x == 1)
				{
					m_listExcelData.InsertItem(iRow , _value);
				}
				else
				{
					m_listExcelData.SetItemText(iRow , x - 1 , _value);
				}				
			}
			else
			{
				pRecord->setValue(filedName , L"" );
			}
		}  

		if(IsBlankLine == true)
		{
			m_ExcelData.removeNode(pRecord);
			blankLine ++;
			if(blankLine > 10)
				break;
		}
		else
		{
			blankLine = 0;
			iRow ++;
		}
	}



	GenerateSQL();
}
void CExcel2SQLiteDlg::OnCbnSelchangeListExcelSheet()
{
	SelectExcelSheet( m_cbExcelSheets.GetCurSel() );
	//GenerateSQL();

	// TODO: Add your control notification handler code here
}

void CExcel2SQLiteDlg::OnCbnSelchangeExcelKey()
{
	// TODO: Add your control notification handler code here
	//GenerateSQL();
}

void CExcel2SQLiteDlg::OnCbnSelchangeSqliteTable()
{
	//GenerateSQL();
	// TODO: Add your control notification handler code here
}

int GetDataCount(const wchar_t* _sql)
{
	bool sqlRet = false;
	SqliteRecordset result_set;
	sqlRet = theDataBase.Query( _sql , result_set);
	if(sqlRet == false)
		return 0;

	int nColum = 0;
	int nRow = 0 ;

	std::vector<std::ds_wstring> vFields;

	nColum = result_set.GetNumOfColumns() ; 
	for(int iColum = 0 ; iColum < nColum ; iColum ++)
	{
		SqliteRecordset::FieldInfo fInfo = result_set.GetFiledInfoByIndex(iColum);
		vFields.push_back( _ToWstring(fInfo.pFieldName).c_str() );
	}				 

	int nRet = 0 ;

	while(result_set.Next())
	{ 
		nRet ++;
	}
	return nRet;
}

bool  _ReconstructSQL(xXmlNode::XmlValues& Args , std::wstring& strSQL)
{
	for(size_t i = 0 ; i < Args.size() ; i ++ )
	{
		xXmlValue* pArg = Args[i];
		std::wstring Value     = pArg->value();
		std::wstring fieldName = pArg->name() ;
		std::wstring _exp =  Value;
		std::wstring _pattern = L"{$" + fieldName + L"}";
		//ToLower(_pattern);

		std::wstring lwSQL = strSQL;
		//ToLower(lwSQL);
		std::wstring::size_type pos = lwSQL.find(_pattern);

		//替换{$FieldName}
		while(pos != std::wstring::npos )
		{
			strSQL.replace( pos , _pattern.length() , _exp );
			pos = strSQL.find(_pattern);
		}
		continue;
	}
	return true;
}

#include <Userenv.h>
#pragma comment(lib,"Userenv.lib")
std::wstring gs_strLogFileName = L"";
void BeginLog(const wchar_t* fileName)
{
	HANDLE hToken = 0;
	OpenProcessToken(GetCurrentProcess() , TOKEN_ALL_ACCESS , &hToken);
	wchar_t _usrDir [1024]={0};
	DWORD _size = 1024;
	GetUserProfileDirectoryW(hToken,_usrDir,&_size);
	std::wstring userDir = std::wstring(_usrDir) + L"\\" + fileName;
	gs_strLogFileName = userDir;
	XEVOL_LOG_REDIR(userDir.c_str());
}

void BeginLog()
{
	BeginLog(L"xevol_database_log.txt");
}

void FinishLog()
{
	XEVOL_LOG_CLOSE();   
	ShellExecuteW(GetActiveWindow() , L"open" , L"notepad.exe" , gs_strLogFileName.c_str() , NULL , SW_SHOW);
}

void CExcel2SQLiteDlg::OnBnClickedBtnExcute()
{
	CString sql ;
	m_textInsertExcel.GetWindowText(sql);
	m_strInsertSQL = sql.GetBuffer();

	m_textUpdateSql.GetWindowText(sql);
	m_strUpdateSQL = sql.GetBuffer();


    CString _TableName;
    m_cbSqliteTable.GetWindowText(_TableName);
    xXmlValue tableName(2324);
    tableName.setName(L"Arg表名");
    tableName.setValue(_TableName.GetBuffer() );

	int nUpdate = 0;
	int nInsert = 0;
	int nRecord = m_ExcelData.countNode();// int_value(L"nCol");
	std::vector<std::wstring> vUpdateRecords;
	
	BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"QuerySQL=%s\n"  , m_strQuerySQL.c_str()  );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"InsertSQL=%s\n" , m_strInsertSQL.c_str() );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"UpdateSQL=%s\n" , m_strUpdateSQL.c_str() );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"开始数据库操作\n");
	try
	{
		theDataBase.BeginTransaction();
		for(int i = 0 ; i < nRecord ; i ++)
		{
			std::wstring sql = L"";

			xXmlNode* pRecord = m_ExcelData.findNode(i);
			xXmlNode::XmlValues args;
			args.push_back(&tableName);

			pRecord->findValue(args);
			sql = m_strQuerySQL;
			_ReconstructSQL(args , sql);

			if( GetDataCount(sql.c_str() ) > 0 )
			{
				sql = m_strUpdateSQL;
				_ReconstructSQL(args , sql);

				bool sqlRet = false;
				SqliteRecordset result_set;
				sqlRet = theDataBase.Execute( sql.c_str() );
				if(sqlRet ) nUpdate ++;
				XEVOL_LOG(eXL_DEBUG_HIGH , L"Update>> %s\n" , sql.c_str() );

			}
			else
			{
				sql = m_strInsertSQL;
				_ReconstructSQL(args , sql);

				bool sqlRet = false;
				SqliteRecordset result_set;
				sqlRet = theDataBase.Execute( sql.c_str() );
				if(sqlRet ) nInsert ++;
				XEVOL_LOG(eXL_DEBUG_HIGH , L"Insert>> %s\n" , sql.c_str() );
			}
		}
		theDataBase.CommitTransaction();
		wchar_t buf[1024] = {0};
		swprintf_x(buf , 1024 , L"插入%d条数据，更新%d数据" , nInsert , nUpdate);
		MessageBox(buf,L"数据操作结果");
	}
	catch (...)
	{
		theDataBase.RollbackTransaction();
		wchar_t buf[1024] = {0};
		swprintf_x(buf , 1024 , L"数据库操作失败，回滚事务");
		MessageBox(buf,L"数据操作结果");
	}

	FinishLog();
	return ;

	// TODO: Add your control notification handler code here
}

void CExcel2SQLiteDlg::OnBnClickedBtnGenSql()
{
    GenerateSQL();
    // TODO: Add your control notification handler code here
}
