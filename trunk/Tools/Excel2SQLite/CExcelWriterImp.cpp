#include "stdafx.h"
#include "CExcelWriterImp.h"
#include <string>
HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...) {
	// Begin variable-argument list...
	va_list marker;
	va_start(marker, cArgs);

	if(!pDisp)
	{
		MessageBox(NULL, L"NULL IDispatch passed to AutoWrap()", L"Error", 0x10010);
		_exit(0);
	}

	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char buf[200];
	char szName[200];

	// Convert down to ANSI
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	// Get DISPID for name passed...
	hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if(FAILED(hr)) 
	{
		sprintf_s(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
		MessageBoxA(NULL, buf, "AutoWrap()", 0x10010);
		_exit(0);
		return hr;
	}

	// Allocate memory for arguments...
	VARIANT *pArgs = new VARIANT[cArgs+1];
	// Extract arguments...
	for(int i=0; i<cArgs; i++) {
		pArgs[i] = va_arg(marker, VARIANT);
	}

	// Build DISPPARAMS
	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	// Handle special-case for property-puts!
	if(autoType & DISPATCH_PROPERTYPUT) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	// Make the call!
	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
	if(FAILED(hr))
	{
		sprintf_s(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
		MessageBoxA(NULL, buf, "AutoWrap()", 0x10010);
		_exit(0);
		return hr;
	}
	// End variable-argument section...
	va_end(marker);

	delete [] pArgs;

	return hr;
}


void MakeCellName(int x  , int y , wchar_t* _name)
{
	int nPre = x / 26;
	int nCol = x % 26;
	std::wstring _a = L"";
	if(nPre) _a.push_back('A' + nPre - 1);
	_a.push_back('A' + nCol - 1);

	swprintf(_name ,L"%s%d" , _a.c_str() , y);
}

CExcelSheet::CExcelSheet(IDispatch* pSheet)
{
	m_HorizontalAlignment = 2;
	m_Sheet.AttachDispatch(pSheet);
}

int  CExcelSheet::get_Rows()
{ 
	//*****
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	CExcel_Range usedRange;
	usedRange.AttachDispatch(m_Sheet.get_UsedRange() );
	CExcel_Range range;
	range.AttachDispatch(usedRange.get_Rows());
	long iRowNum=range.get_Count();                   //已经使用的行数
	return iRowNum;
}


int  CExcelSheet::get_Colums()
{
	//*****
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	CExcel_Range usedRange;
	usedRange.AttachDispatch(m_Sheet.get_UsedRange() );

	CExcel_Range range;
	range.AttachDispatch(usedRange.get_Columns());
	long   iColNum=range.get_Count();                   //已经使用的列数
	return iColNum;
}

int  CExcelSheet::get_StartRow()
{
	//*****
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	CExcel_Range usedRange;

	usedRange.AttachDispatch(m_Sheet.get_UsedRange() );
	long   iStartRow=usedRange.get_Row();               //已使用区域的起始行，从1开始
	return iStartRow;
}

int  CExcelSheet::get_StartColum()
{
	//*****
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	CExcel_Range usedRange;
	usedRange.AttachDispatch(m_Sheet.get_UsedRange() );
	long   iStartCol=usedRange.get_Column();            //已使用区域的起始列，从1开始
	return iStartCol;
}

int   CExcelSheet::set_Value(int x , int y , int Value)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );
	range.put_Item(COleVariant((long)y),COleVariant((long)x),COleVariant((long)Value)); 
    return 0;
}

int   CExcelSheet::set_Value(int x , int y , float Value, const wchar_t* fmtStr)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );
	range.put_Item(COleVariant((long)y),COleVariant((long)x),COleVariant(Value)); 
	if(fmtStr) range.put_NumberFormat( COleVariant(fmtStr) );
	range.ApplyOutlineStyles();
	return 0;
}


void setCellBorder(CExcel_Range& range , CExcelSheet::eCellLineStyle _style , int width)
{
	COleVariant  vRange1;   // 设置单元格的线；
	COleVariant  vRange2;
	COleVariant  vRange3;
	COleVariant  vRange4;

	vRange1.vt =VT_I2;
	vRange1.lVal = (long)_style;   // 线的样式：0- no line; 1-solid; 2-big dot;3-small dot;4-dash dot; 5-dash dot dot;
	vRange2.vt =VT_I2;
	vRange2.lVal =width;  // 线的粗细程度；
	vRange3.vt =VT_I2;
	vRange3.lVal =1;   // 1-black;2-white;3-red;4-green;5-blue; 6-yellow; 7-pink;8-dark blue;
	vRange4.vt = VT_UI4;
	vRange4.uintVal =RGB(0,0,0);  // 我测试后认为，没有实际意义，只有vRange3起作用

	range.BorderAround(vRange1,width,1,vRange4);
}

