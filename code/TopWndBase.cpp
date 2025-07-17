#include "framework.h"
#include "TopWndBase.h"

CTopWndBase::~CTopWndBase()
{
	if (m_hWnd && IsWindow(m_hWnd))
	{
		CTopWndBase *pThis = reinterpret_cast<CTopWndBase*>(::GetWindowLongPtr(m_hWnd, GWLP_USERDATA));
		if (pThis == this)
			::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0L);
	}
}

HWND CTopWndBase::CreateTopWnd()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CTopWndBase::__WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = NULL;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = NULL;

	wcex.lpszClassName = m_wndClassName.c_str();

	RegisterClassExW(&wcex);

	m_hWnd = CreateWindowW(m_wndClassName.c_str(), L"", WS_POPUP,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, nullptr, this);

	if (!m_hWnd)
	{
		LOG << DYN_MAKE_STR_Ws(L"regist top wnd fail");
	}

	return m_hWnd;
}

void CTopWndBase::CloseTopWnd()
{
	if (!::IsWindow(m_hWnd)) return;
	::PostMessage(m_hWnd, WM_CLOSE, 0L, 0L);
}

LRESULT CTopWndBase::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTopWndBase* pThis = NULL;
	if (uMsg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		if (NULL != lpcs)
		{
			pThis = static_cast<CTopWndBase*>(lpcs->lpCreateParams);
			pThis->m_hWnd = hWnd;
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		}
	}
	else {
		pThis = reinterpret_cast<CTopWndBase*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (uMsg == WM_NCDESTROY && pThis != NULL)
		{
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			pThis->m_hWnd = NULL;
		}
	}

	if (pThis) pThis->HandleMessage(uMsg, wParam, lParam);
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
