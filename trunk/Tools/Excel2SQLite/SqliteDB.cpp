#include "stdafx.h"
#include "sqlite3.h"
#include "SqliteDB.h"

wchar_t* convertConstCharToUnicode(const char* psz)
{
	static wchar_t szOut[1024];

	memset(szOut, 0, sizeof(szOut));
    if(psz == NULL) return szOut;
	MultiByteToWideChar(CP_UTF8, 0, psz, strlen(psz), szOut, sizeof(szOut));
	return szOut;
}

char* convertUnicodeToUTF8(const wchar_t* psz , char* szOut , int len)
{

    ::WideCharToMultiByte( CP_UTF8, 0, psz, wcslen( psz ), szOut, len, NULL, NULL );
    return szOut;
}

SqliteRecordset::SqliteRecordset()
	: m_pStmt		(NULL)
	, m_bFinished	(false)
	, m_pFNTIMap	(NULL)
{
}

SqliteRecordset::~SqliteRecordset()
{
	Clear();
	if (m_pFNTIMap)
		free(m_pFNTIMap);
}

void SqliteRecordset::Clear()
{
	if (m_pStmt)
	{
		sqlite3_finalize((sqlite3_stmt*)m_pStmt);
		m_pStmt = NULL;
	}

	m_nNumOfCols = 0;
	m_bFinished = false;
}

void SqliteRecordset::MapFieldNameToIndex()
{
	sqlite3_stmt* pStmt = (sqlite3_stmt*)m_pStmt;

	m_nNumOfCols = sqlite3_column_count(pStmt);
	if (m_pFNTIMap)
		free(m_pFNTIMap);
	m_pFNTIMap = (FieldName*)malloc(sizeof(FieldName) * m_nNumOfCols);

	for(unsigned i = 0; i < m_nNumOfCols; ++ i)
	{
		FieldName& field = m_pFNTIMap[i];
		memset(&field, 0, sizeof(field));
		wcscpy(field.szFieldName, convertConstCharToUnicode(sqlite3_column_name(pStmt, i)));
	}
}

bool SqliteRecordset::Next()
{
	if (m_bFinished)
		return false;

	//执行一次
	sqlite3_stmt* pStmt = (sqlite3_stmt*)m_pStmt;
	int rc = SQLITE_BUSY;
	while(rc == SQLITE_BUSY)
	{
		rc = sqlite3_step(pStmt);
	}

	if (rc == SQLITE_DONE)
	{
		m_bFinished = true;
		return false;
	}

	//得到一行数据
	if (rc != SQLITE_ROW)
		return false;

	return true;
}

void SqliteRecordset::Reset()
{
	m_bFinished = false;
	sqlite3_reset((sqlite3_stmt*)m_pStmt);
}

int SqliteRecordset::MapFieldName(const wchar_t* pName) const
{
	int iIndex;
	for(iIndex = 0; iIndex < (int)m_nNumOfCols; ++ iIndex)
	{
		const wchar_t* pCompareName = m_pFNTIMap[iIndex].szFieldName;
		if (pCompareName[0] == pName[0] && wcsicmp(pCompareName, pName) == 0)
			return iIndex;
	}

	return -1;
}

SqliteRecordset::FieldValue SqliteRecordset::GetValueByIndex(int iIndex)
{
	FieldValue fv;
	memset(&fv, 0, sizeof(FieldValue));

	if (iIndex >= (int)m_nNumOfCols)
		return fv;

	GetColumnValue(iIndex, fv);
	return fv;
}

SqliteRecordset::FieldValue SqliteRecordset::GetValueByName(const wchar_t* pColumnName)
{
	FieldValue fv;
	memset(&fv, 0, sizeof(FieldValue));

	int iIndex = MapFieldName(pColumnName);
	if (iIndex != -1)
		return GetValueByIndex(iIndex);

	GetColumnValue(iIndex, fv);
	return fv;
}