bool CExcelSheet::set_CellBorder(const wchar_t* Cell1 , const wchar_t* Cell2 ,  CExcelSheet::eCellLineStyle _style , int width  )
{
	CExcel_Range range;
	CExcel_Font font;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );
	setCellBorder(range , _style , width);
	return true;
}


bool CExcelSheet::set_CellBorder(int x , int y , CExcelSheet::eCellLineStyle _style , int width  )
{
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );
	range.AttachDispatch( range.get_Item(COleVariant( (long)y),COleVariant((long)x)).pdispVal ); 
	setCellBorder(range , _style , width);
	return true;
}

bool CExcelSheet::merge_Range(const wchar_t* Cell1 , const wchar_t* Cell2)
{
	CExcel_Range range;
	CExcel_Font font;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );

	range.Merge(covOptional);
	return true;
}



bool    CExcelSheet::merge_Range(int x1, int y1 , int x2 , int y2)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"

    wchar_t Cell1[5];
	wchar_t Cell2[5];
	MakeCellName(x1 , y1 , Cell1);
	MakeCellName(x2 , y2 , Cell2);

    return merge_Range(Cell1 , Cell2);
	return true;
}

void   CExcelSheet::set_CellColor(int x , int y , int cl)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	CExcel_Font font;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	range.AttachDispatch( range.get_Item(COleVariant( (long)y),COleVariant((long)x)).pdispVal ); 
	LPDISPATCH pFont = range.get_Font();
	font.AttachDispatch( pFont  );
	font.put_Color( COleVariant((long)cl,VT_UINT)  );

}
void   CExcelSheet::set_ColumnWidth(const wchar_t* Cell1 , const wchar_t* Cell2 , int newWidth)
{
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );
	range.put_ColumnWidth( COleVariant((long)newWidth   , VT_UINT) );
}

void   CExcelSheet::set_RowHeight(const wchar_t* Cell1 , const wchar_t* Cell2 , int newHeight)
{
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );
	range.put_RowHeight( COleVariant((long)newHeight   , VT_UINT) );
}

void   CExcelSheet::set_RowHeight(int x1 , int y1 , int x2 , int y2 , int newHeight)
{
	wchar_t cell1[16] = {0};
	wchar_t cell2[16] = {0};
	MakeCellName(x1, y1, cell1);
	MakeCellName(x2, y2, cell2);
	return set_RowHeight(cell1 , cell2 , newHeight);
}

void   CExcelSheet::set_ColumnWidth(int x1 , int y1 , int x2 , int y2 , int newHeight)
{
	wchar_t cell1[16] = {0};
	wchar_t cell2[16] = {0};
	MakeCellName(x1, y1, cell1);
	MakeCellName(x2, y2, cell2);
	return set_ColumnWidth(cell1 , cell2 , newHeight);
}

void   CExcelSheet::set_CellColor(const wchar_t* Cell1 , const wchar_t* Cell2 , int cl)
{
	CExcel_Range range;
	CExcel_Font font;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );

	LPDISPATCH pFont = range.get_Font();
	font.AttachDispatch( pFont  );
	font.put_Color( COleVariant((long)cl,VT_UINT)  );


}

void   SetCellFont(CExcel_Range& range ,int size , bool bBold , bool bItail)
{
	CExcel_Font font;
	LPDISPATCH pFont = range.get_Font();
	font.AttachDispatch( pFont  );
	BOOL _bBold  = bBold;
	BOOL _bItail = bItail;
	font.put_Size  (COleVariant((long)size   , VT_UINT));
	font.put_Italic(COleVariant((long)_bItail, VT_BOOL));
	font.put_Bold  (COleVariant((long)_bBold , VT_BOOL));
}

void   CExcelSheet::set_CellFont(int x , int y , int size , bool bBold , bool bItail)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	range.AttachDispatch( range.get_Item(COleVariant( (long)y),COleVariant((long)x)).pdispVal ); 
    SetCellFont(range , size , bBold , bItail);
}

