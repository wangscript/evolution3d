#include "ExcelWriter.h"
#include "CExcel_Application.h"
#include "CExcel_Range.h"
#include "CExcel_Workbook.h"
#include "CExcel_Workbooks.h"
#include "CExcel_Worksheet.h"
#include "CExcel_Worksheets.h"
#include "CExcel_Font.h"
class CExcelSheet : public IExcelSheet
{
	CExcel_Worksheet m_Sheet;
public:
	CExcelSheet(IDispatch* pSheet);
	int  get_Rows();
	int  get_Colums();
	int  get_StartRow();
	int  get_StartColum();
	bool set_CellBorder(int x , int y , eCellLineStyle _style , int width  );
	bool set_CellBorder(const wchar_t* cell1 , const wchar_t* cell2 , eCellLineStyle _style , int width  );
	void set_CellFont(int x , int y , int size , bool bBold , bool bItail);
	void set_CellFont(const wchar_t* cell1 , const wchar_t* cell2 , int size , bool bBold , bool bItail);
	void set_CellColor(int x , int y , int cl);
	void set_CellColor(const wchar_t* cell1 , const wchar_t* cell2 , int cl);
	void set_ColumnWidth(const wchar_t* cell1 , const wchar_t* cell2 ,int newWidth);
	void set_RowHeight(const wchar_t* cell1 , const wchar_t* cell2 , int newHeight);
	void set_RowHeight(int x1 , int y1 , int x2 , int y2 , int newHeight);
	void set_ColumnWidth(int x1 , int y1 , int x2 , int y2 , int newWidth);
	int  set_Value(int x , int y , int Value);
	int  set_Value(int x , int y , float Value, const wchar_t* fmtStr);
	int  set_Value(int x , int y , const wchar_t* Value, const wchar_t* fmtStr);
	bool get_Value(int x , int y , wchar_t* Value,int len);
	bool get_Value(int x , int y , int&   Value);
	bool get_Value(int x , int y , float& Value);
    bool merge_Range(int x1, int y1 , int x2 , int y2);
	bool merge_Range(const wchar_t* cell1 , const wchar_t* cell2);
	bool set_HorizontalAlignment(int x , int y , int aglin);
    bool set_HorizontalAlignment(int aglin);
	int  setSize(int w , int h);
	void setName(const wchar_t* name);
	void getName(wchar_t* name , int len);
	void DeleteSheet();

	int ReleaseObject()
	{
		m_Sheet.ReleaseDispatch();
		delete this;
        return 0;
	}
protected:
	int m_HorizontalAlignment;
};

class CExcelWriter : public IExcelWriter
{
    CExcel_Application m_Application;
	CExcel_Workbooks   m_WorkBooks;
	CExcel_Workbook    m_WorkBook;
	bool               m_FirstCreated;
public:
	CExcelWriter();
	~CExcelWriter();
	void          setVisable(bool bVisiable);
	int           nSheet();
	void          delSheet(int idx);
	bool          init(const wchar_t* fileName , bool bVisable);
	IExcelSheet*  addSheet(const wchar_t* sheetName);
	IExcelSheet*  get_Sheet(int idxSheet);
	IExcelSheet*  get_Sheet(const wchar_t* sheetName);
	void          save(const wchar_t* fileName);
	void          close();

protected:
	void          __open(LPDISPATCH& ret , const wchar_t* fileName , COleVariant&	covTrue,	COleVariant& covFalse,	COleVariant& covOptional);
	int ReleaseObject()
	{
		delete this;
        return 0;
	}
};