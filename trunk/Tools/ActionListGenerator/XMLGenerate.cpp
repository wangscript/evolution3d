#include "stdafx.h"
#include "ExcelWriter.h"

USING_NS_XEVOL3D;

struct NewItem
{
	std::wstring m_Name;
	std::wstring m_Color;
	std::wstring m_Shape;
	std::wstring m_NOM;
	std::wstring m_Pos;//位置，是左中还是右
};


int main()
{
	std::wstring excelFile  = _XEVOL_ABSPATH_(L"动作列表.xls");
	std::wstring excelFilex = _XEVOL_ABSPATH_(L"动作列表.xlsx");
	IExcelWriter* pExcel = createExcelWriter();

	if(pExcel->init(excelFile.c_str() , false ) == false)
	{
         pExcel->ReleaseObject();
		 pExcel = NULL;
	}

    pExcel = createExcelWriter();
    if(pExcel->init(excelFilex.c_str() , false ) == false)
    {
        pExcel->ReleaseObject();
        pExcel = NULL;
    }

    if(pExcel == NULL)
	{
		wchar_t file_name[512]={0};
		OPENFILENAMEW ofn; 
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.hwndOwner = ::GetActiveWindow();
		ofn.lpstrFile = file_name;
		ofn.nMaxFile = 512;
		ofn.lpstrFilter = L"Excel文件(*.xlsx;xls)\0*.xlsx;xls\0所有文件(*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.lpstrDefExt = L"xlsx";
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if(GetOpenFileNameW(&ofn) == FALSE)
		{
			return 1;
		}

		excelFilex = file_name;
		pExcel = createExcelWriter();
		if(pExcel->init(excelFilex.c_str() , false ) == false)
		{
			pExcel->ReleaseObject();
			pExcel = NULL;
		}
	}


    IExcelSheet* pActioListSheet = NULL;
    pActioListSheet = pExcel->get_Sheet((int)1);
    if(pActioListSheet == NULL)
    {
        XSAFE_RELEASEOBJECT(pExcel);
        return 1;
    }

    xXmlDocument doc;
	doc.load(_XEVOL_ABSPATH_(L".\\动作列表.xml"));
    xXmlNode* pRootNode =doc.root();
    if(pRootNode == NULL)
    {
        pRootNode = doc.insertNode(L"所有动作");
    }
    int nBlankLine  = 0;
    int iLine = 2;
    std::wstring lastMaxFile = L"";
    while(1)
    {
        wchar_t startFrame[256] = {0};
        wchar_t endFrame  [256] = {0};
        wchar_t actionName[256] = {0};
        wchar_t maxFile   [256] = {0};
        wchar_t actionTime[256] = {0};
        pActioListSheet->get_Value(1 , iLine , startFrame, 256);
        pActioListSheet->get_Value(2 , iLine , endFrame  , 256);
        pActioListSheet->get_Value(3 , iLine , actionName, 256);
        pActioListSheet->get_Value(4 , iLine , maxFile   , 256);
        pActioListSheet->get_Value(5 , iLine , actionTime, 256);
        iLine ++;
        nBlankLine ++;
        if(wcslen_x(startFrame) > 0 )
        {
            nBlankLine = 0;
        }
        if(nBlankLine > 10)
            break;

        if(wcslen_x(maxFile) != 0)
        {
            lastMaxFile = maxFile;
        }

        if( wcslen_x(startFrame) == 0 || wcslen_x(endFrame)  == 0  || wcslen_x(actionName) == 0  )
        {
            continue;
        }

        xXmlNode* pActionNode = pRootNode->insertNode(L"动作");
        pActionNode->setValue(L"StartFrame" , startFrame);
        pActionNode->setValue(L"EndFrame"   , endFrame  );
        pActionNode->setValue(L"ActionName" , actionName);
        pActionNode->setValue(L"MaxFile"    , lastMaxFile.c_str() );
        if(wcslen_x(actionTime) > 0)
        {
            pActionNode->setValue(L"DurTime" , actionTime);
        }
         

    }

	
    doc.save(_XEVOL_ABSPATH_(L".\\动作列表.xml"));
    XSAFE_RELEASEOBJECT(pActioListSheet);
	pExcel->close();
    XSAFE_RELEASEOBJECT(pExcel);
    return 0;

}