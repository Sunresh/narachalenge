#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

class Element {
private:
	HWND hwnd_;
	UINT uMsg_;
	WPARAM wParam_;
	LPARAM lParam_;
	HINSTANCE hInst;

public:
	Element(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) :
		hwnd_(hwnd), uMsg_(uMsg), wParam_(wParam), lParam_(lParam)
	{
		hInst = GetModuleHandle(nullptr);
		button(1,"start", 10, 380, 100, 30); 
		button(2,"stop", 120, 380, 100, 30);
	}
	void button(int id,const std::string& lab,int x, int y, int w, int h) {
		std::wstring label = name(lab);
		CreateWindowW(L"BUTTON", label.c_str(), WS_CHILD | WS_VISIBLE, x, y, w, h, hwnd_, (HMENU)id, hInst, nullptr);
	}
	std::wstring name(const std::string& out_) const {
		return std::wstring(out_.begin(), out_.end());
	}
};