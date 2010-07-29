#pragma once
#ifdef _USRDLL
#define  __declspec(dllexport)
#else
#define  __declspec(dllimport)
#endif



class IExcelSheet
{
public:
	enum eCellLineStyle
	{
		Line_None      = 0 ,
		Line_Solid     = 1,
		Line_BigDot    = 2 , 
		Line_SmallDot  = 3,
		Line_DashDot   = 4,
		Line_DatDotDot = 5
	};

	enum 
	{
       eHA_Left   = 2,
	   eHA_Center = 3,
	   eHA_Right  = 4,
	};
public:
	virtual int      get_Rows() = 0;
	virtual int      get_Colums() = 0;
	virtual int      get_StartRow() = 0;
	virtual int      get_StartColum() = 0;
	virtual bool     set_CellBorder(int x , int y , eCellLineStyle _style , int width  ) =0;
	virtual bool     set_CellBorder(const wchar_t* cell1 , const wchar_t* cell2 , eCellLineStyle _style , int width  )=0;
	virtual void     set_CellFont(int x , int y , int size , bool bBold , bool bItail) = 0;
	virtual void     set_CellFont(const wchar_t* cell1 , const wchar_t* cell2 , int size , bool bBold , bool bItail) = 0;
	virtual void     set_CellColor(int x , int y , int cl) = 0;
	virtual void     set_CellColor(const wchar_t* cell1 , const wchar_t* cell2 , int cl) = 0;

	virtual void     set_ColumnWidth(const wchar_t* cell1 , const wchar_t* cell2 , int newWidth) = 0;
	virtual void     set_RowHeight(const wchar_t* cell1 , const wchar_t* cell2 , int newHeight) = 0;
	virtual void     set_RowHeight(int x1 , int y1 , int x2 , int y2 , int newHeight) = 0;
	virtual void     set_ColumnWidth(int x1 , int y1 , int x2 , int y2 , int newWidth) = 0;
	virtual bool     set_HorizontalAlignment(int x , int y , int aglin) = 0;
	virtual bool     set_HorizontalAlignment(int aglin) = 0;

	virtual int      set_Value(int x , int y , int Value) = 0;
	virtual int      set_Value(int x , int y , float Value, const wchar_t* fmtStr = 0) = 0;
	virtual int      set_Value(int x , int y , const wchar_t* Value, const wchar_t* fmtStr = 0) = 0;
	virtual bool     get_Value(int x , int y , wchar_t* Value,int len) = 0;
	virtual bool     get_Value(int x , int y , int&   Value) = 0;
	virtual bool     get_Value(int x , int y , float& Value) = 0;
	virtual bool     merge_Range(int x1, int y1 , int x2 , int y2) = 0;
    virtual bool     merge_Range(const wchar_t* cell1 , const wchar_t* cell2) = 0;
	virtual int      setSize(int w , int h) = 0;
	virtual void     setName(const wchar_t* name) = 0;
	virtual void     getName(wchar_t* name , int len) = 0;
	virtual void     DeleteSheet() = 0;
	virtual int      ReleaseObject() = 0;

};

class IExcelWriter
{
public:
	virtual void           setVisable(bool bVisiable) = 0;
	virtual int            nSheet() = 0;
	virtual void           delSheet(int idx) = 0;
	virtual bool           init(const wchar_t* fileName , bool bVisable) = 0;
	virtual IExcelSheet*   addSheet(const wchar_t* sheetName) = 0;
	virtual IExcelSheet*   get_Sheet(int idxSheet) = 0;
	virtual IExcelSheet*   get_Sheet(const wchar_t* sheetName) = 0;
	virtual void           save(const wchar_t* fileName) = 0;
	virtual void           close() = 0;
	virtual int            ReleaseObject() = 0;
};
extern "C" 
{
 IExcelWriter* createExcelWriter();
};