void SqliteRecordset::GetColumnValue(int iIndex, FieldValue& _fv)
{
	FieldValue& fv = _fv;
	sqlite3_stmt* pStmt = (sqlite3_stmt*)m_pStmt;

	fv.kType = (ColumnType)sqlite3_column_type(pStmt, iIndex);
	fv.nValLength = sqlite3_column_bytes(pStmt, iIndex);

	switch(fv.kType)
	{
	case kCTInteger:
		if (fv.nValLength <= 4)
			fv.iVal = sqlite3_column_int(pStmt, iIndex);
		else
			fv.iVal64 = sqlite3_column_int64(pStmt, iIndex);
		break;

	case kCTDouble:
		fv.dVal = sqlite3_column_double(pStmt, iIndex);
		break;

	case kCTText:
		fv.pText = (const char*)sqlite3_column_text(pStmt, iIndex);
		break;

	case kCTBlob:
		fv.pBinData = sqlite3_column_blob(pStmt, iIndex);
		break;
	}
}

const char* SqliteRecordset::GetText(int iIndex) const
{
	if (iIndex >= (int)m_nNumOfCols)
		return NULL;

	return (const char*)sqlite3_column_text((sqlite3_stmt*)m_pStmt, iIndex);
}

 const wchar_t* SqliteRecordset::FiledType(const wchar_t* pColumnName)
 {
     int iIndex = MapFieldName(pColumnName);
     if (iIndex == -1)
         return NULL;

     return (const wchar_t*)convertConstCharToUnicode( (const char *)sqlite3_column_decltype((sqlite3_stmt*)m_pStmt, iIndex));
 }

const char* SqliteRecordset::GetText(const wchar_t* pColumnName) const
{
	int iIndex = MapFieldName(pColumnName);
	if (iIndex == -1)
		return NULL;

	return (const char*)sqlite3_column_text((sqlite3_stmt*)m_pStmt, iIndex);
}

int SqliteRecordset::GetInt(int iIndex) const
{
	if (iIndex >= (int)m_nNumOfCols)
		return 0;

	return sqlite3_column_int((sqlite3_stmt*)m_pStmt, iIndex);
}

int SqliteRecordset::GetInt(const wchar_t* pColumnName) const
{
	int iIndex = MapFieldName(pColumnName);
	if (iIndex == -1)
		return 0;

	return sqlite3_column_int((sqlite3_stmt*)m_pStmt, iIndex);
}

double SqliteRecordset::GetDouble(int iIndex) const
{
	if (iIndex >= (int)m_nNumOfCols)
		return 0;

	return sqlite3_column_double((sqlite3_stmt*)m_pStmt, iIndex);
}

double SqliteRecordset::GetDouble(const wchar_t* pColumnName) const
{
	int iIndex = MapFieldName(pColumnName);
	if (iIndex == -1)
		return 0;

	return sqlite3_column_double((sqlite3_stmt*)m_pStmt, iIndex);
}

SqliteRecordset::FieldInfo SqliteRecordset::GetFiledInfoByIndex(int iColumn) const
{
	FieldInfo fi;
	memset(&fi, 0, sizeof(FieldInfo));

	if (iColumn >= (int)m_nNumOfCols)
		return fi;

	sqlite3_stmt* pStmt = (sqlite3_stmt*)m_pStmt;
	fi.kType = (ColumnType)sqlite3_column_type(pStmt, iColumn);
	fi.nValLength = sqlite3_column_bytes(pStmt, iColumn);
	fi.pFieldName = sqlite3_column_name(pStmt, iColumn);
	fi.pFieldDecl = sqlite3_column_decltype(pStmt, iColumn);

	return fi;
}