void   CExcelSheet::set_CellFont(const wchar_t* Cell1 , const wchar_t* Cell2 , int size , bool bBold , bool bItail)
{
	CExcel_Range range;
	CExcel_Font font;
	range.AttachDispatch( m_Sheet.get_UsedRange() );

	COleVariant cell1(Cell1);
	COleVariant cell2(Cell2);
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	range.AttachDispatch(m_Sheet.get_Range(cell1 , cell2) );
	SetCellFont(range , size , bBold , bItail);
	
}

bool   CExcelSheet::set_HorizontalAlignment(int x , int y , int aglin)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	range.AttachDispatch( m_Sheet.get_UsedRange() );
	LPDISPATCH pRange =  range.get_Item (COleVariant((long)y),COleVariant((long)x)).pdispVal;
	range.AttachDispatch( pRange );

	COleVariant hA;
	V_I4(&hA) = aglin;
	range.put_HorizontalAlignment(hA );
	return true;

}

bool   CExcelSheet::set_HorizontalAlignment(int aglin)
{
	m_HorizontalAlignment = aglin;
	return true;
}

int   CExcelSheet::set_Value(int x , int y , const wchar_t* wcsValue , const wchar_t* fmtStr)
{
	//*****
	//设置第一个单元格的值"HI,EXCEL!"
	CExcel_Range range;
	CExcel_Font font;

	range.AttachDispatch( m_Sheet.get_UsedRange() );

	LPDISPATCH pRange =  range.get_Item (COleVariant((long)y),COleVariant((long)x)).pdispVal;
	range.AttachDispatch( pRange );
	
	COleVariant hA;
	hA.vt = 3;
	if(fmtStr)
	{
		range.put_NumberFormat( COleVariant(fmtStr) );
	}
    VARIANT format = range.get_NumberFormat();
	range.put_Value2(COleVariant(wcsValue));

	V_I4(&hA) = m_HorizontalAlignment;
	range.put_HorizontalAlignment(hA );

	return 0;
}

bool CExcelSheet::get_Value(int x , int y , wchar_t* Value,int len)
{
	CExcel_Range range;
	//*****
	//读取第一个单元格的值
	range.AttachDispatch(m_Sheet.get_UsedRange()); 
	range.AttachDispatch(range.get_Item (COleVariant((long)y),COleVariant((long)x)).pdispVal );
	range.ClearFormats();
	COleVariant vResult =range.get_Value2();
	vResult = range.get_Text();
	switch(vResult.vt)
	{
	case VT_BSTR:       //字符串
		{
			wcsncpy(Value , vResult.bstrVal , len);
			return true;
		}
	}

	return false;
}

bool  CExcelSheet::get_Value(int x , int y , int&   Value)
{
	 return false;
}
bool  CExcelSheet::get_Value(int x , int y , float& Value)
{
	 return false;
}

int   CExcelSheet::setSize(int w , int h)
{
    return 0;    
}

void  CExcelSheet::setName(const wchar_t* wcsName)
{
	m_Sheet.put_Name(wcsName);   
}

void  CExcelSheet::getName(wchar_t* name , int len)
{
    CString strName = m_Sheet.get_Name();
	wcsncpy(name , strName , len);
}

void CExcelSheet::DeleteSheet()
{
	m_Sheet.Delete();
}

CExcelWriter::CExcelWriter()
{
	
}
CExcelWriter::~CExcelWriter()
{

}

void          CExcelWriter::delSheet(int idx)
{
	CExcel_Worksheets sheets;
	sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
	CExcel_Worksheet sheet;
	sheet.AttachDispatch( sheets.get_Item( COleVariant( (long)idx) ));
	sheet.Delete();
}
void          CExcelWriter::setVisable(bool bVisiable)
{
	m_Application.put_Visible( bVisiable?TRUE:FALSE);
}
void HandleException(EXCEPTION_POINTERS * excPtr)
{

}
void CExcelWriter::__open(LPDISPATCH& ret , const wchar_t* fileName , COleVariant&	covTrue,	COleVariant& covFalse,	COleVariant& covOptional)
{
	ret = NULL;

	__try{
		ret = m_WorkBooks.Open(fileName,      
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional,covOptional,covOptional );
	}
	__except(HandleException(GetExceptionInformation()) , EXCEPTION_EXECUTE_HANDLER)
	{
		ret = NULL;
		close();
		return ;
	}
}

