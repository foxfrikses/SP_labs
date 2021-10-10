#include "LBWCViews.h"
#include "LBWCViewsPositioner.h"

#include <algorithm>
#include <optional>

using namespace std;

enum IDs {
	ID_CLEAR,
	ID_ADD,
	ID_DELETE,
	ID_TO_RIGHT
};

LBWCViews::LBWCViews(HWND hwnd, HINSTANCE hinst)
	: __parentHwnd(hwnd)
	, __hInst(hinst)
{
  RECT rc;
  GetClientRect(hwnd, &rc);
	const int width = rc.right - rc.left;
	const int height = rc.bottom - rc.top;
	LBWCViewsPositioner positioner;
	auto p = positioner.ComputePositions(width, height);
	
	__leftListHwnd = CreateListBox(p.leftListPosition);
	ShowWindow(__leftListHwnd, SW_SHOWNORMAL);
	__rightListHwnd = CreateListBox(p.rightListPosition);
	ShowWindow(__rightListHwnd, SW_SHOWNORMAL);
	__editHwnd = CreateEdit(p.editPosition);
	ShowWindow(__editHwnd, SW_SHOWNORMAL);
	__addButtonHwnd = CreateButton(p.addBtnPosition, L"Add", ID_ADD);
	ShowWindow(__addButtonHwnd, SW_SHOWNORMAL);
	__clearButtonHwnd = CreateButton(p.clearBtnPosition, L"Clear", ID_CLEAR);
	ShowWindow(__clearButtonHwnd, SW_SHOWNORMAL);
	__deleteButtonHwnd = CreateButton(p.deleteBtnPosition, L"Delete", ID_DELETE);
	ShowWindow(__deleteButtonHwnd, SW_SHOWNORMAL);
	__toRightButtonHwnd = CreateButton(p.toRightBtnPosition, L"To right", ID_TO_RIGHT);
	ShowWindow(__toRightButtonHwnd, SW_SHOWNORMAL);
}

void LBWCViews::Command(WPARAM wparam, LPARAM lparam) 
{
	const IDs id = (IDs)(int)wparam;
	switch (id) {
		case ID_CLEAR:
			OnClearClicked();
			break;
		case ID_ADD:
			OnAddClicked();
			break;
		case ID_DELETE:
			OnDeleteClicked();
			break;
		case ID_TO_RIGHT:
			OnToRightClicked();
			break;
	}
}

std::optional<std::pair<int, wstring>> GetSelected(HWND listBox) {
	int itemIndex = (int) ::SendMessage(listBox, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (itemIndex == LB_ERR) {
		return nullopt;
	}
	wstring str((size_t)::SendMessage(listBox, LB_GETTEXTLEN, (WPARAM)itemIndex, 0), L' ');
	::SendMessage(listBox, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)str.c_str());
	return pair(itemIndex, move(str));
}

wstring GetText(HWND edit) {
	wstring item(GetWindowTextLength(edit) + 1, L' '); // +1 place for '\0'
	GetWindowText(edit, (wchar_t*)item.c_str(), item.size());
	item.resize(item.size() - 1); // erase excess '\0'
	return item;
}

void AddItem(HWND listBox, const wstring& str) {
	LRESULT lResult = SendMessage(listBox, LB_ADDSTRING, NULL, (LPARAM)str.c_str());
}

void DeleteItem(HWND listBox, int index) {
	SendMessage(listBox, LB_DELETESTRING, index, 0);
}


void LBWCViews::OnToRightClicked() 
{
	auto selected = GetSelected(__leftListHwnd);
	if (!selected.has_value())
		return;

	auto& [index, str] = selected.value();

	if (auto [it, inserted] = __rightListData.insert(move(str));
			inserted) {
		AddItem(__rightListHwnd, it->c_str());
	}
}

void LBWCViews::OnClearClicked() 
{
	SendMessage(__leftListHwnd, LB_RESETCONTENT, 0, 0);
	SendMessage(__rightListHwnd, LB_RESETCONTENT, 0, 0);
	__leftListData.clear();
	__rightListData.clear();
}

void LBWCViews::OnDeleteClicked() 
{
	if (auto leftSel = GetSelected(__leftListHwnd); leftSel.has_value()) {
	  auto& [index, str] = leftSel.value();
		__leftListData.erase(str);
		DeleteItem(__leftListHwnd, index);
	}
	
	if (auto rightSel = GetSelected(__rightListHwnd); rightSel.has_value()) {
	  auto& [index, str] = rightSel.value();
		__rightListData.erase(str);
		DeleteItem(__rightListHwnd, index);
	}
}

void LBWCViews::OnAddClicked() 
{
	auto str = GetText(__editHwnd);
	if (str.empty())
		return;

	if (auto it = __leftListData.find(str);
			it == __leftListData.end()) {
		SendMessage(__leftListHwnd, LB_ADDSTRING, 0, (LPARAM)str.c_str());
		__leftListData.insert(std::move(str));
	}

	SetWindowText(__editHwnd, L"");
}

HWND LBWCViews::CreateButton(const Position& pos, LPCWSTR name, int id) const
{
	return CreateWindow(
		L"button",
		name,
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
		pos.x, pos.y, pos.w, pos.h,
		__parentHwnd,
		reinterpret_cast<HMENU>(id),
		__hInst,
		nullptr
	);
}
HWND LBWCViews::CreateListBox(const Position& pos) const
{
	return CreateWindow(
	  L"listbox",
		nullptr,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD,
		pos.x, pos.y, pos.w, pos.h,
		__parentHwnd,
		nullptr,
		__hInst,
		nullptr
	);
}
HWND LBWCViews::CreateEdit(const Position& pos) const
{
	return CreateWindow(
		L"edit",
		L"",
		WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT,
		pos.x, pos.y, pos.w, pos.h,
		__parentHwnd,
		nullptr,
		__hInst,
		nullptr
	);
}

void SetWindowPosition(HWND hwnd, const Position &pos) {
	SetWindowPos( hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}

void LBWCViews::Resize(int width, int height) 
{
	LBWCViewsPositioner positioner;
	auto p = positioner.ComputePositions(width, height);
	SetWindowPosition(__leftListHwnd, p.leftListPosition);
	SetWindowPosition(__rightListHwnd, p.rightListPosition);
	SetWindowPosition(__editHwnd, p.editPosition);
	SetWindowPosition(__deleteButtonHwnd, p.deleteBtnPosition);
	SetWindowPosition(__addButtonHwnd, p.addBtnPosition);
	SetWindowPosition(__clearButtonHwnd, p.clearBtnPosition);
	SetWindowPosition(__toRightButtonHwnd, p.toRightBtnPosition);
}
