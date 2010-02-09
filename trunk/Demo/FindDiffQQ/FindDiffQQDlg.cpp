// FindDiffQQDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindDiffQQ.h"
#include "FindDiffQQDlg.h"
#include "xILImage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define TRAIL_VERSION
#define TRAIL_TIME    2

static bool bAuto  = true;
extern bool bUserLogin;

unsigned char* SnapWindow(HWND hQQGameWnd , HWND hDesktopWnd,xImageSize& imgSize)
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;
	HDC hdc  =  GetWindowDC(hDesktopWnd); //// GetDC(hWnd);


	DWORD erroCode = GetLastError();
	RECT rctQQWnd;
	GetWindowRect(hQQGameWnd,&rctQQWnd);
	int w = rctQQWnd.right - rctQQWnd.left;
	int h = rctQQWnd.bottom - rctQQWnd.top; 

	imgSize.h        = h;
	imgSize.w        = w;
	imgSize.pitch    = w * 4;
	imgSize.fmt      = PIXELFORMAT_R8G8B8A8;


	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= w;							// Power Of Two Width
	bi.bmiHeader.biHeight		= h;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	unsigned char* pBits = 0;

	hdcTemp = CreateCompatibleDC(hdc);//GetDC(0));								// Create The Windows Compatible Device Context
	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
	SelectObject(hdcTemp, hbmpTemp);
	// Select Handle To Our Temp DC And Our Temp Bitmap Object
	//::PrintWindow(hWnd , hdcTemp , 0);
	BitBlt(hdcTemp,0,0,w,h,hdc,rctQQWnd.left,rctQQWnd.top,SRCCOPY|CAPTUREBLT);
	//StretchBlt(hdcTemp,0,0,w,h,hdc,0,0,rct.right - rct.left, rct.bottom - rct.top,SRCCOPY);
	unsigned char* pData = new unsigned char[w  * h * 4];

	for(long y = 0;y<h;y++)
	{
		for(long x = 0; x < w; x ++ )				// Loop Through All Of The Pixels
		{
			pData[4 * x + 0 + y * w  * 4]	= pBits[4 * x + 0 + y * w  * 4];
			pData[4 * x + 1 + y * w  * 4]	= pBits[4 * x + 1 + y * w  * 4];
			pData[4 * x + 2 + y * w  * 4]	= pBits[4 * x + 2 + y * w  * 4];
			pData[4 * x + 3 + y * w  * 4]   = 255;//pBits[4 * x + 3 + y * w  * 4];//255;
		}
	}

	DeleteObject(hbmpTemp);												// Delete The Object
	DeleteDC(hdcTemp);													// Delete The Device Context
	ReleaseDC(hDesktopWnd,hdc);

	return pData;

}

unsigned char* CreateDIBBMP( int w, int h, HDC hdcTemp, HBITMAP &hbmpTemp )
{
	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= w;							// Power Of Two Width
	bi.bmiHeader.biHeight		= h;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	unsigned char* pBits = 0;
	// Create The Windows Compatible Device Context
	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
	return pBits;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CFindDiffQQDlg dialog




CFindDiffQQDlg::CFindDiffQQDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDiffQQDlg::IDD, pParent)
{
	m_hQQGameWnd = NULL;
	m_hIcon = ::LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MAINFRAME) );
}

void CFindDiffQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFindDiffQQDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_ATUO, &CFindDiffQQDlg::OnAtuo)
	ON_COMMAND(ID_STOP, &CFindDiffQQDlg::OnStop)
	ON_UPDATE_COMMAND_UI(ID_ATUO, &CFindDiffQQDlg::OnUpdateAtuo)
	ON_UPDATE_COMMAND_UI(ID_STOP, &CFindDiffQQDlg::OnUpdateStop)
END_MESSAGE_MAP()