SqliteRecordset::FieldInfo SqliteRecordset::GetFiledInfoByName(const wchar_t* pColumnName) const
{
	FieldInfo fi;
	memset(&fi, 0, sizeof(FieldInfo));

	int iColumn = MapFieldName(pColumnName);
	if (iColumn == -1)
		return fi;

	return GetFiledInfoByIndex(iColumn);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
SqliteDB::SqliteDB()
: m_pDB		(NULL)
{
}

SqliteDB::~SqliteDB()
{
	Close();
}

void SqliteDB::Close()
{
	if (m_pDB)
	{
		sqlite3_close((sqlite3*)m_pDB);
		m_pDB = NULL;
	}
}

bool SqliteDB::OpenDatabase(const wchar_t* pDBFileName)
{
	if (m_pDB)
	{
		sqlite3_close((sqlite3*)m_pDB);
		m_pDB = NULL;
	}

	char _fileName[512] = {0};
    convertUnicodeToUTF8(pDBFileName , _fileName , 512);

	if (sqlite3_open( _fileName , (sqlite3**)&m_pDB) != SQLITE_OK)
	{
		m_pDB = NULL;
		return false;
	}

	return true;
}

const wchar_t* SqliteDB::GetLastErrorMsg() const
{
	if (m_pDB)
		return convertConstCharToUnicode(sqlite3_errmsg((sqlite3*)m_pDB));

	return NULL;
}

bool SqliteDB::Execute(const wchar_t* pszSql)
{
	char _sourceSql[4096] = {0};
    convertUnicodeToUTF8(pszSql , _sourceSql , 4096);
	const char* sourceSql = _sourceSql;

	int rc = SQLITE_OK, nRetry = 0;
	const char* szLeftover;
	sqlite3_stmt* pStmt;

	while((rc == SQLITE_OK || (rc == SQLITE_SCHEMA && (++ nRetry < 2))) && pszSql[0])
	{
		//编译SQL语句
		rc = sqlite3_prepare((sqlite3*)m_pDB, sourceSql ? sourceSql : szLeftover, -1, &pStmt, &szLeftover);
		sourceSql = NULL;

		if (rc != SQLITE_OK)
		{
			if (pStmt)
				sqlite3_finalize(pStmt);
			continue;
		}
		if (!pStmt)
			continue;

		//执行
		rc = sqlite3_step(pStmt);
		rc = sqlite3_finalize(pStmt);
		if (rc != SQLITE_OK)
			break;
		else
			return true;
	}

	return false;
}

bool SqliteDB::Query(const wchar_t* pszSql, SqliteRecordset& res)
{
	char _sourceSql[4096] = {0};
	convertUnicodeToUTF8(pszSql , _sourceSql , 4096);

	const char* sourceSql = _sourceSql;
	int rc = SQLITE_OK, nRetry = 0;
	const char* szLeftover;
	sqlite3_stmt* pStmt;

	while((rc == SQLITE_OK || (rc == SQLITE_SCHEMA && (++ nRetry < 2))) && pszSql[0])
	{
		//编译SQL语句
		rc = sqlite3_prepare((sqlite3*)m_pDB, sourceSql ? sourceSql : szLeftover, -1, &pStmt, &szLeftover);
		sourceSql = NULL;

		if (rc != SQLITE_OK)
		{
			if (pStmt)
				sqlite3_finalize(pStmt);
			continue;
		}
		if (!pStmt)
			continue;

		//赋值给结果集合，就可以返回了
		res.Clear();
		res.m_pStmt = pStmt;
		res.m_pDB = this;

		//初始化并映射列信息
		res.MapFieldNameToIndex();

		return true;
	}

	return false;
}

bool SqliteDB::execSQL(const wchar_t* sql)
{
	char _sourceSql[4096] = {0};
	convertUnicodeToUTF8(sql , _sourceSql , 4096);
	char* zErrMsg = 0;
	if(SQLITE_OK!=sqlite3_exec((sqlite3*)m_pDB,_sourceSql,0,0,&zErrMsg))
	{
		::MessageBoxA(NULL,sqlite3_errmsg((sqlite3*)m_pDB),NULL,MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool SqliteDB::BeginTransaction()
{
	 return execSQL( L"begin transaction;"); 
}
bool SqliteDB::CommitTransaction()
{
	return execSQL( L"commit transaction;"); 
}
bool SqliteDB::RollbackTransaction()
{
	return execSQL( L"rollback transaction;");
}