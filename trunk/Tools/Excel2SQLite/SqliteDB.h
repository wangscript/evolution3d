#pragma once

class SqliteRecordset
{
	friend class SqliteDB;
public:
	//!列类型
	enum ColumnType
	{
		kCTKnown	= 0,		//未知
		kCTInteger  = 1,		//整型值
		kCTDouble   = 2,		//浮点数型
		kCTText     = 3,		//文本型
		kCTBlob     = 4,		//二进制数据
		kCTNull     = 5			//NULL列
	} ;

	//!单元值
	struct FieldValue
	{
		ColumnType	kType;		//列类型（其实上就是值类型）
		unsigned		nValLength;	//值长度

		//!可能的值的联合
		union
		{
			int			iVal;
			__int64		iVal64;
			const char*	pText;
			double		dVal;
			const void*	pBinData;
		} ;
	} ;

	//!列信息
	struct FieldInfo
	{
		ColumnType	kType;
		unsigned	nValLength;
		const char	*pFieldName;
		const char	*pFieldDecl;
	} ;	

	//!列名称
	struct FieldName
	{
		wchar_t	szFieldName[256];
	} ;

public:
	SqliteRecordset();
	~SqliteRecordset();

	void Clear();
	bool Next();
	void Reset();

	FieldValue GetValueByIndex(int iIndex);
	FieldValue GetValueByName(const wchar_t* pColumnName);

	const char* GetText(int iIndex) const;
	const char* GetText(const wchar_t* pColumnName) const;
	int GetInt(int iIndex) const;
	int GetInt(const wchar_t* pColumnName) const;
	double GetDouble(int iIndex) const;
	double GetDouble(const wchar_t* pColumnName) const;

	FieldInfo GetFiledInfoByIndex(int iIndex) const;
	FieldInfo GetFiledInfoByName(const wchar_t* pColumnName) const;

	int MapFieldName(const wchar_t* pName) const;
	void GetColumnValue(int iIndex, FieldValue& fv);
    const wchar_t* FiledType(const wchar_t* name);

	inline unsigned GetNumOfColumns() const
	{
		return m_nNumOfCols;
	}
	inline SqliteDB* GetDB() const
	{
		return m_pDB;
	}

private:
	void MapFieldNameToIndex();		

private:
	void*			m_pStmt;
	SqliteDB*		m_pDB;
	unsigned		m_nNumOfCols;
	FieldName		*m_pFNTIMap;
    FieldName       *m_pFieldType;
	bool			m_bFinished;
} ;

class SqliteDB
{
public:
	typedef void (*fndefAsyncExecute)(void* pParam, const char* pSql, bool bSuccess);
	typedef void (*fndefAsyncQuery)(void* pParam, const char* pSql, SqliteRecordset* pRes);

public:
	SqliteDB();
	~SqliteDB();

	bool OpenDatabase(const wchar_t* pDBFileName);
	const wchar_t* GetLastErrorMsg() const;
	void Close();
    
	bool BeginTransaction(); 
	bool CommitTransaction();
	bool RollbackTransaction();

	bool execSQL(const wchar_t* sql);
	bool Execute(const wchar_t* pszSql);
	bool Query(const wchar_t* pszSql, SqliteRecordset& res);
	

	__int64 GetLastInsertID() const;

private:
	void*				m_pDB;
} ;