// CFindDiffQQDlg message handlers
long iStartTime = 0;
BOOL CFindDiffQQDlg::OnInitDialog()
{

	iStartTime = GetTickCount();
	X1 = 10  ;
	Y1 = 173;
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	SetTimer(0,20,NULL);
	//toggleAuto();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindDiffQQDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindDiffQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindDiffQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

using namespace  XEvol3D;

float GetEng(xColor_3ub* Pixel , xImageSize _size , int left , int right , int top , int bottom , int delLeft)
{
	float e = 0.0f;
	for(int y = top  ; y < bottom ; y ++)
	{
		for(int x = left ; x < right ; x ++)
		{
			xColor_3ub* pixel1 = Pixel + _size.w * (_size.h - y) + x ;
			xColor_3ub* pixel2 = Pixel + _size.w * (_size.h - y) + x + delLeft;
			xColor_3ub  del( (unsigned int8)abs((int)pixel1->r -  (int)pixel2->r)  , 
				(unsigned int8)abs((int)pixel1->g -  (int)pixel2->g)  , 
				(unsigned int8)abs((int)pixel1->b -  (int)pixel2->b) );//  , 255);
			e +=  del.abs() / 256.0f;
		}
	}
	return e;
}

bool CFindDiffQQDlg::GrabDataFromClipboard(xILImage& image)
{
	if(FALSE == OpenClipboard() )
	{
		return false;
	}
	if (!IsClipboardFormatAvailable(CF_DIB))
	{
		CloseClipboard();
		return false;
	}
	HANDLE BitMapDataHandle = GetClipboardData(CF_DIB);
	BITMAPINFO* pBitmapInfo = (BITMAPINFO*)GlobalLock(BitMapDataHandle);
	unsigned char* pImageData = (unsigned char*)pBitmapInfo;
	pImageData += sizeof(BITMAPINFO);
	ePIXEL_FORMAT fmt = ePIXEL_FORMAT::PIXELFORMAT_R8G8B8A8;
	if(pBitmapInfo->bmiHeader.biBitCount == 24)
		fmt = PIXELFORMAT_R8G8B8;
	image.data( (const char *)pImageData,pBitmapInfo->bmiHeader.biWidth,pBitmapInfo->bmiHeader.biHeight , fmt);
	GlobalUnlock(BitMapDataHandle);
	CloseClipboard();
	return true;
}
void CFindDiffQQDlg::FindBySnapWindow()
{
    xILImage image ;
	xImageSize _size;
	HWND hWnd = ::GetDesktopWindow();
	unsigned char* pImageData = SnapWindow(m_hQQGameWnd , hWnd,_size);

	image.data( (const char *)pImageData,_size.w,_size.h , _size.fmt);
	delete [] pImageData;
	pImageData = NULL;
	//image.save(L"C:\\Users\\stanlylee\\AppData\\out.bmp");
	image.convert(PIXELFORMAT_R8G8B8);
	image.scale(800,600);
	return DoFindDiff(image);

}

void CFindDiffQQDlg::FindByClipBoard()
{
	xILImage image ;
	if(false == GrabDataFromClipboard(image) )
	{
		CDC* pDC = GetDC();
		pDC->TextOut(10,10 ,TEXT("QQ找茬游戏助手") ) ;
		pDC->TextOut(10,30 ,TEXT("你可以在该窗口上直接点击白色区域") ) ;
		pDC->TextOut(10,50 ,TEXT("也可以在游戏屏幕上点击白色区域") ) ;

#ifdef TRAIL_VERSION
		pDC->TextOut(10,70 ,TEXT("试用版本 每次运行可以试用3分钟") ) ;
#endif

		ReleaseDC(pDC);
		return ;
	}

	image.convert(PIXELFORMAT_R8G8B8);
	image.scale(800,600);
	return DoFindDiff(image);
}

void CFindDiffQQDlg::DoFindDiff(xILImage& image)
{

	int X2;
	X1 = 10  ;
	Y1 = 173;
	X2 = 400 ;
	xColor_3ub* Pixel = (xColor_3ub*)image.data();
	xImageSize _size = image.getSize();

	///找到能量最小的。
	int MinX2 = 400 - 10;
	float minEngine = 800 * 800 * 10000;
	for(X2 = 400 - 3 ; X2 < 400 + 10 ; X2 ++)
	{
		CRect _Rect1(X1 ,177  , 396 , 463);
		CRect _Rect2(X2 ,177  , 783  ,472);
		float e =  GetEng(Pixel , _size , _Rect1.left , _Rect1.right , _Rect1.top , _Rect1.bottom , (_Rect2.left - _Rect1.left) );
		if(e < minEngine)
		{
			minEngine = e ;
			MinX2 = X2;
		}
	}
	
	X2 = MinX2 ;
    
	CRect Rect1(X1 ,173  , 396 , 473);
	CRect Rect2(X2 ,173  , 783  ,472);
	for(int y = Rect1.top  ; y < Rect1.bottom ; y ++)
	{
		for(int x = Rect1.left ; x < Rect1.right ; x ++)
		{
             xColor_3ub* pixel1 = Pixel + _size.w * (_size.h - y) + x ;
			 xColor_3ub* pixel2 = Pixel + _size.w * (_size.h - y) + x + (Rect2.left - Rect1.left);
			 xColor_3ub  del( (unsigned int8)abs((int)pixel1->r -  (int)pixel2->r)  , 
				              (unsigned int8)abs((int)pixel1->g -  (int)pixel2->g)  , 
							  (unsigned int8)abs((int)pixel1->b -  (int)pixel2->b) );//  , 255);

			 if( del.abs() > 2)
			 {
				 pixel1->r = 255;//- pixel2->r;
				 pixel1->g = 255;//- pixel2->g;
				 pixel1->b = 255;//- pixel2->b;
			 }
			 else
			 {
				 pixel1->r *= 0.5;//- pixel2->r;
				 pixel1->g *= 0.5;//- pixel2->g;
				 pixel1->b *= 0.5;//- pixel2->b;
			 }

		}
	}
	

	//绘制到窗口上
	CDC* pDC = GetDC();
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC( pDC );

	BITMAPINFO binfo;
	memset(&binfo,0,sizeof(binfo) );
	binfo.bmiHeader.biBitCount = 24;
	binfo.bmiHeader.biHeight = _size.h ;
	binfo.bmiHeader.biWidth = _size.w;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSize = sizeof(binfo.bmiHeader);
	void* pData = NULL;
	HBITMAP hBitmap = CreateDIBSection(m_MemDC.GetSafeHdc() , &binfo,DIB_RGB_COLORS, &pData,NULL,0);
	memcpy(pData , Pixel , _size.w * _size.h * 3);
	m_MemDC.SelectObject(hBitmap);
	pDC->BitBlt(0,0,_size.w/2 ,Rect1.Height() , &m_MemDC,X1,Rect1.top,SRCCOPY);
	m_MemDC.DeleteDC();
	DeleteObject(hBitmap);
 
	ReleaseDC(pDC);

	// TODO: Add your control notification handler code here
}




void CFindDiffQQDlg::toggleAuto()
{
	// TODO: Add your control notification handler code here
}



BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam )
{
	CFindDiffQQDlg* pDlg = (CFindDiffQQDlg*)lParam;
	return pDlg->FindAWindow(hwnd);
}