bool          CExcelWriter::init(const wchar_t* fileName , bool bVisable)
{
   CoInitialize(NULL);
   
   m_Application.CreateDispatch(L"Excel.Application");
   if(m_Application.m_lpDispatch == NULL)
	   return false;

   m_Application.put_Visible(bVisable);
   m_Application.put_UserControl(FALSE);
   m_WorkBooks.AttachDispatch( m_Application.get_Workbooks() );
   if(fileName == NULL)
   {
	   m_FirstCreated = true;
	   m_WorkBook.AttachDispatch(   m_WorkBooks.Add( COleVariant( (LPCTSTR)NULL)   ) );
	   CExcel_Worksheets sheets;
	   sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
	   int nSheet = sheets.get_Count();
	   for(int i = 0 ; i < nSheet - 1; i ++)
	   {
		   delSheet(1);
	   }
   }
   else
   {
	   COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	   LPDISPATCH pWB = NULL;
	   
	   __open(pWB , fileName ,covTrue ,	covFalse ,	covOptional);
	   if(pWB == NULL)
		   return false;


	   m_WorkBook.AttachDispatch( pWB  );
	   m_FirstCreated = false;

   }
   return true;
}

int CExcelWriter::nSheet()
{
	CExcel_Worksheets sheets;
	sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
	return sheets.get_Count();
}

IExcelSheet*  CExcelWriter::get_Sheet(int idxSheet)
{
	CExcel_Worksheets sheets;
	sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
	LPDISPATCH pDisp =  sheets.get_Item( COleVariant(long(idxSheet) ));
	if(pDisp == NULL)
		return NULL;

	CExcelSheet* Sheet = new CExcelSheet(pDisp);
	return Sheet;
}

IExcelSheet*  CExcelWriter::get_Sheet(const wchar_t* sheetName)
{
	for(int idxSheet = 0 ; idxSheet < nSheet() ; idxSheet ++)
	{
		CExcel_Worksheets sheets;
		sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
		LPDISPATCH pDisp =  sheets.get_Item( COleVariant(long(idxSheet + 1) ));
		if(pDisp == NULL)
			continue;
		CExcelSheet* Sheet = new CExcelSheet(pDisp);
		wchar_t Name[1024] = {0};
		Sheet->getName(Name,1024);
		std::wstring _strName = Name;
		if(_strName == sheetName)
			return Sheet;
		delete Sheet;
	}

	return NULL;
}

IExcelSheet*  CExcelWriter::addSheet(const wchar_t* sheetName)
{
	if(m_FirstCreated == false)
	{
		COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

		CExcel_Worksheets sheets;
		sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
		//LPDISPATCH pDisp =  m_WorkBook.get_ActiveSheet();
		COleVariant vtMissing;
		vtMissing.vt = VT_EMPTY;
		LPDISPATCH pDisp =  sheets.Add( covOptional , covOptional, covOptional , covOptional );
		if(pDisp == NULL)
			return NULL;

		CExcelSheet* Sheet = new CExcelSheet(pDisp);
		Sheet->setName(sheetName);
		return Sheet;
	}
	else
	{
		CExcel_Worksheets sheets;
		sheets.AttachDispatch( m_WorkBook.get_Worksheets() );
		CExcelSheet* Sheet = new CExcelSheet( sheets.get_Item( COleVariant( (long)1) ) );
		m_FirstCreated = false;
		Sheet->setName(sheetName);
		return Sheet;
	}
}


void          CExcelWriter::save(const wchar_t* wcsFileName)
{
	CExcel_Worksheet sheet;
	{
		VARIANT filename;
		filename.vt = VT_BSTR;
		filename.bstrVal = SysAllocString(wcsFileName);
		//AutoWrap(DISPATCH_METHOD, NULL, m_WorkBook, L"SaveAs", 1, filename);
	}

	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	VARIANT filename;
	filename.vt = VT_BSTR;
	filename.bstrVal = SysAllocString(wcsFileName);
	m_WorkBook.SaveAs( filename ,covOptional , covOptional , covOptional , covOptional,covOptional,0,covOptional,covTrue,covOptional,covOptional,covOptional );
  
}

void CExcelWriter::close()
{
	COleVariant	covTrue((short)TRUE),	covFalse((short)FALSE),	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	m_WorkBook.Close(covFalse,covOptional,covOptional);
	m_WorkBook.ReleaseDispatch();
	m_WorkBooks.ReleaseDispatch();
    m_Application.ReleaseDispatch();
}

extern "C" 
{
	 IExcelWriter* createExcelWriter()
	{
		return new CExcelWriter;
	}
};