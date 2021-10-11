#include "SimplePicPrinter.h"

#include "../shared/common.h"
#include <array>

enum {
  ID_CLEAR_BTN,
  ID_DRAW_BTN,
};

struct SPPPositions {
  Position drawBtnPosition;
  Position clearBtnPosition;
  Position printPositon;
};

SPPPositions ComputePositions(int width, int height);

void PrintPicture(const Position& pos, HDC hdc);
void PrintPlus(HDC hdc, RECT rect, bool selected);
void PrintMinus(HDC hdc, RECT rect, bool selected);

SimplePicPrinter::SimplePicPrinter(HWND hwnd, HINSTANCE hinst) 
  : __hwnd(hwnd)
  , __hInst(hinst)
{
	RECT rc;
  GetClientRect(hwnd, &rc);

	auto pos = ComputePositions(rc.right - rc.left, rc.bottom - rc.top);
	__drawButtonHwnd = CreateButton(pos.drawBtnPosition, NULL, ID_DRAW_BTN);
	ShowWindow(__drawButtonHwnd, SW_SHOWNORMAL);
	__clearButtonHwnd = CreateButton(pos.clearBtnPosition, NULL, ID_CLEAR_BTN);
	ShowWindow(__clearButtonHwnd, SW_SHOWNORMAL);
}

void SimplePicPrinter::Command(WPARAM wparam, LPARAM lparam) 
{
	switch (wparam)
	{
		case ID_DRAW_BTN:
			__mode = drawMode;
			InvalidateRect(__hwnd, nullptr, FALSE);
			break;

		case ID_CLEAR_BTN:
			__mode = clearMode;
			InvalidateRect(__hwnd, nullptr, FALSE);
			break;
	}
}

