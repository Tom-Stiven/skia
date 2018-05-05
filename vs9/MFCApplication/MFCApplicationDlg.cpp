
// MFCApplicationDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"

#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkImageDecoder.h"
#include "SkTypeface.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HBITMAP Pixel2Bitmap(void* data, int cx, int cy) {
    BITMAPINFO bmi;
    ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = cx;
    bmi.bmiHeader.biHeight = -cy;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = cx * cy * 4;
    bmi.bmiHeader.biClrUsed = 0;

    HDC hdc = ::GetDC(NULL);

    void* bits = nullptr;
    HBITMAP hbitmap = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS,
        (void**)&bits, NULL, 0);
    if (hbitmap != nullptr) {
        memcpy(bits, data, cx * cy * 4);
    }
    ::ReleaseDC(NULL, hdc);
    return hbitmap;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationDlg 对话框



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCApplicationDlg 消息处理程序

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    //ModifyStyleEx(0, WS_EX_LAYERED);
    //OnPaint();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void drawPng(SkCanvas& canvas, char* path) {
    do {
        SkBitmap bitmap;
        SkImageDecoder::DecodeFile(path, &bitmap);
        if (bitmap.isNull())
            break;
        int w = bitmap.width(), h = bitmap.height();
        canvas.drawBitmap(bitmap, 0, 0);
    } while (0);
}

void drawText(SkCanvas& canvas) {
    SkRect dest = SkRect::MakeLTRB(20, 120, 150, 250);
    const char kText[] = u8"中华人民共和国", kEllipsis[] = u8"...";

    std::string draw_text(kText);
    SkScalar x = 0, y = 0;

    SkPaint paint1;
    paint1.setColor(SK_ColorBLUE);
    paint1.setStyle(SkPaint::kStroke_Style);
    canvas.drawRect(dest, paint1);

    SkPaint paint;
    paint.setTextEncoding(SkPaint::kUTF8_TextEncoding);
    paint.setAntiAlias(true);
    paint.setLCDRenderText(true);
    paint.setTypeface(SkTypeface::CreateFromName(u8"新宋体", SkTypeface::kItalic));
    paint.setTextSize(20);
    paint.setColor(SK_ColorBLACK);

    SkScalar cx_text = paint.measureText(kText, strlen(kText));
    if (cx_text <= dest.width()) {
        x = dest.left() + (dest.width() - cx_text) / 2;
    } else {
        SkScalar cx_ellipsis = paint.measureText(kEllipsis, strlen(kEllipsis));

        SkScalar left = dest.width() - cx_ellipsis;
        size_t num = paint.breakText(kText, strlen(kText), left);

        draw_text.erase(num);
        draw_text.append(kEllipsis);

        cx_text = paint.measureText(draw_text.c_str(), draw_text.size());
        x = dest.left() + (dest.width() - cx_text) / 2;
    }

    SkPaint::FontMetrics fm;
    paint.getFontMetrics(&fm);
    int h = fm.fTop + fm.fBottom;
    // Top
    y = dest.top() - fm.fTop;
    canvas.drawText(draw_text.c_str(), draw_text.size(), x, y, paint);

    // Center
    y = dest.top() + (dest.height() - h) / 2;
    canvas.drawText(draw_text.c_str(), draw_text.size(), x, y, paint);

    // Bottom
    y = dest.bottom() - fm.fBottom;

    canvas.drawText(draw_text.c_str(), draw_text.size(), x, y, paint);
}

void CMFCApplicationDlg::OnPaint() {
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    int cx = rect.Width(), cy = rect.Height();

    SkBitmap bitmap;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, cx, cy);
    bitmap.allocPixels();
    SkCanvas canvas(bitmap);
    canvas.drawColor(SkColorSetRGB(200, 240, 200));

    drawPng(canvas, "res\\bk.png");
    drawPng(canvas, "res\\plane.png");
    drawText(canvas);

    void* pixels = bitmap.getPixels();
    HBITMAP hbitmap = Pixel2Bitmap(pixels, cx, cy);
    if (hbitmap) {
        CDC mdc;
        mdc.CreateCompatibleDC(&dc);
        mdc.SelectObject(hbitmap);

        //CRect rc;
        //GetWindowRect(&rc);
        //POINT pt_win = { rc.left, rc.top }, pt_src = { 0 };
        //SIZE sz_win = { cx, cy };
        //BLENDFUNCTION bf = { AC_SRC_OVER, 0, AC_SRC_ALPHA, 255 };
        //UpdateLayeredWindow(&dc, &pt_win, &sz_win, &mdc, &pt_src, RGB(0,0,0), &bf, ULW_ALPHA);

        dc.BitBlt(0, 0, cx, cy, &mdc, 0, 0, SRCCOPY);
        DeleteObject(hbitmap);
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
