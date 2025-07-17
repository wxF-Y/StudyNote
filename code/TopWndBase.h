#pragma once
class CTopWndBase
{
public:
	CTopWndBase(const std::wstring& wndClassName) : m_wndClassName{ wndClassName } {};
	virtual ~CTopWndBase() ;

	HWND CreateTopWnd();
	void CloseTopWnd();

	//�������洦���Լ����ĵ���Ϣ��
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return LRESULT();};
protected:
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	std::wstring m_wndClassName;
	HWND m_hWnd = NULL;
};