void SetWindowPosition(HWND hwnd, const Position &pos) {
	SetWindowPos(hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}

void SimplePicPrinter::Resize(int width, int height) 
{
	auto p = ComputePositions(width, height);
	SetWindowPosition(__drawButtonHwnd, p.drawBtnPosition);
	SetWindowPosition(__clearButtonHwnd, p.clearBtnPosition);
}

void SimplePicPrinter::Print() 
{
  PAINTSTRUCT ps; 
	BeginPaint(__hwnd, &ps);

  auto hdc = GetDC(__hwnd);
  RECT rc;
  GetClientRect(__hwnd, &rc);
  FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
	
	auto pos = ComputePositions(rc.right - rc.left, 
															rc.bottom - rc.top);
	auto printArea = pos.printPositon;

	if (__mode == drawMode) {
		PrintPicture(printArea, hdc);
	}

  EndPaint(__hwnd, &ps);

	InvalidateRect(__drawButtonHwnd, nullptr, TRUE);
	InvalidateRect(__clearButtonHwnd, nullptr, TRUE);
	UpdateWindow(__drawButtonHwnd);
	UpdateWindow(__clearButtonHwnd);
}

void SimplePicPrinter::DrawItem(LPARAM lparam) 
{
	DRAWITEMSTRUCT* ds = (DRAWITEMSTRUCT*)lparam;
	if (ds->CtlID != ID_DRAW_BTN && ds->CtlID != ID_CLEAR_BTN)
		return;

	switch (ds->CtlID)
	{
		case ID_DRAW_BTN:
		  {
			  RECT rc;
        GetClientRect(__drawButtonHwnd, &rc);
			  PrintPlus(ds->hDC, rc, ds->itemAction == ODA_SELECT);
		  }
		  break;

		case ID_CLEAR_BTN:
		  {
			  RECT rc;
        GetClientRect(__clearButtonHwnd, &rc);
			  PrintMinus(ds->hDC, rc, ds->itemAction == ODA_SELECT);
		  }
		  break;
	}
}

HWND SimplePicPrinter::CreateButton(const Position &pos, LPCWSTR name, int id) const 
{
  return CreateWindow(
    L"button", 
    nullptr, 
    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
    20, 320, 60, 60, 
    __hwnd, 
    (HMENU)id, 
    __hInst, 
    NULL
  );
}

SPPPositions ComputePositions(int clientWidth, int clientHeight) 
{
	//  _ - m         __ - m         _ - m
	// ________________________________
	// | ____________   ____________  |  | - h
	// | |           |  | DrawBtn   | | |   
	// | | Area      |  \___________/ | | - btnHeight
	// | | For       |  ____________  |  | - H
	// | | Printing  |  | ClearBtn  | | |
	// | \__________/   \___________/ | | - btnHeight
	// |______________________________|  | - h

	const int minWidth = 300, minHeight = 300;

	if (clientWidth < minWidth) {
		clientWidth = minWidth;
	}
	if (clientHeight < minHeight) {
		clientHeight = minHeight;
	}

	const int m = 10;
	const int h = 10;
	const int btnHeight = 100;
	const int H = clientHeight - 2 * btnHeight - 2 * h;
	const int afpHeight = clientHeight - 2 * h;
	const int afpWidth = (clientWidth - 3 * m) * 2 / 3;
	const int btnWidth = clientWidth - 3 * m - afpWidth;

	SPPPositions pos;

	pos.printPositon.x = m;
	pos.printPositon.y = h;
	pos.printPositon.w = afpWidth;
	pos.printPositon.h = afpHeight;

	pos.drawBtnPosition.x = m * 2 + afpWidth;
	pos.drawBtnPosition.y = h;
	pos.drawBtnPosition.w = btnWidth;
	pos.drawBtnPosition.h = btnHeight;

	pos.clearBtnPosition.x = m * 2 + afpWidth;
	pos.clearBtnPosition.y = h + H + btnHeight;
	pos.clearBtnPosition.w = btnWidth;
	pos.clearBtnPosition.h = btnHeight;

	return pos;
}

struct PieCoords {
	int x1, y1, x2, y2, x3, y3, x4, y4;
};

void PrintPie(const PieCoords &a, HDC hdc) {
	Pie(hdc, a.x1, a.y1, a.x2, a.y2, a.x3, a.y3, a.x4, a.y4);
}

void PrintRect(const Position &pos, HDC hdc) {
	Rectangle(hdc, pos.x, pos.y, pos.x + pos.w, pos.y + pos.h);
}

void PrintEllipse(const Position& pos, HDC hdc) {
	Ellipse(hdc, pos.x, pos.y, pos.x + pos.w, pos.y + pos.h);
}

void PrintStar(const Position& p, HDC hdc) {
	std::array<POINT, 5> star;

	star[0].x = p.x + p.w * 1 / 5;
	star[0].y = p.y + p.h;

	star[1].x = p.x + p.w / 2;
	star[1].y = p.y;

	star[2].x = p.x + p.w - p.w * 1 / 5;
	star[2].y = p.y + p.h;

	star[3].x = p.x;
	star[3].y = p.y + p.h / 3;

	star[4].x = p.x + p.w;
	star[4].y = p.y + p.h / 3;

	Polygon(hdc, star.data(), star.size());
}

struct Line{int x1, y1, x2, y2;};

void PrintLine(const Line& l, HDC hdc) {
	MoveToEx(hdc, l.x1, l.y1, 0);
	LineTo(hdc, l.x2, l.y2);
}

struct PrintPictureColors {
	SolidPen border;
	SolidPen crownBorder;
	SolidPen trunkBorder;
	SolidPen sunray;
	SolidPen sunBorder;
	SolidPen groundBorder;
	SolidPen starBorder;

	SolidBrush backgroundFill;
	SolidBrush crownFill;
	SolidBrush trunkFill;
	SolidBrush sunFill;
	SolidBrush groundFill;
	SolidBrush starFill;

	PrintPictureColors() 
		: border(1, RGB(0, 0, 0))
		, crownBorder(1, RGB(10, 120, 10))
	  , trunkBorder(1, RGB(100, 50, 3))
	  , sunray(4, RGB(230, 220, 20))
	  , sunBorder(1, RGB(230, 220, 20))
	  , groundBorder(1, RGB(20, 180, 10))
	  , starBorder(1, RGB(120, 190, 200))
	  , backgroundFill(RGB(177, 255, 250))
	  , crownFill(RGB(60, 220, 60))
	  , trunkFill(RGB(134, 70, 13))
	  , sunFill(RGB(230, 255, 30))
	  , groundFill(RGB(30, 255, 10))
	  , starFill(RGB(120, 190, 200))
	{}
};

void PrintPicture(const Position &pos, HDC hdc) 
{
	PrintPictureColors colors;

	auto initialPen = SelectObject(hdc, colors.border);
	auto initialBrush = SelectObject(hdc, colors.backgroundFill);
	Rectangle(hdc, pos.x, pos.y, pos.x + pos.w, pos.y + pos.h);

	Position p{pos.x + 1, pos.y + 1, pos.w - 1, pos.h - 1};

	SelectObject(hdc, colors.trunkBorder);
	SelectObject(hdc, colors.trunkFill);

	Position littleTrunk;
	littleTrunk.x = p.x + p.w * 2 / 10;
	littleTrunk.y = p.y + p.h * 8 / 10;
  littleTrunk.h = p.h - p.h * 8 / 10;
	littleTrunk.w =	p.w / 10;
	PrintRect(littleTrunk, hdc);

	Position bigTrunk;
	bigTrunk.x = p.x + p.w * 7 / 10;
	bigTrunk.y = p.y + p.h * 5 / 10;
  bigTrunk.h = p.h - p.h * 5 / 10;
	bigTrunk.w =	p.w / 10;
	PrintRect(bigTrunk, hdc);

	SelectObject(hdc, colors.crownBorder);
	SelectObject(hdc, colors.crownFill);

	Position littleTreeCrown;
	littleTreeCrown.x = p.x + p.w * 3 / 20;
	littleTreeCrown.y = littleTrunk.y - p.h / 10;
  littleTreeCrown.h = p.h * 2 / 10;
	littleTreeCrown.w =	p.w * 2 / 10;
	PrintEllipse(littleTreeCrown, hdc);

	Position bigTreeCrown;
	bigTreeCrown.x = bigTrunk.x - p.w / 20;
	bigTreeCrown.y = bigTrunk.y - p.h * 2 / 10;
  bigTreeCrown.h = p.h * 4 / 10;
	bigTreeCrown.w = p.w * 2 / 10;
	PrintEllipse(bigTreeCrown, hdc);

	SelectObject(hdc, colors.sunray);

	Line sunRay;
	sunRay.x1 = p.x + p.w;
	sunRay.y1 = p.y;

	sunRay.x2 = p.x + p.w - p.w * 3 / 10;
	sunRay.y2 = p.y + p.h / 10;
	PrintLine(sunRay, hdc);

	sunRay.x2 = p.x + p.w - p.w * 5 / 20;
	sunRay.y2 = p.y + p.h * 2 / 10;
	PrintLine(sunRay, hdc);

	sunRay.x2 = p.x + p.w - p.w * 2 / 10;
	sunRay.y2 = p.y + p.h * 5 / 20;
	PrintLine(sunRay, hdc);

	sunRay.x2 = p.x + p.w - p.w / 10;
	sunRay.y2 = p.y + p.h * 3 / 10;
	PrintLine(sunRay, hdc);
	
	SelectObject(hdc, colors.sunBorder);
	SelectObject(hdc, colors.sunFill);

	PieCoords sun;
	sun.x1 = p.x + p.w - p.w * 2 / 10;
	sun.y1 = p.y - p.h * 2 / 10;
	sun.x2 = p.x + p.w + p.w * 2 / 10;
	sun.y2 = p.y + p.h * 2 / 10;
	sun.x3 = p.x + p.w - p.w * 2 / 10;
	sun.y3 = p.y;
	sun.x4 = p.x + p.w;
	sun.y4 = p.y + p.h * 2 / 10;
	PrintPie(sun, hdc);

	SelectObject(hdc, colors.groundBorder);
	SelectObject(hdc, colors.groundFill);

	PieCoords ground;
	ground.x1 = p.x;
	ground.y1 = p.y + p.h - p.h * 1 / 10;
	ground.x2 = p.x + p.w;
	ground.y2 = p.y + p.h + p.h * 1 / 10;
	ground.x3 = p.x + p.w;
	ground.y3 = p.y + p.h;
	ground.x4 = p.x;
	ground.y4 = p.y + p.h;
	PrintPie(ground, hdc);

	SelectObject(hdc, colors.starBorder);
	SelectObject(hdc, colors.starFill);

	Position star;
	star.x = p.x + p.w / 10;
	star.y = p.y + p.h / 10;
  star.h = p.h / 10;
	star.w = p.w / 10;
	PrintStar(star, hdc);

	star.x += p.w * 3 / 10;
	star.y += p.h * 2 / 10;
	PrintStar(star, hdc);

	SelectObject(hdc, initialPen);
	SelectObject(hdc, initialBrush);
}

void PrintPlus(HDC hdc, RECT rect, bool selected) 
{
	constexpr auto red =  RGB(240, 20, 60);
	constexpr auto white = RGB(255, 255, 255);

	SolidPen redPen(1, red);
	SolidBrush whiteBrush(white);
  SolidBrush redBrush(red);

	auto initialPen = SelectObject(hdc, redPen);
	auto initialBrush = SelectObject(hdc, whiteBrush);

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	if (!selected) {
	  SelectObject(hdc, redBrush);
	  Rectangle(hdc, rect.left, 
							(rect.bottom * 1 + rect.top * 2) / 3, 
							rect.right, 
							(rect.bottom * 2 + rect.top * 1) / 3);
	}

	SelectObject(hdc, initialPen);
	SelectObject(hdc, initialBrush);
}

void PrintMinus(HDC hdc, RECT rect, bool selected) 
{
	constexpr auto red = RGB(240, 20, 60);
	constexpr auto green = RGB(30, 180, 50);
	constexpr auto black = RGB(0, 0, 0);

	if (!selected) {
	  SolidPen redPen(1, red);
	  SolidPen greenPen(1, green);
	  SolidBrush redBrush(red);
	  SolidBrush greenBrush(green);

	  auto initialPen = SelectObject(hdc, redPen);
	  auto initialBrush = SelectObject(hdc, redBrush);
	  Rectangle(hdc, rect.left, rect.top, rect.right, 
							(rect.bottom * 2 + rect.top) / 3);

	  SelectObject(hdc, greenPen);
	  SelectObject(hdc, greenBrush);
	  Rectangle(hdc, rect.left, 
							(rect.bottom * 2 + rect.top) / 3, 
							rect.right, rect.bottom);

	  SelectObject(hdc, initialPen);
	  SelectObject(hdc, initialBrush);
	}
	else {
	  SolidPen blackPen(1, black);
	  SolidBrush blackBrush(black);

	  auto initialPen = SelectObject(hdc, blackPen);
	  auto initialBrush = SelectObject(hdc, blackBrush);
	  Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	  SelectObject(hdc, initialPen);
	  SelectObject(hdc, initialBrush);
	}
}