BOOL CFindDiffQQDlg::FindAWindow(HWND hwnd)
{
	wchar_t windCap[512]={0};
	::GetWindowTextW(hwnd,windCap,512);
	wstring strWndCap = windCap;
	if(strWndCap.find(L"大家来找茬") != wstring::npos)
	{
		m_hQQGameWnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

void CFindDiffQQDlg::OnIdle()
{
#ifdef TRAIL_VERSION
	if(GetTickCount() - iStartTime > 1000 * 60 * TRAIL_TIME)
	{
		static bool bInit = false;
		if(bInit == false)
		{
			bInit = true;
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(rect);
            pDC->Rectangle(rect);
			pDC->TextOut(10,rect.Height()/2 ,TEXT("试用版本 每次运行可以试用3分钟") ) ;
            ReleaseDC(pDC);
		}
		return ;
	}
#endif

	if(bAuto == false)
	{
		FindByClipBoard();
		return ;
	}
	// TODO: Add your message handler code here and/or call default
	if(m_hQQGameWnd == NULL || IsWindow(m_hQQGameWnd) == FALSE)
	{
		FindByClipBoard();
		m_hQQGameWnd = NULL;
		EnumWindows(EnumWindowsProc,(LPARAM)this);
	}
	else
	{
		FindBySnapWindow();
	}
	Sleep(10);
}

void CFindDiffQQDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( ::GetForegroundWindow()  != m_hWnd)
	{
		OnIdle();
	}
	CDialog::OnTimer(nIDEvent);
}

static bool bOnCapture = false;
void CFindDiffQQDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

    bOnCapture = true;
	SetCapture();
	HCURSOR hCursor = LoadCursor(NULL  , IDC_HAND);
	SetCursor(hCursor);
	CDialog::OnRButtonDown(nFlags, point);
}

void CFindDiffQQDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnMouseMove(nFlags, point);
}

void CFindDiffQQDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// This feature requires Windows 2000 or greater.
	// The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
	// TODO: Add your message handler code here and/or call default
	if(bOnCapture)
	{
		ClientToScreen(&point);
		CWnd* pWnd = WindowFromPoint(point);
		if(pWnd)
		{
			while( pWnd->GetParent() )
			{
				pWnd = pWnd->GetParent();
			}

			CString strWindText ;
			pWnd->GetWindowText(strWindText);
			HWND hWND = pWnd->GetSafeHwnd();
			m_hQQGameWnd = hWND;
		}
	}
    HCURSOR hCursor = LoadCursor( NULL  , IDC_ARROW);
	SetCursor(hCursor);
	bOnCapture = false;
	ReleaseCapture();
	CDialog::OnRButtonUp(nFlags, point);
}

void CFindDiffQQDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
    PostQuitMessage(0);
	CDialog::OnClose();
}

void CFindDiffQQDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_hQQGameWnd|| IsWindow(m_hQQGameWnd))
	{
		LONG lParam = (point.y + Y1) << 16 | ( point.x + X1);
		::PostMessage(m_hQQGameWnd,WM_LBUTTONDOWN,MK_LBUTTON , lParam);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CFindDiffQQDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_hQQGameWnd|| IsWindow(m_hQQGameWnd))
	{
		LONG lParam = (point.y + Y1) << 16 | ( point.x + X1);
		::PostMessage(m_hQQGameWnd,WM_LBUTTONUP,MK_LBUTTON , lParam);
	}
	CDialog::OnLButtonUp(nFlags, point);
}
void CFindDiffQQDlg::OnAtuo()
{
	bAuto = !bAuto;
	// TODO: Add your command handler code here
}

void CFindDiffQQDlg::OnStop()
{
	bAuto = !bAuto;
	// TODO: Add your command handler code here
}

void CFindDiffQQDlg::OnUpdateAtuo(CCmdUI *pCmdUI)
{
	if(bAuto)
	   pCmdUI->SetText(L"停止");
	else
	   pCmdUI->SetText(L"启动");
	// TODO: Add your command update UI handler code here
}

void CFindDiffQQDlg::OnUpdateStop(CCmdUI *pCmdUI)
{
		pCmdUI->Enable(bAuto);
	// TODO: Add your command update UI handler code here
}
